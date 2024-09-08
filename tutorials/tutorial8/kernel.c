// Specify functions defined externally (in kernel.asm)
extern char in_port(unsigned short io_port);
extern void out_port(unsigned short io_port, char value);
extern void keyboard_handler();
extern void gdt_flush();
extern void load_idt();

// Include structures useful for handling multiboot
#include "multiboot.h"
// Include the keyboard map (note: Mac users may need the other file)
#include "keyboard_map.h"

// The size of the interrupt descriptor table
#define IDT_SIZE 256

/* Specify a type to convert an address into a callable module */
typedef void callable_module(void);

/*
 * The structure of an entry in our GDT.
 * Includes the segment address, length, type, and access rights
 * (in a format that can be handled by the 16-bit 286)
 *
 * We need the final __attribute__((packed)) to stop the compiler from modifying how it stores this struct in memory
*/
struct gdt_entry {
    unsigned short limit_low;
    unsigned short base_low;
    unsigned char base_middle;
    unsigned char access;
    unsigned char granularity;
    unsigned char base_high;
} __attribute__((packed));

/*
 * The format of our actual pointer to the table
 */
struct gdt_ptr {
        unsigned short limit;
        unsigned int base;
} __attribute__((packed));

/* Our actual GDT */
struct gdt_entry gdt[5];

/* The pointer to our GDT (used for loading it) */
struct gdt_ptr gp;

/* The structure of an interrupt descriptor table entry */
struct idt_entry {
    unsigned short int offset_lowerbits;
    unsigned short int selector;
    unsigned char zero;
    unsigned char type_attr;
    unsigned short int offset_higherbits;
} __attribute__((packed));  

/* The interrupt descriptor table*/
struct idt_entry IDT[IDT_SIZE];

/* The current position of the cursor */
unsigned int cursor_position = 0;

/* Setup a descriptor in the Global Descriptor Table
 * (this takes care of converting the limit, access rights, and granularity to those required by the table entries)
*/
void gdt_set(int num, unsigned long base, unsigned long limit,
                   unsigned char access, unsigned char gran)
{
    /* Setup the descriptor base address */
    gdt[num].base_low = (base & 0xFFFF);
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high = (base >> 24) & 0xFF;

    /* Setup the descriptor limits */
    gdt[num].limit_low = (limit & 0xFFFF);
    gdt[num].granularity = ((limit >> 16) & 0x0F);

    /* Finally, set up the granularity and access flags */
    gdt[num].granularity |= (gran & 0xF0);
    gdt[num].access = access;
}

/*
 * Load our GDT (including setting up the entries)
 */
void load_gdt()
{
    /* Setup the GDT pointer and limit */
    gp.limit = (sizeof(struct gdt_entry) * 5) - 1;
    gp.base = (unsigned int) &gdt;

    /* The first entry must be the NULL descriptor */
    gdt_set(0, 0, 0, 0, 0);

    /* The second entry is our kernel-mode Code Segment.
     * The base address
     * is 0, the limit is 4GBytes, it uses 4KByte granularity,
     * uses 32-bit opcodes, and is a Code Segment descriptor.
     */
    gdt_set(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);

    /* The third entry is our kernel-mode Data Segment. It's EXACTLY the
    *  same as our kernel-mode code segment, but the descriptor type in
    *  this entry's access byte says it's a Data Segment */
    gdt_set(2, 0, 0xFFFFFFFF, 0x92, 0xCF);
    
    /* The fourth entry is our user-mode Code Segment. It's EXACTLY the
    *  same as our kernel-mode code segment, but the privilege level in
    *  this entry's access byte says it's user-mode */
    gdt_set(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);
    
        /* The fifth entry is our user-mode Data Segment. It's EXACTLY the
    *  same as our user-mode code segment, but the descriptor type in
    *  this entry's access byte says it's a Data Segment */
    gdt_set(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);

    /* Flush out the old GDT and install the new changes! */
    gdt_flush();
}

/* Initialise PICs and load IDT */
void idt_init(void) {
    /* ICW1 */
    out_port(0x20, 0x11);
    out_port(0xA0, 0x11);

    /* ICW2 */
    out_port(0x21, 0x20);
    out_port(0xA1, 0x28);

    /* ICW3 */
    out_port(0x21, 0x00);
    out_port(0xA1, 0x00);

    /* ICW4 */
    out_port(0x21, 0x01);
    out_port(0xA1, 0x01);

    /* Mask interrupts */
    out_port(0x21, 0xFF);
    out_port(0xA1, 0xFF);

    /* Load idt */
    unsigned long idt_address = (unsigned long) IDT;
    unsigned long idt_ptr[2];
    idt_ptr[0] = (sizeof (struct idt_entry) * IDT_SIZE) + ((idt_address & 0xffff) << 16);
    idt_ptr[1] = idt_address >> 16;

    load_idt(idt_ptr);
}

/* Sets up keyboard interrupts */
void load_keyboard(void) {
    unsigned long keyboard_address = (unsigned long)keyboard_handler;
    IDT[0x21].offset_lowerbits = keyboard_address & 0xffff;
    IDT[0x21].selector = 0x08; /* KERNEL_CODE_SEGMENT_OFFSET */
    IDT[0x21].zero = 0;
    IDT[0x21].type_attr = 0x8e; /* INTERRUPT_GATE */
    IDT[0x21].offset_higherbits = (keyboard_address & 0xffff0000) >> 16;
}

/* Start listening to keyboard interrupts */
void enable_keyboard(void) {
    out_port(0x21, 0xFD); // 11111101 enables only IRQ1 (keyboard)
}

/* Sets where the cursor is currently pointing */
void set_cursor(unsigned int address) {
  char high = (address >> 8) & 0xFF;  // Get the high bits of the address
  char low = address & 0xFF;  // Get the low bits of the address
  out_port(0x3D4, 14);  // 14 specifies we're sending the high bits
  out_port(0x3D5, high);  // send the high bits
  out_port(0x3D4, 15);  // 15 specifies we're sending the low bits
  out_port(0x3D5, low);  // send the low bits
  cursor_position = address;
}

/* Writes the given character to the given address on the screen with the given colours */
void write_char(unsigned int address, char c, unsigned char foreground, unsigned char background) {
    char *vidptr = (char*)0xb8000; // Where video memory begins
    unsigned char colour = ((background & 0x0F) << 4) | (foreground & 0x0F);
    if (address >= 0 && address <= 80 * 25) {
      vidptr[address * 2] = c;
      vidptr[address * 2 + 1] = colour;
    }
}

/* Writes the given number of characters from the given message to the screen at the current cursor position */
void write(const char *msg, int msg_len) {
    unsigned int current_address = cursor_position;
    for (int i = 0; i < msg_len; i++) {
        if (msg[i] == '\n') {
            // Handle new line characters
            do {
                write_char(current_address, ' ', 7, 0);
                current_address++;
            } while (current_address % 80 != 0);
        } else if (msg[i] == '\t') {
            // Handle tab characters
            for (int j = 0; j < 8; j++) {
              write_char(current_address, ' ', 7, 0);
              current_address++;
            }
        // TODO: You may wish to handle other characters here too
        } else {
            // Handle all other characters
            write_char(current_address, msg[i], 7, 0);
            current_address++;
        }

        // Scroll the screen (if necessary)
        while (current_address >= 80 * 25) {
            char *vidptr = (char*)0xb8000; // Where video memory begins
            for (int j = 0; j < 80 * 24 * 2; j++) {
                vidptr[j] = vidptr[j + 80 * 2];
            }
            for (int j = 0; j < 80 * 2; j+=2) {
                vidptr[80*24*2 + j] = ' ';
                vidptr[80 * 24 * 2 + j + 1] = 0x7;
            }
            current_address -= 80;
        }
    }
    // Update cursor position
    set_cursor(current_address);
}

/* Outputs the given C-string to the current cursor position */
void write_string(const char *msg) {
    int count = 0;
    while (msg[count] != '\0') {
        count++;
    }
    write(msg, count);
}

/* Clears the entire screen */
void clear_screen() {
    set_cursor(0);
    for (int i = 0; i < 25; i++) {
        write_string("\n");
    }
    set_cursor(0);
}

/* Handles keyboard presses by outputting a character to the screen */
void keyboard_handler_main(void) {
    unsigned char status;
    char keycode;

    /* write EOI */
    out_port(0x20, 0x20);

    status = in_port(0x64);
    /* Lowest bit of status will be set if buffer is not empty */
    if (status & 0x01) {
        keycode = in_port(0x60);
        if(keycode < 0)
            return;
        char message[2] = {(char)keyboard_map[keycode], '\0'};
        write(message, 1);
    }
}

/* Kernel entry point */
void kmain(unsigned long multiboot_info_address) {
    load_gdt();
    load_keyboard();
    idt_init();
    enable_keyboard();

    const char *str = "Hello World!";
    clear_screen();
    set_cursor(80 * 12 + 33);
    write_string(str);

    /* Get the multiboot info and extract the address of the module we want to start */
    multiboot_info_t *multiboot_info = (multiboot_info_t *)multiboot_info_address;
    module_t *module = (module_t *) multiboot_info->mods_addr;
    unsigned int module_address = module->mod_start;

    /*
     * Convert the address of the module into a callable_module pointer, and then call it
     * This lets us use the pointer as a function!
     */
    ((callable_module *) module_address)();

    while (1) {}
}

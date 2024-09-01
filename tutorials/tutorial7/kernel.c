#define IDT_SIZE 256

struct idt_entry {
  unsigned short int offset_lowerbits;
  unsigned short int selector;
  unsigned char zero;
  unsigned char type_attr;
  unsigned short int offset_higherbits;
};

struct idt_entry IDT[IDT_SIZE];

extern char in_port(unsigned short io_port);
extern void out_port(unsigned short io_port, char value);
extern void load_idt(unsigned long *idt_ptr);

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
  unsigned long idt_address = (unsigned long)IDT;
  unsigned long idt_ptr[2];
  idt_ptr[0] =
      (sizeof(struct idt_entry) * IDT_SIZE) + ((idt_address & 0xffff) << 16);
  idt_ptr[1] = idt_address >> 16;

  load_idt(idt_ptr);
}

void enable_keyboard() {
  out_port(0x21, 0xFD); // 11111101 enables only 1RQ1 (keyboard)
}

extern void keyboard_handler(void);

void load_keyboard(void) {
    unsigned long keyboard_address = (unsigned long)keyboard_handler;
    IDT[0x21].offset_lowerbits = keyboard_address & 0xffff;
    IDT[0x21].selector = 0x08; /* KERNEL_CODE_SEGMENT_OFFSET */
    IDT[0x21].zero = 0;
    IDT[0x21].type_attr = 0x8e; /* INTERRUPT_GATE */
    IDT[0x21].offset_higherbits = (keyboard_address & 0xffff0000) >> 16;
}

unsigned int cursor_position = 0;

/* Sets where the cursor is currently pointing */
void set_cursor(unsigned int address) {
  char high = (address >> 8) & 0xFF; // Get the high bits of the address
  char low = address & 0xFF;         // Get the low bits of the address
  out_port(0x3D4, 14);               // 14 specifies we're sending the high bits
  out_port(0x3D5, high);             // send the high bits
  out_port(0x3D4, 15);               // 15 specifies we're sending the low bits
  out_port(0x3D5, low);              // send the low bits
  cursor_position = address;
}

void write_char(unsigned int address, char c, unsigned char foreground,
                unsigned char background) {
  char *vidptr = (char *)0xb8000; // Where video memory begins
  unsigned char colour = ((background & 0x0F) << 4) | (foreground & 0x0F);
  vidptr[address * 2] = c;
  vidptr[address * 2 + 1] = colour;
}

void write(const char *msg, unsigned int msg_len) {
  unsigned int current_address = cursor_position;
  for (int i = 0; i < msg_len; i++) {
    if (msg[i] == '\n') { // Newline
      current_address += 80 - (current_address % 80);
      continue;
    }
    if (msg[i] == '\t') { // Tab
      current_address += 4 - (current_address % 4);
      continue;
    }
    write_char(current_address, msg[i], 7, 0);
    current_address++;
  }
  set_cursor(current_address);
}

void write_string(const char *msg) {
  int count = 0;
  while (msg[count] != '\0') {
    count++;
  }
  write(msg, count);
}

void clear_screen() {
  set_cursor(0);
  for (int i = 0; i < 80 * 25; i++) {
    write_char(i, ' ', 7, 0);
  }
  set_cursor(0);
}

#include "keyboard_map.h"

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
        char message[2] = {keyboard_map[keycode], '\0'};
        write(message, 1);
    }
}

/* Kernel entry point */

void kmain() {
  idt_init();
  clear_screen();
  enable_keyboard();
  load_keyboard();
  while (1){};
}

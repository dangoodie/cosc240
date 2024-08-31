extern char in_port(unsigned short io_port);
extern void out_port(unsigned short io_port, char value);

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

/* Kernel entry point */

void kmain() {
  clear_screen();
  const char *str = "Hello\nWorld!";
  set_cursor(100);
  write_string(str);
}

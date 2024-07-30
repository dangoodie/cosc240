/* Kernel entry point */
void kmain() {
    const char *str = "Hello World!";
    char *vidptr = (char*)0xb8000; // This is where video memory begins - we can write characters here to output to screen
    
    /*
     * There are 25 lines, each with 80 columns, and the contents
     * is specified by a two byte value.
     * The first byte is the character to display and
     * the second byte is the formatting attributes, such as
     * the background and foreground colour.
     * Here we will clear the screen to black by outputting spaces
     * with black backgrounds and foregrounds.
    */
    int i = 0;
    while (i < 80 * 25 * 2) {
        vidptr[i] = ' ';      // Specify the character
        vidptr[i + 1] = 0x00; // Specify the colour
        i += 2;
    }
    
    /* Now write out the string */
    i = 0;
    while (str[i] != '\0') {
        vidptr[i * 2] = str[i];   // Set character
        vidptr[i * 2 + 1] = 0x0a; // Set colour
        i += 1;
    }
}

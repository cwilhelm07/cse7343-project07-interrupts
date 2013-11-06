/* kernel.c
   A simple kernel that displays "Hello World!".
   Will write each byte (ascii then color) starting
   on the leftmost column of row 3.
   
   Chris Wilhelm
*/

void printString(char *word) {
   // parse the array until null char is found
   interrupt(0x10, 0xe * 256 + character, 0, 0, 0);
}

int main() {

   // Printing to the screen using interrupts

   printString();

   while(1);
   return 0;
}
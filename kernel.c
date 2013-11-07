/* kernel.c
   A simple kernel that displays "Hello World!".
   Will write each byte (ascii then color) starting
   on the leftmost column of row 3.
   
   Chris Wilhelm
*/

void printString(char*);

int main() {

   // Printing to the screen using interrupts

   printString("Hello World\0");

   while(1);
   return 0;
}

void printString(char *string) {
   int i = 0;
   
   // parse the array until null char is found
   while ( string[i] != '\0' )
   {
      interrupt(0x10, 0xe * 256 + string[i], 0, 0, 0);
      i++;
   }
}
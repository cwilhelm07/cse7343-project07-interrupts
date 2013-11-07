/* kernel.c
   A simple kernel that displays "Hello World!".
   Will write each byte (ascii then color) starting
   on the leftmost column of row 3.
   
   Chris Wilhelm
*/

void printString(char*);
void printChar(char);
void readString(char*);

int main() {
   char line[80];

   // Printing to the screen using interrupts
   printString("Enter a line: \0");
   readString(line);
   printString(line);
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

void printChar(char character) {
   interrupt(0x10, 0xe * 256 + character, 0, 0, 0);
}

void readString(char line[80]) {
   char key;
   int i = 0;

   key = interrupt(0x16, 0, 0, 0, 0);
   while ( key != 0xd )
   {
      // Handle backspace key press
      if ( key == 0x8 )
      {
         i--;
         printString("-BKSPC-");
      }
      else
      {
         printChar(key);
         line[i] = key;
         i++;
      }
      key = interrupt(0x16, 0, 0, 0, 0);
   }
   key = 0xa;
   printChar(key);
   line[i] = key;
   i++;
   key = 0x0;
   printChar(key);
   line[i] = key;
}
/* kernel.c
   Project 7: System Calls
   
   Chris Wilhelm
*/

#define IO_INTERRUPT        0x10
#define SECTOR_RW_INTERRUPT 0x13
#define KEYBOARD_INTERRUPT  0x16

#define AH_OUT_TO_CONSOLE  0x0E

#define ENTER_ASCII        0x0D
#define BKSPC_ASCII        0x08

#define LINE_FEED          0x0A
#define END_OF_STRING      0x00

void printString(char*);
void printChar(char);
void readString(char*);
void readSector(char*, int);
void handleInterrupt21(int, int, int, int);
int mod(int, int);
int div(int, int);

int main() {
   char line[80];
   char buffer[512];

// Step 1 and 2:
   // Printing to the screen using interrupts
   printString("Enter a line: \0");
   readString(line);
   printString(line);

// Step 3:
   // Read message from sector 30
   readSector(buffer, 30);
   printString(buffer);

// Step 4 and 5:
   // Call makeInterrupt21() and interrupt 21
   makeInterrupt21();
   interrupt(0x21, 1, line, 0, 0);
   interrupt(0x21, 0, line, 0, 0);

   while(1);
}

void printString(char *string) {
   int i = 0;
   int ax;
   // parse the array until null char is found
   while ( string[i] != '\0' )
   {
      ax = AH_OUT_TO_CONSOLE * 256 + string[i];
      interrupt(IO_INTERRUPT, ax, 0, 0, 0);
      i++;
   }
}

void printChar(char character) {
   int ax = AH_OUT_TO_CONSOLE * 256 + character;
   interrupt(IO_INTERRUPT, ax, 0, 0, 0);
}

void readString(char line[80]) {
   char key;
   int i = 0;

   key = interrupt(KEYBOARD_INTERRUPT, 0, 0, 0, 0);
   while ( key != ENTER_ASCII )
   {
      // Handle backspace key press
      if ( key == BKSPC_ASCII && i > 0 )
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
      key = interrupt(KEYBOARD_INTERRUPT, 0, 0, 0, 0);
   }
   key = LINE_FEED;
   printChar(key);
   line[i] = key;
   i++;
   key = END_OF_STRING;
   printChar(key);
   line[i] = key;
}

void readSector(char buffer[512], int sector)
{
   int ah = 2; // read sector
   int al = 1; // number of sectors to read
   int ax = ah * 256 + al;

   int ch = div(sector, 36); // track number
   int cl = mod(sector, 18) + 1; // relative sector
   int cx = ch * 256 + cl;

   int dh = mod(div(sector, 18), 2); // head number
   int dl = 0; // device number
   int dx = dh * 256 + dl;

   interrupt(SECTOR_RW_INTERRUPT, ax, buffer, cx, dx);
}

void handleInterrupt21(int ax, int bx, int cx, int dx)
{
   switch ( ax )
   {
      case 0: // print string
         printString(bx);
         break;
      case 1: // read string
         readString(bx);
         break;
      case 2: // read sector
         readSector(bx, cx);
         break;
      default:
         printString("Error: Invalid Command\0");
         break;
   }
}

int mod(int a, int b)
{
   while ( a >= b )
   {
      a = a - b;
   }
   return a;
}

int div(int a, int b)
{
   int quotient = 0;
   while ( (quotient * b) < a )
   {
      quotient++;
   }
   return quotient - 1;
}
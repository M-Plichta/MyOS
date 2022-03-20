#include "globals.h"
#include "vga.h"

static uint16_t * videoram = (uint16_t *) 0xb8000;

static const uint16_t ctrlreg = 0x03D4; // Address on I/O bus
static const uint16_t datareg = 0x03D5; // Address on I/O bus

static const int linelen   = 80;
static const int lines     = 24;
static const int vga_end   = ((lines+1) * linelen);

static int x = 0;
static int y = 0;

static uint16_t colour = FOREGROUND(WHITE) | BACKGROUND(BLACK);

//
//   Move cursor to position x,y
//   - Notice this uses isolated I/O
//
void
setcursor (int x, int y) {
   uint16_t posn;

   posn = y * linelen + x;
   outb (ctrlreg, 14);
   outb (datareg, posn >> 8);
   outb (ctrlreg, 15);
   outb (datareg, posn &  0xff);
}

// Clears the screen.
void
clear( void ) {
   uint16_t * screenp = videoram;

   // Clearing the screen
   for(int x = 0; x < linelen; x++) {
      for(int y = 0; y < lines; y++) {
         *screenp++ = ' ' | colour;
      }
   }

   // Setting the cursor position
   x = 0; y = 0;
   setcursor(x, y);
}

// Initializes the screen.
void
vgainit ( ) {
   clear();
}

void
start_highlight() {
   colour = FOREGROUND(WHITE) | BACKGROUND(BLUE);
}

void
end_highlight() {
   colour = FOREGROUND(WHITE) | BACKGROUND(BLACK);
}


//   Store character value (with colour information) at memory location
//   of current x,y screen position and update x and y ready to move
//   the cursor.
void
putchar (char c) {
   uint16_t * screenp = videoram;
   // if the output is longer than the length of the screen,
   // reset the cursor position to the start of the next line.
   if (x > 80) {
      y++;
      x=0;
   }

   // Special case character for printing nothing (Used in keyboard driver)
   if (c == '\e')
      return;

   // Handing the backspace
   if (c == '\r') {
      if (x > 0) {
         x--;
      } else {
         x = linelen;
         y--;
      }
      *(screenp+((y)*linelen+x)) = ' ' | colour;
      setcursor(x, y);
      return;
   }

   // If a new character is printed,
   // reset the cursor position to the start of the next line.
   // Otherwise, print the character.
   if (c == '\n') {
      y++; x=0;
   } else {
      *(screenp+(y*linelen+x)) = c | colour;
      x++;
   }

   // If the last line is reached, scroll
   if (y == lines - 1) {
      for (int posx = 0; posx < linelen; posx++) {    // Goes through each character,
         for (int posy = 1; posy < lines; posy++) {   // and sets it's position to the line above.
            *(screenp+((posy-1)*linelen+posx)) = *(screenp+((posy)*linelen+posx));
         }
      }

      // once all the lines are shifted up, the current line is cleared
      y--;
      for (int posx = 0; posx < linelen; posx++)
         *(screenp+(y*linelen+posx)) = ' ' | colour;
   }

   // After each new character, update the position of the cursor.
   setcursor (x, y);
}

void
status ( char * str ) {
   uint16_t * screenp = videoram;
   // Varables to store the old values of the cursor
   // so that they can later be restored.
   int resx = x;
   int resy = y;

   // Setting the cursor to override the last line.
   x = 0;
   y = lines;

   setcursor(x, y);

   // Resetting the status bar to all red.
   for (int i = vga_end-linelen; i < vga_end; i++)
      *(screenp+i) = ' ' | FOREGROUND(WHITE) | BACKGROUND(RED);

   // Calling putchar for each char in the string
   int pos = vga_end-linelen;
   while (*(str+x) != '\0')
      *(screenp+pos++) = *(str+x++) | FOREGROUND(WHITE) | BACKGROUND(RED);

   // Resetting the position of the cursor to the original position.
   x = resx;
   y = resy;
   setcursor(x, y);
}

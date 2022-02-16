#include "terminal.h"

// Simple machanism to store a buffer of user input.
// No input larger than 250 characters is allowed,
// for now this is a limitation that we can deal with.
static char buffer[250];
static int  curr_pos = 0;

// Adds a new char to the buffer
void addChar ( char c ) {
    if ( c != '\r' ) {
        buffer[curr_pos] = c;
        curr_pos++;
    } else {
        if ( curr_pos > 0)
            curr_pos--;
    }
}

void submit ( void ) {
    buffer[curr_pos-1] = '\0'; // Add end of string character instead of the new line character

    if ( strcmp("help", buffer) == 0 ) {
        kprintf("   MyOS commands:\n"
                "   help       Lists all available commands (this output)\n"
                "   clear      Clears and resets the terminal\n"
                "   echo       Prints the proceeding string\n"
                );
    } else if ( strcmp("clear", buffer) == 0 ) {
        clear();
    } else {
        char command[5];
        memcpy( command, buffer, 4 );
        command[4] = '\0';

        if ( strcmp("echo", command) == 0 ) {
            if ( strlen(buffer) > 5 ) {
                kprintf("%s\n", buffer+5);
            }
        }
    }

    kprintf("MyOS> ");
    curr_pos = 0;
}


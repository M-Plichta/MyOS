#include <stdarg.h>
#include "globals.h"
#include "vga.h"

#define ABS(N) ((N<0) ? (-N) : (N))

// Swaps two values
void swap(char *x, char *y) {
	char t = *x; *x = *y; *y = t;
}

// Reverses the order of the string produced in the _itoa function
char* reverse(char *str, int i, int j)
{
	while (i < j)
		swap(&str[i++], &str[j--]);
 
	return str;
}
 
static char* _itoa(char* str, long value, int base)
{
	// Check for valid input
	if (value == 0) {
		str[0] = '0';
		str[1] = '\0';
		return str;
	}

	if (base < 2 || base > 16)
		return str;
 
	unsigned long n = ABS(value);
	int pos = 0;

	// While there are digits left, calculate the string output and append
	while (n) {
		int r = ABS(n % base);		// TODO: This of a better solution for the bug where n is negative (Can happen with large binary values)
		str[pos++] = (r > 9) ? 'a' + (r-10) : '0' + r; 
		n = n / base;
	}

       	// Only handles negatives for base 10 (decimal) numbers	
	if (value < 0 && base == 10)
		str[pos++] = '-';

	// Append the string termination char 
	str[pos] = '\0';
    
	return reverse(str, 0, pos - 1);
}

// Calculates the length of a string
int strlen (char* str) {
	int pos = 0;
	while(*(str+pos) != '\0') pos++;

	return pos;
}

// Outputs a string with the correct padding
static void outstr (char* str, int width, char pad) {
	int paddingLength = width - strlen(str);

	// Print the padding (if any)
	while (paddingLength-- > 0)  putchar(pad);

	// Print the output characters
	int pos = 0;
	while (*(str+(pos)) != '\0') putchar(*(str+pos++));
}

// Prints formatted strings with variables.
void kprintf(char* format, ...) {

	#define CURRENT_CHAR *(format+pos)
	#define NEXT_CHAR *(format+pos+1)

	va_list ap;			// Initializing variable list
	va_start(ap, format);

	int pos = 0;
	while (CURRENT_CHAR != '\0') {
		int numSpecial = 0;
		int  paddingSize = 0;			// Initialize padding size
		char paddingChar = ' ';			// Initialize padding character
		if (CURRENT_CHAR == '%') {

			while(1) {
				char intString[100];
				pos++;

				// % will return a '%' character
				if (CURRENT_CHAR == '%') {
					outstr("%", paddingSize, paddingChar);
					break;
				}

				// Handles the case where the padding character is a 0
				if (CURRENT_CHAR == '0')
					paddingChar = '0';	
				
				// Handles multiple padding characters (up to 99).
				if (CURRENT_CHAR >= '1' && CURRENT_CHAR <= '9') {
					if (paddingSize != 0) {
						paddingSize *= 10;
					}
					paddingSize += CURRENT_CHAR - 48;
				}

				// Handles variables of type char
				if (CURRENT_CHAR == 'c')
					putchar(va_arg(ap, int));

				// Handles variables of type string (char *)
				if (CURRENT_CHAR == 's') {
					char * string_to_print = va_arg(ap, char *);
					int index = 0;
					while(*(string_to_print + index) != '\0')
						putchar(*(string_to_print + index++));
					break;
				}
				
				// Handles variables of integer types up to long long int, of base 10 (Decimal)
				if (CURRENT_CHAR == 'd') {
					outstr(_itoa(intString, va_arg(ap, long long), 10), paddingSize, paddingChar);
					break;
				}

				// Handles variables of integer types up to long long int, of base 16 (Hexadecimal)
				if (CURRENT_CHAR == 'x') {
					outstr(_itoa(intString, va_arg(ap, long long), 16), paddingSize, paddingChar);
					break;
				}

				// Handles variables of integer types up to long long int, of base 2  (Binary)
				if (CURRENT_CHAR == 'b') {
					outstr(_itoa(intString, va_arg(ap, long long), 2), paddingSize, paddingChar);
					break;
				}

				// Default padding parameters
				if (CURRENT_CHAR == 'd' || CURRENT_CHAR == 'x' || CURRENT_CHAR == 'b') {
					paddingSize = 0;
					paddingChar = ' ';
				}
	
				// Invalid string when the '%' character was the last character.
				if (CURRENT_CHAR == '\0')
					break;
			}
			numSpecial++;
		} else {
			putchar(CURRENT_CHAR);
		}
		pos++;
	}
}
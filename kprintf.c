#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

void swap(char *x, char *y) {
	char t = *x; *x = *y; *y = t;
}
 
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
		return str;
	}

	if (base < 2 || base > 16)
		return str;
 
	int n = abs(value);
	int pos = 0;

	// While there are digits left, calculate the string output and append
	while (n) {
		int r = n % base;
		str[pos++] = (r > 9) ? 'A' + (r-10) : '0' + r; 
		n = n / base;
	}

       	// Only handles negatives for base 10 (decimal) numbers	
	if (value < 0 && base == 10)
		str[pos++] = '-';

	// Append the string termination char 
	str[pos] = '\0';
    
	return reverse(str, 0, pos - 1);
}

int _strlen (char* str) {
	int pos = 0;
	while(*(str+pos) != '\0') pos++;

	return pos;
}

static void outstr (char* str, int width, char pad) {
	int paddingLength = width - _strlen(str);

	// Print the padding (if any)
	while (paddingLength-- > 0)  putchar(pad);

	// Print the output characters
	int pos = 0;
	while (*(str+(pos)) != '\0') putchar(*(str+pos++));
}

void kprintf(char* format, ...) {

	#define CURRENT_CHAR *(format+pos)
	#define NEXT_CHAR *(format+pos+1)

	int pos = 0;
	int numSpecial = 0;
	while (*(format+pos++) != '\0') {
		if (CURRENT_CHAR == '%' && NEXT_CHAR != '\0') {
			char str[10];
			int strPos = 0;
			do {
				str[strPos] = CURRENT_CHAR;
				pos++;
				strPos++;
			} while (
				CURRENT_CHAR != '\0' 	&&
				CURRENT_CHAR != 'c'	&&	// Char
				CURRENT_CHAR != 's'	&&	// String
				CURRENT_CHAR != 'b'	&&	// Binary
				CURRENT_CHAR != 'd'	&&	// Decimal
				CURRENT_CHAR != 'o'	&&	// Octal
				CURRENT_CHAR != 'x'	&&	// Hexadecimal
			      	CURRENT_CHAR != '%');		// % output
			str[strPos] = '\0';

			/*  --- Special Character output logic --- */
			printf("Str = %s\n", str);

			if (_strlen(str) == 1 && str[0] == '%')
				

			numSpecial++;
		}
	}

	va_list ap;
	va_start(ap, format);
	for (int i = 0; i < numSpecial; i++) {
		printf("Arg%d == %d\n",i,  va_arg(ap, int));
	}
}

int main(void)
{
	//printf("0x%04x\n", 45);
//	kprintf("Something%%%%", 123, 904, 982234, 4);

	kprintf("something %aaaaab");

	return 0;
}

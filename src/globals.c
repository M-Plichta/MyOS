#include "globals.h" 

// Write n copies of val into dest.
void memset(uint8_t *dest, uint8_t val, uint32_t n) {
    ASSERT(n > 0);      // Ensure valid length
    ASSERT(dest);
    uint8_t *temp = (uint8_t *) dest;
    for ( ; n != 0; n--) *temp++ = val;
}

// Copies a block of data from source address to destination address.
void *memcpy(void *dst, const void * src, size_t n) {
    ASSERT(n > 0);      // Ensure valid length
    ASSERT(src && dst); // Source and destination cannot be NULL

    char * char_dst = (char *) dst;
    const char * char_src = ( const char *) src;

    // Writes the source value into the destination address.
    while(n--)
        *(char_dst++) = *(char_src++);

    return char_dst;
}

// Compares the first n values of the source addresses.
int memcmp(const void *src1, const void *src2, size_t n) {
    ASSERT(n > 0);

    uint8_t * one = src1;
    uint8_t * two = src2;

    // If the value are the same, return 0;
    if ( src1 == src2 )
        return 0;

    // Compares each value, and returns with the corresponding value.
    while(n--)
        if ( *(one) != *(two) )
            return ( *(one++) > *(two++) ? 1 : -1 );

    // All values to index n are the same, therefore return 0.
    return 0;
}

// Compares two strings.
// Returns value <0 when s1 is less than s2.
// Returns value >0 when s2 is less than s1.
// Returns value =0 when they are equal.
int strcmp( const char * s1, const char * s2 ) {
    while(*s1 != '\0' && (*s1 == *s2)) {
        s1++;
        s2++;
    }
        
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}
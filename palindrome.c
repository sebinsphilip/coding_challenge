
/*

Author: Sebin Shaji Philip (sebin@kth.se)

compile:
    gcc palindrome.c
    ./a.out 

//SAMPLE INPUT and OUTPUT:
sebin@sebin:~/Desktop/thesis/infineon/code$ ./a.out 
Insert range [a,b]:0,536870911
49151

*/


#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define DEBUG 0 // Keep DEBUG as 0 to improve speed!
#define MAX_UPPER_LIMIT 4294967295 // (2^32 -1)

#if DEBUG 
const char *bit_rep[16] = {
    [ 0] = "0000", [ 1] = "0001", [ 2] = "0010", [ 3] = "0011",
    [ 4] = "0100", [ 5] = "0101", [ 6] = "0110", [ 7] = "0111",
    [ 8] = "1000", [ 9] = "1001", [10] = "1010", [11] = "1011",
    [12] = "1100", [13] = "1101", [14] = "1110", [15] = "1111",
};

void print_byte(uint32_t byte)
{
    printf("%s%s%s%s%s%s%s%s ", bit_rep[byte >>28 & 0x0F], bit_rep[byte >>24 & 0x0F], bit_rep[byte>> 20 & 0x0F], bit_rep[byte >> 16 & 0x0F],
                     bit_rep[byte >> 12 & 0x0F], bit_rep[byte >> 8 & 0x0F], bit_rep[byte>> 4 & 0x0F], bit_rep[byte & 0x0F]);
}
#endif




int is_palindrome (uint32_t n)
{
	uint32_t x = 0;

#if DEBUG
    while (x < n) 
    {
        print_byte (x); print_byte (n);
        x = x<<1 | (n&1);
        n >>= 1;
        printf ("changed!\n");

    }
    print_byte (x); print_byte (n); printf ("\n==========\n");
#else
    while (x < n) x = x<<1 | (n&1), n >>= 1;
#endif

	return n == x || n == x>>1;
}

int main () {
    uint32_t a,b,i, current_decimal, count=0;
    printf ("Insert range [a,b]:");
    scanf ("%u,%u", &a, &b);

#if DEBUG
    printf ("%u %u\n", a, b);
#endif

    if (!((a>=0) && (a<=b) && (b<=MAX_UPPER_LIMIT)))
    {
        printf ("Invalid input!\n");
        return -1;
    }
    /* Observation 1: Binary pattern ending with '0' wont be a valid palindrome
    pattern => only consider odd numbers. eg: 10010 , will not be palindrome 
    
    The regular expression would be ' 1 (0/1)* 1 '
    */

    if (0 == a)
    {
        // we consider '0' as palindrome
        a += 1;
        count++;
    }
    else if (0 == a%2)
    {
        //Move to next odd number, according to Observation 1
        a += 1;
    }


    for (i=a; i<=b ; i+=2)
    {
        //current_decimal = i;
        if (is_palindrome (i))
        {
            //printf ("%d => palindrome \n", i);
            count++;
        }

    }

#if DEBUG
    printf ("Total palindromes in range: %d\n", count);
#else
    printf ("%d\n", count);
#endif

    return 0;
}
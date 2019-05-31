/*******************************************************************************
* NAME: Bit Array                                                              *
*                                                                              *
* PURPOSE: bit array manipulation functions                                    *
*                                                                              *
* AUTHOR: Bareket Umiel Sar Shalom                                             *
* DATE UPDATED:  04.11.18                                                      *
*******************************************************************************/
#include <stdio.h>
#include "bit_array.h"

/*utility macros to generate the lookup table for BitArrayMirrorLUT*/
#define R2(n) n,	 n + 2*64,	 n + 1*64,	 n + 3*64
#define R4(n) R2(n), R2(n + 2*16), R2(n + 1*16), R2(n + 3*16)
#define R6(n) R4(n), R4(n + 2*4 ), R4(n + 1*4 ), R4(n + 3*4 )
#define REVERSE_BITS R6(0), R6(2), R6(1), R6(3)

/*utility macros to generate the lookup table for BitArrayCountOnLUT*/
#define B2(n) n, n + 1, n + 1, n + 2
#define B4(n) B2(n), B2(n + 1), B2(n + 1), B2(n + 2)
#define B6(n) B4(n), B4(n + 1), B4(n + 1), B4(n + 2)
#define COUNT_BITS B6(0), B6(1), B6(1), B6(2)

static const unsigned long lookup_mirror[256] = { REVERSE_BITS };
static const unsigned int lookup_count[256] = { COUNT_BITS };
/******************************************************************************/

int BitArrayIsOn(unsigned long bit_array, size_t bit)
{
	return ((bit_array >> (bit - 1)) & 1);
}


int BitArrayIsOff(unsigned long bit_array, size_t bit)
{
	return (!((bit_array >> (bit - 1)) & 1));
}

/******************************************************************************/

unsigned long BitArraySetOn(unsigned long bit_array, size_t bit)
{

	return (bit_array | ((unsigned long)1 << (bit - 1)));
}


unsigned long BitArraySetOff(unsigned long bit_array, size_t bit)
{
		return (bit_array & ~((unsigned long)1 << (bit - 1)));
}

/******************************************************************************/

unsigned long BitArrayFlip(unsigned long bit_array, size_t bit)
{
	return (bit_array ^ (1 << (bit - 1)));
}
/******************************************************************************/

unsigned long BitArrayRotateRight(unsigned long bit_array, unsigned int n)
{
	unsigned long first_bit_state = 0;
	unsigned long result = bit_array;
	int array_bits = sizeof(unsigned long) * 8;

	unsigned long i = 0;
	for( ; i < n ; ++i)
	{
		first_bit_state = (BitArrayIsOn(result, 1));
		result >>= 1;
		result |= (first_bit_state << (array_bits-1));
	}

	return (result);
}

unsigned long BitArrayRotateLeft(unsigned long bit_array, unsigned int n)
{
	unsigned long last_bit_state = 0;
	unsigned long result = bit_array;

	unsigned long i = 0;
	for( ; i < n ; ++i)
	{
		last_bit_state = (BitArrayIsOn(result, 64));
		result <<= 1;
		result |= (last_bit_state);
	}

	return (result);
}
/******************************************************************************/

unsigned long BitArrayMirror(unsigned long bit_array)
{
    bit_array= ((bit_array & 0x5555555555555555) << 1) |
     			((bit_array & 0xAAAAAAAAAAAAAAAA) >> 1);
    /* Swap _<>_ */
    bit_array= ((bit_array & 0x3333333333333333) << 2) |
   				  ((bit_array & 0xCCCCCCCCCCCCCCCC) >> 2);
    /* Swap __<>__*/
    bit_array= ((bit_array & 0x0F0F0F0F0F0F0F0F) << 4) |
   				 ((bit_array & 0xF0F0F0F0F0F0F0F0) >> 4);
    /* Swap ____<>____*/
    bit_array= ((bit_array & 0x00FF00FF00FF00FF) << 8) |
    			 ((bit_array & 0xFF00FF00FF00FF00) >> 8);
    /* Swap ________<>________*/
    bit_array= ((bit_array & 0x0000FFFF0000FFFF) << 16) |
    			 ((bit_array & 0xFFFF0000FFFF0000) >> 16);
    /* Swap ________________<>________________*/
    bit_array= ((bit_array & 0x00000000FFFFFFFF) << 32) |
    			 ((bit_array & 0xFFFFFFFF00000000) >> 32);
    /* Swap ________________________________<>________________________________*/
	return (bit_array);
}

unsigned long BitArrayMirrorLUT(unsigned long bit_array)
{
	unsigned long reverse = 0;

/*	 Split, Reverse and Rearrange each chunk*/
	reverse = lookup_mirror[bit_array & 0xff] << 56 |  /* consider first 8 bits*/
	  lookup_mirror[(bit_array >>  8) & 0xff] << 48 |	/* consider next 8 bits*/
	  lookup_mirror[(bit_array >> 16) & 0xff] << 40 | 	/* consider next 8 bits*/
	  lookup_mirror[(bit_array >> 24) & 0xff] << 32 |  	/* consider next 8 bits*/
	  lookup_mirror[(bit_array >> 32) & 0xff] << 24 |	/* consider next 8 bits*/
	  lookup_mirror[(bit_array >> 40) & 0xff] << 16 | 	/* consider next 8 bits*/
	  lookup_mirror[(bit_array >> 48) & 0xff] <<  8 |  	/* consider next 8 bits*/
	  lookup_mirror[(bit_array >> 56) & 0xff];			/* consider last 8 bits*/

	return reverse;
}
/******************************************************************************/

size_t BitArrayCountOn(unsigned long bit_array)
{
	size_t counter = 0;

	while (bit_array)
	{
		counter += (bit_array & 1);

		bit_array >>= 1;
	}

	return (counter);
}

size_t BitArrayCountOff(unsigned long bit_array)
{

	int array_bits = sizeof(unsigned long) * 8;

	size_t counter = array_bits - BitArrayCountOn(bit_array);

	return (counter);
}

size_t BitArrayCountOnLUT(unsigned long bit_array)
{
	unsigned long count;

	count = lookup_count[bit_array & 0xff] +	/* consider first 8 bits*/
			lookup_count[(bit_array >>  8) & 0xff] + 	/* consider next 8 bits*/
			lookup_count[(bit_array >> 16) & 0xff] +	/* consider next 8 bits*/
			lookup_count[(bit_array >> 24) & 0xff] +	/* consider next 8 bits*/
			lookup_count[(bit_array >> 32) & 0xff] + 	/* consider next 8 bits*/
			lookup_count[(bit_array >> 40) & 0xff] +	/* consider next 8 bits*/
			lookup_count[(bit_array >> 48) & 0xff] +	/* consider next 8 bits*/
			lookup_count[(bit_array >> 56) & 0xff]; 	/* consider last 8 bits*/

	return count;
}

size_t BitArrayCountOffLUT(unsigned long bit_array)
{
	return(8 * sizeof(unsigned long) - BitArrayCountOnLUT(bit_array));
}
/*******************************************************************************
*******************************************************************************/

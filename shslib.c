/*---------------------------------------------------------------------------
	Copyright © 2023 Tom Stevelt and Silver Hammer Software LLC
	Tom Stevelt <tstevelt@silverhammersoftware.com>
	This software is free software; you can redistribute it and/or modify
	it under the terms of the MIT license. See LICENSE for details.
---------------------------------------------------------------------------*/

#include	"shslib.h"


unsigned long num_Random;

double d_random ()
{
	long 	xl;
	double	rv;

		xl = random ();

	num_Random++;

	rv = (double) xl / (double)RAND_MAX;

	return ( rv );
}

double d_random_range ( double low, double high )
{
	double	xd, rv;

	if ( low >= high )
	{
		return ( low );
	}

	xd = d_random ();

	rv = xd * ( high - low ) + low;

	if ( rv > high )
		rv = high;

	return ( rv );
}

int findmax ( double Array[], int Count )
{
	double m = Array[0];
	int MaxIndex = 0;

	for ( int i = 1; i < Count; ++i)
	{
		if ( m < Array[i] )
		{
			m = Array[i];
			MaxIndex = i;
		}
	}

	return ( MaxIndex );
}

short flip ( double probability )
{
	double	rv;

	if ( probability == 1.0 )
	{
		return ( 1 );
	}
	else
	{
		rv = d_random ();
		if ( rv <= probability )
			return ( 1 );
		else
			return ( 0 );
	}
}

int GetTokensD ( char xbuf[], char delim[], char *toks[], int maxtoks )
{
	int		rv = 0;
	int		xl, xi, xf;

	if ( maxtoks < 0 )
		return ( rv );

	xl = nsStrlen ( xbuf );
	xf = 1;
	for ( xi = 0; xi < xl; xi++ )
	{
		if ( nsStrchr ( delim, xbuf[xi] ) != (char *) 0 )
		{
			xbuf[xi] = '\0';
			if ( xf == 0 )
				xf = 1;
		}
		else
		{
			if ( xf )
			{
				xf = 0;
				toks[rv] = &xbuf[xi];
				rv++;

				if ( rv >= maxtoks )
					return ( rv );
			}
		}
	}

	return ( rv );
}

int GetTokensW ( char xbuf[], char *toks[], int maxtoks )
{
	int		rv = 0;
	int		xl, xi, xf;

	if ( maxtoks < 0 )
		return ( rv );

	xl = nsStrlen ( xbuf );
	xf = 1;
	for ( xi = 0; xi < xl; xi++ )
	{
		switch ( xbuf[xi] )
		{
			case ' ':
			case '\n':
			case '\r':
			case '\t':
			case '\b':	xbuf[xi] = '\0';
						if ( xf == 0 )
							xf = 1;
						break;

			default:	if ( xf )
						{
							xf = 0;
							toks[rv] = &xbuf[xi];
							rv++;

							if ( rv >= maxtoks )
								return ( rv );

						}
						break;
		}
	}

	return ( rv );
}

void MinMaxScale ( double Array[], int StartIndex, int Length )
{
	int		ndx;
	double	Min, Max;

	Min = Max = -1.0;

	for ( ndx = StartIndex; ndx < StartIndex + Length; ndx++ )
	{
		if ( Min == -1.0 || Min > Array[ndx] )
		{
			Min = Array[ndx];
		}
		if ( Max == -1.0 || Max < Array[ndx] )
		{
			Max = Array[ndx];
		}
	}

	for ( ndx = StartIndex; ndx < StartIndex + Length; ndx++ )
	{
		Array[ndx] = ( Array[ndx] - Min ) / ( Max - Min );
	}
}

long random_range ( long low, long high )
{
	double	xd;
	long	rv;

	if ( low >= high )
		return ( low );

	xd = d_random ();

	rv = xd * ( high - low + 1 ) + low;

	if ( rv > high )
		rv = high;

	return ( rv );
}

void seed_random_with_usec ( void )
{
	struct timeval		tv;
	unsigned int		Nut;
	unsigned int		Corn;
	unsigned int		Seed;

	gettimeofday ( &tv, NULL );

	Nut  = tv.tv_usec % 3;
	Nut  = Nut *  1000000000;

	Corn = tv.tv_sec % 1000;
	Corn = Corn * 1000000;

	Seed = Nut + Corn + tv.tv_usec;

	srand ( Seed );

}

int nsStrlen (const char *cpOne )
{
	if ( cpOne == (char *)0  )
		return ( 0 );

	return ( strlen( cpOne ) );
}

char *nsStrchr(const char *cpOne, int intChar )
{
	if ( cpOne == (char *)0  )
		return ( (char *)0 );

	return ( strchr(cpOne, intChar ) );
}

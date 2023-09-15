/*---------------------------------------------------------------------------
	Copyright © 2023 Tom Stevelt and Silver Hammer Software LLC
	Tom Stevelt <tstevelt@silverhammersoftware.com>
	This software is free software; you can redistribute it and/or modify
	it under the terms of the MIT license. See LICENSE for details.
---------------------------------------------------------------------------*/

#include	"nn1hmo.h"

double sigmoid ( double x )
{
	double	rv;

	rv = 1 / ( 1 + exp(-x));

	return ( rv );
}


double dSigmoid ( double x )
{
	double	rv;

	rv = x * ( 1 - x );

	return ( rv );
}

void foo ()
{
	FILE	*fp;
	int		nr;
	double	Value;

	printf ( "sizeof(double) %ld\n", sizeof(double) );
	printf ( "sizeof(int)    %ld\n", sizeof(int) );
	printf ( "sizeof(long)   %ld\n", sizeof(long) );
	printf ( "sizeof(size_t) %ld\n", sizeof(size_t) );

	printf ( "\n=== foo: testing INDEX macro ===\n" );
	for ( int i = 0; i < 9 * 3 ; i++ )
	{
		printf ( "i %d\n", i );
	}
	for ( int i = 0; i < 9; i++ )
	{
		for ( int j = 0; j < 3; j++ )
		{
			printf ( "i %d j %d index %d\n", i, j, INDEX(i,3,j) );
		}
	}

	printf ( "\n=== foo: testing sigmoid ===\n" );
	for ( double x = -5.0; x < 5.0; x += 0.25 )
	{
		printf ( "%6.2f  %5.3f\n", x, sigmoid(x) );
	}

	printf ( "\n=== foo: read doubles from model.dat ===\n" );
	if (( fp = fopen ( "model.dat", "r" )) == NULL )
	{
		printf ( "Cannot open model.dat\n" );
		return;
	}

	do
	{
		int		xo = 0;;
		nr = fread ( &Value, sizeof(double), 1, fp );
		if ( nr == 1 )
		{
			printf ( " %10.4f", Value );
			xo += 15;
			if ( xo > 117 )
			{
				printf ( "\n" );
				xo = 0;
			}
		}
			
	} while ( nr == 1 );

	printf ( "\n" );

	fclose ( fp );

}

double CurrentSecondOfDay ()
{
	double			rv;
	char			buffer[20];
	struct timeval	tv;

	gettimeofday ( &tv, NULL );

	sprintf ( buffer, "%ld.%ld", tv.tv_sec % 86400, tv.tv_usec );

	rv = atof ( buffer );

	return ( rv );
}

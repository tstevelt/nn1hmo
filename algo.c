/*---------------------------------------------------------------------------
	Copyright © 2023 Tom Stevelt and Silver Hammer Software LLC
	Tom Stevelt <tstevelt@silverhammersoftware.com>
	This software is free software; you can redistribute it and/or modify
	it under the terms of the MIT license. See LICENSE for details.
---------------------------------------------------------------------------*/

#include    "nn1hmo.h"

typedef struct
{
	double	lr;
	int		neurons;
	double	score;
	int		index;
} CHROMOSONE;

#define		POP_COUNT		24
static	CHROMOSONE	pop[POP_COUNT];
static	CHROMOSONE	oldpop[POP_COUNT];
static	int		PopCount;

#define		PROB_CROSS		0.99
#define		PROB_MUTATE		0.02

#define		GEN_COUNT		5

static int cmprec ( CHROMOSONE *a, CHROMOSONE *b )
{
	/*----------------------------------------------------------
		reverse sort by score (accuracy)
	----------------------------------------------------------*/
	if ( a->score < b->score )
	{
		return ( 1 );
	}
	if ( a->score > b->score )
	{
		return ( -1 );
	}
	return ( 0 );
}

void select_parents ( int *Parent1, int *Parent2 )
{
	int		r1, r2;

	r1 = r2 = random_range ( 0, PopCount - 1 );
	
	while ( r1 == r2 )
	{
		r2 = random_range ( 0, PopCount - 1 );
	}

	/*----------------------------------------------------------
		unlike golf, higher score is best.
	----------------------------------------------------------*/
	if ( pop[r1].score > pop[r2].score )
	{
		*Parent1 = r1;
	}
	else
	{
		*Parent1 = r2;
	}

	do
	{
		r1 = r2 = random_range ( 0, PopCount - 1 );
		
		while ( r1 == r2 )
		{
			r2 = random_range ( 0, PopCount - 1 );
		}

		if ( pop[r1].score > pop[r2].score )
		{
			*Parent2 = r1;
		}
		else
		{
			*Parent2 = r2;
		}

	} while ( *Parent1 == *Parent2 );
}

static void print_pop ( int g )
{
	printf ( "Generation %d\n", g );

	/*----------------------------------------------------------
		999  9999999999 999999999 9999999999
	----------------------------------------------------------*/
	printf ( "IND  LEARN RATE   NUERONS  ACCURACY\n" );
	
	for ( int p = 0; p < PopCount; p++ )
		printf ( "%3d: %10.4f %8d %10.4f\n", p, pop[p].lr, pop[p].neurons, pop[p].score );
}

static void NoDupeNeuronCount ( int i )
{
	int		NewNeurons, IsDupe;

	do 
	{
		IsDupe = 0;
		NewNeurons =  (int) d_random_range ( (double) OutputCount, (double) InputCount );
		for ( int p = 0; p < PopCount; p++ )
		{
			if ( NewNeurons == pop[p].neurons )
			{
				IsDupe = 1;
				break;
			}
		}
	} while ( IsDupe );

	pop[i].neurons = NewNeurons;
}

static int replace_dupes ()
{
	int		rv = 0;

	qsort ( pop, PopCount, sizeof(CHROMOSONE), (int(*)()) cmprec );

	for ( int p = 0; p < PopCount - 1; p++ )
	{
		if ( (int)(1000*pop[p].lr) != (int)(1000*pop[p+1].lr) )
		{
			continue;
		}
		if ( pop[p].neurons != pop[p+1].neurons )
		{
			continue;
		}

		/*----------------------------------------------------------
			replace this one, because the next two may also be dupes
			just assume that randomness will prevent creation of a dupe.
		----------------------------------------------------------*/
		printf ( " dupe %3d: %10.4f %8d -> ", p, pop[p].lr, pop[p].neurons );

		pop[p].lr      = d_random_range ( 0.001, 0.3 );
		// pop[p].neurons = (int) d_random_range ( (double) OutputCount, (double) InputCount );
		NoDupeNeuronCount ( p );
		pop[p].score   = 0.0;

		printf ( " %10.4f %8d\n", pop[p].lr, pop[p].neurons );

		rv++;
	}
	return ( rv );
}

void algo ()
{
	int		CrossCount, MutateCount, DupeCount;
	int		Seed = SEED_USEC;

	switch ( Seed )
	{
		case SEED_NONE:
			break;
		case SEED_USEC:
			seed_random_with_usec ();
			break;
		case SEED_SAME:
			srand ( 12345 );
			break;
	}

	printf ( "Input Columns %ld  Output Columns %ld\n", InputCount, OutputCount );
	if ( InputCount - OutputCount - 1< POP_COUNT )
	{
		/*----------------------------------------------------------
			-1 'cuz have to leave room for possible mutate Neurons
		----------------------------------------------------------*/
		PopCount = InputCount - OutputCount - 1;
		printf ( "PopCount set to %d\n", PopCount );
	}
	else
	{
		PopCount = POP_COUNT;
	}

	for ( int p = 0; p < PopCount; p++ )
	{
		pop[p].lr      = d_random_range ( 0.001, 0.3 );
		// pop[p].neurons = (int) d_random_range ( (double) OutputCount, (double) InputCount );
		NoDupeNeuronCount ( p );
		pop[p].score   = 0.0;
	}

	print_pop ( 0 );

	EpochCount = 10;

	CrossCount = MutateCount = DupeCount = 0;

	for ( int g = 0; g < GEN_COUNT; g++ )
	{
		printf ( "%d ", g + 1 );
		for ( int p = 0; p < PopCount; p++ )
		{
			printf ( "." );

			LearningRate = pop[p].lr;
			HiddenNeurons[0] = pop[p].neurons;

			pop[p].score = train ( Seed, 0 );

			// free ( InputArray );
			free ( WeightsIH );
			free ( HiddenBias );
			free ( HiddenArray );
			free ( WeightsHO );
			free ( OutputBias );
			free ( OutputArray );
		}
		printf ( "\n" );

		qsort ( pop, PopCount, sizeof(CHROMOSONE), (int(*)()) cmprec );

		memcpy ( oldpop, pop, sizeof(pop) );

//		if ( g <= 1 )
//		{
//		}
			print_pop ( g + 1 );

		/*----------------------------------------------------------
			cross
		----------------------------------------------------------*/
		for ( int p = 0; p < PopCount; p += 2 )
		{
			int p1, p2;

			select_parents ( &p1, &p2 );

			pop[p].lr   = oldpop[p2].lr;
			pop[p+1].lr = oldpop[p1].lr;

			CrossCount++;
		}

		for ( int p = 0; p < PopCount; p++ )
		{
			if ( flip ( PROB_MUTATE ) )
			{
				pop[p].lr = d_random_range ( 0.001, 0.3 );
				NoDupeNeuronCount ( p );
				MutateCount++;
			}
		}

		DupeCount += replace_dupes ();
	}

	print_pop ( GEN_COUNT );

	printf ( "CrossCount %d, MutateCount %d, DupeCount %d\n",  CrossCount, MutateCount, DupeCount );
}

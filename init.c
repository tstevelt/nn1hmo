/*---------------------------------------------------------------------------
	Copyright © 2023 Tom Stevelt and Silver Hammer Software LLC
	Tom Stevelt <tstevelt@silverhammersoftware.com>
	This software is free software; you can redistribute it and/or modify
	it under the terms of the MIT license. See LICENSE for details.
---------------------------------------------------------------------------*/

#include	"nn1hmo.h"

#define		INIT_LOW	-0.5
#define		INIT_HIGH	0.5

void init ( int SeedMode )
{
	int		i, j;
	static int firstpass = 1;

	switch ( SeedMode )
	{
		case SEED_NONE:
			break;
		case SEED_USEC:
			seed_random_with_usec ();
			break;
		case SEED_SAME:
			srand ( 12345 );
			break;
		default:
			printf ( "init: unknown SeedMode %d\n", SeedMode );
	}


	/*----------------------------------------------------------
		allocate memory for all layers and weights
	----------------------------------------------------------*/

	/*----------------------------------------------------------
		input array
	----------------------------------------------------------*/
	if ( firstpass ) 
	{
		if (( InputArray = calloc ( InputCount, sizeof(double) )) == NULL )
		{
			printf ( "Cannot calloc InputArray\n" );
			exit ( 1 );
		}
		firstpass = 0;
	}

	/*----------------------------------------------------------
		2d weights between input and hidden layers
	----------------------------------------------------------*/
	if (( WeightsIH = calloc ( InputCount * HiddenNeurons[0], sizeof(double) )) == NULL )
	{
		printf ( "Cannot calloc WeightsIH\n" );
		exit ( 1 );
	}

	/*----------------------------------------------------------
		hidden bias array
	----------------------------------------------------------*/
	if (( HiddenBias = calloc ( HiddenNeurons[0], sizeof(double) )) == NULL )
	{
		printf ( "Cannot calloc HiddenBias\n" );
		exit ( 1 );
	}

	/*----------------------------------------------------------
		hidden layer array
	----------------------------------------------------------*/
	if (( HiddenArray = calloc ( HiddenNeurons[0], sizeof(double) )) == NULL )
	{
		printf ( "Cannot calloc HiddenArray\n" );
		exit ( 1 );
	}

	/*----------------------------------------------------------
		2d weights between hidden and output layers
	----------------------------------------------------------*/
	if (( WeightsHO = calloc ( HiddenNeurons[0] * OutputCount, sizeof(double) )) == NULL )
	{
		printf ( "Cannot calloc WeightsHO\n" );
		exit ( 1 );
	}
	if ( Debug )
	{
		printf ( "Sizeof *WeightsHO = %ld\n",  HiddenNeurons[0] * OutputCount * sizeof(double) );
	}

	/*----------------------------------------------------------
		output bias array
	----------------------------------------------------------*/
	if (( OutputBias = calloc ( OutputCount, sizeof(double) )) == NULL )
	{
		printf ( "Cannot calloc OutputBias\n" );
		exit ( 1 );
	}

	/*----------------------------------------------------------
		output layer array
	----------------------------------------------------------*/
	if (( OutputArray = calloc ( OutputCount, sizeof(double) )) == NULL )
	{
		printf ( "Cannot calloc OutputArray\n" );
		exit ( 1 );
	}

	/*----------------------------------------------------------
		initialize WeightIH with random numbers
	----------------------------------------------------------*/
	for ( i = 0; i < InputCount; i++ )
	{
		for ( j = 0; j < HiddenNeurons[0]; j++ )
		{
			WeightsIH[INDEX(i,HiddenNeurons[0],j)] = d_random_range ( INIT_LOW, INIT_HIGH );
		}
	}

	/*----------------------------------------------------------
		initialize HiddenBias with random numbers
	----------------------------------------------------------*/
	for ( i = 0; i < HiddenNeurons[0]; i++ )
	{
		HiddenBias[i] = d_random_range ( INIT_LOW, INIT_HIGH );
	}

	/*----------------------------------------------------------
		initialize WeightHO with random numbers
	----------------------------------------------------------*/
	for ( i = 0; i < HiddenNeurons[0]; i++ )
	{
		for ( j = 0; j < OutputCount; j++ )
		{
			WeightsHO[INDEX(i,OutputCount,j)] = d_random_range ( INIT_LOW, INIT_HIGH );
		}
	}

	/*----------------------------------------------------------
		initialize OutputBias with random numbers
	----------------------------------------------------------*/
	for ( i = 0; i < OutputCount; i++ )
	{
		OutputBias[i] = d_random_range ( INIT_LOW, INIT_HIGH );
	}

}

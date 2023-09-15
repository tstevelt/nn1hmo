/*---------------------------------------------------------------------------
	Copyright © 2023 Tom Stevelt and Silver Hammer Software LLC
	Tom Stevelt <tstevelt@silverhammersoftware.com>
	This software is free software; you can redistribute it and/or modify
	it under the terms of the MIT license. See LICENSE for details.
---------------------------------------------------------------------------*/

#include	"nn1hmo.h"

void savemodel ()
{
	FILE	*fp;

	if (( fp = fopen ( ModelFilename, "w" )) == NULL )
	{
		printf ( "Cannot create model file %s\n", ModelFilename );
		exit ( 1 );
	}

	if ( Debug )
	{
		printf ( "sizeof double %ld\n", sizeof(double) );
	}

	/*----------------------------------------------------------
		bias ... weights ... array
	----------------------------------------------------------*/
	fwrite ( &LearningRate, sizeof(double), 1, fp );
	fwrite ( &InputCount, sizeof(long), 1, fp );
	fwrite ( &OutputCount, sizeof(long), 1, fp );
	fwrite ( &HiddenNeurons[0], sizeof(long), 1, fp );

	fwrite ( HiddenBias,    sizeof(double), HiddenNeurons[0], fp );
	fwrite ( WeightsIH,     sizeof(double), InputCount * HiddenNeurons[0], fp );
	fwrite ( HiddenArray,   sizeof(double), HiddenNeurons[0], fp );

	fwrite ( OutputBias,    sizeof(double), OutputCount, fp );
	fwrite ( WeightsHO,     sizeof(double), HiddenNeurons[0] * OutputCount, fp );

	fclose ( fp );
}

void loadmodel ()
{
	FILE	*fp;

	if (( InputArray = calloc ( InputCount, sizeof(double) )) == NULL )
	{
		printf ( "test: Could not calloc InputArray\n" );
		exit ( 1 );
	}

	if (( OutputArray = calloc ( OutputCount, sizeof(double) )) == NULL )
	{
		printf ( "test: Could not calloc OutputArray\n" );
		exit ( 1 );
	}

	if (( fp = fopen ( ModelFilename, "r" )) == NULL )
	{
		printf ( "Cannot open model file %s\n", ModelFilename );
		exit ( 1 );
	}

	fread ( &LearningRate, sizeof(double), 1, fp );
	fread ( &InputCount, sizeof(long), 1, fp );
	fread ( &OutputCount, sizeof(long), 1, fp );
	fread ( &HiddenNeurons[0], sizeof(long), 1, fp );

	printf ( "%ld %ld %ld\n", InputCount, OutputCount, HiddenNeurons[0] );

	if (( HiddenBias = calloc ( HiddenNeurons[0], sizeof(double) )) == NULL )
	{
		printf ( "loadmodel: Could not calloc HiddenBias\n" );
		exit ( 1 );
	}
	if (( WeightsIH = calloc ( InputCount * HiddenNeurons[0], sizeof(double) )) == NULL )
	{
		printf ( "loadmodel: Could not calloc WeightsIH\n" );
		exit ( 1 );
	}
	if (( HiddenArray = calloc ( HiddenNeurons[0], sizeof(double) )) == NULL )
	{
		printf ( "loadmodel: Could not calloc HiddenArray\n" );
		exit ( 1 );
	}
	if (( OutputBias = calloc ( OutputCount, sizeof(double) )) == NULL )
	{
		printf ( "loadmodel: Could not calloc OutputBias\n" );
		exit ( 1 );
	}
	if (( WeightsHO = calloc ( HiddenNeurons[0] * OutputCount, sizeof(double) )) == NULL )
	{
		printf ( "loadmodel: Could not calloc WeightsHO\n" );
		exit ( 1 );
	}

	fread ( HiddenBias,    sizeof(double), HiddenNeurons[0], fp );
	fread ( WeightsIH,     sizeof(double), InputCount * HiddenNeurons[0], fp );
	fread ( HiddenArray,   sizeof(double), HiddenNeurons[0], fp );

	fread ( OutputBias,    sizeof(double), OutputCount, fp );
	fread ( WeightsHO,     sizeof(double), HiddenNeurons[0] * OutputCount, fp );

	fclose ( fp );
}

void  WriteModelAndExit ( int Signal )
{
	printf ( "\n\n**** WriteModelAndExit: signal %d ****\n\n", Signal );

	savemodel ();

	time ( &EndTime );

	long TotalTime = EndTime - StartTime;
	int	Seconds = TotalTime % 60;
	int	Minutes = TotalTime / 60;

	printf ( "Time %d minutes and %d seconds\n",  Minutes, Seconds );
	fflush ( stdout );

	exit ( 1 );
}

void dump ()
{
	int		w, i, j;

	loadmodel ();

	printf ( "Learning rate %g\n", LearningRate );

	printf ( "\n\nHiddenBias\n" );
	for ( i = 0, w = 0; i < HiddenNeurons[0]; i++ )
	{
		w += printf ( " %10.4f", HiddenBias[i] );

		if ( w + 11 >= WindowWidth )
		{
			printf ( "\n" );
			w = 0;
		}
	}

	printf ( "\n\nWeightsIH\n" );
	for ( i = 0, w = 0; i < InputCount; i++ )
	{
		w = printf ( "%2d:", i );

		for ( j = 0; j < HiddenNeurons[0]; j++ )
		{
			w += printf ( " %10.4f", WeightsIH[INDEX(i,HiddenNeurons[0],j)] );

			if ( w + 11 >= WindowWidth )
			{
				printf ( "\n" );
				w = 0;
			}
		}
		printf ( "\n" );
		w = 0;
	}

	printf ( "\n\nOutputBias\n" );
	for ( i = 0, w = 0; i < OutputCount; i++ )
	{
		w += printf ( " %10.4f", OutputBias[i] );

		if ( w + 11 >= WindowWidth )
		{
			printf ( "\n" );
			w = 0;
		}
	}

	printf ( "\n\nWeightsHO\n" );
	for ( i = 0, w = 0; i < HiddenNeurons[0]; i++ )
	{
		w = printf ( "%2d:", i );

		for ( j = 0; j < OutputCount; j++ )
		{
			w += printf ( " %10.4f", WeightsHO[INDEX(i,OutputCount,j)] );

			if ( w + 11 >= WindowWidth )
			{
				printf ( "\n" );
				w = 0;
			}
		}

		// if ( w + 11 >= WindowWidth )
		{
			printf ( "\n" );
			w = 0;
		}
	}
}

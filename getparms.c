/*---------------------------------------------------------------------------
	Copyright © 2023 Tom Stevelt and Silver Hammer Software LLC
	Tom Stevelt <tstevelt@silverhammersoftware.com>
	This software is free software; you can redistribute it and/or modify
	it under the terms of the MIT license. See LICENSE for details.
---------------------------------------------------------------------------*/

#include	"nn1hmo.h"

void getparms ( char *ParmFile )
{
	FILE	*pfp;
	char	xbuffer[1024];
#define		MAXPARM		10
	char	*tokens[MAXPARM];
	int		tokcnt, xt;

	if (( pfp = fopen ( ParmFile, "r" )) == NULL )
	{
		printf ( "Cannot open parameter file %s\n", ParmFile );
		exit ( 1 );
	}

	while ( fgets ( xbuffer, sizeof(xbuffer), pfp ) != NULL )
	{
		if ( xbuffer[0] == '#' )
		{
			continue;
		}

		if (( tokcnt = GetTokensW ( xbuffer, tokens, MAXPARM )) < 2 )
		{
			continue;
		}

		if ( strcmp ( tokens[0], "mode" ) == 0 )
		{
			if ( strcmp ( tokens[1], "algo" ) == 0 )
			{
				RunMode = MODE_ALGO;
			}
			else if ( strcmp ( tokens[1], "train" ) == 0 )
			{
				RunMode = MODE_TRAIN;
			}
			else if ( strcmp ( tokens[1], "test" ) == 0 )
			{
				RunMode = MODE_TEST;
			}
			else if ( strcmp ( tokens[1], "predict" ) == 0 )
			{
				RunMode = MODE_PREDICT;
			}
			else
			{
				printf ( "Unknown mode\n" );
				exit ( 1 );
			}
		}
		else if ( strcmp ( tokens[0], "datafile" ) == 0 )
		{
			strcpy ( DataFilename, tokens[1] );
		}
		else if ( strcmp ( tokens[0], "model" ) == 0 )
		{
			strcpy ( ModelFilename, tokens[1] );
		}
		else if ( strcmp ( tokens[0], "inputs" ) == 0 )
		{
			InputCount = atoi ( tokens[1] );
		}
		else if ( strcmp ( tokens[0], "outputs" ) == 0 )
		{
			OutputCount = atoi ( tokens[1] );
		}
		else if ( strcmp ( tokens[0], "normalize" ) == 0 )
		{
			Normalize = tokens[1][0];
			switch( Normalize )
			{
				case 'n':
				case 'r':
				case 'c':
				case 'a':
					break;
				default:
					printf ( "normalize requires n (none) r (row wise) or c (column wise) a (all)\n" );
					exit ( 1 );
			}
		}
		else if ( strcmp ( tokens[0], "id" ) == 0 )
		{
			switch( tokens[1][0] )
			{
				case 'n':
				case 'N':
				case '0':
					HasID = 0;
					break;

				case 'y':
				case 'Y':
				case '1':
					HasID = 1;
					break;

				default:
					printf ( "id should be either 'y' or 'n' \n" );
					exit ( 1 );
			}
		}
		else if ( strcmp ( tokens[0], "epochs" ) == 0 )
		{
			EpochCount = atoi ( tokens[1] );
		}
		else if ( strcmp ( tokens[0], "accuracy" ) == 0 )
		{
			TargetAccuracy = atof ( tokens[1] );
		}
		else if ( strcmp ( tokens[0], "learnrate" ) == 0 )
		{
			LearningRate = atof ( tokens[1] );
		}
		else if ( strcmp ( tokens[0], "layers" ) == 0 )
		{
			HiddenLayers = atoi ( tokens[1] );

			if (  HiddenLayers > 1 )
			{
				printf ( "getparms: nn1hmo currently only supports one layer\n" );
				exit ( 1 );
			}

			if ( tokcnt < 2 + HiddenLayers )
			{
				printf ( "layers syntax error\n" );
				exit ( 1 );
			}
			for ( xt = 2; xt < 2 + HiddenLayers; xt++ )
			{
				HiddenNeurons[xt-2] = atoi(tokens[xt]);
			}
		}
		else if ( strcmp ( tokens[0], "split" ) == 0 )
		{
			SplitTrain = atof ( tokens[1] );
			if ( xt > 2 )
			{
				SplitTest = atof ( tokens[2] );
			}
			else
			{
				SplitTest  = 1.0 - SplitTrain;
			}
		}
		else
		{
			printf ( "Unknown parameter %s\n", tokens[0] );
		}
	}

	fclose ( pfp );
}

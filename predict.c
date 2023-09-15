/*---------------------------------------------------------------------------
	Copyright © 2023 Tom Stevelt and Silver Hammer Software LLC
	Tom Stevelt <tstevelt@silverhammersoftware.com>
	This software is free software; you can redistribute it and/or modify
	it under the terms of the MIT license. See LICENSE for details.
---------------------------------------------------------------------------*/

#include	"nn1hmo.h"

void predict ()
{
	FILE	*fp;
	char	xbuffer[10240];
	char	*tokens[InputCount+2];
	int		tokcnt, xt, lineno;
	char	*Indentifier;
	double	Prediction = 0.0;
	int		TrueCount, FalseCount;

	loadmodel ();

	if (( fp = fopen ( DataFilename, "r" )) == NULL )
	{
		printf ( "Cannot open predict file %s\n", DataFilename );
		exit ( 1 );
	}

	TrueCount = FalseCount = 0;
	lineno = 0;
	while ( fgets ( xbuffer, sizeof(xbuffer), fp ) != NULL )
	{
		lineno++;
				
		if (( tokcnt = GetTokensD ( xbuffer, ",\n\r", tokens, InputCount + 2 )) < InputCount+1 )
		{
			printf ( "syntax error line %d, tokcnt %d\n", lineno, tokcnt );
			continue;
		}

		Indentifier = tokens[0];
		for ( xt = 0; xt < InputCount; xt++ )
		{
			InputArray[xt] = atof ( tokens[xt+1] );
		}

		if ( Normalize == 'r' )
		{
			MinMaxScale ( InputArray, 0, InputCount );
		}
		else if ( Normalize == 'c' )
		{
			printf ( "predict: fixit normalize columns\n" );
			exit ( 1 );
		}

		/*----------------------------------------------------------
			compute hidden layer activation
		----------------------------------------------------------*/
		for ( int j = 0; j < HiddenNeurons[0]; j++ )
		{
			double	activation = HiddenBias[j];

			for ( int i = 0; i < InputCount; i++ )
			{
				activation += InputArray[i] * WeightsIH[INDEX(i,HiddenNeurons[0],j)];
			}
			HiddenArray[j] = sigmoid ( activation );
		}

		/*----------------------------------------------------------
			compute output layer activation
		----------------------------------------------------------*/
		for ( int j = 0; j < OutputCount; j++ )
		{
			double	activation = OutputBias[j];

			for ( int i = 0; i < HiddenNeurons[0]; i++ )
			{
				activation += HiddenArray[i] * WeightsHO[INDEX(i,OutputCount,j)];
			}
			OutputArray[j] = sigmoid ( activation );
		}

		if ( OutputCount == 1 )
		{
			Prediction = OutputArray[0];
		
			if ( Prediction > 0.5 )
			{
				TrueCount++;
			}
			else
			{
				FalseCount++;
			}

			printf ( "%s %.4f %s\n", Indentifier, Prediction, Prediction > 0.5 ? "TRUE" : "" );
		}
		else
		{
			/*----------------------------------------------------------
				call softmax, returns index of the max value
			----------------------------------------------------------*/
#ifdef USE_SOFTMAX
			int MaxIndex = softmax ( OutputArray, OutputCount );
#else
			int MaxIndex = findmax ( OutputArray, OutputCount );
#endif
			printf ( "%s %d\n", Indentifier, MaxIndex );
		}
	}

	fclose ( fp );

//	printf ( "TRUE %d, %.2f PERCENT OF CURRENT\n",
//		TrueCount, 100.0 * TrueCount / (double) (TrueCount + FalseCount) );
}

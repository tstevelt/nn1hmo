/*---------------------------------------------------------------------------
	Copyright © 2023 Tom Stevelt and Silver Hammer Software LLC
	Tom Stevelt <tstevelt@silverhammersoftware.com>
	This software is free software; you can redistribute it and/or modify
	it under the terms of the MIT license. See LICENSE for details.
---------------------------------------------------------------------------*/

#include	"nn1hmo.h"

void test ()
{
	FILE	*fp;
	char	xbuffer[10240];
	char	*tokens[InputCount+OutputCount+2];
	int		tokcnt, xt, lineno;
	double	Prediction = 0.0;
	int		TrueCount, FalseCount;
	int		InputLabel[InputCount];

	loadmodel ();

	if (( fp = fopen ( DataFilename, "r" )) == NULL )
	{
		printf ( "Cannot open test file %s\n", DataFilename );
		exit ( 1 );
	}

	/*----------------------------------------------------------
		count number of input lines
	----------------------------------------------------------*/
	int TotalLines = 0;
	while ( fgets ( xbuffer, sizeof(xbuffer), fp ) != NULL )
	{
		TotalLines++;
	}
	int TrainLines = TotalLines * SplitTrain;
	int TestLines = TotalLines * SplitTest;
	rewind ( fp );

	TrueCount = FalseCount = 0;
	lineno = 0;
	while ( fgets ( xbuffer, sizeof(xbuffer), fp ) != NULL )
	{
		lineno++;
		if ( lineno <= TrainLines )
		{
			continue;
		}
				
		if (( tokcnt = GetTokensD ( xbuffer, ",\n\r", tokens, InputCount + OutputCount + 2 )) < InputCount+OutputCount )
		{
			printf ( "syntax error line %d, tokcnt %d\n", lineno, tokcnt );
			continue;
		}

		for ( xt = 0; xt < InputCount; xt++ )
		{
			InputArray[xt] = atof ( tokens[xt] );
		}

		if ( Normalize == 'r' )
		{
			MinMaxScale ( InputArray, 0, InputCount );
		}
//		else if ( Normalize == 'c' )
//		{
//			printf ( "test: fixit normalize columns\n" );
//			exit ( 1 );
//		}

		int		OneCount = 0;
		for ( int xo = 0; xt < InputCount + OutputCount; xt++, xo++ )
		{
			InputLabel[xo] = atoi ( tokens[xt] );
			if ( InputLabel[xo] == 1 )
			{
				OneCount++;
			}
		}
		if ( OutputCount > 1 && OneCount != 1 )
		{
			printf ( "One Count = %d in line %d\n", OneCount, lineno );
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
		
			if ( InputLabel[0] == 1 && Prediction > 0.5 )
			{
				TrueCount++;
			}
			else if ( InputLabel[0] == 0 && Prediction < 0.5 )
			{
				TrueCount++;
			}
			else
			{
				FalseCount++;
			}
			
			if ( Debug )
			{
				printf ( "%d %d %.4f\n", lineno, InputLabel[0], Prediction );
			}
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
			/*----------------------------------------------------------
				statistics: hit/miss and softmax sharpness ratio.
			----------------------------------------------------------*/
			if ( InputLabel[MaxIndex] == 1 )
			{
				TrueCount++;
			}
			else
			{
				FalseCount++;
			}
		}
	}

	fclose ( fp );

	if ( TrueCount + FalseCount != TestLines )
	{
		printf ( "INFO: SplitTest %.2f, TestLines %d, count %d\n", SplitTest, TestLines, TrueCount + FalseCount );
	}

	printf ( "MATCH %d, NO MATCH %d, ACCURACY %.2f\n",
		TrueCount, 
		FalseCount,
		100.0 * TrueCount / (double) (TrueCount + FalseCount) );
}

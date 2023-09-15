/*---------------------------------------------------------------------------
	Copyright © 2023 Tom Stevelt and Silver Hammer Software LLC
	Tom Stevelt <tstevelt@silverhammersoftware.com>
	This software is free software; you can redistribute it and/or modify
	it under the terms of the MIT license. See LICENSE for details.
---------------------------------------------------------------------------*/

#include	"nn1hmo.h"

typedef struct
{
	double	Data[MAXINPUTS];
	double	Label[MAXOUTPUTS];
} DATA_RECORD;

static	DATA_RECORD	DataArray[MAXROWS];
static	int			DataCount;

typedef struct
{
	int		Index;
	double	Rank;
} SHUFFLE_RECORD;

static	SHUFFLE_RECORD	ShuffleArray[MAXROWS];

static void LoadData ()
{
	FILE	*fp;
	char	xbuffer[10240];
	int		lineno;
	char	*tokens[InputCount+OutputCount+2];
	int		tokcnt, xt;
	int		TrueCount, FalseCount;

	if (( fp = fopen ( DataFilename, "r" )) == NULL )
	{
		printf ( "Cannot open training file %s\n", DataFilename );
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
	rewind ( fp );

	/*----------------------------------------------------------
		load SplitTrain percent of lines into DataArray
	----------------------------------------------------------*/
	TrueCount = FalseCount = 0;
	lineno = DataCount = 0;
	while ( fgets ( xbuffer, sizeof(xbuffer), fp ) != NULL )
	{
		lineno++;

		if ( lineno > TrainLines )
		{
			break;
		}
				
		if (( tokcnt = GetTokensD ( xbuffer, ",\n\r", tokens, InputCount + OutputCount + 2 )) < InputCount+OutputCount )
		{
			printf ( "syntax error line %d\n", lineno );
			continue;
		}

		if ( DataCount >= MAXROWS )
		{
			printf ( "Exceeds MAXROWS\n" );
			exit ( 1 );
		}

		for ( xt = 0; xt < InputCount; xt++ )
		{
			DataArray[DataCount].Data[xt] = atof ( tokens[xt] );
		}

		if ( Normalize == 'r' )
		{
			MinMaxScale ( DataArray[DataCount].Data, 0, InputCount );
		}

		if ( OutputCount == 1 )
		{
			DataArray[DataCount].Label[0] = atoi ( tokens[xt] );
			if ( DataArray[DataCount].Label[0] == 1 )
			{
				TrueCount++;
			}
			else
			{
				FalseCount++;
			}
		}
		else
		{
			int		OneCount = 0;
			for ( int xo = 0; xt < InputCount + OutputCount; xt++, xo++ )
			{
				DataArray[DataCount].Label[xo] = atoi ( tokens[xt] );
				if ( DataArray[DataCount].Label[xo] == 1 )
				{
					OneCount++;
				}
			}
			if ( OneCount != 1 )
			{
				printf ( "One Count = %d in line %d\n", OneCount, lineno );
			}
		}

		ShuffleArray[DataCount].Index = DataCount;
		DataCount++;
	}

	if ( Normalize == 'c' )
	{
		printf ( "\nColumn wise normalizing ... " );
		int nc = 0;
		for ( int i = 0; i < InputCount; i++ )
		{
			double	Min, Max;
			Min = Max = DataArray[0].Data[i];
			for ( int r = 1; r < DataCount; r++ )
			{
				if ( Min > DataArray[r].Data[i] )
				{
					Min = DataArray[r].Data[i];
				}
				if ( Max < DataArray[r].Data[i] )
				{
					Max = DataArray[r].Data[i];
				}
			}
			for ( int r = 0; r < DataCount; r++ )
			{
				DataArray[r].Data[i] = ( DataArray[r].Data[i] - Min ) / ( Max - Min );
				nc++;
			}
		}
		printf ( "%d values\n", nc );
	}
	else if ( Normalize == 'a' )
	{
		printf ( "\nNormalizing all of columns ... " );
		int nc = 0;
		double	Min, Max;
		Min = Max = DataArray[0].Data[0];
		for ( int i = 0; i < InputCount; i++ )
		{
			for ( int r = 0; r < DataCount; r++ )
			{
				if ( Min > DataArray[r].Data[i] )
				{
					Min = DataArray[r].Data[i];
				}
				if ( Max < DataArray[r].Data[i] )
				{
					Max = DataArray[r].Data[i];
				}
			}
		}
		for ( int i = 0; i < InputCount; i++ )
		{
			for ( int r = 0; r < DataCount; r++ )
			{
				DataArray[r].Data[i] = ( DataArray[r].Data[i] - Min ) / ( Max - Min );
				nc++;
			}
		}
		printf ( "%d values\n", nc );
	}

	if ( OutputCount == 1 )
	{
		printf ( "Loaded %d records, %d true, %d false\n", DataCount, TrueCount, FalseCount );
	}
	else
	{
		printf ( "Loaded %d records\n", DataCount );
	}
}

static int cmprec ( SHUFFLE_RECORD *a, SHUFFLE_RECORD *b )
{
	if ( a->Rank < b->Rank )
	{
		return ( -1 );
	}

	return ( 1 );
}

static void Shuffle ()
{
	for ( int i = 0; i < DataCount; i++ )
	{
		ShuffleArray[i].Rank = d_random ();
	}
	qsort ( ShuffleArray, DataCount, sizeof(SHUFFLE_RECORD), (int(*)()) cmprec );
}

double train ( int SeedMode, int ProgressMode )
{
	int		TruePos, TrueNeg, FalsePos, FalseNeg;
	int		HitCount, MissCount;
	double	EpochStart, EpochEnd, Elapsed;
	int		ndx;
	int		Consecutive = 0;
	double	HitMaxSoftmax, HitOtherSoftmax, MissMaxSoftmax, MissOtherSoftmax;
	double	Accuracy;
	static	int	firstpass = 1;

	if ( firstpass )
	{
		LoadData ();
		firstpass = 0;
	}

	/*----------------------------------------------------------
		initialize learning rate, bias arrays and weight arrays.
	----------------------------------------------------------*/
	init ( SeedMode );
	
	for ( int epoch = 0; epoch < EpochCount; epoch++ )
	{
		EpochStart = CurrentSecondOfDay ();
		if ( Debug )
		{
			printf ( "Epoch: %d\n", epoch );
		}
		TruePos = TrueNeg = FalsePos = FalseNeg = 0;
		HitCount = MissCount = 0;
		HitMaxSoftmax = HitOtherSoftmax = MissMaxSoftmax = MissOtherSoftmax = 0.0;

		Shuffle ();

		for ( int row = 0; row < DataCount; row++ )
		{
			ndx = ShuffleArray[row].Index;

			/*----------------------------------------------------------
				compute hidden layer activation
			----------------------------------------------------------*/
			for ( int j = 0; j < HiddenNeurons[0]; j++ )
			{
				double	activation = HiddenBias[j];

				for ( int i = 0; i < InputCount; i++ )
				{
					activation += DataArray[ndx].Data[i] * WeightsIH[INDEX(i,HiddenNeurons[0],j)];
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

				if ( isnan(OutputArray[j]) )
				{
					printf ( "OutputArray[%d] is not a number in row %d. activation is %g\n", j, row, activation );
				}

				/*----------------------------------------------------------
					track performance
				----------------------------------------------------------*/
				if ( OutputCount == 1 )
				{
					if ( DataArray[ndx].Label[0] == 1 && OutputArray[j] < 0.5 )
					{
						FalseNeg++;
					}
					else if ( DataArray[ndx].Label[0] == 1 && OutputArray[j] > 0.5 )
					{
						TruePos++;
					}
					else if ( DataArray[ndx].Label[0] == 0 && OutputArray[j] < 0.5 )
					{
						TrueNeg++;
					}
					else if ( DataArray[ndx].Label[0] == 0 && OutputArray[j] > 0.5 )
					{
						FalsePos++;
					}
				}
			}

			/*----------------------------------------------------------
				use softmax for multiple output
			----------------------------------------------------------*/
			if ( OutputCount > 1 )
			{
				char	*cp;

				if ( Debug )
				{
					printf ( "BEFORE " );
					for ( int j = 0; j < OutputCount; j++ )
					{
						printf ( " %6.3f", OutputArray[j] );
					}
					printf ( "\n" );
				}

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
				if ( DataArray[ndx].Label[MaxIndex] == 1 )
				{
					HitCount++;
					cp = "hit";

					for ( int i = 0; i < OutputCount; i++ )
					{
						if ( i == MaxIndex )
						{
							HitMaxSoftmax += OutputArray[i];
						}
						else
						{
							HitOtherSoftmax += OutputArray[i];
						}
					}
				}
				else
				{
					MissCount++;
					cp = "miss";

					for ( int i = 0; i < OutputCount; i++ )
					{
						if ( i == MaxIndex )
						{
							MissMaxSoftmax += OutputArray[i];
						}
						else
						{
							MissOtherSoftmax += OutputArray[i];
						}
					}
				}

				if ( Debug )
				{
					printf ( "OUTPUT " );
					for ( int j = 0; j < OutputCount; j++ )
					{
						printf ( " %6.3f", OutputArray[j] );
					}
					printf ( "\n" );

					printf ( "LABEL  " );
					for ( int j = 0; j < OutputCount; j++ )
					{
						printf ( " %6.3f", DataArray[ndx].Label[j] );
					}
					printf ( "\n" );

					int LabelIndex = findmax ( DataArray[ndx].Label, OutputCount );
					int OutputIndex = findmax (  OutputArray, OutputCount );
					printf ( "softmax %d output %d label %d %s\n", MaxIndex, OutputIndex, LabelIndex, cp );
				}
			}

			/*----------------------------------------------------------
				Back propogate
			----------------------------------------------------------*/

			/*----------------------------------------------------------
				Compute change in output weights
			----------------------------------------------------------*/
			double	deltaOutput[OutputCount];

			for ( int j = 0; j < OutputCount; j++ )
			{
				double	error = DataArray[ndx].Label[j] - OutputArray[j];

				deltaOutput[j] = error * dSigmoid(OutputArray[j]);
			}

			/*----------------------------------------------------------
				Compute changes in hidden weights
			----------------------------------------------------------*/
			double	deltaHidden[HiddenNeurons[0]];
			for ( int j = 0; j < HiddenNeurons[0]; j++ )
			{
				double	error = 0.0;
				for ( int k = 0; k < OutputCount; k++ )
				{
					error += deltaOutput[k] * WeightsHO[INDEX(j,OutputCount,k)];
				}
				deltaHidden[j] = error * dSigmoid(HiddenArray[j]);
			}

			/*----------------------------------------------------------
				Apply change in output weights
			----------------------------------------------------------*/
			for ( int j = 0; j < OutputCount; j++ )
			{
				OutputBias[j] += deltaOutput[j] * LearningRate;
				for ( int k = 0; k < HiddenNeurons[0]; k++ )
				{
					WeightsHO[INDEX(k,OutputCount,j)] += HiddenArray[k] * deltaOutput[j] * LearningRate;
				}
			}

			/*----------------------------------------------------------
				Apply change in hidden weights
			----------------------------------------------------------*/
			for ( int j = 0; j < HiddenNeurons[0]; j++ )
			{
				HiddenBias[j] += deltaHidden[j] * LearningRate;
				for ( int k = 0; k < InputCount; k++ )
				{
					WeightsIH[INDEX(k,HiddenNeurons[0],j)] += DataArray[ndx].Data[k] * deltaHidden[j] * LearningRate;
				}
			}
		}

		/*----------------------------------------------------------
			report performance
		----------------------------------------------------------*/
		EpochEnd = CurrentSecondOfDay ();
		Elapsed = EpochEnd - EpochStart;

		if ( OutputCount == 1 )
		{
			Accuracy = (double) (TruePos + TrueNeg) / (double) (TruePos + TrueNeg + FalsePos + FalseNeg);
			if ( ProgressMode > 0 )
			{
				printf ( "Epoch %d: %.3f seconds, True Positive %d, True Negative %d, False Positive %d, False Negative %d, Accuracy %g\n",
						epoch, Elapsed, TruePos, TrueNeg, FalsePos, FalseNeg, Accuracy );
			}
		}
		else
		{
			Accuracy = (double) (HitCount) / (double) (HitCount + MissCount);

			double Average = HitOtherSoftmax / (double) (OutputCount - 1);
			double HitSharp = HitMaxSoftmax / Average;

				   Average = MissOtherSoftmax / (double) (OutputCount - 1);
			double MissSharp = MissMaxSoftmax / Average;

			if ( ProgressMode > 0 )
			{
				printf ( "Epoch %d: %.3f seconds, HitCount %d, MissCount %d, Accuracy %g, HitSharp %g. MissSharp %g\n", 
						epoch, Elapsed, HitCount, MissCount, Accuracy, HitSharp, MissSharp );
			}
		}

		fflush ( stdout );

		if ( Accuracy > TargetAccuracy )
		{
			Consecutive++;
			if ( Consecutive >= CONSECUTIVE_ACCURACY )
			{
				printf ( "Reached %d consecutive accuracy target, stopping.\n", CONSECUTIVE_ACCURACY );
				break;
			}
		}
		else
		{
			Consecutive = 0;
		}
	}

	if ( RunMode == MODE_TRAIN )
	{
		savemodel ();
	}

	return ( Accuracy );

}

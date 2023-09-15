/*----------------------------------------------------------------------------
	Program : nn1hmo.c
	Author  : Silver Hammer Software LLC
	Author  : Tom Stevelt
	Date    : Aug 2023
	Synopsis: Neural Network with 1 Hidden and Multiple Outputs.
	Return  : 

	Copyright © 2023 Tom Stevelt and Silver Hammer Software LLC
	Tom Stevelt <tstevelt@silverhammersoftware.com>
	This software is free software; you can redistribute it and/or modify
	it under the terms of the MIT license. See LICENSE for details.

	Who		Date		Modification
	---------------------------------------------------------------------
	tms		08/31/2023	Copied from nn1h (1 hidden and 1 output)

----------------------------------------------------------------------------*/

#define		MAIN

#include	"nn1hmo.h"

int main ( int argc, char *argv[] )
{
	time ( &StartTime );

	getargs ( argc, argv );

	switch ( RunMode )
	{
		case MODE_TRAIN:
			StartSignals ();
			printf ( "training on  %s\n", DataFilename );
			printf ( "split ratio  %.2f/%.2f\n", SplitTrain, SplitTest );
			printf ( "input  count %ld\n", InputCount );
			for ( int i = 0; i < HiddenLayers; i++ )
			{
				printf ( "hidden layer %d has %ld neurons\n", i + 1, HiddenNeurons[i] );
			}
			printf ( "output count %ld\n", OutputCount );
			printf ( "epoch  count %d\n", EpochCount );
			printf ( "target accu  %g\n", TargetAccuracy );
			printf ( "learn rate   %g\n", LearningRate );
			train ( SEED_NONE, 1 );
			break;
		case MODE_ALGO:
			algo ();
			break;
		case MODE_TEST:
			printf ( "testing on  %s\n", DataFilename );
			printf ( "split ratio %.2f/%.2f\n", SplitTrain, SplitTest );
			test ();
			break;
		case MODE_PREDICT:
			predict ();
			break;
		case MODE_DUMP:
			dump ();
			break;
		case MODE_FOO:
			foo ();
			break;
	}

	time ( &EndTime );

	TotalTime = EndTime - StartTime;
	Seconds = TotalTime % 60;
	Minutes = TotalTime / 60;

	printf ( "Time %ld minutes and %ld seconds\n",  Minutes, Seconds );

	return ( 0 );
}

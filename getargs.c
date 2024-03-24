/*---------------------------------------------------------------------------
	Copyright © 2023 Tom Stevelt and Silver Hammer Software LLC
	Tom Stevelt <tstevelt@silverhammersoftware.com>
	This software is free software; you can redistribute it and/or modify
	it under the terms of the MIT license. See LICENSE for details.
---------------------------------------------------------------------------*/

#include	"nn1hmo.h"

static void Usage ()
{
	printf ( "USAGE: nn1hmo [options] -p file [options]\n" );
	printf ( "USAGE: nn1hmo {-train|-test|-predict|-learn|-algo}\n" );
	printf ( "USAGE: nn1hmo  -dump model [-fmt|-raw]\n" );
	printf ( "USAGE: nn1hmo  -foo\n" );
	printf ( "options:\n" );
	printf ( " -f file  data file\n" );
	printf ( " -g file  generate default parameter file\n" );
	printf ( " -m file  model file\n" );
	printf ( " -id      data file first column is ID\n" );
	printf ( " -e #     epochs (for -train), default %d\n", DEFAULT_EPOCHS );
	printf ( " -a #.#   target accuracy, default is 0.99\n" );
	printf ( " -w #     window width (for -dump), default 132\n" );
	printf ( " -split x split file y or n.  n ignores setting in parm file\n" );
	printf ( " -n flag  normalize data. flag n/r/c/a\n" );
	printf ( " -d       debug\n" );
	printf ( "Note that position of parameter file controls overrides\n" );
	exit ( 1 );
}

void getargs ( int argc, char *argv[] )
{
	int		xa;

	RunMode = 0;

	EpochCount = DEFAULT_EPOCHS;
	strcpy ( ModelFilename, DEFAULT_MODEL_FILE );
	LearningRate = DEFAULT_LEARNING_RATE;
	TargetAccuracy = DEFAULT_TARGET_ACCURACY;
	SplitTrain = 0.8;
	SplitTest  = 0.2;
	Normalize = 0;
	HasID = 0;

	WindowWidth = 132;

	for ( xa = 1; xa < argc; xa++ )
	{
		if ( xa + 1 < argc && strcmp ( argv[xa], "-p" ) == 0 )
		{
			xa++;
			getparms ( argv[xa] );
		}
		else if ( xa + 1 < argc && strcmp ( argv[xa], "-g" ) == 0 )
		{
			xa++;
			genparms ( argv[xa] );
		}
		else if ( strcmp ( argv[xa], "-train" ) == 0 )
		{
			RunMode = MODE_TRAIN;
		}
		else if ( strcmp ( argv[xa], "-algo" ) == 0 )
		{
			RunMode = MODE_ALGO;
		}
		else if ( strcmp ( argv[xa], "-test" ) == 0 )
		{
			RunMode = MODE_TEST;
		}
		else if ( strcmp ( argv[xa], "-predict" ) == 0 )
		{
			RunMode = MODE_PREDICT;
		}
		else if ( strcmp ( argv[xa], "-learn" ) == 0 )
		{
			RunMode = MODE_LEARN;
		}
		else if ( xa + 1 < argc && strcmp ( argv[xa], "-dump" ) == 0 )
		{
			RunMode = MODE_DUMP_FMT;
			xa++;
			strcpy ( ModelFilename, argv[xa] );
		}
		else if ( RunMode == MODE_DUMP_FMT && strcmp ( argv[xa], "-fmt" ) == 0 )
		{
			RunMode = MODE_DUMP_FMT;
		}
		else if ( RunMode == MODE_DUMP_FMT && strcmp ( argv[xa], "-raw" ) == 0 )
		{
			RunMode = MODE_DUMP_RAW;
		}
		else if ( strcmp ( argv[xa], "-foo" ) == 0 )
		{
			RunMode = MODE_FOO;
		}
		else if ( xa + 1 < argc && strcmp ( argv[xa], "-f" ) == 0 )
		{
			xa++;
			strcpy ( DataFilename, argv[xa] );
		}
		else if ( xa + 1 < argc && strcmp ( argv[xa], "-m" ) == 0 )
		{
			xa++;
			strcpy ( ModelFilename, argv[xa] );
		}
		else if ( xa + 1 < argc && strcmp ( argv[xa], "-e" ) == 0 )
		{
			xa++;
			EpochCount = atoi ( argv[xa] );
		}
		else if ( xa + 1 < argc && strcmp ( argv[xa], "-a" ) == 0 )
		{
			xa++;
			TargetAccuracy =  atof ( argv[xa] );
		}
		else if ( xa + 1 < argc && strcmp ( argv[xa], "-w" ) == 0 )
		{
			xa++;
			WindowWidth = atoi ( argv[xa] );
		}
		else if ( strcmp ( argv[xa], "-id" ) == 0 )
		{
			HasID = 1;
		}
		else if ( strcmp ( argv[xa], "-d" ) == 0 )
		{
			Debug = 1;
		}
		else if ( xa + 1 < argc &&  strcmp ( argv[xa], "-n" ) == 0 )
		{
			xa++;
			Normalize = argv[xa][0];
			switch( Normalize )
			{
				case 'n':
				case 'r':
				case 'c':
				case 'a':
					break;
				default:
					printf ( "-n  requires n (none) r (row wise) or c (column wise) a (all)\n" );
					exit ( 1 );
			}
		}
		else if ( xa + 1 < argc &&  strcmp ( argv[xa], "-split" ) == 0 )
		{
			xa++;
			SplitFile = toupper(argv[xa][0]);
			switch( SplitFile )
			{
				case 'N':
				case 'Y':
					break;
				default:
					printf ( "-split requires either y or n\n" );
					exit ( 1 );
			}
		}
		else
		{
			printf ( "Unknown arg %s\n", argv[xa] );
			exit ( 1 );
		}
	}
	
	if ( RunMode == 0 )
	{
		Usage ();
	}
}

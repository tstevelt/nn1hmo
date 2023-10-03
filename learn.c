/*---------------------------------------------------------------------------
	Copyright © 2023 Tom Stevelt and Silver Hammer Software LLC
	Tom Stevelt <tstevelt@silverhammersoftware.com>
	This software is free software; you can redistribute it and/or modify
	it under the terms of the MIT license. See LICENSE for details.
---------------------------------------------------------------------------*/

#include	"nn1hmo.h"

void learn ( int SeedMode, int ProgressMode )
{
	loadmodel ();

	double Accuracy = train ( SeedMode, ProgressMode );

	savemodel ();

	printf ( "learn: accuracy %.2f\n", Accuracy );
}

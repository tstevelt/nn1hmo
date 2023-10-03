/*---------------------------------------------------------------------------
	Copyright © 2023 Tom Stevelt and Silver Hammer Software LLC
	Tom Stevelt <tstevelt@silverhammersoftware.com>
	This software is free software; you can redistribute it and/or modify
	it under the terms of the MIT license. See LICENSE for details.
---------------------------------------------------------------------------*/

#include	"nn1hmo.h"

void genparms ( char *ParmFile )
{
	FILE	*pfp;

	if ( access ( ParmFile, F_OK ) == 0 )
	{
		printf ( "Parameter file %s exists, generate cancelled\n", ParmFile );
		exit ( 1 );
	}

	if (( pfp = fopen ( ParmFile, "w" )) == NULL )
	{
		printf ( "Cannot create parameter file %s\n", ParmFile );
		exit ( 1 );
	}

	fprintf ( pfp, "\n# %s parameter file (auto generated).\n\n", ParmFile );

	fprintf ( pfp, "#mode algo\n" );
	fprintf ( pfp, "mode train\n" );
	fprintf ( pfp, "#mode test\n" );
	fprintf ( pfp, "#mode predict\n\n" );

	fprintf ( pfp, "datafile /var/local/datasets/DIR/FILE\n\n" );

	fprintf ( pfp, "split	0.8 0.2\n\n" );

	fprintf ( pfp, "normalize n/r/c/a\n\n" );

	fprintf ( pfp, "id n/y\n\n" );

	fprintf ( pfp, "# Maxinum INPUTS is %d\n", MAXINPUTS );
	fprintf ( pfp, "inputs	INPUTS\n\n" );

	fprintf ( pfp, "# Hidden Layers and Number of neuron in each layer\n" );
	fprintf ( pfp, "layers	1	NEURONS\n\n" );

	fprintf ( pfp, "# Maxinum OUTPUTS is %d\n", MAXOUTPUTS );
	fprintf ( pfp, "outputs	OUTPUTS\n\n" );

	fprintf ( pfp, "learnrate	%g\n\n", DEFAULT_LEARNING_RATE );

	fprintf ( pfp, "epochs	%d\n\n", DEFAULT_EPOCHS );

	fprintf ( pfp, "accuracy	%g\n\n", DEFAULT_TARGET_ACCURACY );

	fprintf ( pfp, "model %s\n\n", DEFAULT_MODEL_FILE );

	fclose ( pfp );

	exit ( 0 );
}

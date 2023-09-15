/*---------------------------------------------------------------------------
	Copyright © 2023 Tom Stevelt and Silver Hammer Software LLC
	Tom Stevelt <tstevelt@silverhammersoftware.com>
	This software is free software; you can redistribute it and/or modify
	it under the terms of the MIT license. See LICENSE for details.
---------------------------------------------------------------------------*/

#include	"nn1hmo.h"

typedef void (*sighandler_t)(int);

sighandler_t sigset(int sig, sighandler_t disp);

void StartSignals()
{
	/*----------------------------------------------------------
		supposed to use sigaction() instead.
	----------------------------------------------------------*/
	sigset ( SIGINT, WriteModelAndExit );
	sigset ( SIGUSR1, WriteModelAndExit );

	// default all other signals
}

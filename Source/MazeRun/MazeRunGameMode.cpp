// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "MazeRun.h"
#include "MazeRunGameMode.h"
#include "MazeRunBall.h"

AMazeRunGameMode::AMazeRunGameMode()
{
	// set default pawn class to our ball
	DefaultPawnClass = AMazeRunBall::StaticClass();
}

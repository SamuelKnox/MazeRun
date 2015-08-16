// Fill out your copyright notice in the Description page of Project Settings.

#include "MazeRun.h"
#include "MazeHUD.h"


AMazeHUD::AMazeHUD(const class FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	recHeight = 20.0f;
	recWidth = 50.0f;
	recPoint = FVector2D(0, 0);
}

void AMazeHUD::DrawHUD()
{

}

// Fill out your copyright notice in the Description page of Project Settings.

#include "MazeRun.h"
#include "MazeHUD.h"
#include "MazeRunBall.h"
#include "Engine/Canvas.h"
#include <string>
using namespace std;

AMazeHUD::AMazeHUD(const class FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	recHeight = -80.0f;
	recWidth = -70.0f;
	mazeFontSize = FVector2D(2.0f, 2.0f);
	recPoint = FVector2D(-170.0f, -80.0f);
	textSeparatorOffset = 40.0f;
	currentTime = 0;
	isDead = false;
	textStartPoint = FVector2D(-215.0f, -140.0f);
	ConstructorHelpers::FObjectFinder<UFont> FontObject(TEXT("Font'/Game/Rolling/Fonts/FerroRosso.FerroRosso'"));
	mazeFont = FontObject.Object;
}

void AMazeHUD::DrawHUD()
{
	float textWidth;
	float textHeight;
	const FVector2D Center(this->Canvas->ClipX * 0.5f, this->Canvas->ClipY * 0.5f); //Draw a holding rectangle
	DrawRect(FColor::Black, Center.X + recPoint.X, Center.Y + recPoint.Y, recWidth, recHeight);

	GetTextSize(TEXT("Time"), textWidth, textHeight, mazeFont, mazeFontSize.X);
	DrawText(TEXT("Time"), textStartPoint, mazeFont, mazeFontSize, FColor::Green); //The title

	int precision = 2;
	FNumberFormattingOptions NumberFormat;					//Text.h
	NumberFormat.MinimumIntegralDigits = 1;
	NumberFormat.MaximumIntegralDigits = 10000;
	NumberFormat.MinimumFractionalDigits = precision;
	NumberFormat.MaximumFractionalDigits = precision;
	FString TimeString = FText::AsNumber(currentTime, &NumberFormat).ToString();  // Get time to 2 decimal places!
	GetTextSize(TimeString, textWidth, textHeight, mazeFont, mazeFontSize.X);
	DrawText(TimeString, FVector2D(textStartPoint.X, textStartPoint.Y + 
		textSeparatorOffset), mazeFont, mazeFontSize, FColor::Green);
}

void AMazeHUD::Tick(float DeltaTime)
{
	currentTime += DeltaTime;
	if (Cast<AMazeRunBall>(GetWorld()->GetFirstPlayerController()->GetPawn())->isDead) isDead = true;
	else if (!Cast<AMazeRunBall>(GetWorld()->GetFirstPlayerController()->GetPawn())->isDead && isDead)
	{
		currentTime = 0;
		isDead = false;
	}
}

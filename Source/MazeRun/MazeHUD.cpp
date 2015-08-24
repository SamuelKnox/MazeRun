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
	recHeight = -120.0f;
	recWidth = -190.0f;
	mazeFontSize = FVector2D(2.0f, 2.0f);
	recPoint = FVector2D(-170.0f, -80.0f);
	textSeparatorOffset = 40.0f;
	currentTime = 0;
	deathColorSpeed = 4.0f;
	isDead = false;
	textStartPoint = FVector2D(-275.0f, -180.0f);
	ConstructorHelpers::FObjectFinder<UFont> FontObject(TEXT("Font'/Game/Rolling/Fonts/Freezer.Freezer'"));
	mazeFont = FontObject.Object; //Find object then cast and set it
	textColor = FLinearColor::Green;
	topTime = 0;
}

void AMazeHUD::DrawHUD()
{
	float textWidth;
	float textHeight; 
	
	const FVector2D Center(this->Canvas->ClipX * 0.5f, this->Canvas->ClipY * 0.5f); //Draw a holding rectangle
	DrawRect(FColor::Black, Center.X + recPoint.X, Center.Y + recPoint.Y, recWidth, recHeight);

	GetTextSize(TEXT("Time"), textWidth, textHeight, mazeFont, mazeFontSize.X);
	DrawText(TEXT("Time"), textStartPoint, mazeFont, mazeFontSize, textColor); //The title

	int precision = 2;
	FNumberFormattingOptions NumberFormat; //For 2 decimal places
	NumberFormat.MinimumIntegralDigits = 1;
	NumberFormat.MaximumIntegralDigits = 10000;
	NumberFormat.MinimumFractionalDigits = precision;
	NumberFormat.MaximumFractionalDigits = precision;
	FString TimeString = FText::AsNumber(currentTime, &NumberFormat).ToString();  // Get time to 2 decimal places!
	GetTextSize(TimeString, textWidth, textHeight, mazeFont, mazeFontSize.X);
	DrawText(TimeString, FVector2D(textStartPoint.X, textStartPoint.Y + 
		textSeparatorOffset), mazeFont, mazeFontSize, textColor);
	
	FString TopScoreString = FString("Top Score: ") + FText::AsNumber(topTime, &NumberFormat).ToString(); //Use same number format
	GetTextSize(TopScoreString, textWidth, textHeight, mazeFont, mazeFontSize.X); //For top score as timer score
	DrawText(TopScoreString, FVector2D(textStartPoint.X, textStartPoint.Y +
		(textSeparatorOffset * 2.0f)), mazeFont, mazeFontSize, textColor);
}

void AMazeHUD::Tick(float DeltaTime)
{
	currentTime += DeltaTime;
	if (Cast<AMazeRunBall>(GetWorld()->GetFirstPlayerController()->GetPawn())->isDead)
	{
		if (!isDead && Cast<AMazeRunBall>(GetWorld()->GetFirstPlayerController()->GetPawn())->isWon) 
			topTime = currentTime;  //Get that top score ONCE
		isDead = true; //Initiate HUD death
		
	}
	else if (!Cast<AMazeRunBall>(GetWorld()->GetFirstPlayerController()->GetPawn())->isDead && isDead)
	{
		currentTime = 0; //Reset the clock
		isDead = false;
	}

	if (isDead && !Cast<AMazeRunBall>(GetWorld()->GetFirstPlayerController()->GetPawn())->isWon) //Interpolate between green and red upon death
	{
		FLinearColor deathColor = FLinearColor::Red;
		textColor = FMath::CInterpTo(textColor, deathColor, DeltaTime, deathColorSpeed);
	}
	else textColor = FLinearColor::Green; //Else happy green
}

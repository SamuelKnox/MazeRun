// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/HUD.h"
#include "MazeHUD.generated.h"

/**
 * 
 */
UCLASS()
class MAZERUN_API AMazeHUD : public AHUD
{
	GENERATED_BODY()
public:
	AMazeHUD(const FObjectInitializer& ObjectInitializer);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Font) //Font for HUD
		UFont* mazeFont;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Draw) //Width of rectangle to hold font
		float recWidth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Draw) //Height of rectangle to hold font
		float recHeight;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Draw) //Where to put rectangle
		FVector2D recPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Draw) //Where to put text
		FVector2D textStartPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Draw) //Size of font
		FVector2D mazeFontSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Draw) //Vertically separating text
		float textSeparatorOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Draw) //how fast to interpolate from green to red text
		float deathColorSpeed;

	virtual void DrawHUD() override;

	virtual void Tick( float DeltaSeconds ) override;

	bool isDead;
	FLinearColor textColor;
	float currentTime;
	float topTime;
	
};

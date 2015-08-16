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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Draw) //Size of font
		FVector2D mazeFontSize;

	virtual void DrawHUD() override;
	
};

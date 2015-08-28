// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "FastBox.generated.h"

UCLASS()
class MAZERUN_API AFastBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFastBox(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Box) //Space of entrance in FastBox
	UBoxComponent* Box;

	UFUNCTION(BlueprintCallable, Category = Collision)
		virtual void OnBoxEnter(class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION(BlueprintCallable, Category = Collision)
		virtual void OnBoxLeave(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SpeedUpWorld();

	void NormalizeWorldSpeed();

	float fastSpeed;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	
	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "DeathSpike.generated.h"

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class ESpikeBehaviorEnum : uint8
{
	Retracting 	UMETA(DisplayName = "Retract"),
	Extending UMETA(DisplayName = "Extend"),
	RestingAtExtend 	UMETA(DisplayName = "RestExtend"),
	RestingAtRetract	UMETA(DisplayName = "RestRetract")
};

UCLASS(config = Game)
class MAZERUN_API ADeathSpike : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADeathSpike(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
		UStaticMeshComponent* SpikeMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Rest) //Time range for resting in wall from min to max
		float restingTimeMin;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Rest)
		float restingTimeMax;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Rest) //Time for resting while extended
		float extendedRestTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Action) //How long it takes to extend spike
		float extendDuration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Action) //How long it takes to retract spike
		float retractDuration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Action) //Distance that spike extends
		float extendDistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Action) //Speed that spike extends
		float extendSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Action) //Speed that spike retracts
		float retractSpeed;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	void ExtendSpike(float DeltaTime);

	void RetractSpike(float DeltaTime);

	void ProduceRandomRest();
	
	float overallTime;

	float restingTime; //Resting while retracted

	FVector initialLocation; //initial location of spike

	FVector targetLocation; //target of spike extend

	ESpikeBehaviorEnum behavior; //Whatever it's doing at the moment
};

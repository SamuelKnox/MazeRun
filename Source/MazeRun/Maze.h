// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Engine/StaticMeshActor.h"
#include "Maze.generated.h"

UCLASS(config = Game)
class MAZERUN_API AMaze : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AMaze(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Maze)
		bool ShowTests;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MazeGen101isMax) //Maze xMax
		float MazeXKeepODD;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MazeGen101isMax) //Maze yMax
		float MazeYKeepODD;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MazeGen101isMax) // seed for randomizing maze
		float MazeSeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MazeGen101isMax) // seed for randomizing maze
		float offset;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = MazeTiles)
		UClass* TileGroundBP;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = MazeTiles)
		UClass* TileBlockBP;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = MazeTiles)
		UClass* TileStartBP;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = MazeTiles)
		UClass* TileEndBP;
	UFUNCTION(BlueprintCallable, Category = MazeGen)
		void GenMaze(float tileX, float tileY);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

};

template <typename AMazeGen>
FORCEINLINE AMazeGen* SpawnBP(
	UWorld* TheWorld,
	UClass* TheBP,
	const FVector& Loc,
	const FRotator& Rot,
	const bool bNoCollisionFail = true,
	AActor* Owner = NULL,
	APawn* Instigator = NULL
	){
	if (!TheWorld) return NULL;
	if (!TheBP) return NULL;
	//~~~~~~~~~~~

	FActorSpawnParameters SpawnInfo;
	SpawnInfo.bNoCollisionFail = bNoCollisionFail;
	SpawnInfo.Owner = Owner;
	SpawnInfo.Instigator = Instigator;
	SpawnInfo.bDeferConstruction = false;


	return TheWorld->SpawnActor<AMazeGen>(TheBP, Loc, Rot, SpawnInfo);
}

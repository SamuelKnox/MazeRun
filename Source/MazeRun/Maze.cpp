// Fill out your copyright notice in the Description page of Project Settings.

#include "MazeRun.h"
#include "Maze.h"
#include "MazeRunBall.h"


// Sets default values
const int MazeSizeMax = 101;
// Sets default values
AMaze::AMaze(const class FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MazeXKeepODD = MazeSizeMax;
	MazeYKeepODD = MazeSizeMax;
	offset = 200.0f;
	MazeSeed = 0;
	GroundSpikeOffset = FVector(0.0f, 0.0f, -200.0f);
	SpikeSpawnChance = 30.0f;
}

// Called when the game starts or when spawned
void AMaze::BeginPlay()
{
	Super::BeginPlay();
	GenMaze(MazeXKeepODD, MazeYKeepODD);
	UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->
		SetActorLocation(this->GetActorLocation() + FVector(offset, offset, offset));
	AMazeRunBall* Player = Cast<AMazeRunBall>(GetWorld()->GetFirstPlayerController()->GetPawn());
	Player->SetStartupLocation(this->GetActorLocation() + FVector(offset, offset, offset));
}

// Called every frame
void AMaze::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMaze::GenMaze(float tileX, float tileY){
	srand(MazeSeed); //Seed random value
	if (((int)tileX) % 2 == 0) tileX++; //Set tiles so that there will be a proper amount of spacing - no even sizes!
	if (((int)tileY) % 2 == 0) tileY++;
	if (tileX > MazeSizeMax) tileX = MazeSizeMax; //Make sure it goes over the max value
	if (tileY > MazeSizeMax) tileY = MazeSizeMax;
	float CaptureX = 0.0f;
	float CaptureY = 0.0f;

	int RandomEndTileLoc = rand() % ((int)tileX - 1) + 1;

	AStaticMeshActor* grid[MazeSizeMax][MazeSizeMax];

	for (int x = 0; x<tileX; x++){
		for (int y = 0; y<tileY; y++){
			if (y == 0 || x == 0 || y == tileY - 1 || x == tileX - 1 || (y % 2 == 0 && x % 2 == 0)){
				//                          (X.Xf,Y.Yf,Z.Zf)
				const FVector  GenSpawnLoc(CaptureX, CaptureY, 0);
				const FRotator GenSpawnRot(0.0f, 0.0f, 0.0f);
				AStaticMeshActor* BlockTile = SpawnBP<AStaticMeshActor>(GetWorld(), TileBlockBP,
					GenSpawnLoc, GenSpawnRot, true, this);
				BlockTile->AddActorWorldOffset(this->GetActorLocation());
				grid[x][y] = BlockTile;
			}
			else{
				const FVector  GenSpawnLoc(CaptureX, CaptureY, 0);
				const FRotator GenSpawnRot(0.0f, 0.0f, 0.0f);

				AStaticMeshActor* GroundTile = SpawnBP<AStaticMeshActor>(GetWorld(), TileGroundBP,
					GenSpawnLoc, GenSpawnRot, true, this);
				GroundTile->AddActorWorldOffset(this->GetActorLocation());

				float random = ((float)rand()) / (float)RAND_MAX; //Spawn spike in the ground with a certain percent chance
				if ((x * y != 1) && random * 100.0f < SpikeSpawnChance) SpawnBP<AActor>(GetWorld(), SpikeBP, //from 0 to 100 percent, also not the starting tile
					GroundTile->GetActorLocation() + GroundSpikeOffset, GenSpawnRot, true, GroundTile);
				
				grid[x][y] = GroundTile;
			}
			//-------------Starting Tile Spawn---------------
			if (CaptureX == offset && CaptureY == offset){
				grid[1][1]->Destroy();

				const FVector  GenSpawnLoc(offset, offset, 0);
				const FRotator GenSpawnRot(0.0f, 0.0f, 0.0f);
				//Tile Start
				AStaticMeshActor* StartTile = SpawnBP<AStaticMeshActor>(GetWorld(), TileStartBP,
					GenSpawnLoc, GenSpawnRot, true, this);
				StartTile->AddActorWorldOffset(this->GetActorLocation());
				grid[1][1] = StartTile;
			}
			//-------------Ending Tile Spawn---------------
			if (y == tileY - 1 && x == tileX - 1){
				grid[x - 1][y - 1]->Destroy();

				const FVector  GenSpawnLoc(((tileX - 2) * offset),
					((tileY - 2) * offset), 0);
				const FRotator GenSpawnRot(0.0f, 0.0f, 0.0f);
				// Tile End
				AStaticMeshActor* EndTile = SpawnBP<AStaticMeshActor>(GetWorld(), TileEndBP,
					GenSpawnLoc, GenSpawnRot, true, this);
				EndTile->AddActorWorldOffset(this->GetActorLocation());
				grid[x - 1][y - 1] = EndTile;
			}
			CaptureY += offset;
			if (CaptureY >= offset * tileY){ CaptureY = 0; }
		}
		CaptureX += offset;
		if (CaptureX >= offset * tileX){ CaptureX = 0; }
	}
	//-----------------------------------------------------------------------------------------------------------------------------------------------
	for (int y = 2; y < tileY - 1; y += 2) {
		int dx = 2;
		int dy = y;
		int rnd4;
		//Choose a random direction to move to
		switch (rnd4 = rand() % 4){
		case 0: dx++; break;
		case 1: dx--; break;
		case 2: dy++; break;
		case 3: dy--; break;
		}
		//if (bd.getPixel(dx, dy) != Status.WALL) {
		if (grid[dx][dy]->GetActorLabel() != "Block") {
			FVector f = grid[dx][dy]->GetActorLocation();
			grid[dx][dy]->Destroy();

			const FVector  GenSpawnLoc(f);
			const FRotator GenSpawnRot(0.0f, 0.0f, 0.0f);

			AStaticMeshActor* BlockTile = SpawnBP<AStaticMeshActor>(GetWorld(), TileBlockBP,
				GenSpawnLoc, GenSpawnRot, true, this);
			grid[dx][dy] = BlockTile;
		}
		else{
			y -= 2;
		}
	}
	for (int x = 4; x < tileX - 1; x += 2) {
		for (int y = 2; y < tileY - 1; y += 2) {
			int dx = x;
			int dy = y;
			int rnd3;

			switch (rnd3 = rand() % 3){
			case 0: dy++; break;
			case 1: dy--; break;
			case 2: dx++; break;
			}
			//if (bd.getPixel(dx, dy) != Status.WALL) {
			if (grid[dx][dy]->GetName() != "Block") {
				FVector f = grid[dx][dy]->GetActorLocation();
				grid[dx][dy]->Destroy();

				const FVector  GenSpawnLoc(f);
				const FRotator GenSpawnRot(0.0f, 0.0f, 0.0f);

				AStaticMeshActor* BlockTile = SpawnBP<AStaticMeshActor>(GetWorld(), TileBlockBP,
					GenSpawnLoc, GenSpawnRot, true, this);
				grid[dx][dy] = BlockTile;
			}
			else{
				y -= 2;
			}
		}
	}
}


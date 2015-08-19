// Fill out your copyright notice in the Description page of Project Settings.

#include "MazeRun.h"
#include "MazeRunBall.h"
#include "WinBox.h"


void AWinBox::OnWinHit(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, //Makes ball disappear magically on hit
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor == this) return;
	if (OtherActor != NULL && OtherActor == GetWorld()->GetFirstPlayerController()->GetPawn()) //Easy check for first player pawn
	{
		AMazeRunBall* Player = Cast<AMazeRunBall>(GetWorld()->GetFirstPlayerController()->GetPawn()); //Cast to player for easy access
		Player->Die();
		Player->isWon = true;
	}
}
// Sets default values
AWinBox::AWinBox(const class FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	WinBox = ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this, TEXT("WinBox"));
	RootComponent = WinBox;
	WinBox->OnComponentBeginOverlap.AddDynamic(this, &AWinBox::OnWinHit);

}

// Called when the game starts or when spawned
void AWinBox::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWinBox::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}


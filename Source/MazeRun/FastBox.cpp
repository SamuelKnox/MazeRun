// Fill out your copyright notice in the Description page of Project Settings.

#include "MazeRun.h"
#include "FastBox.h"
#include "MazeRunBall.h"
#include "DeathSpike.h"


void AFastBox::OnBoxEnter(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, //Makes ball disappear magically on hit
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor == this) return;
	if (OtherActor != NULL && OtherActor == GetWorld()->GetFirstPlayerController()->GetPawn()) SpeedUpWorld(); //Easy check for first player pawn
}

void AFastBox::OnBoxLeave(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == this) return;
	if (OtherActor != NULL && OtherActor == GetWorld()->GetFirstPlayerController()->GetPawn()) NormalizeWorldSpeed(); //Easy check for first player pawn
}
// Sets default values
AFastBox::AFastBox(const class FObjectInitializer& ObjectInitializer)
:Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Box = ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this, TEXT("Box"));
	RootComponent = Box;
	Box->OnComponentBeginOverlap.AddDynamic(this, &AFastBox::OnBoxEnter); //Add enter function as delegate
	Box->OnComponentEndOverlap.AddDynamic(this, &AFastBox::OnBoxLeave); //Add leave function as delegate
	fastSpeed = 3.0f;
}

// Called when the game starts or when spawned
void AFastBox::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFastBox::SpeedUpWorld()
{
	Cast<AMazeRunBall>(GetWorld()->GetFirstPlayerController()->GetPawn())->CustomTimeDilation = fastSpeed;
	Cast<AMazeRunBall>(GetWorld()->GetFirstPlayerController()->GetPawn())->Music->SetPitchMultiplier(fastSpeed);
	UGameplayStatics::SetGlobalTimeDilation(this, fastSpeed);

	TArray<AActor*> spikes;
	UGameplayStatics::GetAllActorsOfClass(this, ADeathSpike::StaticClass(), spikes);
	for (auto Iter(spikes.CreateIterator()); Iter; Iter++)
	{
		Cast<ADeathSpike>(*Iter)->TimeDilation = fastSpeed;
	}
}

void AFastBox::NormalizeWorldSpeed()
{
	Cast<AMazeRunBall>(GetWorld()->GetFirstPlayerController()->GetPawn())->CustomTimeDilation = 1.0f;
	Cast<AMazeRunBall>(GetWorld()->GetFirstPlayerController()->GetPawn())->Music->SetPitchMultiplier(1.0f);
	UGameplayStatics::SetGlobalTimeDilation(this, 1.0f);

	TArray<AActor*> spikes;
	UGameplayStatics::GetAllActorsOfClass(this, ADeathSpike::StaticClass(), spikes);
	for (auto Iter(spikes.CreateIterator()); Iter; Iter++)
	{
		Cast<ADeathSpike>(*Iter)->TimeDilation = 1.0f;
	}
}

// Called every frame
void AFastBox::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}


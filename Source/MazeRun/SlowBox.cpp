// Fill out your copyright notice in the Description page of Project Settings.

#include "MazeRun.h"
#include "SlowBox.h"
#include "MazeRunBall.h"
#include "DeathSpike.h"


// Sets default values
void ASlowBox::OnBoxEnter(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, //Makes ball disappear magically on hit
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor == this) return;
	if (OtherActor != NULL && OtherActor == GetWorld()->GetFirstPlayerController()->GetPawn()) SpeedUpWorld(); //Easy check for first player pawn
}

void ASlowBox::OnBoxLeave(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == this) return;
	if (OtherActor != NULL && OtherActor == GetWorld()->GetFirstPlayerController()->GetPawn()) NormalizeWorldSpeed(); //Easy check for first player pawn
}
// Sets default values
ASlowBox::ASlowBox(const class FObjectInitializer& ObjectInitializer)
:Super(ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Box = ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this, TEXT("Box"));
	RootComponent = Box;
	Box->OnComponentBeginOverlap.AddDynamic(this, &ASlowBox::OnBoxEnter); //Add enter function as delegate
	Box->OnComponentEndOverlap.AddDynamic(this, &ASlowBox::OnBoxLeave); //Add leave function as delegate
	slowSpeed = 0.3f;
}

// Called when the game starts or when spawned
void ASlowBox::BeginPlay()
{
	Super::BeginPlay();

}

void ASlowBox::SpeedUpWorld()
{
	Cast<AMazeRunBall>(GetWorld()->GetFirstPlayerController()->GetPawn())->CustomTimeDilation = slowSpeed;
	Cast<AMazeRunBall>(GetWorld()->GetFirstPlayerController()->GetPawn())->Music->SetPitchMultiplier(slowSpeed);
	UGameplayStatics::SetGlobalTimeDilation(this, slowSpeed);

	TArray<AActor*> spikes;
	UGameplayStatics::GetAllActorsOfClass(this, ADeathSpike::StaticClass(), spikes);
	for (auto Iter(spikes.CreateIterator()); Iter; Iter++)
	{
		Cast<ADeathSpike>(*Iter)->TimeDilation = slowSpeed;
	}
}

void ASlowBox::NormalizeWorldSpeed()
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
void ASlowBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


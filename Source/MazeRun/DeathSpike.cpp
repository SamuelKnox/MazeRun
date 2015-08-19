// Fill out your copyright notice in the Description page of Project Settings.

#include "MazeRun.h"
#include "DeathSpike.h"
#include "MazeRunBall.h"


void ADeathSpike::OnDangerHit(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, //Makes ball disappear magically on hit
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor == this) return;
	if (OtherActor != NULL && OtherActor == GetWorld()->GetFirstPlayerController()->GetPawn()) //Easy check for first player pawn
	{
		AMazeRunBall* Player = Cast<AMazeRunBall>(GetWorld()->GetFirstPlayerController()->GetPawn()); //Cast to player for easy access
		Player->Die();
	}
}

// Sets default values
ADeathSpike::ADeathSpike(const class FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SpikeMesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("SpikeMesh"));
	RootComponent = SpikeMesh;
	overallTime = 0.0f;
	restingTimeMin = 3.0f;
	restingTimeMax = 7.0f;
	extendedRestTime = 2.0f;
	extendDuration = 0.5f;
	retractDuration = 1.0f;
	extendDistance = 30.0f;
	extendSpeed = 2.0f;
	retractSpeed = 2.0f;
	soundDistance = 1200.0f;
	behavior = ESpikeBehaviorEnum::RestingAtRetract;
	SpikeMesh->OnComponentBeginOverlap.AddDynamic(this, &ADeathSpike::OnDangerHit); //Add hit function as delegate
}

// Called when the game starts or when spawned
void ADeathSpike::BeginPlay()
{
	Super::BeginPlay();
	initialLocation = GetActorLocation();
	FVector upVector = GetActorUpVector(); //Spike initially pointing up; will change based on rotation
	upVector.Normalize();
	targetLocation = initialLocation + (upVector * extendDistance); //Set target on distance in direction
	ProduceRandomRest();
}

void ADeathSpike::ProduceRandomRest()
{
	float random = ((float)rand()) / (float)RAND_MAX; //Random number
	float diff = restingTimeMax - restingTimeMin; //Difference for range
	float r = random * diff; //Decimal value 0.0f to 1.0f
	restingTime = restingTimeMin + r;
}

// Called every frame
void ADeathSpike::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	overallTime += DeltaTime;
	switch (behavior)
	{
	case ESpikeBehaviorEnum::RestingAtExtend:
		if (overallTime > extendedRestTime)
		{
			behavior = ESpikeBehaviorEnum::Retracting;
			overallTime = 0;
		}
		break;
	case ESpikeBehaviorEnum::RestingAtRetract:
		if (overallTime > restingTime)
		{
			if (FVector::Dist(GetActorLocation(), GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation()) //Attenuating spike sound
				< soundDistance) UGameplayStatics::PlaySoundAtLocation(GetWorld(), 
				Cast<USoundBase>(StaticLoadObject(USoundBase::StaticClass(), NULL, 
				TEXT("/Game/Rolling/Sounds/SpikeTrap.SpikeTrap"))), GetActorLocation());
			
			behavior = ESpikeBehaviorEnum::Extending;
			overallTime = 0;
		}
		break;
	case ESpikeBehaviorEnum::Extending:
		if (overallTime > extendDuration)
		{
			behavior = ESpikeBehaviorEnum::RestingAtExtend;
			overallTime = 0;
		}
		else ExtendSpike(DeltaTime);
	case ESpikeBehaviorEnum::Retracting:
		if (overallTime > retractDuration)
		{
			behavior = ESpikeBehaviorEnum::RestingAtRetract;
			overallTime = 0;
		}
		else RetractSpike(DeltaTime);
		break;
	}
}

void ADeathSpike::ExtendSpike(float DeltaTime)
{
	SetActorLocation(FMath::VInterpTo(GetActorLocation(), targetLocation, DeltaTime, extendSpeed));
}

void ADeathSpike::RetractSpike(float DeltaTime)
{
	SetActorLocation(FMath::VInterpTo(GetActorLocation(), initialLocation, DeltaTime, retractSpeed));
}


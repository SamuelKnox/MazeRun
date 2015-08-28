// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "MazeRun.h"
#include "MazeRunBall.h"

AMazeRunBall::AMazeRunBall()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BallMesh(TEXT("/Game/Rolling/Meshes/BallMesh.BallMesh"));
	PrimaryActorTick.bCanEverTick = true;
	// Create mesh component for the ball
	Ball = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ball0"));
	Ball->SetStaticMesh(BallMesh.Object);
	Ball->BodyInstance.SetCollisionProfileName(UCollisionProfile::PhysicsActor_ProfileName);
	Ball->SetSimulatePhysics(true);
	Ball->SetAngularDamping(0.1f);
	Ball->SetLinearDamping(0.1f);
	Ball->BodyInstance.MassScale = 3.5f;
	Ball->BodyInstance.MaxAngularVelocity = 800.0f;
	Ball->SetNotifyRigidBodyCollision(true);
	RootComponent = Ball;

	// Create a camera boom attached to the root (ball)
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm0"));
	SpringArm->AttachTo(RootComponent);
	SpringArm->bDoCollisionTest = false;
	SpringArm->bAbsoluteRotation = true; // Rotation of the ball should not affect rotation of boom
	SpringArm->RelativeRotation = FRotator(-90.f, 0.f, 0.f);
	SpringArm->TargetArmLength = 1200.f;
	SpringArm->bEnableCameraLag = false;
	SpringArm->CameraLagSpeed = 3.f;

	// Create a camera and attach to boom
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera0"));
	Camera->AttachTo(SpringArm, USpringArmComponent::SocketName);
	Camera->PostProcessSettings.bOverride_FilmWhitePoint = true;
	initialTint = Camera->PostProcessSettings.FilmWhitePoint;
	Camera->bUsePawnControlRotation = false; // We don't want the controller rotating the camera

	Music = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio0"));
	
	// Set up forces
	RollTorque = 50000000.0f;
	JumpImpulse = 350000.0f;
	bCanJump = true; // Start being able to jump
	isDead = false;
	deadTime = 0;
	maxDeadTime = 3.0f;
	deathColorSpeed = 4.0f;
	startupLocation = FVector(0, 0, 0);
}


void AMazeRunBall::Die()
{
	if (isDead) return;
	Ball->SetVisibility(false); //Make ball disappear and stop in its tracks
	Ball->SetPhysicsLinearVelocity(FVector::ZeroVector);
	Ball->SetPhysicsAngularVelocity(FVector::ZeroVector);
	Ball->SetSimulatePhysics(false);
	UGameplayStatics::SpawnEmitterAtLocation(this, //AND EXPLOOOODE
		Cast<UParticleSystem>(StaticLoadObject(UParticleSystem::StaticClass(), NULL,
		TEXT("/Game/StarterContent/Particles/P_Explosion.P_Explosion"))), GetActorLocation(), GetActorRotation(), true);
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), Cast<USoundBase>(StaticLoadObject(USoundBase::StaticClass(), NULL,
		TEXT("/Game/Rolling/Sounds/Explosion.Explosion"))), GetActorLocation(), 1.0f, CustomTimeDilation);
	isDead = true;
}
void AMazeRunBall::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// set up gameplay key bindings
	InputComponent->BindAxis("MoveRight", this, &AMazeRunBall::MoveRight);
	InputComponent->BindAxis("MoveForward", this, &AMazeRunBall::MoveForward);

	InputComponent->BindAction("Jump", IE_Pressed, this, &AMazeRunBall::Jump);
}

void AMazeRunBall::MoveRight(float Val)
{
	const FVector Torque = FVector(-1.f * Val * RollTorque, 0.f, 0.f);
	Ball->AddTorque(Torque);
}

void AMazeRunBall::MoveForward(float Val)
{
	const FVector Torque = FVector(0.f, Val * RollTorque, 0.f);
	Ball->AddTorque(Torque);	
}

void AMazeRunBall::Jump()
{
	if (isDead) deadTime = maxDeadTime;
	if(bCanJump)
	{
		const FVector Impulse = FVector(0.f, 0.f, JumpImpulse);
		Ball->AddImpulse(Impulse);
		bCanJump = false;
	}
}

void AMazeRunBall::NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);
	bCanJump = true;
}

void AMazeRunBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (isDead)
	{
		if(!isWon) Camera->PostProcessSettings.FilmWhitePoint = FMath::CInterpTo(Camera->PostProcessSettings.FilmWhitePoint, 
			FLinearColor::Red, DeltaTime, deathColorSpeed);
		deadTime += DeltaTime;
		if (deadTime > maxDeadTime) //Count the seconds of death
		{
			deadTime = 0;
			isDead = false;
			isWon = false;
			Ball->SetVisibility(true);
			Ball->SetSimulatePhysics(true);
			this->SetActorLocation(startupLocation);
		}
	}
	else Camera->PostProcessSettings.FilmWhitePoint = initialTint;
}

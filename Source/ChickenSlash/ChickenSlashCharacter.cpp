// Copyright Epic Games, Inc. All Rights Reserved.

#include "ChickenSlashCharacter.h"

#include "ChickenPawn.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "Materials/Material.h"
#include "Engine/World.h"

AChickenSlashCharacter::AChickenSlashCharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = false; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create a decal in the world to show the cursor's location
	CursorToWorld = CreateDefaultSubobject<UDecalComponent>("CursorToWorld");
	CursorToWorld->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset(TEXT("Material'/Game/TopDownCPP/Blueprints/M_Cursor_Decal.M_Cursor_Decal'"));
	if (DecalMaterialAsset.Succeeded())
	{
		CursorToWorld->SetDecalMaterial(DecalMaterialAsset.Object);
	}
	CursorToWorld->DecalSize = FVector(16.0f, 32.0f, 32.0f);
	CursorToWorld->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	RightFoot = CreateDefaultSubobject<USphereComponent>(TEXT("RightFoot"));
	RightFoot->SetupAttachment(GetMesh(), FName("ball_r"));
	RightFoot->SetSphereRadius(16);
	RightFoot->SetCollisionProfileName("OverlapAllDynamic");
	
	LeftFoot = CreateDefaultSubobject<USphereComponent>(TEXT("LeftFoot"));
	LeftFoot->SetupAttachment(GetMesh(), FName("ball_l"));
	LeftFoot->SetSphereRadius(16);
	LeftFoot->SetCollisionProfileName("OverlapAllDynamic");
}

void AChickenSlashCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AChickenSlashCharacter::OnChickenHit);
	//LeftFoot->OnComponentBeginOverlap.AddDynamic(this, &AChickenSlashCharacter::OnChickenHit);
}

void AChickenSlashCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

	if (CursorToWorld != nullptr)
	{
		if (APlayerController* PC = Cast<APlayerController>(GetController()))
		{
			FHitResult TraceHitResult;
			PC->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);
			FVector CursorFV = TraceHitResult.ImpactNormal;
			FRotator CursorR = CursorFV.Rotation();
			CursorToWorld->SetWorldLocation(TraceHitResult.Location);
			CursorToWorld->SetWorldRotation(CursorR);
		}
	}
}

void AChickenSlashCharacter::OnChickenHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	if (OtherActor != this && OtherComponent->IsSimulatingPhysics())
	{
		FVector Vector = (OtherComponent->GetComponentLocation() - GetActorLocation());
		Vector.Normalize();
		
		UCharacterMovementComponent* Movement = GetCharacterMovement();
		float Scale = FMath::Clamp(Movement->Velocity.SizeSquared() / (Movement->MaxWalkSpeed * Movement->MaxWalkSpeed), 0.2f, 1.f);
		
		OtherComponent->AddImpulse(Vector * Scale * ChickenKickPower);
	}
}

bool TryGetStartVelocity(FVector startGlobalPosition, FVector targetGlobalPosition, float radAngle, float gravity, FVector& startVelocity)
{
	FVector lineDirection = targetGlobalPosition - startGlobalPosition;
	float heightDiff = lineDirection.Z;
	lineDirection.Z = 0;

	float length = lineDirection.Size();
	lineDirection.Z = length * FMath::Tan(radAngle);
	length += heightDiff / FMath::Tan(radAngle);
	if (length <= 0)
	{
		startVelocity = lineDirection;
		return false;
	}

	float velocity = FMath::Sqrt(length * gravity / FMath::Sin(2 * radAngle));
	lineDirection.Normalize();
	startVelocity = velocity * lineDirection;
	return true;
}

void AChickenSlashCharacter::LaunchGrenade()
{
	FVector Goal = CursorToWorld->GetComponentLocation();
	FVector ActorLocation = GetActorLocation();
	FVector Direction;
	
	FActorSpawnParameters SpawnInfo; 
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn; // Forces the pawn to spawn even if colliding

	AGrenadeProjectile *Grenade = GetWorld()->SpawnActor<AGrenadeProjectile>(GrenadePrefab.Get(),
		GetActorLocation() + GetController()->GetControlRotation().RotateVector(GrenadeLaunchOffset), FRotator(), SpawnInfo);

	if (!TryGetStartVelocity(ActorLocation, Goal, StartRadAngle, Grenade->ProjectileMovementComponent->GetGravityZ(),
				Direction))
	{
		Direction.Normalize();
		Direction = Grenade->ProjectileMovementComponent->InitialSpeed * Direction;
	}
	
	Grenade->FireInDirection(Direction);
}




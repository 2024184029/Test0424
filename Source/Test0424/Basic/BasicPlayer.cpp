// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ABasicPlayer::ABasicPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	GetMesh()->SetRelativeLocationAndRotation(
		FVector(0, 0, -GetCapsuleComponent()->GetScaledCapsuleHalfHeight()),
		FRotator(0, -90.f, 0)
	);

}

// Called when the game starts or when spawned
void ABasicPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABasicPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABasicPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* UIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (UIC)
	{
		UIC->BindAction(IA_Move, ETriggerEvent::Triggered, this, &ABasicPlayer::Move);
		UIC->BindAction(IA_Look, ETriggerEvent::Triggered, this, &ABasicPlayer::Look);
		UIC->BindAction(IA_Jump, ETriggerEvent::Triggered, this, &ABasicPlayer::Jump);
		UIC->BindAction(IA_Jump, ETriggerEvent::Canceled, this, &ABasicPlayer::StopJumping);
		UIC->BindAction(IA_Jump, ETriggerEvent::Completed, this, &ABasicPlayer::StopJumping);
	}

}

void ABasicPlayer::Move(const FInputActionValue& Value)
{
	FVector2D Direction = Value.Get<FVector2D>();

	FRotator CameraRotation = GetControlRotation();
	FRotator CameraRotationInFloor = FRotator(0, CameraRotation.Yaw, 0);
	FVector CameraForwardVectorInFloor = UKismetMathLibrary::GetForwardVector(CameraRotationInFloor);
	FVector CameraRightVectorInFloor = UKismetMathLibrary::GetRightVector(CameraRotationInFloor);
	
	AddMovementInput(CameraForwardVectorInFloor * Direction.X);
	AddMovementInput(CameraRightVectorInFloor * Direction.Y);
}

void ABasicPlayer::Look(const FInputActionValue& Value)
{
	FVector2D RotationDirection = Value.Get<FVector2D>();

	AddControllerYawInput(RotationDirection.X);
	AddControllerPitchInput(RotationDirection.Y);
}


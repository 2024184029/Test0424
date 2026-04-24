// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "../Test0424.h"

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
		UIC->BindAction(IA_Lean, ETriggerEvent::Triggered, this, &ABasicPlayer::Lean);
		UIC->BindAction(IA_Lean, ETriggerEvent::Completed, this, &ABasicPlayer::Lean);

		UIC->BindAction(IA_Jump, ETriggerEvent::Triggered, this, &ABasicPlayer::Jump);
		UIC->BindAction(IA_Jump, ETriggerEvent::Completed, this, &ABasicPlayer::StopJumping);
		UIC->BindAction(IA_Jump, ETriggerEvent::Canceled, this, &ABasicPlayer::StopJumping);
	}
}

void ABasicPlayer::BigHead()
{
	//bIsBigHead = bIsBigHead ? false : true;
	bIsBigHead = ~bIsBigHead;
}

void ABasicPlayer::Move(const FInputActionValue& Value)
{
	FVector2D Direction = Value.Get<FVector2D>();

	//카메라 방향으로 바닥을 기준으로 움직이게 설정
	//폰 회전이나 스프링암 회전은 컨트롤러 회전에 따른 정의 따라 회전함
	FRotator CameraRotation = GetControlRotation();

	FRotator CameraRotaitionInFloor = FRotator(0, CameraRotation.Yaw, 0);

	FVector CameraForwardInFloor = UKismetMathLibrary::GetForwardVector(CameraRotaitionInFloor);

	FVector CameraRightInFloor = UKismetMathLibrary::GetRightVector(CameraRotaitionInFloor);

	AddMovementInput(CameraForwardInFloor * Direction.X);

	AddMovementInput(CameraRightInFloor * Direction.Y);




	//AddMovementInput(FVector(Direction.X, Direction.Y, 0));
}

void ABasicPlayer::Look(const FInputActionValue& Value)
{
	FVector2D RotationDirection = Value.Get<FVector2D>();

	//회전은 PlayerController에 전달 해둠
	AddControllerPitchInput(RotationDirection.Y);
	AddControllerYawInput(RotationDirection.X);

	//AddActorLocalRotation(
	//	FRotator(RotationDirection.Y, RotationDirection.X, 0) *
	//	UGameplayStatics::GetWorldDeltaSeconds(GetWorld()) *
	//	360.0f
	//);
}

void ABasicPlayer::Lean(const FInputActionValue& Value)
{
	float Direction = Value.Get<float>();

	TargetLeanAngle = 30.0f * Direction;

	UE_LOG(LogTest0424, Warning, TEXT("Direction %f"), Direction);
}
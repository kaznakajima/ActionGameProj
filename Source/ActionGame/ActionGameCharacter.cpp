// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "ActionGameCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/DamageType.h"
#include "GameFramework/SpringArmComponent.h"
#include "MyCharacterMovementComponent.h"
#include "Engine.h"

AActionGameCharacter::AActionGameCharacter(const FObjectInitializer& ObjectInitilizer) 
	: Super(ObjectInitilizer.SetDefaultSubobjectClass<UMyCharacterMovementComponent>(CharacterMovementComponentName))
{
	// �J�v�Z���R���W�����̏�����
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// ��]�͂̏�����
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// �L�����N�^�[�̌������J�����ɉe������Ȃ�
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// CharacterMovement�̃Z�b�g�A�b�v
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// SpringArm�̃Z�b�g�A�b�v
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f;	
	CameraBoom->bUsePawnControlRotation = true;

	// �J�����̃Z�b�g�A�b�v
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); 
	FollowCamera->bUsePawnControlRotation = false;
}

//////////////////////////////////////////////////////////////////////////
// Input

void AActionGameCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// �L�[���͂̃Z�b�g�A�b�v
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AActionGameCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AActionGameCharacter::MoveRight);

	// �J������]����
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AActionGameCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AActionGameCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AActionGameCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AActionGameCharacter::TouchStopped);

	// VR�̃Z�b�g�A�b�v
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AActionGameCharacter::OnResetVR);
}


void AActionGameCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AActionGameCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	bPressedJump = true;
}

void AActionGameCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	bPressedJump = false;
}

void AActionGameCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AActionGameCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

// �O��ړ�
void AActionGameCharacter::MoveForward(float Value)
{
	// �ړ��\�łȂ��Ȃ烊�^�[��
	if (CanMove() == false) return;

	if ((Controller != NULL) && (Value != 0.0f))
	{
		// �O������̎擾
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// �O������x�N�^�[���擾
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

// ���E�ړ�
void AActionGameCharacter::MoveRight(float Value)
{
	// �ړ��\�łȂ��Ȃ烊�^�[��
	if (CanMove() == false) return;

	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// ���E�����̎擾
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// ���E�����x�N�^�[�̎擾
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

// �ړ��\���ǂ����Ԃ�
bool AActionGameCharacter::CanMove()
{
	// �U��������𒆂��_���[�W���󂯂Ă���ꍇ��false
	if (Attacking || Avoiding || Damaging) return false;

	// �������Ă��Ȃ��Ȃ�true
	return true;
}

// �_���[�W����
void AActionGameCharacter::GiveDamage(AActor* actor, float defence)
{
	AController* PlayerController = GetController();

	// �_���[�W�v�Z
	float damage = MyParam.Power - defence;

	TSubclassOf<UDamageType> const ValidDamageTypeClass = TSubclassOf<UDamageType>(UDamageType::StaticClass());
	FDamageEvent DamageEvent(ValidDamageTypeClass);

	actor->TakeDamage(damage, DamageEvent, PlayerController, this);
}

// �R���W�����L����
void AActionGameCharacter::UseCollision(class UPrimitiveComponent* boxCol)
{
	boxCol->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

// �R���W����������
void AActionGameCharacter::UnUseCollision(class UPrimitiveComponent* boxCol_1, class UPrimitiveComponent* boxCol_2)
{
	boxCol_1->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	boxCol_2->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// �������
void AActionGameCharacter::AvoidAction()
{
	// ��𒆂Ȃ�
	if (Avoiding) {
		GetCharacterMovement()->Velocity = FVector(0, 0, 0);
		// �ʏ��Ԃ�
		Avoiding = false;
		// ���C��߂�
		GetCharacterMovement()->GroundFriction = 8.0f;
		// �R���W�����L����
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
		// Timer�̃��Z�b�g
		GetWorld()->GetTimerManager().ClearTimer(TimeHandle);
	}
	// ��𒆂łȂ��Ȃ�
	else {
		// �����Ԃ�
		Avoiding = true;
		// ���C�𖳎�
		GetCharacterMovement()->GroundFriction = 0.0f;
		// �R���W����������
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
		// �_�b�V��
		AvoidDash();
	}
}

// ������̃_�b�V��
void AActionGameCharacter::AvoidDash()
{
	// �_�b�V���̃x�N�g���̌v�Z
	FVector DashVec = GetCapsuleComponent()->GetForwardVector() * 3000.0f;
	// �_�b�V���J�n
	LaunchCharacter(DashVec, true, true);
	// Timer�̃Z�b�g
	GetWorld()->GetTimerManager().SetTimer(TimeHandle, this, &AActionGameCharacter::AvoidAction, 0.2f, false);
}
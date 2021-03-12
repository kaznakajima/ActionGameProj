// Fill out your copyright notice in the Description page of Project Settings.

#include "GameCharacterBase.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/DamageType.h"
#include "MyCharacterMovementComponent.h"
#include "Engine.h"

// Sets default values
AGameCharacterBase::AGameCharacterBase(const FObjectInitializer& ObjectInitilizer) : 
	Super(ObjectInitilizer.SetDefaultSubobjectClass<UMyCharacterMovementComponent>(CharacterMovementComponentName))
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

	IsActive = false;
}

// Called when the game starts or when spawned
void AGameCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGameCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AGameCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

// �O��ړ�
void AGameCharacterBase::MoveForward(float Value)
{
	// ���͂�Axis���i�[
	if (Value == 0.0f) InputVec.X = 0.0f;

	if ((Controller != NULL) && (Value != 0.0f))
	{
		// �O������̎擾
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// �O������x�N�^�[���擾
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		InputVec.X = Value;

		// �ړ��\�łȂ��Ȃ烊�^�[��
		if (CanMove() == false || IsDeath) return;

		AddMovementInput(Direction, Value);
		MakeNoise(1, this, GetActorLocation());
	}
}

// ���E�ړ�
void AGameCharacterBase::MoveRight(float Value)
{
	// ���͂�Axis���i�[
	if (Value == 0.0f) InputVec.Y = 0.0f;

	if ((Controller != NULL) && (Value != 0.0f))
	{
		// ���E�����̎擾
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// ���E�����x�N�^�[�̎擾
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		InputVec.Y = Value;

		// �ړ��\�łȂ��Ȃ烊�^�[��
		if (CanMove() == false || IsDeath) return;

		AddMovementInput(Direction, Value);
		MakeNoise(1, this, GetActorLocation());
	}
}

// �W�����v����
void AGameCharacterBase::Jump()
{
	// �W�����v�ɐ�����t����
	if (IsActive && Avoiding == false
		&& Damaging == false && Attacking == false && IsDeath == false) {
		bPressedJump = true;
		JumpKeyHoldTime = 0.0f;
	}
}

// �ړ��\���ǂ����Ԃ�
bool AGameCharacterBase::CanMove()
{
	// �U��������𒆂��_���[�W���󂯂Ă���ꍇ��false
	if (Attacking || Avoiding || Damaging || IsActive == false) return false;

	// �������Ă��Ȃ��Ȃ�true
	return true;
}

// �R���W�����L����
void AGameCharacterBase::OnUseCollision_Implementation(class UPrimitiveComponent* Col)
{
	if (Col != nullptr) Col->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

// �R���W����������
void AGameCharacterBase::OnUnUseCollision_Implementation(class UPrimitiveComponent* Col_1, class UPrimitiveComponent* Col_2)
{
	if (Col_1 != nullptr) Col_1->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if (Col_2 != nullptr)Col_2->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AGameCharacterBase::OnDamage_Implementation(AActor* actor, float defence)
{
	AController* PlayerController = GetController();

	// �_���[�W�v�Z
	float damage = MyParam.Power - defence;

	// �_���[�W�C�x���g�̎擾
	TSubclassOf<UDamageType> const ValidDamageTypeClass = TSubclassOf<UDamageType>(UDamageType::StaticClass());
	FDamageEvent DamageEvent(ValidDamageTypeClass);

	// �_���[�W����
	actor->TakeDamage(damage, DamageEvent, PlayerController, this);
}


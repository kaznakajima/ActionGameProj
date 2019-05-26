// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/DamageType.h"
#include "MyCharacterMovementComponent.h"
#include "Engine.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter(const FObjectInitializer& ObjectInitilizer)
	: Super(ObjectInitilizer.SetDefaultSubobjectClass<UMyCharacterMovementComponent>(CharacterMovementComponentName))
{
	// �J�v�Z���R���W�����̏�����
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// �L�����N�^�[�̌������J�����ɉe������Ȃ�
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// CharacterMovement�̃Z�b�g�A�b�v
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;
}


void AEnemyCharacter::Jump()
{
	if (Avoiding) AvoidCancel();
	bPressedJump = true;
	JumpKeyHoldTime = 0.0f;
}

// �ړ��\���ǂ����Ԃ�
bool AEnemyCharacter::CanMove()
{
	// �U��������𒆂��_���[�W���󂯂Ă���ꍇ��false
	if (Attacking || Avoiding || Damaging) return false;

	// �������Ă��Ȃ��Ȃ�true
	return true;
}

// �_���[�W����
void AEnemyCharacter::GiveDamage(AActor* actor, float defence)
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

// �R���W�����L����
void AEnemyCharacter::UseCollision(class UPrimitiveComponent* boxCol)
{
	boxCol->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

// �R���W����������
void AEnemyCharacter::UnUseCollision(class UPrimitiveComponent* boxCol_1, class UPrimitiveComponent* boxCol_2)
{
	boxCol_1->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	boxCol_2->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// �������
void AEnemyCharacter::AvoidAction()
{
	// ��𒆂Ȃ�
	if (Avoiding) return;
	// ��𒆂łȂ��Ȃ�
	else {
		// �U�����Ȃ�U�����~
		if (Attacking) Attacking = false;
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

// ����̃L�����Z��
void AEnemyCharacter::AvoidCancel()
{
	// Velocity�̃��Z�b�g
	GetCharacterMovement()->Velocity = FVector(0, 0, GetCharacterMovement()->Velocity.Z);
	// ���C��߂�
	GetCharacterMovement()->GroundFriction = 8.0f;
	// �R���W�����L����
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
	// Timer�̍Đݒ�
	GetWorld()->GetTimerManager().ClearTimer(TimeHandle);

	// �󒆔���
	UCharacterMovementComponent* MyCharacterMovement = GetCharacterMovement();
	bool IsAir = (MyCharacterMovement->MovementMode == EMovementMode::MOVE_Falling);

	// �󒆂̏�ԂȂ烊�^�[��
	if (IsAir) {
		// Timer�̃Z�b�g
		GetWorld()->GetTimerManager().SetTimer(TimeHandle, this, &AEnemyCharacter::AvoidCancel, 0.2f, false);
		return;
	}

	// �ʏ��Ԃ�
	Avoiding = false;
}

// ������̃_�b�V��
void AEnemyCharacter::AvoidDash()
{
	// ���݂̕����̎擾
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// �����x�N�^�[�̎擾
	const FVector DirectionY = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y) * InputVec.Y;
	const FVector DirectionX = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X) * InputVec.X;

	// ���͏�Ԃɂ���ă_�b�V���̕���������
	// ���͂��Ȃ��ꍇ�A�O�������Ƀ_�b�V��
	if (InputVec.X == 0 && InputVec.Y == 0) DashVec = GetCapsuleComponent()->GetForwardVector();
	// ���͂�����ꍇ�A���͕����Ƀ_�b�V��
	else if (InputVec.X != 0 || InputVec.Y != 0) {
		DashVec = DirectionY + DirectionX;
		DashVec.Normalize();

		// �ڕW�̕������擾
		FRotator myRotate = DashVec.Rotation();
		SetActorRotation(myRotate, ETeleportType::TeleportPhysics);
	}

	// �_�b�V���J�n
	LaunchCharacter(DashVec * 3000.0f, true, true);
	// Timer�̃Z�b�g
	GetWorld()->GetTimerManager().SetTimer(TimeHandle, this, &AEnemyCharacter::AvoidCancel, 0.2f, false);
}


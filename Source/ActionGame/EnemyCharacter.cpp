// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/DamageType.h"
#include "MyCharacterMovementComponent.h"
#include "Engine.h"

// �R���X�g���N�^
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

// �ړ��\���ǂ����Ԃ�
bool AEnemyCharacter::CanMove()
{
	// �U��������𒆂��_���[�W���󂯂Ă���ꍇ��false
	if (Attacking || Damaging) return false;

	// �������Ă��Ȃ��Ȃ�true
	return true;
}

// �_���[�W�C�x���g
void AEnemyCharacter::OnDamage_Implementation(AActor* actor, float defence)
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
void AEnemyCharacter::OnUseCollision_Implementation(class UPrimitiveComponent* Col)
{
	if (Col != nullptr) Col->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

// �R���W����������
void AEnemyCharacter::OnUnUseCollision_Implementation(class UPrimitiveComponent* Col_1, class UPrimitiveComponent* Col_2)
{
	if (Col_1 != nullptr) Col_1->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if (Col_2 != nullptr)Col_2->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

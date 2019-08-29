// Fill out your copyright notice in the Description page of Project Settings.

#include "BossCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/DamageType.h"
#include "MyCharacterMovementComponent.h"
#include "Engine.h"

ABossCharacter::ABossCharacter(const FObjectInitializer& ObjectInitilizer)
	: Super(ObjectInitilizer.SetDefaultSubobjectClass<UMyCharacterMovementComponent>(CharacterMovementComponentName))
{
	// カプセルコリジョンの初期化
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// キャラクターの向きをカメラに影響されない
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// CharacterMovementのセットアップ
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;
}

// ダメージ処理
void ABossCharacter::GiveDamage(AActor* actor, float defence)
{
	AController* PlayerController = GetController();

	// ダメージ計算
	float damage = MyParam.Power - defence;

	// ダメージイベントの取得
	TSubclassOf<UDamageType> const ValidDamageTypeClass = TSubclassOf<UDamageType>(UDamageType::StaticClass());
	FDamageEvent DamageEvent(ValidDamageTypeClass);

	// ダメージ処理
	actor->TakeDamage(damage, DamageEvent, PlayerController, this);
}

// コリジョン有効化
void ABossCharacter::UseCollision(class UPrimitiveComponent* boxCol)
{
	boxCol->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

// コリジョン無効化
void ABossCharacter::UnUseCollision(class UPrimitiveComponent* boxCol_1, class UPrimitiveComponent* boxCol_2)
{
	boxCol_1->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	boxCol_2->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}


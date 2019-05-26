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


void AEnemyCharacter::Jump()
{
	if (Avoiding) AvoidCancel();
	bPressedJump = true;
	JumpKeyHoldTime = 0.0f;
}

// 移動可能かどうか返す
bool AEnemyCharacter::CanMove()
{
	// 攻撃中か回避中かダメージを受けている場合はfalse
	if (Attacking || Avoiding || Damaging) return false;

	// 何もしていないならtrue
	return true;
}

// ダメージ処理
void AEnemyCharacter::GiveDamage(AActor* actor, float defence)
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
void AEnemyCharacter::UseCollision(class UPrimitiveComponent* boxCol)
{
	boxCol->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

// コリジョン無効化
void AEnemyCharacter::UnUseCollision(class UPrimitiveComponent* boxCol_1, class UPrimitiveComponent* boxCol_2)
{
	boxCol_1->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	boxCol_2->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// 回避処理
void AEnemyCharacter::AvoidAction()
{
	// 回避中なら
	if (Avoiding) return;
	// 回避中でないなら
	else {
		// 攻撃中なら攻撃中止
		if (Attacking) Attacking = false;
		// 回避状態へ
		Avoiding = true;
		// 摩擦を無視
		GetCharacterMovement()->GroundFriction = 0.0f;
		// コリジョン無効化
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
		// ダッシュ
		AvoidDash();
	}
}

// 回避のキャンセル
void AEnemyCharacter::AvoidCancel()
{
	// Velocityのリセット
	GetCharacterMovement()->Velocity = FVector(0, 0, GetCharacterMovement()->Velocity.Z);
	// 摩擦を戻す
	GetCharacterMovement()->GroundFriction = 8.0f;
	// コリジョン有効化
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
	// Timerの再設定
	GetWorld()->GetTimerManager().ClearTimer(TimeHandle);

	// 空中判定
	UCharacterMovementComponent* MyCharacterMovement = GetCharacterMovement();
	bool IsAir = (MyCharacterMovement->MovementMode == EMovementMode::MOVE_Falling);

	// 空中の状態ならリターン
	if (IsAir) {
		// Timerのセット
		GetWorld()->GetTimerManager().SetTimer(TimeHandle, this, &AEnemyCharacter::AvoidCancel, 0.2f, false);
		return;
	}

	// 通常状態へ
	Avoiding = false;
}

// 回避時のダッシュ
void AEnemyCharacter::AvoidDash()
{
	// 現在の方向の取得
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// 方向ベクターの取得
	const FVector DirectionY = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y) * InputVec.Y;
	const FVector DirectionX = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X) * InputVec.X;

	// 入力状態によってダッシュの方向を決定
	// 入力がない場合、前方方向にダッシュ
	if (InputVec.X == 0 && InputVec.Y == 0) DashVec = GetCapsuleComponent()->GetForwardVector();
	// 入力がある場合、入力方向にダッシュ
	else if (InputVec.X != 0 || InputVec.Y != 0) {
		DashVec = DirectionY + DirectionX;
		DashVec.Normalize();

		// 目標の方向を取得
		FRotator myRotate = DashVec.Rotation();
		SetActorRotation(myRotate, ETeleportType::TeleportPhysics);
	}

	// ダッシュ開始
	LaunchCharacter(DashVec * 3000.0f, true, true);
	// Timerのセット
	GetWorld()->GetTimerManager().SetTimer(TimeHandle, this, &AEnemyCharacter::AvoidCancel, 0.2f, false);
}


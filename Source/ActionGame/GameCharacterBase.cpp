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
	// カプセルコリジョンの初期化
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// 回転力の初期化
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// キャラクターの向きをカメラに影響されない
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// CharacterMovementのセットアップ
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// SpringArmのセットアップ
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f;
	CameraBoom->bUsePawnControlRotation = true;

	// カメラのセットアップ
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

// 前後移動
void AGameCharacterBase::MoveForward(float Value)
{
	// 入力のAxisを格納
	if (Value == 0.0f) InputVec.X = 0.0f;

	if ((Controller != NULL) && (Value != 0.0f))
	{
		// 前後方向の取得
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// 前後方向ベクターを取得
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		InputVec.X = Value;

		// 移動可能でないならリターン
		if (CanMove() == false || IsDeath) return;

		AddMovementInput(Direction, Value);
		MakeNoise(1, this, GetActorLocation());
	}
}

// 左右移動
void AGameCharacterBase::MoveRight(float Value)
{
	// 入力のAxisを格納
	if (Value == 0.0f) InputVec.Y = 0.0f;

	if ((Controller != NULL) && (Value != 0.0f))
	{
		// 左右方向の取得
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// 左右方向ベクターの取得
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		InputVec.Y = Value;

		// 移動可能でないならリターン
		if (CanMove() == false || IsDeath) return;

		AddMovementInput(Direction, Value);
		MakeNoise(1, this, GetActorLocation());
	}
}

// ジャンプ処理
void AGameCharacterBase::Jump()
{
	// ジャンプに制限を付ける
	if (IsActive && Avoiding == false
		&& Damaging == false && Attacking == false && IsDeath == false) {
		bPressedJump = true;
		JumpKeyHoldTime = 0.0f;
	}
}

// 移動可能かどうか返す
bool AGameCharacterBase::CanMove()
{
	// 攻撃中か回避中かダメージを受けている場合はfalse
	if (Attacking || Avoiding || Damaging || IsActive == false) return false;

	// 何もしていないならtrue
	return true;
}

// コリジョン有効化
void AGameCharacterBase::OnUseCollision_Implementation(class UPrimitiveComponent* Col)
{
	if (Col != nullptr) Col->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

// コリジョン無効化
void AGameCharacterBase::OnUnUseCollision_Implementation(class UPrimitiveComponent* Col_1, class UPrimitiveComponent* Col_2)
{
	if (Col_1 != nullptr) Col_1->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if (Col_2 != nullptr)Col_2->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AGameCharacterBase::OnDamage_Implementation(AActor* actor, float defence)
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


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

AActionGameCharacter::AActionGameCharacter(const FObjectInitializer& ObjectInitilizer) 
	: Super(ObjectInitilizer.SetDefaultSubobjectClass<UMyCharacterMovementComponent>(CharacterMovementComponentName))
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
}

//////////////////////////////////////////////////////////////////////////
// Input

void AActionGameCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// キー入力のセットアップ
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AActionGameCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AActionGameCharacter::MoveRight);

	// カメラ回転入力
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AActionGameCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AActionGameCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AActionGameCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AActionGameCharacter::TouchStopped);

	// VRのセットアップ
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

// 前後移動
void AActionGameCharacter::MoveForward(float Value)
{
	// 移動可能でないならリターン
	if (CanMove() == false) return;

	if ((Controller != NULL) && (Value != 0.0f))
	{
		// 前後方向の取得
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// 前後方向ベクターを取得
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

// 左右移動
void AActionGameCharacter::MoveRight(float Value)
{
	// 移動可能でないならリターン
	if (CanMove() == false) return;

	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// 左右方向の取得
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// 左右方向ベクターの取得
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

// 移動可能かどうか返す
bool AActionGameCharacter::CanMove()
{
	// 攻撃中か回避中かダメージを受けている場合はfalse
	if (Attacking || Avoiding || Damaging) return false;

	// 何もしていないならtrue
	return true;
}

// ダメージ処理
void AActionGameCharacter::GiveDamage(AActor* actor, float defence)
{
	AController* PlayerController = GetController();

	// ダメージ計算
	float damage = MyParam.Power - defence;

	TSubclassOf<UDamageType> const ValidDamageTypeClass = TSubclassOf<UDamageType>(UDamageType::StaticClass());
	FDamageEvent DamageEvent(ValidDamageTypeClass);

	actor->TakeDamage(damage, DamageEvent, PlayerController, this);
}

// コリジョン有効化
void AActionGameCharacter::UseCollision(class UPrimitiveComponent* boxCol)
{
	boxCol->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

// コリジョン無効化
void AActionGameCharacter::UnUseCollision(class UPrimitiveComponent* boxCol_1, class UPrimitiveComponent* boxCol_2)
{
	boxCol_1->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	boxCol_2->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// 回避処理
void AActionGameCharacter::AvoidAction(bool isAvoid)
{
	// 空中判定
	UCharacterMovementComponent* myMovement = GetCharacterMovement();
	bool IsAir = (myMovement->MovementMode == EMovementMode::MOVE_Falling);

	// プレイヤーの移動量
	FVector playerVelocity = GetCharacterMovement()->Velocity;

	// 回避中なら
	if (isAvoid) {
		if (IsAir) GetCharacterMovement()->Velocity = FVector(0, 0, 0);
		// 通常状態へ
		Avoiding = false;
		// コリジョン有効化
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
	}
	// 回避中でないなら
	else {
		if (playerVelocity.X == 0 || playerVelocity.Y == 0) return;
		// 回避状態へ
		Avoiding = true;
		// コリジョン無効化
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	}
}

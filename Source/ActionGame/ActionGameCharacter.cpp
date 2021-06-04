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

// コンストラクタ
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

	IsActive = false;
}

// 入力イベントのセットアップ
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
}

// キャラクターの回転
void AActionGameCharacter::TurnAtRate(float Rate)
{
	// 有効でないならリターン
	if (IsActive == false) return;

	// 左右回転
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

// キャラクターの回転
void AActionGameCharacter::LookUpAtRate(float Rate)
{
	// 有効でないならリターン
	if (IsActive == false) return;

	// 上下回転
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

// 自身の位置を初期位置に
void AActionGameCharacter::InitPosition()
{
	SetActorLocation(InitPos);

	// 向きもリセット
	FRotator myRotate(0, -180, 0);
	SetActorRotation(myRotate, ETeleportType::TeleportPhysics);
}

// 攻撃時に自動ターゲットをするかどうか
bool AActionGameCharacter::CheckTargetForcus()
{
	if (InputVec == FVector::ZeroVector) return true;

	return false;
}

// 入力方向に攻撃を行う
void AActionGameCharacter::AttackDirectionForcus()
{
	// 入力方向を取得
	AttackVec = GetInputVector();
	AttackVec.Normalize();

	// 入力方向から向きを取得
	FRotator myRotate = AttackVec.Rotation();
	SetActorRotation(myRotate, ETeleportType::TeleportPhysics);
}

// 入力方向を返す
FVector AActionGameCharacter::GetInputVector()
{
	// 現在の方向の取得
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// 方向ベクターの取得
	const FVector DirectionY = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y) * InputVec.Y;
	const FVector DirectionX = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X) * InputVec.X;

	// ここに入力方向を格納する
	const FVector Vec = DirectionX + DirectionY;

	return Vec;
}

// 空中攻撃処理
bool AActionGameCharacter::CheckFlyAttack()
{
	// 自身のMovementを取得
	UCharacterMovementComponent* MyCharacterMovement = GetCharacterMovement();
	// 落下しているかどうか
	bool IsAir = (MyCharacterMovement->MovementMode == EMovementMode::MOVE_Falling);

	// 空中攻撃がすでに行われていない場合実行(空中攻撃は原則1回)
	if (IsAir && AirAttackCount < 3) {
		MyCharacterMovement->GravityScale = 0.0f;
		MyCharacterMovement->Velocity = FVector(0.0f, 0.0f, 0.0f);
		AirAttackCount++;
		return true;
	}
	else if(AirAttackCount >= 3) {
		CancelFlyAttack();
		return false;
	}
	return true;
}

// 空中攻撃中止
void AActionGameCharacter::CancelFlyAttack()
{
	// 自身のMovementを取得
	UCharacterMovementComponent* MyCharacterMovement = GetCharacterMovement();
	// 落下しているかどうか
	bool IsAir = (MyCharacterMovement->MovementMode == EMovementMode::MOVE_Falling);
	if(IsAir == false) AirAttackCount = 0;
	MyCharacterMovement->GravityScale = 1.0f;
}

// 回避処理
void AActionGameCharacter::AvoidAction()
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
		GetCapsuleComponent()->SetCollisionObjectType(ECollisionChannel::ECC_PhysicsBody);
		// ダッシュ
		AvoidDash();
	}
}

// 回避のキャンセル
void AActionGameCharacter::AvoidCancel()
{
	// Velocityのリセット
	GetCharacterMovement()->Velocity = FVector(0, 0, GetCharacterMovement()->Velocity.Z);
	// 摩擦を戻す
	GetCharacterMovement()->GroundFriction = 8.0f;
	// コリジョン有効化
	GetCapsuleComponent()->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	// Timerの再設定
	GetWorld()->GetTimerManager().ClearTimer(TimeHandle);

	// 通常状態へ
	Avoiding = false;

	UE_LOG(LogTemp, Display, TEXT("AvoidCancel"));
}

// 回避時のダッシュ
void AActionGameCharacter::AvoidDash()
{
	// 入力状態によってダッシュの方向を決定
	// 入力がない場合、前方方向にダッシュ
	if (InputVec.X == 0 && InputVec.Y == 0) {
		DashVec = GetCapsuleComponent()->GetForwardVector();
		DashVec.Normalize();
	}
	// 入力がある場合、入力方向にダッシュ
	else if (InputVec.X != 0 || InputVec.Y != 0) {
		DashVec = GetInputVector();
		DashVec.Normalize();

		// 目標の方向を取得
		FRotator myRotate = DashVec.Rotation();
		SetActorRotation(myRotate, ETeleportType::TeleportPhysics);

		DashVec = GetCapsuleComponent()->GetForwardVector();
		DashVec.Normalize();
	}

	// ダッシュ開始
	DashVec.Z = -0.1f;
	GetCharacterMovement()->Velocity = FVector(0, 0, GetCharacterMovement()->Velocity.Z);
	LaunchCharacter(DashVec * 1500.0f, false, true);
}
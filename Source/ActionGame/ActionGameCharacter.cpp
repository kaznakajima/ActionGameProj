#include "ActionGameCharacter.h"

//#include "Public/DirectInputPadJoystick.h"

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
		if (CanMove() == false) return;

		AddMovementInput(Direction, Value);
	}
}

// 左右移動
void AActionGameCharacter::MoveRight(float Value)
{
	// 入力のAxisを格納
	if (Value == 0.0f) InputVec.Y = 0.0f;

	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// 左右方向の取得
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// 左右方向ベクターの取得
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		InputVec.Y = Value;

		// 移動可能でないならリターン
		if (CanMove() == false) return;

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

	// ダメージイベントの取得
	TSubclassOf<UDamageType> const ValidDamageTypeClass = TSubclassOf<UDamageType>(UDamageType::StaticClass());
	FDamageEvent DamageEvent(ValidDamageTypeClass);

	// ダメージ処理
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
void AActionGameCharacter::AvoidAction()
{
	// 回避中なら
	if (Avoiding) AvoidCancel();
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
void AActionGameCharacter::AvoidCancel()
{
	// Velocityのリセット
	GetCharacterMovement()->Velocity = FVector(0, 0, 0);
	// 通常状態へ
	Avoiding = false;
	// 摩擦を戻す
	GetCharacterMovement()->GroundFriction = 8.0f;
	// コリジョン有効化
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
	// Timerのリセット
	GetWorld()->GetTimerManager().ClearTimer(TimeHandle);
}

// 回避時のダッシュ
void AActionGameCharacter::AvoidDash()
{
	// 現在の方向の取得
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// 方向ベクターの取得
	const FVector DirectionY = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y) * InputVec.Y;
	const FVector DirectionX = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X) * InputVec.X;

	// 入力状態によってダッシュの方向を決定
	// 入力がない場合、前方方向にダッシュ
	if(InputVec.X == 0 && InputVec.Y == 0) DashVec = GetCapsuleComponent()->GetForwardVector();
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
	GetWorld()->GetTimerManager().SetTimer(TimeHandle, this, &AActionGameCharacter::AvoidCancel, 0.2f, false);
}

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

	IsActive = false;
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
}

// �L�����N�^�[�̉�]
void AActionGameCharacter::TurnAtRate(float Rate)
{
	// �L���łȂ��Ȃ烊�^�[��
	if (IsActive == false) return;

	// ���E��]
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

// �L�����N�^�[�̉�]
void AActionGameCharacter::LookUpAtRate(float Rate)
{
	// �L���łȂ��Ȃ烊�^�[��
	if (IsActive == false) return;

	// �㉺��]
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

// ���g�̈ʒu�������ʒu��
void AActionGameCharacter::InitPosition()
{
	SetActorLocation(InitPos);

	// ���������Z�b�g
	FRotator myRotate(0, -180, 0);
	SetActorRotation(myRotate, ETeleportType::TeleportPhysics);
}

// �O��ړ�
void AActionGameCharacter::MoveForward(float Value)
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
void AActionGameCharacter::MoveRight(float Value)
{
	// ���͂�Axis���i�[
	if (Value == 0.0f) InputVec.Y = 0.0f;

	if ( (Controller != NULL) && (Value != 0.0f) )
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
void AActionGameCharacter::Jump()
{
	// �W�����v�ɐ�����t����
	if (IsActive && Avoiding == false 
		&& Damaging == false && Attacking == false && IsDeath == false) {
		bPressedJump = true;
		JumpKeyHoldTime = 0.0f;
	}
}

// �ړ��\���ǂ����Ԃ�
bool AActionGameCharacter::CanMove()
{
	// �U��������𒆂��_���[�W���󂯂Ă���ꍇ��false
	if (Attacking || Avoiding || Damaging || IsActive == false) return false;

	// �������Ă��Ȃ��Ȃ�true
	return true;
}

// �U�����Ɏ����^�[�Q�b�g�����邩�ǂ���
bool AActionGameCharacter::CheckTargetForcus()
{
	if (InputVec == FVector::ZeroVector) return true;

	return false;
}

// ���͕����ɍU�����s��
void AActionGameCharacter::AttackDirectionForcus()
{
	// ���͕������擾
	AttackVec = GetInputVector();
	AttackVec.Normalize();

	// ���͕�������������擾
	FRotator myRotate = AttackVec.Rotation();
	SetActorRotation(myRotate, ETeleportType::TeleportPhysics);
}

// ���͕�����Ԃ�
FVector AActionGameCharacter::GetInputVector()
{
	// ���݂̕����̎擾
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// �����x�N�^�[�̎擾
	const FVector DirectionY = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y) * InputVec.Y;
	const FVector DirectionX = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X) * InputVec.X;

	// �����ɓ��͕������i�[����
	const FVector Vec = DirectionX + DirectionY;

	return Vec;
}

// �󒆍U������
bool AActionGameCharacter::CheckFlyAttack()
{
	// ���g��Movement���擾
	UCharacterMovementComponent* MyCharacterMovement = GetCharacterMovement();
	// �������Ă��邩�ǂ���
	bool IsAir = (MyCharacterMovement->MovementMode == EMovementMode::MOVE_Falling);

	// �󒆍U�������łɍs���Ă��Ȃ��ꍇ���s(�󒆍U���͌���1��)
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

// �󒆍U�����~
void AActionGameCharacter::CancelFlyAttack()
{
	// ���g��Movement���擾
	UCharacterMovementComponent* MyCharacterMovement = GetCharacterMovement();
	// �������Ă��邩�ǂ���
	bool IsAir = (MyCharacterMovement->MovementMode == EMovementMode::MOVE_Falling);
	if(IsAir == false) AirAttackCount = 0;
	MyCharacterMovement->GravityScale = 1.0f;
}

// �������
void AActionGameCharacter::AvoidAction()
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
		GetCapsuleComponent()->SetCollisionObjectType(ECollisionChannel::ECC_PhysicsBody);
		// �_�b�V��
		AvoidDash();
	}
}

// ����̃L�����Z��
void AActionGameCharacter::AvoidCancel()
{
	// Velocity�̃��Z�b�g
	GetCharacterMovement()->Velocity = FVector(0, 0, GetCharacterMovement()->Velocity.Z);
	// ���C��߂�
	GetCharacterMovement()->GroundFriction = 8.0f;
	// �R���W�����L����
	GetCapsuleComponent()->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	// Timer�̍Đݒ�
	GetWorld()->GetTimerManager().ClearTimer(TimeHandle);

	// �ʏ��Ԃ�
	Avoiding = false;
}

// ������̃_�b�V��
void AActionGameCharacter::AvoidDash()
{
	// ���͏�Ԃɂ���ă_�b�V���̕���������
	// ���͂��Ȃ��ꍇ�A�O�������Ƀ_�b�V��
	if(InputVec.X == 0 && InputVec.Y == 0) DashVec = GetCapsuleComponent()->GetForwardVector();
	// ���͂�����ꍇ�A���͕����Ƀ_�b�V��
	else if (InputVec.X != 0 || InputVec.Y != 0) {
		DashVec = GetInputVector();
		DashVec.Normalize();

		// �ڕW�̕������擾
		FRotator myRotate = DashVec.Rotation();
		SetActorRotation(myRotate, ETeleportType::TeleportPhysics);
	}

	// �_�b�V���J�n
	DashVec.Z = -1.0f;
	LaunchCharacter(DashVec * 3000.0f, true, true);
	// Timer�̃Z�b�g
	GetWorld()->GetTimerManager().SetTimer(TimeHandle, this, &AActionGameCharacter::AvoidCancel, 0.2f, false);
}

void AActionGameCharacter::OnDamage_Implementation(AActor* actor, float defence)
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
void AActionGameCharacter::OnUseCollision_Implementation(class UPrimitiveComponent* Col)
{
	if(Col != nullptr) Col->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

// �R���W����������
void AActionGameCharacter::OnUnUseCollision_Implementation(class UPrimitiveComponent* Col_1, class UPrimitiveComponent* Col_2)
{
	if(Col_1 != nullptr) Col_1->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if(Col_2 != nullptr)Col_2->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
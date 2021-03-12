// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterInterface.h"
#include "GameCharacterBase.generated.h"

UCLASS(config = Game)
class ACTIONGAME_API AGameCharacterBase : public ACharacter, public ICharacterInterface
{
	GENERATED_BODY()
public:

	// �R���X�g���N�^
	AGameCharacterBase(const FObjectInitializer& ObjectInitilizer);

	// ���E��]
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	// �㉺��]
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

	// �L�����N�^�[�X�e�[�^�X
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterParam")
		FCharacterStatus MyParam;

	// �ړ��ł��邩�ǂ����Ԃ�
	bool CanMove();

	// �_���[�W�C�x���g
	void OnDamage_Implementation(AActor* actor, float defence) override;

	// �R���W�����L����
	void OnUseCollision_Implementation(class UPrimitiveComponent* Col) override;

	// �R���W����������
	void OnUnUseCollision_Implementation(class UPrimitiveComponent* Col_1, class UPrimitiveComponent* Col_2) override;

	// �L�����ǂ���
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterParam")
		bool IsActive;

	// ���S����
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterParam")
		bool IsDeath;

	// �U�������ǂ���
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterParam")
		bool Attacking;

	// ��𒆂��ǂ���
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterParam")
		bool Avoiding;

	// �_���[�W���󂯂Ă��邩
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterParam")
		bool Damaging;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// �O��ړ�
	void MoveForward(float Value);

	// ���E�ړ�
	void MoveRight(float Value);

	// �W�����v
	void Jump();

	// SpringArm�̒�`
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	// �J�����̒�`
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;

	// �_�b�V�������x�N�^�[
	UPROPERTY()
		FVector DashVec;
	// �U�������x�N�^�[
	UPROPERTY()
		FVector AttackVec;
	// ���͕����x�N�^�[
	UPROPERTY()
		FVector InputVec;
	// Timer���Ǘ�����ϐ�
	UPROPERTY()
		FTimerHandle TimeHandle;

	// ���݂�HP
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterParam")
		float CurrentHP;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// SpringArm��Ԃ�
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	// �J������Ԃ�
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

};

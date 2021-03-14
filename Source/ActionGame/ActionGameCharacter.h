// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterInterface.h"
#include "ActionGameCharacter.generated.h"

UCLASS(config=Game)
class AActionGameCharacter : public ACharacter, public ICharacterInterface
{
	GENERATED_BODY()

	// SpringArm�̒�`
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	// �J�����̒�`
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:

	// �R���X�g���N�^
	AActionGameCharacter(const FObjectInitializer& ObjectInitilizer);

	// ���E��]
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	// �㉺��]
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	// �L�����N�^�[�X�e�[�^�X
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterParam")
	FCharacterStatus MyParam;

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
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="CharacterParam")
	bool Attacking;

	// ��𒆂��ǂ���
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterParam")
	bool Avoiding;

	// �_���[�W���󂯂Ă��邩
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterParam")
	bool Damaging;

protected:

	// ���S�C�x���g
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Action")
	void PlayerDeath();

	// �O��ړ�
	void MoveForward(float Value);

	// ���E�ړ�
	void MoveRight(float Value);

	// �W�����v
	void Jump();

	// ���E���_�ύX
	void TurnAtRate(float Rate);

	// �㉺���_�ύX
	void LookUpAtRate(float Rate);

	// ���g�̈ʒu�������ʒu��
	UFUNCTION(BlueprintCallable, Category = "CharacterPos")
	void InitPosition();

	// �_�b�V���U�����ł��邩�ǂ���
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterParam")
	bool CanDashAttack;

	// �ړ��ł��邩�ǂ����Ԃ�
	bool CanMove();

	// �U�����Ɏ����^�[�Q�b�g�����邩
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "PlayerAction")
	bool CheckTargetForcus();

	// ���͕����ɍU�����s��
	UFUNCTION(BlueprintCallable, Category = "PlayerAction")
	void AttackDirectionForcus();

	// ���͕����x�N�^�[��Ԃ�
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "CharacterParam")
	FVector GetInputVector();

	// �󒆍U�����~
	UFUNCTION(BlueprintCallable, Category = "PlayerAction")
	void CancelFlyAttack();

	// �󒆍U������
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "PlayerAction")
	bool CheckFlyAttack();

	// �������
	UFUNCTION(BlueprintCallable, Category = "PlayerAction")
	void AvoidAction();
	// ������̃_�b�V��
	UFUNCTION()
	void AvoidDash();
	// ����̃L�����Z��
	UFUNCTION(BlueprintCallable, Category = "PlayerAction")
	void AvoidCancel();

	// ���g�̏����ʒu
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterParam")
	FVector InitPos;
	// �󒆍U����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterParam")
	int AirAttackCount;
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

protected:

	// ���͏��̃Z�b�g�A�b�v
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	// SpringArm��Ԃ�
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	// �J������Ԃ�
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};


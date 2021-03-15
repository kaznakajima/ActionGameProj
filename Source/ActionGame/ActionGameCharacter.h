// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CharacterInterface.h"
#include "GameCharacterBase.h"
#include "ActionGameCharacter.generated.h"

UCLASS(config=Game)
class AActionGameCharacter : public AGameCharacterBase
{
	GENERATED_BODY()
public:

	// �R���X�g���N�^
	AActionGameCharacter(const FObjectInitializer& ObjectInitilizer);

protected:

	// ���S�C�x���g
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Action")
	void PlayerDeath();

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

protected:

	// ���͏��̃Z�b�g�A�b�v
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};


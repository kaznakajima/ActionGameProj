// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterInterface.h"
#include "ActionGameCharacter.generated.h"

UCLASS(config=Game)
class AActionGameCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	AActionGameCharacter(const FObjectInitializer& ObjectInitilizer);

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	// �L�����N�^�[�X�e�[�^�X
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterParam")
	FCharacterStatus MyParam;
	
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

	// �_���[�W����
	UFUNCTION(BlueprintCallable, Category = "Camera")
	void CameraReset();

	// �_�b�V���U�����ł��邩�ǂ���
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterParam")
	bool CanDashAttack;

	// �ړ��ł��邩�ǂ����Ԃ�
	bool CanMove();

	// �_���[�W����
	UFUNCTION(BlueprintCallable, Category = "Status")
	void GiveDamage(AActor* actor, float defence);

	// �U������p�R���W�����L����
	UFUNCTION(BlueprintCallable, Category = "Collision")
	void UseCollision(class UPrimitiveComponent* boxCol);

	// �U������p�R���W����������
	UFUNCTION(BlueprintCallable, Category = "Collision")
	void UnUseCollision(class UPrimitiveComponent* boxCol_1, class UPrimitiveComponent* boxCol_2);

	// �󒆍U������
	UFUNCTION(BlueprintCallable, Category = "PlayerAction")
	void CheckFlyAttack(bool isAttack);

	// �������
	UFUNCTION(BlueprintCallable, Category = "PlayerAction")
	void AvoidAction();
	// ����̃L�����Z��
	UFUNCTION(BlueprintCallable, Category = "PlayerAction")
	void AvoidCancel();

	// �_�b�V�������x�N�^�[
	UPROPERTY()
	FVector DashVec;
	// ���͕����x�N�^�[
	UPROPERTY()
	FVector InputVec;
	// Timer���Ǘ�����ϐ�
	UPROPERTY()
	FTimerHandle TimeHandle;
	// ������̃_�b�V��
	UFUNCTION()
	void AvoidDash();

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};


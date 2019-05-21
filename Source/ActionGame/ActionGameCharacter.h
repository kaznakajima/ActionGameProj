// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ActionGameCharacter.generated.h"

// �L�����N�^�[�̃X�e�[�^�X
USTRUCT(BlueprintType)
struct FCharacterStatus
{
	GENERATED_USTRUCT_BODY()

	// �L�����N�^�[ID
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterStatus")
	int ID;

	// �L�����N�^�[��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterStatus")
	FText Name;

	// �ő�HP
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterStatus")
	float MaxHP;
	
	// �U����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterStatus")
	float Power;
	
	// �����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterStatus")
	float Defence;
};

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

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	// �W�����v
	void Jump();

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

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
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};


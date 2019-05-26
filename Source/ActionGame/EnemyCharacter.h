// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterInterface.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class ACTIONGAME_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter(const FObjectInitializer& ObjectInitilizer);

	// �L�����N�^�[�X�e�[�^�X
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterParam")
		FCharacterStatus MyParam;

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

	// �W�����v
	void Jump();

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

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterInterface.h"
#include "EnemyCharacter.generated.h"

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	IDLE UMETA(DisplayName = "IDLE"),
	MOVE UMETA(DisplayName = "MOVE"),
	ATTACK UMETA(DisplayName = "ATTACK"),
};

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

	// �L�����N�^�[�̃X�e�[�g
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterParam")
	EEnemyState MyState;

	// �U�������ǂ���
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterParam")
	bool Attacking;

	// �_���[�W���󂯂Ă��邩
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterParam")
	bool Damaging;

protected:

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Action")
	void EnemyAction();
 
	// �W�����v
	void Jump();

	// �ړ��ł��邩�ǂ����Ԃ�
	bool CanMove();

	// HP
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Status")
	float CurrentHP;

	// �_���[�W����
	UFUNCTION(BlueprintCallable, Category = "Status")
	void GiveDamage(AActor* actor, float defence);

	// �U������p�R���W�����L����
	UFUNCTION(BlueprintCallable, Category = "Collision")
	void UseCollision(class UPrimitiveComponent* boxCol);

	// �U������p�R���W����������
	UFUNCTION(BlueprintCallable, Category = "Collision")
	void UnUseCollision(class UPrimitiveComponent* boxCol_1, class UPrimitiveComponent* boxCol_2);

	// Timer���Ǘ�����ϐ�
	UPROPERTY()
	FTimerHandle TimeHandle;
};

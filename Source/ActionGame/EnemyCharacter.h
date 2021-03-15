// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameCharacterBase.h"
#include "CharacterInterface.h"
#include "EnemyCharacter.generated.h"

// �G�l�~�[�̃X�e�[�g
UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	IDLE UMETA(DisplayName = "IDLE"),
	PATOROL UMETA(DisplayName = "PATROL"),
	MOVE UMETA(DisplayName = "MOVE"),
	ATTACK UMETA(DisplayName = "ATTACK")
};

// �G�l�~�[�̃^�C�v(�ߋ����A������)
UENUM(BlueprintType)
enum class EEnemyType : uint8
{
	SHORT_RANGE UMETA(DisplayName = "Short"),
	LONG_RANGE   UMETA(DisplayName = "Long")
};

// ���I�}���`�L���X�g�f���Q�[�g(�C�x���g�f�B�X�p�b�`���[)�̐錾
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEnemyOnEventDispather, float, Delation, float, Interval);

UCLASS()
class ACTIONGAME_API AEnemyCharacter : public AGameCharacterBase
{
	GENERATED_BODY()

public:
	// �R���X�g���N�^
	AEnemyCharacter(const FObjectInitializer& ObjectInitilizer);

	// ���I�}���`�L���X�g�f���Q�[�g(�C�x���g�f�B�X�p�b�`���[)�̒�`
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Event")
	FEnemyOnEventDispather OnHitStop;

	// �L�����N�^�[�̃^�C�v
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterParam")
	EEnemyType MyType;

	// �L�����N�^�[�̃X�e�[�g
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterParam")
	EEnemyState MyState;

	// �v���C���[��⑫���Ă��邩
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterParam")
	bool IsSerch;

	// �^�[�Q�b�gActor
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterParam")
	AActor* TargetActor;

protected:

	// �G�̃A�N�V���������C�x���g(�U������)
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Action")
	void EnemyPreparationAction();

	// ���S�C�x���g
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Action")
	void EnemyDeath();

	// �^�[�Q�b�g���m
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Action")
	void BeginSerch(AActor* _actor);
	// �^�[�Q�b�g���X�g
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Action")
	void SerchLost();
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterInterface.h"
#include "BossCharacter.generated.h"

// �G�l�~�[�̃X�e�[�g
UENUM(BlueprintType)
enum class EBossState : uint8
{
	PATOROL UMETA(DisplayName = "PATROL"),
	BATTLE UMETA(DisplayName = "BATTLE"),
	LONGBATTLE UMETA(DisplayName = "LONGRANGE")
};

UCLASS()
class ACTIONGAME_API ABossCharacter : public ACharacter, public ICharacterInterface
{
	GENERATED_BODY()

public:
	// �R���X�g���N�^
	ABossCharacter(const FObjectInitializer& ObjectInitilizer);

	// �L�����N�^�[�X�e�[�^�X
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterParam")
	FCharacterStatus MyParam;

	// �_���[�W�C�x���g
	void OnDamage_Implementation(AActor* actor, float defence) override;

	// �R���W�����L����
	void OnUseCollision_Implementation(class UPrimitiveComponent* Col) override;

	// �R���W����������
	void OnUnUseCollision_Implementation(class UPrimitiveComponent* Col_1, class UPrimitiveComponent* Col_2) override;

	// �{�X�̃X�e�[�g
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChracterParam")
	EBossState MyState;

	// �A�N�e�B�u���ǂ���
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterParam")
	bool IsActive;

	// ���S����
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterParam")
	bool IsDeath;

	// �U�������ǂ���
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterParam")
	bool Attacking;

	// �_���[�W���󂯂Ă��邩
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterParam")
	bool Damaging;

	// �v���C���[��⑫���Ă��邩
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterParam")
	bool IsSerch;
	
	// �^�[�Q�b�gActor
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterParam")
	AActor* TargetActor;

protected:

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Action")
	void EnemyAction();

	// ���S�C�x���g
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Action")
	void EnemyDeath();

	// �^�[�Q�b�g���m
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Action")
	void BeginSerch(AActor* _actor);
	// �^�[�Q�b�g���X�g
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Action")
	void SerchLost();

	// HP
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Status")
	float CurrentHP;

};

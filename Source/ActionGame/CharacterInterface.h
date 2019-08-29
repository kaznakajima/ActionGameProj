// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CharacterInterface.generated.h"

UINTERFACE()
class ACTIONGAME_API UCharacterInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

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

	// �U���͈�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterParam")
	float AttackRange;
};

// �L�����N�^�[�C���^�[�t�F�[�X
class ACTIONGAME_API ICharacterInterface
{
	GENERATED_IINTERFACE_BODY()
		
public:

	// �R���W�����L����
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "InterfaceAction")
	void OnUseCollision(class UPrimitiveComponent* Col);

	// �R���W����������
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "InterfaceAction")
	void OnUnUseCollision(class UPrimitiveComponent* Col_1, class UPrimitiveComponent* Col_2);

	// �_���[�W����
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "InterfaceAction")
	void OnDamage(AActor* actor, float defence);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CharacterInterface.generated.h"

UINTERFACE(BlueprintType)
class ACTIONGAME_API UCharacterInterface : public UInterface
{
	GENERATED_BODY()
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

class ICharacterInterface
{
	GENERATED_IINTERFACE_BODY()
};

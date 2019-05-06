// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Engine/DataTable.h"
#include "MyDataAsset.generated.h"

USTRUCT(BlueprintType)
struct FMyData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	// �L�����N�^�[ID
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ID;

	// �L�����N�^�[��
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name;

	// �ő�HP
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHP;

	// �U����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Power;

	// �����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Defence;
};

USTRUCT(BlueprintType)
struct FMyDataAssetRecord
{
	GENERATED_USTRUCT_BODY()
public:
	// �L�����N�^�[ID
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ID;

	// �L�����N�^�[��
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name;

	// �ő�HP
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int HP;

	// �U����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Attack;

	// �����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Defence;
};

UCLASS()
class ACTIONGAME_API UMyDataAsset : public UDataAsset
{
	GENERATED_BODY()
public:
#if WITH_EDITORONLY_DATA
		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EditData")
		UDataTable* DataTable;
#endif
	UFUNCTION(meta = (CallInEditor = "true"))
		void Import();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<FMyDataAssetRecord> Data;

};

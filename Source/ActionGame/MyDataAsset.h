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
	// キャラクターID
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ID;

	// キャラクター名
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name;

	// 最大HP
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHP;

	// 攻撃力
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Power;

	// 守備力
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Defence;

	// 攻撃範囲
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackRange;
};

USTRUCT(BlueprintType)
struct FMyDataAssetRecord
{
	GENERATED_USTRUCT_BODY()
public:
	// キャラクターID
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ID;

	// キャラクター名
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name;

	// 最大HP
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int HP;

	// 攻撃力
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Attack;

	// 守備力
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Defence;

	// 攻撃範囲
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackRange;
};

UCLASS()
class ACTIONGAME_API UMyDataAsset : public UDataAsset
{
	GENERATED_BODY()
public:
#if WITH_EDITORONLY_DATA
	    // データテーブル
		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EditData")
		UDataTable* DataTable;
#endif
	// テーブルをインポートする
	UFUNCTION(meta = (CallInEditor = "true"))
	void Import();

	// データリスト
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FMyDataAssetRecord> Data;

};

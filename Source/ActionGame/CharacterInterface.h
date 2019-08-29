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

// キャラクターのステータス
USTRUCT(BlueprintType)
struct FCharacterStatus
{
	GENERATED_USTRUCT_BODY()

	// キャラクターID
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterStatus")
	int ID;

	// キャラクター名
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterStatus")
	FText Name;

	// 最大HP
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterStatus")
	float MaxHP;

	// 攻撃力
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterStatus")
	float Power;

	// 守備力
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterStatus")
	float Defence;

	// 攻撃範囲
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterParam")
	float AttackRange;
};

// キャラクターインターフェース
class ACTIONGAME_API ICharacterInterface
{
	GENERATED_IINTERFACE_BODY()
		
public:

	// コリジョン有効化
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "InterfaceAction")
	void OnUseCollision(class UPrimitiveComponent* Col);

	// コリジョン無効化
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "InterfaceAction")
	void OnUnUseCollision(class UPrimitiveComponent* Col_1, class UPrimitiveComponent* Col_2);

	// ダメージ処理
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "InterfaceAction")
	void OnDamage(AActor* actor, float defence);
};

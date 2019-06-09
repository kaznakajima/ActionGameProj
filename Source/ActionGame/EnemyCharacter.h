// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterInterface.h"
#include "EnemyCharacter.generated.h"

// エネミーのステート
UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	IDLE UMETA(DisplayName = "Idle"),
	MOVE UMETA(DisplayName = "Move"),
	ATTACK UMETA(DisplayName = "Attack")
};

// エネミーのタイプ(近距離、遠距離)
UENUM(BlueprintType)
enum class EEnemyType : uint8
{
	SHORT_RANGE UMETA(DisplayName = "Short"),
	LONG_RANGE   UMETA(DisplayName = "Long")
};

UCLASS()
class ACTIONGAME_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter(const FObjectInitializer& ObjectInitilizer);

	// キャラクターステータス
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterParam")
	FCharacterStatus MyParam;

	// キャラクターのタイプ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterParam")
	EEnemyType MyType;

	// キャラクターのステート
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterParam")
	EEnemyState MyState;

	// 攻撃中かどうか
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterParam")
	bool Attacking;

	// ダメージを受けているか
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterParam")
	bool Damaging;

protected:

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Action")
	void EnemyAction();
 
	// ジャンプ
	void Jump();

	// 移動できるかどうか返す
	bool CanMove();

	// HP
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Status")
	float CurrentHP;

	// ダメージ処理
	UFUNCTION(BlueprintCallable, Category = "Status")
	void GiveDamage(AActor* actor, float defence);

	// 攻撃判定用コリジョン有効化
	UFUNCTION(BlueprintCallable, Category = "Collision")
	void UseCollision(class UPrimitiveComponent* boxCol);

	// 攻撃判定用コリジョン無効化
	UFUNCTION(BlueprintCallable, Category = "Collision")
	void UnUseCollision(class UPrimitiveComponent* boxCol_1, class UPrimitiveComponent* boxCol_2);

	// Timerを管理する変数
	UPROPERTY()
	FTimerHandle TimeHandle;
};

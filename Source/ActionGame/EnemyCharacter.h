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
	IDLE UMETA(DisplayName = "IDLE"),
	PATOROL UMETA(DisplayName = "PATROL"),
	MOVE UMETA(DisplayName = "MOVE"),
	ATTACK UMETA(DisplayName = "ATTACK")
};

// エネミーのタイプ(近距離、遠距離)
UENUM(BlueprintType)
enum class EEnemyType : uint8
{
	SHORT_RANGE UMETA(DisplayName = "Short"),
	LONG_RANGE   UMETA(DisplayName = "Long")
};

// 動的マルチキャストデリゲート(イベントディスパッチャー)の宣言
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEnemyOnEventDispather, float, Delation, float, Interval);

UCLASS()
class ACTIONGAME_API AEnemyCharacter : public ACharacter, public ICharacterInterface
{
	GENERATED_BODY()

public:
	// コンストラクタ
	AEnemyCharacter(const FObjectInitializer& ObjectInitilizer);

	// 動的マルチキャストデリゲート(イベントディスパッチャー)の定義
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Event")
	FEnemyOnEventDispather OnHitStop;

	// キャラクターステータス
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterParam")
	FCharacterStatus MyParam;

	// ダメージイベント
	void OnDamage_Implementation(AActor* actor, float defence) override;

	// コリジョン有効化
	void OnUseCollision_Implementation(class UPrimitiveComponent* Col) override;

	// コリジョン無効化
	void OnUnUseCollision_Implementation(class UPrimitiveComponent* Col_1, class UPrimitiveComponent* Col_2) override;

	// キャラクターのタイプ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterParam")
	EEnemyType MyType;

	// キャラクターのステート
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterParam")
	EEnemyState MyState;

	// 死亡判定
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterParam")
	bool IsDeath;

	// 攻撃中かどうか
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterParam")
	bool Attacking;

	// ダメージを受けているか
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterParam")
	bool Damaging;

	// プレイヤーを補足しているか
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterParam")
	bool IsSerch;

	// ターゲットActor
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterParam")
	AActor* TargetActor;

protected:

	// 敵のアクション準備イベント(攻撃準備)
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Action")
	void EnemyPreparationAction();

	// 死亡イベント
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Action")
	void EnemyDeath();

	// ターゲット検知
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Action")
	void BeginSerch(AActor* _actor);
	// ターゲットロスト
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Action")
	void SerchLost();

	// 移動できるかどうか返す
	bool CanMove();

	// HP
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Status")
	float CurrentHP;
};

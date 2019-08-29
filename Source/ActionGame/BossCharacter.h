// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterInterface.h"
#include "BossCharacter.generated.h"

// エネミーのステート
UENUM(BlueprintType)
enum class EBossState : uint8
{
	PATOROL UMETA(DisplayName = "PATROL"),
	BATTLE UMETA(DisplayName = "BATTLE"),
	LONGBATTLE UMETA(DisplayName = "LONGRANGE")
};

UCLASS()
class ACTIONGAME_API ABossCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABossCharacter(const FObjectInitializer& ObjectInitilizer);

	// キャラクターステータス
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterParam")
	FCharacterStatus MyParam;

	// ボスのステート
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChracterParam")
	EBossState MyState;

	// アクティブかどうか
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterParam")
	bool IsActive;

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

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Action")
	void EnemyAction();

	// 死亡イベント
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Action")
	void EnemyDeath();

	// ターゲット検知
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Action")
	void BeginSerch(AActor* _actor);
	// ターゲットロスト
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Action")
	void SerchLost();

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

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterInterface.h"
#include "EnemyCharacter.generated.h"

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

	// 攻撃中かどうか
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterParam")
		bool Attacking;

	// 回避中かどうか
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterParam")
		bool Avoiding;

	// ダメージを受けているか
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterParam")
		bool Damaging;

protected:

	// ジャンプ
	void Jump();

	// ダッシュ攻撃ができるかどうか
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterParam")
	bool CanDashAttack;

	// 移動できるかどうか返す
	bool CanMove();

	// ダメージ処理
	UFUNCTION(BlueprintCallable, Category = "Status")
	void GiveDamage(AActor* actor, float defence);

	// 攻撃判定用コリジョン有効化
	UFUNCTION(BlueprintCallable, Category = "Collision")
	void UseCollision(class UPrimitiveComponent* boxCol);

	// 攻撃判定用コリジョン無効化
	UFUNCTION(BlueprintCallable, Category = "Collision")
	void UnUseCollision(class UPrimitiveComponent* boxCol_1, class UPrimitiveComponent* boxCol_2);

	// 回避処理
	UFUNCTION(BlueprintCallable, Category = "PlayerAction")
	void AvoidAction();
	// 回避のキャンセル
	UFUNCTION(BlueprintCallable, Category = "PlayerAction")
	void AvoidCancel();

	// ダッシュ方向ベクター
	UPROPERTY()
	FVector DashVec;
	// 入力方向ベクター
	UPROPERTY()
	FVector InputVec;
	// Timerを管理する変数
	UPROPERTY()
	FTimerHandle TimeHandle;
	// 回避時のダッシュ
	UFUNCTION()
	void AvoidDash();

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterInterface.h"
#include "BossCharacter.generated.h"

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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

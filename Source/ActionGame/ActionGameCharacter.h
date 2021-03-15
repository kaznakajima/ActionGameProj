// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CharacterInterface.h"
#include "GameCharacterBase.h"
#include "ActionGameCharacter.generated.h"

UCLASS(config=Game)
class AActionGameCharacter : public AGameCharacterBase
{
	GENERATED_BODY()
public:

	// コンストラクタ
	AActionGameCharacter(const FObjectInitializer& ObjectInitilizer);

protected:

	// 死亡イベント
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Action")
	void PlayerDeath();

	// 左右視点変更
	void TurnAtRate(float Rate);

	// 上下視点変更
	void LookUpAtRate(float Rate);

	// 自身の位置を初期位置に
	UFUNCTION(BlueprintCallable, Category = "CharacterPos")
	void InitPosition();

	// ダッシュ攻撃ができるかどうか
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterParam")
	bool CanDashAttack;

	// 攻撃時に自動ターゲットをするか
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "PlayerAction")
	bool CheckTargetForcus();

	// 入力方向に攻撃を行う
	UFUNCTION(BlueprintCallable, Category = "PlayerAction")
	void AttackDirectionForcus();

	// 入力方向ベクターを返す
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "CharacterParam")
	FVector GetInputVector();

	// 空中攻撃中止
	UFUNCTION(BlueprintCallable, Category = "PlayerAction")
	void CancelFlyAttack();

	// 空中攻撃判定
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "PlayerAction")
	bool CheckFlyAttack();

	// 回避処理
	UFUNCTION(BlueprintCallable, Category = "PlayerAction")
	void AvoidAction();
	// 回避時のダッシュ
	UFUNCTION()
	void AvoidDash();
	// 回避のキャンセル
	UFUNCTION(BlueprintCallable, Category = "PlayerAction")
	void AvoidCancel();

	// 自身の初期位置
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterParam")
	FVector InitPos;
	// 空中攻撃回数
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterParam")
	int AirAttackCount;

protected:

	// 入力情報のセットアップ
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};


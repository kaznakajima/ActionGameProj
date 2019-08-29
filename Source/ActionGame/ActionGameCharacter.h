// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterInterface.h"
#include "ActionGameCharacter.generated.h"

UCLASS(config=Game)
class AActionGameCharacter : public ACharacter, public ICharacterInterface
{
	GENERATED_BODY()

	// SpringArmの定義
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	// カメラの定義
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:

	// コンストラクタ
	AActionGameCharacter(const FObjectInitializer& ObjectInitilizer);

	// 左右回転
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	// 上下回転
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	// キャラクターステータス
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterParam")
	FCharacterStatus MyParam;

	// ダメージイベント
	void OnDamage_Implementation(AActor* actor, float defence) override;

	// コリジョン有効化
	void OnUseCollision_Implementation(class UPrimitiveComponent* Col) override;

	// コリジョン無効化
	void OnUnUseCollision_Implementation(class UPrimitiveComponent* Col_1, class UPrimitiveComponent* Col_2) override;

	// 有効かどうか
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterParam")
	bool IsActive;
	
	// 死亡判定
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterParam")
	bool IsDeath;

	// 攻撃中かどうか
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="CharacterParam")
	bool Attacking;

	// 回避中かどうか
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterParam")
	bool Avoiding;

	// ダメージを受けているか
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterParam")
	bool Damaging;

protected:

	// 死亡イベント
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Action")
	void PlayerDeath();

	// 前後移動
	void MoveForward(float Value);

	// 左右移動
	void MoveRight(float Value);

	// ジャンプ
	void Jump();

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

	// 移動できるかどうか返す
	bool CanMove();

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
	// ダッシュ方向ベクター
	UPROPERTY()
	FVector DashVec;
	// 攻撃方向ベクター
	UPROPERTY()
	FVector AttackVec;
	// 入力方向ベクター
	UPROPERTY()
	FVector InputVec;
	// Timerを管理する変数
	UPROPERTY()
	FTimerHandle TimeHandle;
	
	// 現在のHP
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterParam")
	float CurrentHP;

protected:

	// 入力情報のセットアップ
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	// SpringArmを返す
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	// カメラを返す
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};


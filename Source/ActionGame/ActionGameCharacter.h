// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterInterface.h"
#include "ActionGameCharacter.generated.h"

UCLASS(config=Game)
class AActionGameCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	AActionGameCharacter(const FObjectInitializer& ObjectInitilizer);

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	// キャラクターステータス
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterParam")
	FCharacterStatus MyParam;
	
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

	// ダメージ処理
	UFUNCTION(BlueprintCallable, Category = "Camera")
	void CameraReset();

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

	// 空中攻撃処理
	UFUNCTION(BlueprintCallable, Category = "PlayerAction")
	void CheckFlyAttack(bool isAttack);

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

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};


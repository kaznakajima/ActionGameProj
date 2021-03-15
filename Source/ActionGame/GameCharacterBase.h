// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterInterface.h"
#include "GameCharacterBase.generated.h"

UCLASS(config = Game)
class ACTIONGAME_API AGameCharacterBase : public ACharacter, public ICharacterInterface
{
	GENERATED_BODY()
public:

	// コンストラクタ
	AGameCharacterBase(const FObjectInitializer& ObjectInitilizer);

	// 左右回転
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	// 上下回転
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

	// キャラクターステータス
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterParam")
		FCharacterStatus MyParam;

	// 移動できるかどうか返す
	bool CanMove();

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
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterParam")
		bool Attacking;

	// 回避中かどうか
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterParam")
		bool Avoiding;

	// ダメージを受けているか
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterParam")
		bool Damaging;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// 前後移動
	void MoveForward(float Value);

	// 左右移動
	void MoveRight(float Value);

	// ジャンプ
	void Jump();

	// SpringArmの定義
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	// カメラの定義
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;

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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// SpringArmを返す
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	// カメラを返す
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

};

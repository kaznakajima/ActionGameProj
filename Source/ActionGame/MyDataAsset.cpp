// Fill out your copyright notice in the Description page of Project Settings.

#include "MyDataAsset.h"

void UMyDataAsset::Import()
{
#if WITH_EDITORONLY_DATA
	if (!DataTable ||
		!DataTable->GetRowStruct()->IsChildOf(FMyData::StaticStruct()))
	{
		return;
	}

	// データをリセット
	Data.Empty();

	// データの行数を取得
	auto Names = DataTable->GetRowNames();

	// データの取得
	for (int i = 0; i < Names.Num(); i++)
	{
		// データの抽出
		auto record = DataTable->FindRow<FMyData>(Names[i], FString());

		// 値の設定
		FMyDataAssetRecord asset;
		asset.ID = record->ID;
		asset.Name = FText::FromName(Names[i]);
		asset.HP = record->MaxHP;
		asset.Attack = record->Power;
		asset.Defence = record->Defence;
		asset.AttackRange = record->AttackRange;

		Data.Add(asset);
	}

#endif
}
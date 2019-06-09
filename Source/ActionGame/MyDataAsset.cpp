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

	// �f�[�^�����Z�b�g
	Data.Empty();

	// �f�[�^�̍s�����擾
	auto Names = DataTable->GetRowNames();

	// �f�[�^�̎擾
	for (int i = 0; i < Names.Num(); i++)
	{
		// �f�[�^�̒��o
		auto record = DataTable->FindRow<FMyData>(Names[i], FString());

		// �l�̐ݒ�
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
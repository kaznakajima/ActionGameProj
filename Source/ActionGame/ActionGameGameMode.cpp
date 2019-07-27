// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "ActionGameGameMode.h"
#include "ActionGameCharacter.h"
#include "UObject/ConstructorHelpers.h"

AActionGameGameMode::AActionGameGameMode()
{
	// デフォルトのキャラクターを設定
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Character/Player/BP_PlayerCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

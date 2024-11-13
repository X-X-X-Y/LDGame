// Fill out your copyright notice in the Description page of Project Settings.


#include "GameDevUtil/LDGameplayTags.h"
#include "Engine/EngineTypes.h"
#include "GameplayTagsManager.h"

namespace LDGameplayTags
{
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_TD_Move, "InputTag.TD.Move", "Top Down Move input.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Player_Select,"InputTag.Player.Select","Player Move input");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Player_Move,"InputTag.Player.Move","Player Move input");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Player_DMove,"InputTag.Player.DMove","Player Mouse Move input");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Player_Spin,"InputTag.Player.Spin","Player View Rotation");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Player_Zoom,"InputTag.Player.Zoom","Player View Zoom");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(XiuShiHeroTag_TestXiuShi,"XiuShi.TestMan","Use Test XiuShi");
}
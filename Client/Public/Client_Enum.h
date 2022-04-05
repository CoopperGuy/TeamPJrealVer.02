#pragma once
namespace Client
{
	enum SCENE { SCENE_STATIC, SCENE_LOGO, SCENE_LOBBY, SCENE_LOADING, SCENE_GAMEPLAY, SCENE_STAGE1, SCENE_STAGE2, SCENE_STAGE3, SCENE_STAGE4,SCENE_TEST, SCENE_EFFECT,
		SCENE_KIM, SCENE_LEE, SCENE_JUNG, SCENE_SEO, SCENE_END };

	enum class	VISIBILITY { VISIBLE, INVISIBLE, NONE };

	enum class CurState
	{
		IDLE, WALK, RUN, ATT, Skill, JUMP, Max
	};
	enum class Skill
	{
		Slot1, Slot2, Slot3, Slot4, Max_Slot
	};

	enum class Equip
	{
		Weapon, Armor, Glove, Boots, Helm, Pants, Max_Equip
	};
}
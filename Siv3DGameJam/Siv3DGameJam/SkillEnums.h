#pragma once
#include <Siv3D.hpp>
#include "GameData.h"

enum class SkillEnums
{
	Damage,
	Heal,
	Guard,
	Buff,
	Debuff,
	Undo,
	Cancel,
	Unknown
};
enum class ValueType { Flat, Percent };

struct EffectSpec {
	SkillEnums  kind = SkillEnums::Unknown;
	// 共通スカラー
	StatusId     scaleStat = StatusId::Unknown;
	double      scaleK = 0.0;
	double      flatAdd = 0.0;
	int         times = 1;
	// Guard/Buff/Debuff用
	StatusId     affectStat = StatusId::Unknown;
	ValueType   valueType = ValueType::Flat;
	double      value = 0.0;
	int         durationT = 0;
	// Undo/Cancel用
	int         steps = 1;
};

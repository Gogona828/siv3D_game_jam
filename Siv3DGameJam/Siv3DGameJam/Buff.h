#pragma once
#include <Siv3D.hpp>

enum class BuffType
{
    DamageReduction,
    AttackMultiplier,
};

struct Buff
{
    String name;
    BuffType type;
    double value;
    int duration; // in turns
};

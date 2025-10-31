#pragma once
# include<Siv3D.hpp>
# include "GameData.h"

class BossCharacterView
{
private:
	const Font font{ FontMethod::MSDF, 48 };
	Texture characterTexture = Texture(U"assets/maingame/battle/boss.png");
	Texture buffTexture = Texture(U"assets/maingame/battle/battle_atk_buff.png");
	Texture breakTexture = Texture(U"assets/maingame/battle/battle_break.png");
	int32 maxHp = GameData::getInstance().bossInfos().hp;
public:
	void draw() const;
};


#pragma once
# include<Siv3D.hpp>
# include "GameData.h"

class BossCharacterView
{
private:
	const Font font{ FontMethod::MSDF, 48 };
	Texture characterTexture = Texture(U"assets/maingame/chara_image/404_normal.png");	// 代替え
	int32 maxHp = GameData::getInstance().bossInfos().hp;
public:
	void draw() const;
};


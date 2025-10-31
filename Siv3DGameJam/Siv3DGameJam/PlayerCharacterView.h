#pragma once
# include<Siv3D.hpp>

class PlayerCharacterView
{
private:
	const Font font{ FontMethod::MSDF, 48 };
	Texture characterTexture;

	int32 maxHp = 0;
	int32 maxMp = 0;
public:
	PlayerCharacterView();
	~PlayerCharacterView() = default;
	void draw() const;
	void setMaxStatus(int32 max_hp, int32 max_mp);
};


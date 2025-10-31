#pragma once
# include<Siv3D.hpp>

class BehaviorInfoView
{
private:
	const Font font{ FontMethod::MSDF, 48 };
public:
	void draw() const;
	String skillInfo = U"マウスカーソル合わせた行動の説明が入る";
};


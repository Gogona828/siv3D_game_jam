# include "BossCharacterView.h"
# include "MasterData.h"
# include "GameData.h"
using namespace s3d;

void BossCharacterView::draw() const
{
	GameData& gameData = GameData::getInstance();

	Rect characterInfoRect{ Arg::topCenter(Scene::Width()/2, 0), 800, 450};
	const Transformer2D t0{ Mat3x2::Translate(characterInfoRect.center()) };
	auto boss = characterTexture.resized(800, 800 * 2 /3).draw(Arg::topCenter(0, -300));
	{
		const Transformer2D t1{ Mat3x2::Translate(boss.center()) };
		auto break_texture = breakTexture.resized(Vec2 {118, 150} / 2).draw(Arg::center(120, -70));
		font(Format(gameData.bossInfos().breakValue)).draw(TextStyle::Outline(0.5, ColorF{ 0 }), 36, Arg::center(break_texture.rightX(), break_texture.bottomY() - 10));
	}

#pragma region Status
	{
		const Vec2 statusOffset = Vec2{ 0, 30 };
		const Transformer2D t1{ Mat3x2::Translate(statusOffset) };
		// HPバーとの隙間
		int32 padding = 15;

		// HPバー
		RectF hpBar(Arg::center(0, 25), 450, 10);
		hpBar.draw(Arg::left = ColorF{ 1, 1, 0 }, Arg::right = ColorF{ 0, 1, 0 });

		// HPの数値テキスト
		RectF maxHpRect = font(U"/" + Format(maxHp)).draw(14, Arg::bottomRight(hpBar.rightX(), padding), ColorF{1.0});
		font(Format(gameData.bossInfos().hp)).draw(18, Arg::bottomRight(maxHpRect.leftX(), padding), ColorF{ 1.0 });

		// バフデバフアイコン
		auto buff = buffTexture.resized(50).draw(Arg::bottomLeft(Vec2{ hpBar.leftX(), padding }));
		font(U"2").draw(TextStyle::Outline(0.5, ColorF{0}), 24, Arg::bottomLeft(buff.rightX(), buff.bottomY()));
	}
#pragma endregion
}

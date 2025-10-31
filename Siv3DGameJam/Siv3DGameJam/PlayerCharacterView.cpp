# include "PlayerCharacterView.h"
# include "MasterData.h"
# include "GameData.h"
using namespace s3d;

PlayerCharacterView::PlayerCharacterView()
{
	MasterData& masterData = MasterData::getInstance();
	GameData& gameData = GameData::getInstance();

	characterTexture = Texture(masterData.getTexturePath(gameData.infos().pcTextureId));
	maxHp = gameData.infos().reliability;
	maxMp = gameData.infos().availability;
}

void PlayerCharacterView::draw() const
{
	GameData& gameData = GameData::getInstance();

	// 親のピボット設定
	// 基本的にcharacterInfoOffsetをずらせばこのスコープ内のUIが移動する
	Rect characterInfoRect{ Arg::center(120, 400), 500 };
	const Transformer2D t0{ Mat3x2::Translate(characterInfoRect.center())};
	characterTexture.resized(400, 400).draw(Arg::center(0, 0));

#pragma region HPMP
	{
		const Vec2 statusOffset = Vec2{ -100, 15 };
		const Transformer2D t1{ Mat3x2::Translate(statusOffset) };

		RectF(0, 0, 200, 80).draw(ColorF{ 0, 0, 0, 0.8 });
		font(U"HP").draw(18, Vec2{ 10, 0 }, ColorF{ 1.0 });
		RectF maxHpRect = font(U"/" + Format(maxHp)).draw(14, Arg::bottomRight(190, 24), ColorF{ 1.0 });
		font(Format(gameData.infos().reliability)).draw(18, Arg::bottomRight(maxHpRect.leftX(), 24)/*Vec2{ 125, 0 }*/, ColorF{1.0});
		RectF{ 10, 25, 180, 10 }.draw(Arg::left = ColorF{ 1, 1, 0 }, Arg::right = ColorF{ 0, 1, 0 });

		font(U"MP").draw(18, Vec2{ 10, 40 }, ColorF{ 1.0 });
		RectF maxMpRect = font(U"/" + Format(maxMp)).draw(14, Arg::bottomRight(190, 64), ColorF{ 1.0 });
		font(Format(gameData.infos().availability)).draw(18, Arg::bottomRight(maxMpRect.leftX(), 64), ColorF{1.0});
		RectF{ 10, 65, 180, 10 }.draw(Arg::left = ColorF{ 0, 1, 1 }, Arg::right = ColorF{ 0, 0, 1 });
	}
#pragma endregion

#pragma region Break
	{
		const Vec2 breakOffset = Vec2{ 60, -100 };
		const Transformer2D t1{ Mat3x2::Translate(breakOffset) };

		RectF breakRect{ Arg::center(0, 0), 60, 60 };
		breakTexture.resized(Vec2{ 118, 150 } / 2).draw(Arg::center(breakRect.center()));
		{
			Transformer2D t2{ Mat3x2::Translate(breakRect.rightX(), breakRect.bottomY()) };
			font(U"3").draw(TextStyle::Outline(0.5, ColorF{ 0 }), 40, Arg::center(0, 0));
		}
	}
#pragma endregion
}

void PlayerCharacterView::setMaxStatus(int32 max_hp, int32 max_mp)
{
	maxHp = max_hp;
	maxMp = max_mp;
}

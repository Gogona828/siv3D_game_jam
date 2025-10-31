# include "BossCharacterView.h"
# include "MasterData.h"
# include "GameData.h"
using namespace s3d;

void BossCharacterView::draw() const
{
	GameData& gameData = GameData::getInstance();

	Rect characterInfoRect{ Arg::topCenter(Scene::Width()/2, 0), 800, 300};
	const Transformer2D t0{ Mat3x2::Translate(characterInfoRect.center()) };
	characterTexture.resized(600, 600).draw(Arg::topCenter(0, -300));

#pragma region HP
	{
		const Vec2 statusOffset = Vec2{ 0, 50 };
		const Transformer2D t1{ Mat3x2::Translate(statusOffset) };

		RectF maxHpRect = font(U"/" + Format(maxHp)).draw(14, Arg::bottomRight(250, 24), ColorF{ 1.0 });
		font(Format(gameData.bossInfos().hp)).draw(18, Arg::bottomRight(maxHpRect.leftX(), 24), ColorF{ 1.0 });
		RectF{ Arg::center(0, 25), 500, 10 }.draw(Arg::left = ColorF{ 1, 1, 0 }, Arg::right = ColorF{ 0, 1, 0 });
	}
#pragma endregion
}

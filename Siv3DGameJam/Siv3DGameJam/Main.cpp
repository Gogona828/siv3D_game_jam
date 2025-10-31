# include <Siv3D.hpp>
#include "Common.h" 

// 前方宣言
class TitleScene;
class GameScene;
class TrainingScene;
class GameData;
class ResultScene; // ← 追加！

# include "TitleScene.h"
# include "GameScene.h"
# include "TrainingScene.h"
# include "GameData.h"
# include "BattleScene.h"
# include "ResultScene.h" // ← 追加！
# include "JsonReader.h"

void Main()
{
	Window::Resize(896, 504);
	s3d::FontAsset::Register(U"Title", 40);
	s3d::FontAsset::Register(U"Game", 40);
	s3d::FontAsset::Register(U"Battle", 40);

	App app;
	app.add<TitleScene>(U"Title");
	app.add<GameScene>(U"Game");
	app.add<TrainingScene>(U"Training");
	app.add<BattleScene>(U"Battle");
	app.add<ResultScene>(U"Result"); // ← 追加！
	app.init(U"Title");

	const Font font{ FontMethod::MSDF, 48 };
	Array<int> ans = JsonReader::readData(U"assets/maingame/training/data/txt.json", U"UpStatusProbability");

	while (s3d::System::Update()) {
		app.update();
		const Point cursorPos = Cursor::Pos();
		Print << static_cast<int>(GameData::getInstance().evolutedCharacterTextureId);
		//font(U"{}"_fmt(cursorPos)).draw(40, Vec2{ 40, 40 }, ColorF{ 0.1 });
	}
}

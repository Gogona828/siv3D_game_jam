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
# include "SkillGrantService.h"
# include "SkillCatalog.h"
# include "ObtainedSkills.h"


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

	static SkillCatalog catalog;
	catalog.loadCSV(U"assets/data/AllSkillData.csv");
	static ObtainedSkills obtained;
	SkillGrantService& skillAPI = SkillGrantService::getInstance();
	skillAPI.configure(&catalog, &obtained);

	const Font font{ FontMethod::MSDF, 48 };

	while (s3d::System::Update()) {
		app.update();
		const Point cursorPos = Cursor::Pos();
		//font(U"{}"_fmt(cursorPos)).draw(40, Vec2{ 40, 40 }, ColorF{ 0.1 });
	}
}

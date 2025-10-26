# include <Siv3D.hpp>

// 公式 SceneManager を型エイリアス
using App = s3d::SceneManager<s3d::String, void>;

// 前方宣言
class TitleScene;
class GameScene;

# include "TitleScene.h"
# include "GameScene.h"

void Main()
{
	s3d::FontAsset::Register(U"Title", 40);
	s3d::FontAsset::Register(U"Game", 40);

	App app;
	app.add<TitleScene>(U"Title");
	app.add<GameScene>(U"Game");
	app.init(U"Title");

	while (s3d::System::Update()) {
		app.update();
	}
}

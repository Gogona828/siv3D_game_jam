#pragma once
# include <Siv3D.hpp>
#include "Common.h"
#include "ClickEffect.h"
# include "AudioManager.h"
# include "MasterData.h"

enum class TrainingState
{
	CanInputFile,
	Event,
	AnimatingStatus, // ★ この状態を追加
	AfterEvent,
	EndTraining
};
struct SystemStatusAddData
{
	int addId = -1;
	int addValue = 0;
};
enum class EventType
{
	Attack,
	Heal,
	Buff,
	Debuff,
	None
};

class BranchStatusBar
{
public:
	BranchStatusBar(Vec2 pos, double width, double height)
		: m_rect{ pos, width, height } {
	}

	void update()
	{
		if(!m_animating)
			return;
		m_timer += s3d::Scene::DeltaTime();
		double t = Clamp(m_timer / 0.5, 0.0, 1.0);  // 0.5秒でアニメーション完了
		m_value = Math::Lerp(m_startValue, m_targetValue, t);

		if (t >= 1.0)
		{
			m_animating = false;
		}
	}

	void draw() const
	{
		const Vec2 center = m_rect.center();
		const double halfW = m_rect.w * 0.5;
		m_rect.draw(ColorF{ 0.2,0.2,0.2});
		if (m_value < 0)
		{
			// 左向き（赤）
			const double rate = Clamp(-m_value, 0.0, 1.0);
			const double w = halfW * rate;

			// 中心から左に伸ばす
			RectF filled(center.x - w, m_rect.y, w, m_rect.h);
			filled.draw(Palette::Red);
		}
		else
		{
			// 右向き（青）
			const double rate = Clamp(m_value, 0.0, 1.0);
			const double w = halfW * rate;

			// 中心から右に伸ばす
			RectF filled(center.x, m_rect.y, w, m_rect.h);
			filled.draw(Palette::Skyblue);
		}
		//中央分離帯
		RectF filled(center.x - 1.0, m_rect.y - 3.0, 2, m_rect.h + 6.0);
		filled.draw(Palette::White);
	}
	void setTarget(double value)
	{
		m_startValue = m_value;
		m_targetValue = Clamp(value, -1.0, 1.0);
		m_timer = 0.0;
		m_animating = true;
	}
	// ★ この関数を追加
	RectF rect() const
	{
		return m_rect;
	}

private:
	RectF m_rect;
	double m_value = 0;
	double m_startValue = 0;
	double m_targetValue = 0;
	double m_timer = 0.0;
	bool m_animating = false;
};

class StatusBar
{
public:
	StatusBar(Vec2 pos, double width, double height)
		: m_rect{ pos, width, height } {
	}

	void update()
	{
		if (!m_animating)
			return;
		m_timer += s3d::Scene::DeltaTime();
		double t = Clamp(m_timer / 0.5, 0.0, 1.0);  // 0.5秒でアニメーション完了
		m_value = Math::Lerp(m_startValue, m_targetValue, t);

		if (t >= 1.0)
		{
			m_animating = false;
		}
	}

	void draw() const
	{
		const double halfW = m_rect.w;
		// 右向き（青）
		const double rate = Clamp(m_value, 0.0, 1.0);
		const double w = halfW * rate;
		m_rect.draw(ColorF{0.2,0.2,0.2});
		// 中心から右に伸ばす
		RectF filled(m_rect.x, m_rect.y, w, m_rect.h);
		filled.draw(Palette::Red);

	}
	void setTarget(double value)
	{
		m_startValue = m_value;
		m_targetValue = Clamp(value, 0.0, 1.0);
		m_timer = 0.0;
		m_animating = true;
	}
	// ★ この関数を追加
	RectF rect() const
	{
		return m_rect;
	}

private:
	RectF m_rect;
	double m_value = 0;
	double m_startValue = 0;
	double m_targetValue = 0;
	double m_timer = 0.0;
	bool m_animating = false;
};

class GetItemViewUnit
{
public:
	GetItemViewUnit(int id)
		:eventId(id), font{ FontMethod::MSDF, 48 }{
		String eventFileName = MasterData::getSkillName(eventId);
		String right = eventFileName.split(U'.').back();
		m_texture = Texture(MasterData::getTexturePath(MasterData::getTextureId(right)));
		m_name = MasterData::getSkillName(eventId);
	}
private:
	int eventId;
	String m_name;
	Texture m_texture;
	Font font;
public:
	void draw(Vec2 pos) const
	{
		m_texture.resized(75).draw(pos);
		font(m_name).draw(18,pos + Vec2{ 75 + 10, 75 / 2 - 10});
	}
};

class TrainingScene : public App::Scene
{
public:
	TrainingScene(const InitData& init);
private:
	ClickEffect clickEffect;

	s3d::Texture m_background;
	s3d::Texture m_dropshadow;
	s3d::Texture m_training_guide;

	s3d::Texture m_btn_howto;
	s3d::Texture m_btn_explorer;
	s3d::Texture m_btn_hover;
	s3d::Texture m_btn_restart;

	s3d::String m_hovered;
	Vec2 m_explorerPos;
	Vec2 m_howToPos;
	Vec2 m_restartPos;
	double m_buttonScale;  // ボタンの描画倍率
	double m_lastClickTimeExplorer;
	double m_lastClickTimeHowTo;
	double m_lastClickTimeRestart;

	void UpdateCutinTexture();  // 現在の進化形態に応じたカットインをロードする

	s3d::Audio m_btnSE;
	s3d::Audio m_clickSE;

	s3d::Audio m_eatSE;
	s3d::Audio m_growSE;
	s3d::Audio m_cutinSE;

	s3d::Audio m_bgMusic;

	struct DropAnim
	{
		Texture texture;
		Vec2 pos;
		double scale = 1.0;
		double alpha = 1.0;
		double time = 0.0;
	};

	Array<DropAnim> dropAnims;

	s3d::Texture cutinTexture;
	//double cutinTimer = 0.0;
	//double cutinDuration = 0.5; // 0.5秒で全体表示
	//bool cutinPlaying = false;


	double m_characterAnimTimer = 0.0;
	bool m_characterAnimPlaying = false;
	double m_characterAnimDuration = 0.6; // 拡縮アニメ時間（秒）

	// カットイン関連
	bool cutinClosing = false;
	bool cutinPlaying = false;
	bool cutinStarted = false;
	bool cutinWaiting = false;   // カットイン表示だけして待機中
	float cutinDuration = 0.05; // 例: 0.5秒
	float waitAfterCutin = 0.0;
	double waitTimeAfterCutin = 1.0; // 秒（今は0.5）

	const Font font{ FontMethod::MSDF, 48 };
	String characterImagePath = U"assets/maingame/chara_image/origin_normal.png";
	Texture characterTexture = s3d::Texture(characterImagePath);
	TrainingState m_state = TrainingState::CanInputFile;
	bool evoluted = false;

	int currentTurn = 0;
	int maxTurn = 10;

	//入力されたファイルデータ
	DroppedFilePath droppedFile;
	Array<FilePath> filePathList;//すでに入力されたパスリスト
	String fileExtension;//拡張子格納用
	int size = 0;//ファイルサイズ格納用
	uint64 hash;//ハッシュ値格納用
	Array<SystemStatusAddData> statusAddData;

	Array<BranchStatusBar> m_bars = {
			{ Vec2{ 20, 190 }, 200, 15 },
			{ Vec2{ 20, 240 }, 200, 15 },
			{ Vec2{ 20, 290 }, 200, 15 },
			{ Vec2{ 20, 340 }, 200, 15 },
			{ Vec2{ 20, 390 }, 200, 15 },
	};
	Array<GetItemViewUnit> getItemUnits;
	StatusBar overloadBar{ Vec2{ 20, 460 }, 200, 15 };

	bool initialized = false;

	//アニメーション用タイマー
	float timer = 0;
	float maxTimer = 10;
	float cutinTimer = 0;
	float cutinMaxTimer = 1;
	bool cutinFlag = false;

	//イベント描画用
	bool eventDrawing = false;
	EventType nowEventType;
	Texture eventTexture;
	String eventText;
	Texture eventBackGround;

	void update() override;
	void draw() const override;
	Array<SystemStatusAddData> statusTable();
	EventType eventTypeTable();
	Array<int> eventIdTable(EventType type,int n);
	void ChangeStatus(Array<SystemStatusAddData> data);
	void SetCharacter(String path);
	void SetEventData(String texturePath, String text)
	{
		eventTexture = Texture(texturePath);
		eventText = text;
	}

	int32 characterTextureId = 0;

	// ★ 追加: ステータスアニメーション用
	Array<double> m_animatedStatusValues; // アニメーション中の中間値 (6個)
	Array<double> m_targetStatusValues;   // アニメーションの目標値 (6個)
	Array<int> m_statusChanges;           // 今回の変化量 (色分け用)
	Stopwatch m_statusAnimTimer;          // アニメーション用タイマー
	const double m_statusAnimDuration = 0.5; // アニメーションの長さ (0.5秒)

	// ファイルスキャンモードの状態
	enum class FileScanMode
	{
		Inactive,		// 非アクティブ（通常の育成画面）
		WaitingDrop,	// スキャンモード中（ドロップ待ち）
		Displaying,		// スキャンモード中（情報表示中）
	};

	// スキャンしたファイル情報を保持する構造体
	struct ScannedFileInfo
	{
		String fileName = U"";
		String extension = U"";
		String upParam = U"-";
		String downParam = U"-";
		String eventProbability = U"中";
	};

	// --- メンバー変数の追加 ---

	// スキャンモード関連
	FileScanMode m_fileScanMode = FileScanMode::Inactive;
	Texture m_btn_scan;			// スキャンボタン用テクスチャ（explorerを流用）
	Vec2 m_scanPos;				// スキャンボタンの座標
	double m_lastClickTimeScan = 0.0; // スキャンボタンのダブルクリック判定用
	RectF m_closeButtonRect;	// スキャンモードを閉じるバツボタン
	Texture m_closeButtonTex;	// バツボタンのテクスチャ
	ScannedFileInfo m_scannedFileInfo; // スキャン結果の保持

	// --- ヘルパー関数の宣言追加 ---
	bool loadScanJson(const FilePath& path);
	String getParamNameByIndex(int index) const;
	String getEventProbabilityString(const Array<int>& probabilityArray) const;
};

# include "PlayerCharacterView.h"
# include "MasterData.h"
# include "GameData.h"


using namespace s3d;

PlayerCharacterView::PlayerCharacterView()
	: characterTexture(U"assets/maingame/battle/player.png") // Default player texture
{
	// コンストラクタの本体は空でよい
}

void PlayerCharacterView::update()
{
	// Update action animation
	if (m_isActionAnimating)
	{
		const double t = m_actionAnimTimer.sF() / m_actionAnimDuration;
		if (t >= 1.0)
		{
			m_yOffset = 0.0;
			m_isActionAnimating = false;
		}
		else
		{
			// 0.0 -> 0.5 (中間) -> 1.0 (終了)
			// EaseOutCubic を使い、中間で -30px まで上がる
			if (t < 0.5)
			{
				// 0.0 -> 0.5 (上がる)
				m_yOffset = Math::Lerp(0.0, -30.0, EaseOutCubic(t * 2.0));
			}
			else
			{
				m_yOffset = Math::Lerp(-30.0, 0.0, EaseInCubic((t - 0.5) * 2.0));
			}
		}
	}

	// Update blinking animation
	if (m_isBlinking)
	{
		const double blink_t = m_blinkTimer.sF();
		if (blink_t > m_blinkDuration)
		{
			m_isBlinking = false;
			m_blinkVisible = true; // 確実に表示状態に戻す
		}
		else
		{
			// 0.1秒ごとに表示/非表示を切り替える
			m_blinkVisible = (static_cast<int>(blink_t * 10.0) % 2 == 0);
		}
	}
}

// ★ 点滅開始トリガー
void PlayerCharacterView::playDamageBlink()
{
	if (m_isBlinking) return; // アニメーション中は無視
	m_isBlinking = true;
	m_blinkTimer.restart();
}

// ★ アニメーション開始トリガーを追加
void PlayerCharacterView::playAttackAnimation()
{
	if (m_isActionAnimating) return;
	m_isActionAnimating = true;
	m_actionAnimTimer.restart();
}

void PlayerCharacterView::draw() const
{
	GameData& gameData = GameData::getInstance();

	// 描画のたびにGameDataから最新情報を取得
	const auto& pcInfo = gameData.infos();

	// 親のピボット設定
	// 基本的にcharacterInfoOffsetをずらせばこのスコープ内のUIが移動する
	const Rect characterInfoRect{ Arg::center(120, 400), 500 };
	// ★ 修正: m_yOffset を Translate に追加
	const Transformer2D t0{ Mat3x2::Translate(characterInfoRect.center() + Vec2(0, m_yOffset)) };
	if (m_blinkVisible)
	{
		characterTexture.resized(400, 400).draw(Arg::center(0, 0));
	}

#pragma region HPMP
	{
		const Vec2 statusOffset = Vec2{ -100, 15 };
		const Transformer2D t1{ Mat3x2::Translate(statusOffset) };

		RectF(0, 0, 200, 80).draw(ColorF{ 0, 0, 0, 0.8 });
		font(U"HP").draw(18, Vec2{ 10, 0 }, ColorF{ 1.0 });
		RectF maxHpRect = font(U"/" + Format(maxHp)).draw(14, Arg::bottomRight(190, 24), ColorF{ 1.0 }); // 最大HP
		font(Format(pcInfo.reliability)).draw(18, Arg::bottomRight(maxHpRect.leftX(), 24), ColorF{1.0}); // 現在HP
		RectF{ 10, 25, 180, 10 }.draw(Arg::left = ColorF{ 1, 1, 0 }, Arg::right = ColorF{ 0, 1, 0 });

		font(U"MP").draw(18, Vec2{ 10, 40 }, ColorF{ 1.0 });
		RectF maxMpRect = font(U"/" + Format(maxMp)).draw(14, Arg::bottomRight(190, 64), ColorF{ 1.0 }); // MPの最大値は20で固定
		font(Format(pcInfo.mp)).draw(18, Arg::bottomRight(maxMpRect.leftX(), 64), ColorF{1.0}); // 現在MP
		RectF{ 10, 65, 180, 10 }.draw(Arg::left = ColorF{ 0, 1, 1 }, Arg::right = ColorF{ 0, 0, 1 });
	}
#pragma endregion

#pragma region Break
	{
		const Vec2 breakOffset = Vec2{ 60, -100 };
		const Transformer2D t2{ Mat3x2::Translate(breakOffset) };

		RectF breakRect{ Arg::center(0, 0), 60, 60 };
		breakTexture.resized(Vec2{ 118, 150 } / 2).draw(Arg::center(breakRect.center()));
		// Transformer2D t2 のスコープ内で処理
		font(Format(gameData.infos().breakValue)).draw(TextStyle::Outline(0.5, ColorF{ 0 }), 40, Arg::center(breakRect.rightX(), breakRect.bottomY()));
	}
#pragma endregion
}

void PlayerCharacterView::setMaxStatus(int32 max_hp, int32 max_mp)
{
	maxHp = max_hp;
	maxMp = max_mp;
}

Vec2 PlayerCharacterView::getCenterPos() const
{
	return Vec2(120, 400);
}

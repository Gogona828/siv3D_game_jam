#pragma once
# include<Siv3D.hpp>

class PlayerCharacterView
{
private:
	const Font font{ FontMethod::MSDF, 48 };
	Texture characterTexture;
	Texture breakTexture = Texture(U"assets/maingame/battle/battle_break.png");

	int32 maxHp = 0;
	int32 maxMp = 0;

	// ★ アクションアニメーション用
	Stopwatch m_actionAnimTimer;
	double m_actionAnimDuration = 0.4; // アニメーションの総時間
	double m_yOffset = 0.0; // Y座標のオフセット
	bool m_isActionAnimating = false;

	// ★ 被ダメージ点滅用
	Stopwatch m_blinkTimer;
	double m_blinkDuration = 0.5; // 点滅の総時間
	bool m_isBlinking = false;
	bool m_blinkVisible = true; // 点滅中の表示/非表示

public:
	PlayerCharacterView();
	~PlayerCharacterView() = default;
	void update(); // ★ update() を追加
	void draw() const;
	void setMaxStatus(int32 max_hp, int32 max_mp);

	void playAttackAnimation(); // ★ アニメーション開始トリガー
	void playDamageBlink(); // ★ 点滅開始トリガー
	Vec2 getCenterPos() const;
};

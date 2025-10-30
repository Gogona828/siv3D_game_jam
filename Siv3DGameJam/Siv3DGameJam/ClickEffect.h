#pragma once
#include <Siv3D.hpp>

struct ClickParticle
{
	Vec2 pos;
	Vec2 velocity;
	String text;
	double alpha = 1.0;
};

class ClickEffect
{
private:
	Array<ClickParticle> m_particles;
	Font m_font{ 20 };

public:
	void update()
	{
		for (auto& p : m_particles)
		{
			p.pos += p.velocity;
			p.alpha -= 0.05; // 徐々に消える
		}
		m_particles.remove_if([](const ClickParticle& p) { return p.alpha <= 0.0; });
	}

	void draw() const
	{
		for (const auto& p : m_particles)
		{
			m_font(p.text).drawAt(p.pos, ColorF(1.0, 1.0, 1.0, p.alpha));
		}
	}

	void spawn(const Vec2& pos, int count = 20)
	{
		for (int i = 0; i < count; ++i)
		{
			double angle = Random(0.0, 2 * Math::Pi);
			double speed = Random(1.0, 4.0);
			m_particles.push_back({ pos, Vec2{ speed * Math::Cos(angle), speed * Math::Sin(angle) }, RandomBool() ? U"0" : U"1" });
		}
	}
};

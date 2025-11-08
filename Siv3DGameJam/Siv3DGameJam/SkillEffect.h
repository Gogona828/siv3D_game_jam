#pragma once
#include <Siv3D.hpp>

struct SkillParticle
{
    Vec2 pos;
    Vec2 velocity;
    double angle;
    double rotationSpeed;
    double alpha;
    Texture texture;
};

class SkillEffect
{
private:
    Array<SkillParticle> m_particles;

public:
    void update()
    {
        for (auto& p : m_particles)
        {
            p.pos += p.velocity;
            p.angle += p.rotationSpeed;
            p.alpha -= 0.02;
        }
        m_particles.remove_if([](const SkillParticle& p) { return p.alpha <= 0.0; });
    }

    void draw() const
    {
        for (const auto& p : m_particles)
        {
            p.texture.rotated(p.angle).drawAt(p.pos, ColorF(1.0, p.alpha));
        }
    }

    void spawnDefenseEffect(const Vec2& pos)
    {
        const Texture shieldTexture{ U"Shield" }; // Using an emoji as a texture
        for (int i = 0; i < 30; ++i)
        {
            const double angle = Random(0.0, 360.0);
            const double speed = Random(1.0, 3.0);
            const Vec2 velocity = Vec2{ speed, 0 }.rotated(angle);
            const double rotationSpeed = Random(-0.1, 0.1);
            m_particles.push_back({ pos, velocity, 0.0, rotationSpeed, 1.0, shieldTexture });
        }
    }

    void spawnHealEffect(const Vec2& pos)
    {
        const Texture healTexture{ U"Heal" };
        for (int i = 0; i < 20; ++i)
        {
            const double angle = Random(0.0, 360.0);
            const double speed = Random(0.5, 2.0);
            const Vec2 velocity = Vec2{ 0, -speed }.rotated(angle);
            m_particles.push_back({ pos, velocity, 0.0, 0.0, 1.0, healTexture });
        }
    }

    void spawnBuffEffect(const Vec2& pos)
    {
        const Texture buffTexture{ U"Buff" };
        for (int i = 0; i < 20; ++i)
        {
            const double angle = Random(-Math::Pi / 4, Math::Pi / 4);
            const double speed = Random(1.0, 3.0);
            const Vec2 velocity = Vec2{ 0, -speed }.rotated(angle);
            m_particles.push_back({ pos, velocity, 0.0, 0.0, 1.0, buffTexture });
        }
    }
};

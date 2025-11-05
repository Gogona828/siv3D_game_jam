#include "UIElement.h"

void UIElement::BranchStatusBar::update()
{
	if (!m_animating)
		return;
	m_timer += s3d::Scene::DeltaTime();
	double t = Clamp(m_timer / 0.6, 0.0, 1.0);  // 0.5秒でアニメーション完了
	m_value = Math::Lerp(m_startValue, m_targetValue, t);

	if (t >= 1.0)
	{
		m_animating = false;
	}
}

void UIElement::BranchStatusBar::draw() const
{
	const Vec2 center = m_rect.center();
	const double halfW = m_rect.w * 0.5;
	m_rect.draw(ColorF{ 0.2,0.2,0.2 });
	if (m_value < 0)
	{
		// 左向き（赤）
		const double rate = Clamp(-m_value, 0.0, 1.0);
		const double w = halfW * rate;
		// 中心から左に伸ばす
		RectF filled(center.x - w, m_rect.y, w, m_rect.h);
		RectF bg(center.x, m_rect.y, halfW * m_targetValue, m_rect.h);
		bg.draw(ColorF(Palette::White, 0.5));
		filled.draw(Palette::Red);
	}
	else
	{
		// 右向き（青）
		const double rate = Clamp(m_value, 0.0, 1.0);
		const double w = halfW * rate;
		// 中心から右に伸ばす
		RectF filled(center.x, m_rect.y, w, m_rect.h);
		RectF bg(center.x, m_rect.y, halfW * m_targetValue, m_rect.h);
		bg.draw(ColorF(Palette::White, 0.5));
		filled.draw(Palette::Skyblue);
	}
	//中央分離帯
	RectF filled(center.x - 1.0, m_rect.y - 3.0, 2, m_rect.h + 6.0);
	filled.draw(Palette::White);
}
void UIElement::BranchStatusBar::setTarget(double value)
{
	m_startValue = m_value;
	m_targetValue = Clamp(value, -1.0, 1.0);
	m_timer = 0.0;
	m_animating = true;
}

void UIElement::StatusBar::update()
{
	if (!m_animating)
		return;
	m_timer += s3d::Scene::DeltaTime();
	double t = Clamp(m_timer / 0.6, 0.0, 1.0);  // 0.5秒でアニメーション完了
	m_value = Math::Lerp(m_startValue, m_targetValue, t);
	if (t >= 1.0)
	{
		m_animating = false;
	}
}

void UIElement::StatusBar::draw() const
{
	const double halfW = m_rect.w;
	// 右向き（青）
	const double rate = Clamp(m_value, 0.0, 1.0);
	const double w = halfW * rate;
	m_rect.draw(ColorF{ 0.2,0.2,0.2 });
	// 中心から右に伸ばす
	RectF filled(m_rect.x, m_rect.y, w, m_rect.h);
	RectF bg(m_rect.x, m_rect.y, m_rect.w * m_targetValue, m_rect.h);
	bg.draw(ColorF(Palette::White, 0.5));
	filled.draw(Palette::Red);
}

void UIElement::StatusBar::setTarget(double value)
{
	m_startValue = m_value;
	m_targetValue = Clamp(value, 0.0, 1.0);
	m_timer = 0.0;
	m_animating = true;
}

void UIElement::StatusWidgetUnit::setValue(float value)
{
	m_statusBar.setTarget(value);
}
void UIElement::StatusWidgetUnit::update()
{
	m_statusBar.update();
}
void UIElement::StatusWidgetUnit::draw() const
{
	m_font(m_name).draw(18, m_pos, ColorF{ 1.0 });
	m_statusBar.draw();
}

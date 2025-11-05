#pragma once
#include "MasterData.h"
namespace UIElement
{
	class BranchStatusBar
	{
	public:
		BranchStatusBar(Vec2 pos, double width, double height)
			: m_rect{ pos, width, height } {
		}

		void update();
		void draw() const;
		void setTarget(double value);
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

		void update();

		void draw() const;
		void setTarget(double value);
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
			:eventId(id), font{ FontMethod::MSDF, 48 } {
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
			font(m_name).draw(18, pos + Vec2{ 75 + 10, 75 / 2 - 10 });
		}
	};

	class StatusWidgetUnit
	{
	public:
		StatusWidgetUnit(String name,String rank,Vec2 pos, double width, double height,Font font) :
			m_name(name),
			m_rank(rank),
			m_pos(pos),
			m_statusBar(Vec2{pos.x,pos.y + 30}, width, height),
			m_font(font)
		{ }
		void setValue(float value);
		void update();
		void draw() const;

	private:
		Vec2 m_pos;
		String m_name;
		String m_rank;
		Font m_font;
		BranchStatusBar m_statusBar;
	};
}

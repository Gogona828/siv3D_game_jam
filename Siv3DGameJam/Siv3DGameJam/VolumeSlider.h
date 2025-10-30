# pragma once
# include <Siv3D.hpp>

class VolumeSlider
{
public:
	// pos: 左上座標, width: スライダー幅, height: スライダー高さ
	// label: 左に表示するテキスト, initialValue: 0.0~1.0
	VolumeSlider(const s3d::Vec2& pos, double width, double height, const s3d::String& label, double initialValue = 0.2)
		: m_pos(pos)
		, m_width(width)
		, m_height(height)
		, m_label(label)
		, m_value(s3d::Clamp(initialValue, 0.0, 1.0))
		, m_font(24) // Fontをここで一度だけ生成
	{
	}

	void update()
	{
		RectF rect(m_pos, m_width, m_height);

		// クリック開始でドラッグフラグON
		if (MouseL.down() && rect.mouseOver())
		{
			m_dragging = true;
		}

		// ドラッグ中はマウス位置で値更新
		if (m_dragging)
		{
			double t = (Cursor::Pos().x - m_pos.x) / m_width;
			m_value = Clamp(t, 0.0, 1.0);
		}

		// マウス離したらドラッグ終了
		if (MouseL.up())
		{
			m_dragging = false;
		}
	}

	void draw() const
	{
		s3d::RectF rect(m_pos, m_width, m_height);

		// ラベル描画（左側）
		m_font(m_label).draw(m_pos - s3d::Vec2(57.5, 10), s3d::Palette::White);

		// スライダー背景
		rect.draw(s3d::ColorF(0.3));
		rect.stretched(-4).drawFrame(2, 0, s3d::ColorF(0.7));

		// つまみ
		double knobX = m_pos.x + m_value * m_width;
		double knobY = m_pos.y + m_height / 2;
		s3d::Circle(knobX, knobY, m_height / 2).draw(s3d::ColorF(0.9));
	}

	// 値取得・設定
	double value() const { return m_value; }
	void setValue(double v) { m_value = s3d::Clamp(v, 0.0, 1.0); }

	// 位置取得（必要なら）
	s3d::Vec2 pos() const { return m_pos; }

private:
	bool m_dragging = false;
	s3d::Vec2 m_pos;
	double m_width;
	double m_height;
	s3d::String m_label;
	double m_value;

	s3d::Font m_font; // Fontはここで保持、毎フレーム生成しない
};

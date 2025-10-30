# pragma once
# include <Siv3D.hpp>

inline s3d::ColorF GetRankColor(const s3d::String& rank)
{
	if (rank == U"E") return s3d::ColorF(0.5, 0.5, 0.5);   // 灰
	if (rank == U"C") return s3d::ColorF(0.3, 0.6, 1.0);   // 青
	if (rank == U"B") return s3d::ColorF(0.3, 1.0, 0.3);   // 緑
	if (rank == U"A") return s3d::ColorF(1.0, 1.0, 0.2);   // 黄
	if (rank == U"S") return s3d::ColorF(1.0, 0.3, 0.3);   // 赤
	return s3d::Palette::White;
}

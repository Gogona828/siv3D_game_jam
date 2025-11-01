#pragma once
// StatAlias.hpp
#pragma once
#include <Siv3D.hpp>

// 日本語/表記ゆれを内部キーに正規化（必要に応じて増やす）
inline const s3d::HashTable<s3d::String, s3d::String>& StatAlias()
{
	static const s3d::HashTable<s3d::String, s3d::String> map =
	{
		{ U"攻撃系", U"atk" },
		{ U"防御系", U"def" },
		{ U"回復系", U"heal" },
		{ U"バフ系", U"buff"}
	};
	return map;
}

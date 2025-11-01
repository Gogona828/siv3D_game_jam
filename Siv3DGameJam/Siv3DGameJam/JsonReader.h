#pragma once
#include <Siv3D.hpp>

class JsonReader
{
public:
	static bool readData(const String& filePath, const String& key, Array<int>& outArray)
	{
		const JSON json = JSON::Load(filePath);
		if (!json)
		{
			return false;
		}

		// 指定キーが存在しない場合
		if (!json.hasElement(key))
		{
			return false;
		}

		const auto& value = json[key];

		// 値が配列の場合
		if (value.isArray())
		{
			for (const auto& v : value.arrayView())
			{
				outArray << v.get<int>();
			}
		}
		// 値が単一数値の場合（EventFireProbability が [100] → OK）
		else if (value.isNumber())
		{
			outArray << value.get<int>();
		}
		else
		{
			return false; // 型が違う
		}

		return true;
	}
};

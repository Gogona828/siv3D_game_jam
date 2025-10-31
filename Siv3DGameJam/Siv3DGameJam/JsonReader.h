#pragma once
#include <Siv3D.hpp>

class JsonReader
{
public:
	static Array<int> readData(const String& filePath, const String& key)
	{
		const JSON json = JSON::Load(filePath);
		if (!json)
		{
			throw Error{ U"JSON 読み込み失敗: {}"_fmt(filePath) };
		}

		for (const auto& item : json.arrayView())
		{
			if (item.hasElement(key))
			{
				Array<int> result;
				for (const auto& v : item[key].arrayView())
				{
					result << v.get<int>();
				}
				return result;
			}
		}

		throw Error{ U"指定キー {} が JSON 内に存在しません。"_fmt(key) };
	}
};

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

		for (const auto& item : json.arrayView())
		{
			if (item.hasElement(key))
			{
				Array<int> result;
				for (const auto& v : item[key].arrayView())
				{
					result << v.get<int>();
				}
				outArray = result;
				return true;
			}
		}
		return false;
	}
};

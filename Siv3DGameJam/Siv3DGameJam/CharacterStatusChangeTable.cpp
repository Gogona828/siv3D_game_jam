#include "CharacterStatusChangeTable.h"
#include "TrainingScene.h"
#include "JsonReader.h"
#include "GameData.h"

Array<SystemStatusAddData> CharacterStatusChangeTable::statusTable()
{
	Array<SystemStatusAddData> statusAddData;
	//ステータスの変動基本値を決定する
	//現状は「ハッシュの3,5,2桁目の和を16で割った余り×1GBをマックスとした容量倍率（１～３倍）」で決定
	int digit = ((m_hash / 100) % 10) + (((m_hash / 10000) % 10) + (m_hash / 10) % 10) % 16;
	float sizeInGB = m_size / (1024 * 1024 * 1024);
	float mul = Math::Lerp(1.0, 3.0, sizeInGB);
	int param = static_cast<int>(digit * mul * 1.3);
	//さらに各ステータスRandom 0-5の範囲で変動を加える
	//ステータス変動は「変動基本値 + Random（０－５の範囲）を加算」「（変動基本値/3）＋ Random(0-5)を減算」「容量(GB) + Random(0-10)を加算（Zipを除く）」の３つ
	String jsonPath = U"assets/maingame/training/data/{}.json"_fmt(m_fileExtension);
	Array<int> upStatusProbability;

	bool missLoad = false;
	if (JsonReader::readData(jsonPath, U"UpStatusProbability", upStatusProbability))
	{
		int allWeight = 0;
		for (const auto& weight : upStatusProbability)
		{
			allWeight += weight;
		}
		int rnd = Random(1, allWeight);
		int cumulativeWeight = 0;
		for (int i = 0; i < upStatusProbability.size(); i++)
		{
			cumulativeWeight += upStatusProbability[i];
			if (rnd <= cumulativeWeight)
			{
				SystemStatusAddData upStatusData;
				upStatusData.addId = i;
				Array<int> upStatusValueMultiply;
				if (JsonReader::readData(jsonPath, U"UpStatusValueMultiply", upStatusValueMultiply))
				{
					upStatusData.addValue = (param + Random(0, 5)) * (upStatusValueMultiply[i] * 0.01);
				}
				else
				{
					//該当データがなければ1倍で処理
					upStatusData.addValue = (param + Random(0, 5)) * 1;
				}
				statusAddData.push_back(upStatusData);
				break;
			}
		}
	}
	else
	{
		missLoad = true;
	}
	Array<int> downStatusProbability;
	if (JsonReader::readData(jsonPath, U"DownStatusProbability", downStatusProbability))
	{
		int allWeight = 0;
		for (const auto& weight : downStatusProbability)
		{
			allWeight += weight;
		}
		int rnd = Random(1, allWeight);
		int cumulativeWeight = 0;
		for (int i = 0; i < downStatusProbability.size(); i++)
		{
			cumulativeWeight += downStatusProbability[i];
			if (rnd <= cumulativeWeight)
			{
				SystemStatusAddData downStatusData;
				downStatusData.addId = i;
				Array<int> downStatusValueMultiply;
				if (JsonReader::readData(jsonPath, U"DownStatusValueMultiply", downStatusValueMultiply))
				{
					downStatusData.addValue = ((param / 2) + Random(0, 5)) * -1 * (downStatusValueMultiply[i] * 0.01);
				}
				else
				{
					//該当データがなければ1倍で処理
					downStatusData.addValue = ((param / 2) + Random(0, 5)) * -1 * 1;
				}
				statusAddData.push_back(downStatusData);
				break;
			}
		}
	}
	else
	{
		missLoad = true;
	}
	if (m_fileExtension == U"zip")
	{
		SystemStatusAddData overloadStatusData;
		overloadStatusData.addId = static_cast<int>(StatusId::Overload);
		overloadStatusData.addValue = (m_size / (1024 * 1024 * 1024) + Random(7, 10)) * -1 * 7.0;
		statusAddData.push_back(overloadStatusData);
	}
	else
	{
		SystemStatusAddData overloadStatusData;
		overloadStatusData.addId = static_cast<int>(StatusId::Overload);
		overloadStatusData.addValue = Math::Abs(m_size / (1024 * 1024 * 1024) + Random(3, 6) * 3.5);
		statusAddData.push_back(overloadStatusData);
	}
	if (missLoad)
	{
		//該当がない場合はランダムでステータス変動
		SystemStatusAddData upStatusData;
		upStatusData.addId = Random(0, 4);
		upStatusData.addValue = param + Random(0, 5);
		statusAddData.push_back(upStatusData);
		SystemStatusAddData downStatusData;
		downStatusData.addId = Random(0, 4);
		downStatusData.addValue = ((param / 2) + Random(0, 5)) * -1;
		statusAddData.push_back(downStatusData);

		SystemStatusAddData overloadStatusData;
		overloadStatusData.addId = static_cast<int>(StatusId::Overload);
		overloadStatusData.addValue = Math::Abs(m_size / (1024 * 1024 * 1024) + Random(3, 18));
		statusAddData.push_back(overloadStatusData);
	}
	return statusAddData;
}

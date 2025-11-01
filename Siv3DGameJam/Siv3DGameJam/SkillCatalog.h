#pragma once
# include<Siv3D.hpp>

struct SkillRow
{
	String key;
	String name;
	String descriptionText;
	String genre;
	String abstructTargetEvent;
	Array<String> columns;
};

class SkillCatalog
{
public:
	// CSVの列名インデックス（必要に応じて調整）
	struct CsvMap {
		size_t colName = 0;
		size_t colDescription = 1;
		size_t colGenre = 4;
		size_t colAbstructTargetEvent = 5;
	};

	bool loadCSV(const FilePath& csvPath);
	const SkillRow* findKey(const String& key) const;
	Array<String> keys() const;
private:
	Array<size_t> keepCols = { 0, 1, 4, 5 };
	HashTable<String, SkillRow> rows;
};


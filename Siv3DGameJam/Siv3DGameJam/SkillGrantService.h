#pragma once
# include "SkillCatalog.h"
# include "ObtainedSkills.h"
# include "SkillFileIO.h"

class SkillGrantService
{
public:
	static SkillGrantService& getInstance();

	void configure(SkillCatalog* _catalog, ObtainedSkills* _obtained)
	{
		catalog = _catalog;
		obtained = _obtained;
		ready = (catalog && obtained);
	}

	bool isReady() const noexcept { return ready; };

	// 外部システムから直接キー指定で付与（例: "FIX.atk"）
	bool grantByKey(const String& key)
	{
		if (!ready) return false;
		const SkillRow* row = catalog->findKey(key);
		if (!row) return false;
		const bool okFile = SkillFileIO::saveVisibleSkillFile(*row);
		const bool okList = obtained->add(*row);
		return okFile && okList;
	}

	// 外部から「候補キーを渡す」/「抽選関数を渡す」など拡張したい場合のフック
	using Picker = std::function<Optional<String>(const SkillCatalog&)>;
	bool grantByPicker(Picker picker)
	{
		if (!picker) return false;
		if (auto k = picker(*catalog))
		{
			return grantByKey(*k);
		}
		return false;
	}

	const SkillCatalog* getCatalog() const noexcept { return catalog; }
	ObtainedSkills* getObtained() const noexcept { return obtained; }

	// シングルトンなのでコピーとムーブを禁止
	SkillGrantService(const SkillGrantService&) = delete;
	SkillGrantService& operator=(const SkillGrantService&) = delete;

private:
	SkillGrantService() = default;
	~SkillGrantService() = default;
	
	SkillCatalog* catalog = nullptr;
	ObtainedSkills* obtained = nullptr;
	bool ready = false;
};

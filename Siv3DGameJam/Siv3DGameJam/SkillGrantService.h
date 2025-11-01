#pragma once
# include "SkillCatalog.h"
# include "ObtainedSkills.h"
# include "SkillFileIO.h"
#include <filesystem>
using namespace s3d;

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

	void resetSkillFolder()
	{
		const FilePath dir = U"assets/skill/";
		const FilePath abs = FileSystem::FullPath(dir);

		// まるごと消す（存在しなくてもOK）
#if SIV3D_PLATFORM(WINDOWS)
		std::filesystem::remove_all(Unicode::ToWstring(abs));
#else
		std::filesystem::remove_all(Unicode::ToUTF8(abs));
#endif

		// 作り直す
		FileSystem::CreateDirectories(dir);
	}

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

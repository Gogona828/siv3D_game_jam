#include "DropZone.h"
#include "SkillContainer.h"
using namespace s3d;

DropZone::DropZone(const SizeF& zoneSize, double gap)
{
	layoutHorizontalCenter(zoneSize, gap);
}

void DropZone::setDropBoxTexture(int32 num, String path, String front)
{
	zones[num].zoneTexture = Texture(path);
	if (front.isEmpty())
	{
		zones[num].frontVisble = false;
		return;
	}

	zones[num].frontTexture = Texture(front);
	zones[num].frontVisble = true;
}

void DropZone::layoutHorizontalCenter(const SizeF& zoneSize, double gap)
{
	const double totalWidth = (zoneSize.x * 4) + gap * 3;
	const Vec2 origin{ 550 - (totalWidth * 0.5) , 320};

	// originを中心にボックス配置
	for (size_t i = 0; i < 4; i++)
	{
		const double x = origin.x + i * (zoneSize.x + gap);
		const double y = origin.y;
		zones[i].rect = RectF{ x, y, zoneSize.x, zoneSize.y };
		zones[i].zoneTexture = Texture(U"assets/maingame/battle/battle_druganddrop.png");
	}
}

void DropZone::configZone(size_t i, const Array<String>& allowExts)
{
	if (i >= zones.size()) return;
	zones[i].allowExts = allowExts;
}

/// @brief
/// スロットへの代入をする
/// 敵もここから代入
/// @param i 
/// @param path 
/// @param forceOverWrite 
/// @return 
bool DropZone::assignSlot(size_t i, const FilePath& path, bool forceOverWrite)
{
	if (i >= zones.size()) return false;
	auto& zone = zones[i];

	if (!isAllowed(zone, path))
	{
		return false;
	}

	if (allComplete() && !forceOverWrite)
	{
		return false;
	}

	zone.slotPath = path;

#pragma region ボスならリターン弾く
	auto toLowerASCII = [](s3d::StringView sv) -> s3d::String
		{
			s3d::String out;
			out.reserve(sv.size());
			for (s3d::char32 ch : sv)
			{
				if (U'A' <= ch && ch <= U'Z') ch = (ch - U'A') + U'a';
				out.push_back(ch);
			}
			return out;
		};

	const s3d::String lowerPath = toLowerASCII(s3d::String{ path });
	const s3d::String lowerName = toLowerASCII(s3d::FileSystem::FileName(path));

	const bool bossInName = (lowerName.indexOf(s3d::StringView{ U"boss" }) != s3d::String::npos);

	const bool bossInDir =
		(lowerPath.indexOf(s3d::StringView{ U"/boss/" }) != s3d::String::npos) ||
		(lowerPath.indexOf(s3d::StringView{ U"\\boss\\" }) != s3d::String::npos) ||
		lowerPath.starts_with(s3d::StringView{ U"boss/" }) ||
		lowerPath.starts_with(s3d::StringView{ U"boss\\" }) ||
		lowerPath.ends_with(s3d::StringView{ U"/boss" }) ||
		lowerPath.ends_with(s3d::StringView{ U"\\boss" });

	const bool hasBossInPath = (bossInName || bossInDir);

	if (hasBossInPath) return true;
#pragma endregion

	return true;
}

String DropZone::getSkillKey(int slot) const
{
	if (slot < 0 || static_cast<size_t>(slot) >= zones.size()) {
		return U"";
	}
	const auto& opt = zones[slot].slotPath;
	if (!opt) {
		return U"";
	}
	const FilePath& fp = *opt;

	const String base = FileSystem::BaseName(fp);
	// 拡張子（.無し）
	const String ext = FileSystem::Extension(fp).lowercase(); 

	if (base.isEmpty()) return U"";
	// "FIX.atk" 形式
	return ext.isEmpty() ? base : (base + U"." + ext);
}

void DropZone::resetAllSlots()
{
	for (auto& zone : zones)
	{
		zone.slotPath.reset();
	}
}

bool DropZone::isZoneComplete(size_t i) const
{
	if (i >= zones.size()) return false;
	return zones[i].slotPath.has_value();
}

bool DropZone::allComplete() const
{
	for (const auto& zone : zones)
	{
		if (!zone.slotPath) return false;
	}
	return true;
}

bool DropZone::isAllowed(const Zone& zone, const FilePath& path)
{
	if (zone.allowExts.isEmpty()) return true;
	const String ext = FileSystem::Extension(path).lowercase();
	for (const auto& extension : zone.allowExts)
	{
		if (ext == extension.lowercased())
		{
			return true;
		}
	}
	return false;
}

void DropZone::update()
{
	// マウスオーバーチェック
	for (auto& zone : zones)
	{
		zone.mouseOver = zone.rect.mouseOver();
	}

	for (const auto& drop : DragDrop::GetDroppedFilePaths())
	{
		for (size_t i = 0; i < zones.size(); i++)
		{
			auto& zone = zones[i];
			if (!zone.rect.intersects(drop.pos)) continue;

			const bool isOk = assignSlot(i, drop.path, false);

			if (isOk)
			{
				// 認証
				setDropBoxTexture(i, playerBoard, playerFront);
				// TODO: スキル生成
				const String key = getSkillKey(static_cast<int>(i));
				if (!key.isEmpty())
				{
					SkillContainer::getInstance().registerSkill(key);
				}
			}
			else
			{
				if (!isAllowed(zone, drop.path))
				{
					// 入らないタイプ
					Console << U"[REJECT][" << i << U"] 拡張子不一致: ." << FileSystem::Extension(drop.path);
				}
				else if (allComplete())
				{
					// lock
					Console << U"[LOCKED][" << i << U"] すべてのスロットが埋まりロック中";
				}
			}
		}
	}
}

void DropZone::draw() const
{
	for (const auto& zone : zones)
	{
		zone.zoneTexture.resized(zone.rect.size).draw(zone.rect.pos);
		if (!zone.frontVisble) continue;
		zone.frontTexture.resized(zone.rect.size / 2).draw(Arg::center(zone.rect.center()));
		/*RectF rect = RectF(zone.rect.leftX() + 50 / 2, zone.rect.centerY());
		Texture(U"assets/maingame/battle/battle_triangle.png").draw(rect.pos);*/
	}
}

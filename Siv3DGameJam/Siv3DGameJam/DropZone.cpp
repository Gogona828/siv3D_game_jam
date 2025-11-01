#include "DropZone.h"
using namespace s3d;

DropZone::DropZone(const SizeF& zoneSize, double gap)
{
	layoutHorizontalCenter(zoneSize, gap);
}

void DropZone::setDropBoxTexture(int32 num, String path, String front)
{
	m_zones[num].zoneTexture = Texture(path);
	if (front.isEmpty())
	{
		m_zones[num].frontVisble = false;
		return;
	}

	m_zones[num].frontTexture = Texture(front);
	m_zones[num].frontVisble = true;
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
		m_zones[i].rect = RectF{ x, y, zoneSize.x, zoneSize.y };
		m_zones[i].zoneTexture = Texture(U"assets/maingame/battle/battle_druganddrop.png");
	}
}

void DropZone::configZone(size_t i, const Array<String>& allowExts)
{
	if (i >= m_zones.size()) return;
	m_zones[i].allowExts = allowExts;
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
	if (i >= m_zones.size()) return false;
	auto& zone = m_zones[i];

	if (!isAllowed(zone, path))
	{
		return false;
	}

	if (allComplete() && !forceOverWrite)
	{
		return false;
	}

	zone.slotPath = path;
	return true;
}

void DropZone::resetAllSlots()
{
	for (auto& zone : m_zones)
	{
		zone.slotPath.reset();
	}
}

bool DropZone::isZoneComplete(size_t i) const
{
	if (i >= m_zones.size()) return false;
	return m_zones[i].slotPath.has_value();
}

bool DropZone::allComplete() const
{
	for (const auto& zone : m_zones)
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
	for (auto& zone : m_zones)
	{
		zone.mouseOver = zone.rect.mouseOver();
	}

	for (const auto& drop : DragDrop::GetDroppedFilePaths())
	{
		for (size_t i = 0; i < m_zones.size(); i++)
		{
			auto& zone = m_zones[i];
			if (!zone.rect.intersects(drop.pos)) continue;

			const bool isOk = assignSlot(i, drop.path, false);

			if (isOk)
			{
				// 認証
				setDropBoxTexture(i, playerBoard, playerFront);
				// TODO: スキル生成

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
	for (const auto& zone : m_zones)
	{
		zone.zoneTexture.resized(zone.rect.size).draw(zone.rect.pos);
		if (!zone.frontVisble) continue;
		zone.frontTexture.resized(zone.rect.size / 2).draw(Arg::center(zone.rect.center()));
		/*RectF rect = RectF(zone.rect.leftX() + 50 / 2, zone.rect.centerY());
		Texture(U"assets/maingame/battle/battle_triangle.png").draw(rect.pos);*/
	}
}

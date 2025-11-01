#pragma once
# include<Siv3D.hpp>

class DropZone
{
public:
	struct Zone
	{
		RectF rect;
		Array<String> allowExts;
		Optional<FilePath> slotPath;
		Texture zoneTexture;
		bool mouseOver = false;
	};

	DropZone(const SizeF& zoneSize = SizeF{ Vec2{ 322, 245 } / 2.2 }, double gap = 15.0);

	void configZone(size_t i, const Array<String>& allowExts = {});
	void update();
	void draw() const;

	bool assignSlot(size_t i, const FilePath& path, bool forceOverWrite = false);

	void resetAllSlots();

	bool isZoneComplete(size_t i) const;
	bool allComplete() const;

	Zone& zone(size_t i)
	{
		return m_zones[i];
	}
	const Zone& zone(size_t i) const
	{
		return m_zones[i];
	}

	void setDropBoxTexture(int32 num, String path);

private:
	std::array<Zone, 4> m_zones;
	void layoutHorizontalCenter(const SizeF& zoneSize, double gap);
	static bool isAllowed(const Zone& zone, const FilePath& path);
};


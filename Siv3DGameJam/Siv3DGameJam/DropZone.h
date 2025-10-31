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
		bool mouseOver = false;
	};

	DropZone(const SizeF& zoneSize = SizeF{ 130, 100 }, double gap = 30.0);

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

private:
	std::array<Zone, 4> m_zones;
	void layoutHorizontalCenter(const SizeF& zoneSize, double gap);
	static bool isAllowed(const Zone& zone, const FilePath& path);
};


#pragma once
# include <Siv3D.hpp>

class AudioManager
{
public:
	static AudioManager& Get();

	void playBGM(const s3d::Audio& bgm);
	void stopBGM();
	void playSE(const s3d::Audio& se);

	void setBGMVolume(double vol);
	void setSEVolume(double vol);

	double getBGMVolume() const;
	double getSEVolume() const;

private:
	AudioManager();
	~AudioManager();

	s3d::Audio m_currentBGM;
	double m_bgmVolume = 0.5;
	double m_seVolume = 0.5;
};

# include "AudioManager.h"

AudioManager& AudioManager::Get()
{
	static AudioManager instance;
	return instance;
}

AudioManager::AudioManager() {}
AudioManager::~AudioManager() { stopBGM(); }

void AudioManager::playBGM(const s3d::Audio& bgm)
{
	if (m_currentBGM != bgm)
	{
		stopBGM();
		m_currentBGM = bgm;
		if (!m_currentBGM.isEmpty())
		{
			m_currentBGM.setLoop(true);
			m_currentBGM.setVolume(m_bgmVolume);
			m_currentBGM.play();
		}
	}
}

void AudioManager::stopBGM()
{
	if (!m_currentBGM.isEmpty())
		m_currentBGM.stop();
}

void AudioManager::playSE(const s3d::Audio& se)
{
	se.playOneShot(m_seVolume);
}

void AudioManager::setBGMVolume(double vol)
{
	m_bgmVolume = s3d::Clamp(vol, 0.0, 1.0);
	if (!m_currentBGM.isEmpty())
		m_currentBGM.setVolume(m_bgmVolume);
}

void AudioManager::setSEVolume(double vol)
{
	m_seVolume = s3d::Clamp(vol, 0.0, 1.0);
}

double AudioManager::getBGMVolume() const { return m_bgmVolume; }
double AudioManager::getSEVolume() const { return m_seVolume; }

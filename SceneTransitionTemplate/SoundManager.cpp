#include "SoundManager.h"

struct Sound
{
private:
	std::string soundName = "";
	std::string soundPath = "";
	int soundHandle;

public:

	void Set(std::string name, std::string path)
	{
		soundName = name;
		soundPath = path;
		soundHandle = LoadSoundMem(path.c_str());
	}

	std::string GetName()const
	{
		return soundName;
	}

	std::string GetPath()const
	{
		return soundPath;
	}

	int GetHandle()const
	{
		return soundHandle;
	}

	void DeleteSound()
	{
		DeleteSoundMem(soundHandle);
	}
};

SoundManager::SoundManager()
{
}

SoundManager::~SoundManager()
{
}

void SoundManager::Load(std::string name, std::string path, bool isBgm)
{
	//
	if (isBgm)
	{
		m_bgm.resize(m_bgm.size() + 1);

		for (int i = 0; i < m_bgm.size(); i++)
		{
			//存在していて
			if (m_bgm[i])
			{
				//
				if (m_bgm[i]->GetPath() == path)
				{

				}
			}
			//
			else
			{
				m_bgm[i] = new Sound;
				m_bgm[i]->Set(name, path);
			}
		}
	}

	else
	{
		m_se.resize(m_se.size() + 1);

		for (int i = 0; i < m_se.size(); i++)
		{
			if (m_se[i])
			{
				if (m_se[i]->GetPath() == path)
				{
					m_se.pop_back();
					return;
				}
			}
			else
			{
				m_se[i] = new Sound;
				m_se[i]->Set(name, path);
			}
		}
	}
}

void SoundManager::PlayBGM(std::string name, bool isFromStart)
{
	for (int i = 0; i < m_bgm.size(); i++)
	{
		if (m_bgm[i] && m_bgm[i]->GetName() == name)
		{
			if (CheckPlaying(m_bgm[i]->GetHandle()))
			{
				return;
			}

			PlaySoundMem(m_bgm[i]->GetHandle(), DX_PLAYTYPE_BACK, isFromStart);
			return;
		}
	}
}

void SoundManager::PlaySE(std::string name)
{
	for (int i = 0; i < m_se.size(); i++)
	{
		//
		if (m_se[i] && m_se[i]->GetName() == name)
		{
			if (CheckPlaying(m_se[i]->GetHandle()))
			{
				return;
			}

			PlaySoundMem(m_se[i]->GetHandle(), DX_PLAYTYPE_BACK, false);
			return;
		}
	}
}

void SoundManager::StopBGM(std::string name)
{
	for (int i = 0; i < m_bgm.size(); i++)
	{
		//
				//存在しているかつ、引数の名前と一致する名前
		if (m_bgm[i] && m_bgm[i]->GetName() == name)
		{
			//流れていなかったら何もしない
			if (!CheckPlaying(m_bgm[i]->GetHandle()))
			{
				return;
			}
			StopSoundMem(m_bgm[i]->GetHandle());
			return;
		}
	}
}

void SoundManager::Delete()
{
	for (int i = 0; i < m_bgm.size(); i++)
	{
		//存在していて
		if (m_bgm[i])
		{
			m_bgm[i]->DeleteSound();
			delete m_bgm[i];
			m_bgm[i] = nullptr;
		}
	}

	for (int i = 0; i < m_se.size(); i++)
	{
		//存在していて
		if (m_se[i])
		{
			m_se[i]->DeleteSound();
			delete m_se[i];
			m_se[i] = nullptr;
		}
	}
}

bool SoundManager::isPlayingSound(std::string name)
{
	for (int i = 0; i < m_bgm.size(); i++)
	{
		//存在しているかつ、引数の名前と一致する名前
		if (m_bgm[i] && m_bgm[i]->GetName() == name)
		{
			//流れていなかったら何もしない
			return CheckPlaying(m_bgm[i]->GetHandle());
		}
	}
}

void SoundManager::ChangeBgmVolume(int volume)
{
	m_bgmVolume = static_cast<float>(volume * 0.1f);
	for (int i = 0; i < m_bgm.size(); i++)
	{
		ChangeVolumeSoundMem(static_cast<int>(255 * m_bgmVolume), m_bgm[i]->GetHandle());
	}
}

void SoundManager::ChangeSeVolume(int volume)
{
	m_seVolume = static_cast<float>(volume * 0.1f);
	for (int i = 0; i < m_se.size(); i++)
	{
		ChangeVolumeSoundMem(static_cast<int>(255 * m_seVolume), m_se[i]->GetHandle());
	}
}

#include "SoundManager.h"


SoundManager* SoundManager::m_instance = nullptr;

namespace
{
	constexpr float kFadeFrame = 30.0f;
}

struct Sound
{
private:
	std::string SoundName = "";
	std::string SoundPath = "";
	int SoundHandle = -1;

public:

	/// <summary>
	/// サウンドの名前やサウンドのパス名をセットする
	/// </summary>
	/// <param name="name"></param>
	/// <param name="path"></param>
	void Set(std::string name, std::string path)
	{
		SoundName = name;
		SoundPath = path;
		SoundHandle = LoadSoundMem(path.c_str());
	}

	std::string GetName() const
	{
		return SoundName;
	}

	std::string GetPath() const
	{
		return SoundPath;
	}

	int GetHandle() const
	{
		return SoundHandle;
	}

	void DeleteSound()
	{
		DeleteSoundMem(SoundHandle);
	}

};

SoundManager::~SoundManager()
{
	for (auto& bgm : m_bgm)
	{
		bgm->DeleteSound();
	}

	for (auto& se : m_se)
	{
		se->DeleteSound();
	}

	InitSoundMem();
}

void SoundManager::Load(std::string name, std::string path, bool isBGM)
{
	//BGMフラグがtrueの時は、BGMのvectorに入れる
	if (isBGM)
	{
		m_bgm.resize(m_bgm.size() + 1);

		for (int i = 0; i < m_bgm.size(); i++)
		{
			//存在していて
			if (m_bgm[i])
			{
				//パスが同じ場合は増やした枠を減らして返す
				if (m_bgm[i]->GetPath() == path)
				{
					m_bgm.pop_back();
					return;
				}
			}
			//パスが同じものが存在していない場合、ロードをする
			else
			{
				m_bgm[i] = std::make_shared<Sound>();
				m_bgm[i]->Set(name, path);
				ChangeVolumeSoundMem(static_cast<int>(255 * m_bgmVolume), m_bgm[i]->GetHandle());
			}
		}
	}
	//BGMフラグがfalseの時は、SEのvectorに入れる
	else
	{
		m_se.resize(m_se.size() + 1);

		for (int i = 0; i < m_se.size(); i++)
		{
			//存在していて
			if (m_se[i])
			{
				//パスが同じ場合は増やした枠を減らして返す
				if (m_se[i]->GetPath() == path)
				{
					m_se.pop_back();
					return;
				}
			}
			//パスが同じものが存在していない場合、ロードをする
			else
			{
				m_se[i] = std::make_shared<Sound>();
				m_se[i]->Set(name, path);
				ChangeVolumeSoundMem(static_cast<int>(255 * m_seVolume), m_se[i]->GetHandle());
			}
		}
	}
}

void SoundManager::PlayBgm(std::string name, bool isFromStart)
{
	for (int i = 0; i < m_bgm.size(); i++)
	{
		//存在しているかつ、引数の名前と一致する名前
		if (m_bgm[i] && m_bgm[i]->GetName() == name)
		{
			//流れていたら何もしない
			if (CheckPlaying(m_bgm[i]->GetHandle()))
			{
				return;
			}

			PlaySoundMem(m_bgm[i]->GetHandle(), DX_PLAYTYPE_BACK, isFromStart);
			return;
		}
	}
}

void SoundManager::PlaySe(std::string name)
{
	for (int i = 0; i < m_se.size(); i++)
	{
		//存在しているかつ、引数の名前と一致する名前
		if (m_se[i] && m_se[i]->GetName() == name)
		{
			PlaySoundMem(m_se[i]->GetHandle(), DX_PLAYTYPE_BACK, true);
			return;
		}
	}
}

void SoundManager::FadeOutBgm(std::string name, int fadeFrame)
{
	float dif = m_bgmVolume * ((kFadeFrame - static_cast<float>(fadeFrame)) / kFadeFrame);
	for (int i = 0; i < m_bgm.size(); i++)
	{
		//存在しているかつ、引数の名前と一致する名前
		if (m_bgm[i] && m_bgm[i]->GetName() == name)
		{
			//BGMvolume = dif;
			ChangeVolumeSoundMem(static_cast<int>(255 * dif), m_bgm[i]->GetHandle());

			if (255 * dif <= 0.0f)
			{
				StopSoundMem(m_bgm[i]->GetHandle());
			}
			return;
		}
	}
	return;
}

void SoundManager::StopBgm(std::string name)
{
	for (int i = 0; i < m_bgm.size(); i++)
	{
		//存在しているかつ、引数の名前と一致する名前
		if (m_bgm[i] && m_bgm[i]->GetName() == name)
		{
			int tempHandle = m_bgm[i]->GetHandle();

			//流れていなかったら何もしない
			if (!CheckPlaying(tempHandle))
			{
				return;
			}

			StopSoundMem(tempHandle);

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
			//delete m_BGM[i];
			m_bgm[i] = nullptr;
		}
	}

	for (int i = 0; i < m_se.size(); i++)
	{
		//存在していて
		if (m_se[i])
		{
			m_se[i]->DeleteSound();
			//delete m_SE[i];
			m_se[i] = nullptr;
		}
	}
	return;
}

bool SoundManager::isPlayingSound(std::string name)
{
	for (int i = 0; i < m_bgm.size(); i++)
	{
		//存在しているかつ、引数の名前と一致する名前があったとき
		if (m_bgm[i] && m_bgm[i]->GetName() == name)
		{
			//流れているかどうかを返す
			return CheckPlaying(m_bgm[i]->GetHandle());
		}
	}
	return false;
}

void SoundManager::ChangeBGMVolume(float volume)
{
	m_bgmVolume = volume;
	for (int i = 0; i < m_bgm.size(); i++)
	{
		ChangeVolumeSoundMem(static_cast<int>(255 * m_bgmVolume), m_bgm[i]->GetHandle());
	}
	return;
}

void SoundManager::ChangeSEVolume(float volume)
{
	m_seVolume = volume;
	for (int i = 0; i < m_se.size(); i++)
	{
		ChangeVolumeSoundMem(static_cast<int>(255 * m_seVolume), m_se[i]->GetHandle());
	}
	return;
}

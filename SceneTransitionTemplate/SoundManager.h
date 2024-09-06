#pragma once
#include "DxLib.h"
#include <string>
#include <vector>

struct Sound;

class SoundManager
{
public:
	SoundManager();
	virtual ~SoundManager();

	void Load(std::string name, std::string path, bool isBgm);

	void PlayBGM(std::string name, bool isFromStart);
	void PlaySE(std::string name);

	void StopBGM(std::string name);
	
	void Delete();

	bool isPlayingSound(std::string name);

	void ChangeBgmVolume(int volume);
	void ChangeSeVolume(int volume);

private:
	std::vector<Sound*> m_bgm;
	std::vector<Sound*> m_se;

	float m_bgmVolume = 0.5f;
	float m_seVolume = 0.5f;

	bool CheckPlaying(int handle) { return CheckSoundMem(handle); }

};


#pragma once
#include "DxLib.h"
#include <string>
#include <vector>
#include <memory>

struct Sound;

class SoundManager
{
private:
	//シングルトンパターンなのでコンストラクタはprivateにおいておく
	SoundManager() {};

public:
	virtual ~SoundManager();

	//コピーコンストラクタから実体の生成ができてしまうため
	//コピーコンストラクタを禁止する
	SoundManager(const SoundManager&) = delete;
	SoundManager& operator=(const SoundManager&) = delete;
	SoundManager(SoundManager&&) = delete;
	SoundManager& operator= (const SoundManager&&) = delete;

	/// <summary>
	/// SoundManagerはGetInstance()を通した参照からしか利用できない
	/// </summary>
	/// <returns></returns>
	static SoundManager& GetInstance()
	{
		if (!m_instance)
		{
			m_instance = new SoundManager;
		}

		return *m_instance;
	}

	//これをし忘れると普通にメモリリーク
	static void Destroy()
	{
		delete m_instance;
		m_instance = nullptr;
	}

public:

	/// <summary>
	/// サウンドを読み込む
	/// </summary>
	/// <param name="name">サウンドの名前</param>
	/// <param name="path">サウンドのパス名</param>
	/// <param name="isBGM"> true = BGM , false = SE </param>
	void Load(std::string name, std::string path, bool isBGM);

	/// <summary>
	/// 指定した名前のサウンドを流す
	/// </summary>
	/// <param name="name">流したいサウンドの名前</param>
	/// <param name="isFromStart"> true = 最初から , false = 途中から </param>
	void PlayBgm(std::string name, bool isFromStart);

	void PlaySe(std::string name);

	void FadeOutBgm(std::string name, int fadeFrame);

	/// <summary>
	/// 指定した名前のサウンドを止める
	/// </summary>
	/// <param name="name">止めたいサウンドの名前</param>
	void StopBgm(std::string name);

	/// <summary>
	/// eternalフラグがfalseのサウンドを削除する
	/// </summary>
	void Delete();
	
	/// <summary>
	/// 指定したサウンドが流れているかどうか 
	/// </summary>
	/// <param name="name">サウンドの名前</param>
	/// <returns> true = 流れている , false = 流れていない </returns>
	bool isPlayingSound(std::string name);

	//
	void ChangeBGMVolume(float volume);
	void ChangeSEVolume(float volume);

private:

	bool CheckPlaying(int handle) { return CheckSoundMem(handle); }

private:
	//staticにすることで
	//Singletonのポインタがプログラム起動時に一つ作られるようにする
	static SoundManager* m_instance;

private:
	std::vector<std::shared_ptr<Sound>> m_bgm;
	std::vector<std::shared_ptr<Sound>> m_se;

	float m_bgmVolume;
	float m_seVolume;

};


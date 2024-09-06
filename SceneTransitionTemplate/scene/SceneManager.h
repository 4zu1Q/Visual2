#pragma once
#include <list>
#include <memory>

#include<windows.h>
#include"SoundManager.h"

struct SettingInfo
{
	int bgmVolume = 5;
	int seVolume = 5;
	bool isFull = false;
};

class SceneBase;
class SoundManager;
class SceneManager
{
public:
	SceneManager();
	virtual ~SceneManager();

	virtual void Init();	//シーンに入るときの初期化処理
	virtual void Update();	//毎フレーム行う更新処理
	virtual void Draw();	//毎フレーム行う描画処理
	virtual void End();	//シーンを抜けるときの処理

	//ゲーム終了のフラグ取得する
	virtual bool& GetClose() { return m_isClose; }
	virtual void SetClose(bool close) { m_isClose = close; }

	void SetInfo(int BGMvolume, int SEvolume, bool isFull);

	int GetBGMvolume() { return m_bgmVolume; }
	int GetSEvolume() { return m_seVolume; }
	bool GetisFull() { return m_isFull; }

public:

	std::shared_ptr<SceneBase> m_pScene;

	SoundManager m_pSoundManager;

private:
	//ゲームを終了させるフラグ
	bool m_isClose = false;

	int m_bgmVolume = 5;
	int m_seVolume = 5;
	bool m_isFull;

	void SetBgmVolume(int volume) { m_bgmVolume = volume; }
	void SetSeVolume(int volume) { m_seVolume = volume; }
	void SetIsFull(bool isFull) { m_isFull = isFull; }



};


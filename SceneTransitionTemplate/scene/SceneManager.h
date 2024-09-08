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

	virtual void Init();	//�V�[���ɓ���Ƃ��̏���������
	virtual void Update();	//���t���[���s���X�V����
	virtual void Draw();	//���t���[���s���`�揈��
	virtual void End();	//�V�[���𔲂���Ƃ��̏���

	//�Q�[���I���̃t���O�擾����
	virtual bool& GetClose() { return m_isClose; }
	virtual void SetClose(bool close) { m_isClose = close; }

	void SetInfo(int BGMvolume, int SEvolume, bool isFull);

	int GetBGMvolume() { return m_bgmVolume; }
	int GetSEvolume() { return m_seVolume; }
	bool GetisFull() { return m_isFull; }

public:

	std::shared_ptr<SceneBase> m_pScene;

	SoundManager m_pSoundManager;

	//�Q�[�����I��������t���O
	bool m_isClose = false;
private:

	int m_bgmVolume = 5;
	int m_seVolume = 5;
	bool m_isFull;

	void SetBgmVolume(int volume) { m_bgmVolume = volume; }
	void SetSeVolume(int volume) { m_seVolume = volume; }
	void SetIsFull(bool isFull) { m_isFull = isFull; }

};


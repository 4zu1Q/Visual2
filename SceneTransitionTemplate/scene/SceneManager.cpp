#include "SceneManager.h"
#include "SceneTitle.h"
#include "SceneDebug.h"

SceneManager::SceneManager():
	m_pScene(nullptr),
	m_isFull(false)
{
	//BGM�̃��[�h
	m_pSoundManager.Load("TitleBgm", "data/sound/bgm/TitleBgm.mp3", true);
	m_pSoundManager.Load("GamePlayBgm", "data/sound/bgm/GamePlayBgm.mp3", true);

	//�I������Se�����[�h
	m_pSoundManager.Load("SelectSe", "data/sound/se/SelectSe.mp3", false);
	m_pSoundManager.Load("DecisionSe", "data/sound/se/DecisionSe.mp3", false);
	m_pSoundManager.Load("CancelSe", "data/sound/se/CancelSe.mp3", false);


	//�L�����N�^�[��Se�����[�h
	m_pSoundManager.Load("AttackSe", "data/sound/se/GamePlayBgm.mp3", false);
	m_pSoundManager.Load("SkillSe", "data/sound/se/GamePlayBgm.mp3", false);
	m_pSoundManager.Load("DamageSe", "data/sound/se/GamePlayBgm.mp3", false);
	m_pSoundManager.Load("HearSe", "data/sound/se/GamePlayBgm.mp3", false);

}

SceneManager::~SceneManager()
{
}

void SceneManager::Init()
{
	//�ŏ��̃V�[���̃��������m�ۂ���
#ifdef _DEBUG
	m_pScene = std::make_shared<SceneDebug>();
#else
	m_pScene = std::make_shared<SceneTitle>();
#endif
	m_pScene->Init();
}

void SceneManager::Update()
{
	//SceneBase* pNext = m_pScene->Update();

	m_pSoundManager.ChangeBgmVolume(m_bgmVolume);
	m_pSoundManager.ChangeSeVolume(m_seVolume);

	std::shared_ptr<SceneBase>pNext = m_pScene->Update();
	if (pNext != m_pScene)
	{
		//���ݏ������̃V�[���̏I������
		m_pScene->End();

		//Update���Ԃ����V�����V�[���ɑJ�ڂ���
		m_pScene = pNext;
		m_pScene->Init();

	}
}

void SceneManager::Draw()
{
	m_pScene->Draw();
}

void SceneManager::End()
{
	m_pScene->End();
}

void SceneManager::SetInfo(int BGMvolume, int SEvolume, bool isFull)
{
	SetBgmVolume(BGMvolume);
	SetSeVolume(SEvolume);
	SetIsFull(isFull);
}


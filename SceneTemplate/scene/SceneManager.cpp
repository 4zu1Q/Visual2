#include "SceneManager.h"
#include "SceneTitle.h"
#include "util/Game.h"

namespace
{

	//�����p�����o�[�̕����̍��W
	constexpr int kUpdateBarStringPosX = 0;
	constexpr int kUpdateBarStringPosY = Game::kScreenHeight - 48;
	constexpr int kDrawBarPosStringPosX = 0;
	constexpr int kDrawBarPosStringPosY = Game::kScreenHeight - 32;

	//�����p�����o�[�̍��W
	constexpr int kExplainUpdateBarPosX1 = 34;
	constexpr int kExplainUpdateBarPosY1 = Game::kScreenHeight - 46;
	constexpr int kExplainUpdateBarPosX2 = 62;
	constexpr int kExplainUpdateBarPosY2 = Game::kScreenHeight - 34;
	constexpr int kExplainDrawBarPosX1 = 34;
	constexpr int kExplainDrawBarPosY1 = Game::kScreenHeight - 30;
	constexpr int kExplainDrawBarPosX2 = 62;
	constexpr int kExplainDrawBarPosY2 = Game::kScreenHeight - 18;


	//�����o�[�̐F
	constexpr int kUpdateBarColor = 0x0000ff;
	constexpr int kDrawBarColor = 0xff0000;

	//�����o�[�̍��W
	constexpr int kBarPosX = 0;
	constexpr int kBarPosY = Game::kScreenHeight - 16;
}

SceneManager::SceneManager():
	m_pScene(nullptr)
{
}

SceneManager::~SceneManager()
{
}

void SceneManager::Init()
{
	//�ŏ��̃V�[���̃��������m�ۂ���
	m_pScene = std::make_shared<SceneTitle>();
	m_pScene->Init();
}

void SceneManager::Update()
{
	//�X�V�̃��[�f�B���O���Ԃ��擾
	LONGLONG start = GetNowHiPerformanceCount();

	//SceneBase* pNext = m_pScene->Update();
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
	LONGLONG start = GetNowHiPerformanceCount();


	m_pScene->Draw();

#ifdef _DEBUG
	m_drawTime = static_cast<float>(GetNowHiPerformanceCount() - start);

	//�����o�[�̕\���@
	//����
	DrawString(kUpdateBarStringPosX, kUpdateBarStringPosY, "����", 0xffffff, 0x000000);
	DrawBox(kExplainUpdateBarPosX1, kExplainUpdateBarPosY1, kExplainUpdateBarPosX2, kExplainUpdateBarPosY2, kUpdateBarColor, true);
	DrawString(kDrawBarPosStringPosX, kDrawBarPosStringPosY, "�`��", 0xffffff, 0x000000);
	DrawBox(kExplainDrawBarPosX1, kExplainDrawBarPosY1, kExplainDrawBarPosX2, kExplainDrawBarPosY2, kDrawBarColor, true);

	float rate = static_cast<float>(m_updateTime + m_drawTime) / 16666.6f;
	float width = static_cast<float>(Game::kScreenWidth * rate);
	DrawBox(kBarPosX, kBarPosY, static_cast<int>(width), Game::kScreenHeight, kDrawBarColor, true);

	rate = static_cast<float>(m_updateTime) / 16666.6f;
	width = static_cast<float>(Game::kScreenWidth * rate);
	DrawBox(kBarPosX, kBarPosY, static_cast<int>(width), Game::kScreenHeight, kUpdateBarColor, true);
#endif
}

void SceneManager::End()
{
	m_pScene->End();
}

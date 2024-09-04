#include "SceneManager.h"
#include "SceneTitle.h"
#include "SceneDebug.h"

SceneManager::SceneManager():
	m_pScene(nullptr)
{
}

SceneManager::~SceneManager()
{
}

void SceneManager::Init()
{
	//最初のシーンのメモリを確保する
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
	std::shared_ptr<SceneBase>pNext = m_pScene->Update();
	if (pNext != m_pScene)
	{
		//現在処理中のシーンの終了処理
		m_pScene->End();

		//Updateが返した新しいシーンに遷移する
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


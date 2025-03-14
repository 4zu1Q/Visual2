#include "SceneManager.h"
#include <cassert>
#include "SceneBase.h"
#ifdef _DEBUG
#include "SceneDebug.h"
#else
#include "SceneTitle.h"
#endif
#include "Pad.h"
#include "Mouse.h"

SceneManager::SceneManager():
	m_pScene()
{
}
SceneManager::~SceneManager()
{

}

void SceneManager::init()
{
#ifdef _DEBUG
	m_pScene.reset(new SceneDebug);
#else
	m_pScene.reset(new SceneTitle);
#endif
	m_pScene->startLoad();	
}

void SceneManager::end()
{
	assert(m_pScene);
	if (!m_pScene)	return;

	m_pScene->endAll();
}

bool SceneManager::update()
{
	assert(m_pScene);
	if (!m_pScene)	return false;

	Pad::update();
	Mouse::update();

	m_pScene->updateAll();
	if (m_pScene->isSceneEnd())
	{
		// �V�[���J��

		m_pScene->end();	// �O�̃V�[���̏I������
		SceneBase* pNext = m_pScene->getNextScene();
		if (!pNext)
		{
			return false;	// nullptr�ŏI��
		}
		m_pScene.reset(pNext);
		m_pScene->startLoad();
	}
	return true;
}

void SceneManager::draw()
{
	assert(m_pScene);
	if (!m_pScene)	return;

	m_pScene->drawAll();
}
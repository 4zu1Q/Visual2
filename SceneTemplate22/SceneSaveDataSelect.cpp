#include "DxLib.h"

#include "util/Pad.h"
#include "util/Game.h"
#include "util/Vec2.h"

#include "SceneManager.h"
#include "SceneSaveDataSelect.h"
#include "SceneTitle.h"
#include "SceneSelect.h"

SceneSaveDataSelect::SceneSaveDataSelect(SceneManager& manager):
	SceneBase(manager)
{
	m_sceneTrans = e_SceneTrans::kSaveData1;

}

SceneSaveDataSelect::~SceneSaveDataSelect()
{
}

void SceneSaveDataSelect::Update()
{
	Pad::Update();
	UpdateFade();

	if (!m_isToNextScene)
	{
		//上を押した場合
		if (Pad::IsTrigger(PAD_INPUT_UP))
		{
			if (m_sceneTrans != e_SceneTrans::kSaveData1)
			{
				m_sceneTrans = static_cast<e_SceneTrans>(static_cast<int>(m_sceneTrans) - 1);
			}
		}

		//下を押した場合
		if (Pad::IsTrigger(PAD_INPUT_DOWN))
		{
			if (m_sceneTrans != e_SceneTrans::kSaveData3)
			{
				m_sceneTrans = static_cast<e_SceneTrans>(static_cast<int>(m_sceneTrans) + 1);
			}
		}

		//Aボタンを押した場合
		if (Pad::IsTrigger(PAD_INPUT_1))
		{
			//enum変数が同じだった場合
			if (m_sceneTrans == e_SceneTrans::kSaveData1)
			{
				StartFadeOut();
				m_isToNextScene = true;
			}

			if (m_sceneTrans == e_SceneTrans::kSaveData2)
			{
				StartFadeOut();
				m_isToNextScene = true;
			}

			if (m_sceneTrans == e_SceneTrans::kSaveData3)
			{
				StartFadeOut();
				m_isToNextScene = true;
			}
		}
	}


}

void SceneSaveDataSelect::Draw()
{
}

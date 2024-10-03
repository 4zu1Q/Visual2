#include "DxLib.h"

#include "util/Pad.h"

#include "SceneSelect.h"
#include "SceneManager.h"


SceneSelect::SceneSelect(SceneManager& manager) :
	SceneBase(manager)
{
}

SceneSelect::~SceneSelect()
{
}

void SceneSelect::Update()
{
	Pad::Update();
	UpdateFade();


	if (!m_isToNextScene)
	{
		//
		if (Pad::IsTrigger(PAD_INPUT_UP))
		{
			if (m_sceneTrans != e_SceneTrans::kSelect)
			{
				m_sceneTrans = static_cast<e_SceneTrans>(static_cast<int>(m_sceneTrans) - 1);
			}
		}

		//
		if (Pad::IsTrigger(PAD_INPUT_DOWN))
		{
			if (m_sceneTrans != e_SceneTrans::kQuit)
			{
				m_sceneTrans = static_cast<e_SceneTrans>(static_cast<int>(m_sceneTrans) + 1);
			}
		}

		//
		if (Pad::IsTrigger(PAD_INPUT_1))
		{
			if (m_sceneTrans == e_SceneTrans::kSelect)
			{
				StartFadeOut();
				m_isToNextScene = true;
			}

			if (m_sceneTrans == e_SceneTrans::kOption)
			{
				m_pManager.PushScene(std::make_shared<SceneSelect>(m_pManager));
				return;
			}
		}
	}


	//シーンフラグがたった場合
	if (m_isToNextScene)
	{
		if (!IsFadingOut())
		{
			if (m_sceneTrans == e_SceneTrans::kSelect)
			{
				m_pManager.ChangeScene(std::make_shared<SceneSelect>(m_pManager));
			}
		}
	}

}

void SceneSelect::Draw()
{
	DrawString(0, 0, "Scene Select", 0xffffff, false);
	DrawFade();
}

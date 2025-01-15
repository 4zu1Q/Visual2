#include "PlayerProduction.h"

#include "util/Pad.h"
#include "util/AnimController.h"
#include "util/EffectManager.h"
#include "util/SoundManager.h"
#include "util/Game.h"

#include <cmath>
#include <cassert>

namespace
{

	//プレイヤーのモデルファイル名
	const char* const kModelFilename = "Data/Model/Player/Player.mv1";

	const char* const kSwordModelFileName = "Data/Model/Weapon/Player_Sword.mv1";

	//モデルのスケール値
	constexpr float kModelScale = 5.0f;
	constexpr float kWeaponScale = 1.0f;

	/*　プレイヤーのアニメーションの種類　*/
	const char* const kAnimInfoFilename = "Data/Master/AnimPlayerProductionMaster.csv";

	//他シーンに使うアニメーション
	const char* const kAnimIdle = "Idle";
	const char* const kAnimDash = "Dash";
	const char* const kAnimSit = "Sit";
	const char* const kAnimSitDown = "SitDown";
	const char* const kAnimSitUp = "SitUp";
	const char* const kAnimGameOver = "GameOver";
	const char* const kAnimRetry = "Retry";
	const char* const kAnimGameClear = "GameClear";



	//プレイヤーの座標
	constexpr float kPosX = 2.0;
	constexpr float kPosY = -1.0;
	constexpr float kPosZ = -60.0;

	constexpr float kRotationY = 20.0f;
}

PlayerProduction::PlayerProduction():
	m_pos(VGet(0,0,0)),
	m_cameraPos(VGet(0,0,0)),
	m_isTitle(false),
	m_isGameOver(false),
	m_isGameClear(false)
{
	m_modelH = MV1LoadModel(kModelFilename);
	assert(m_modelH > -1);

	m_pAnim = std::make_shared<AnimController>();

}

PlayerProduction::~PlayerProduction()
{
	MV1DrawModel(m_modelH);
	m_modelH = -1;
}

void PlayerProduction::Initialize(Game::e_PlayerProduction playerScene)
{
	MV1SetScale(m_modelH, VGet(kModelScale, kModelScale, kModelScale));
	MV1SetPosition(m_modelH, m_pos);

	if (playerScene == Game::e_PlayerProduction::kTitle)
	{
		m_pAnim->Initialize(kAnimInfoFilename, m_modelH, kAnimSit);
		MV1SetRotationXYZ(m_modelH, VGet(0.0f, 100, 0.0f));

	}
	else if (playerScene == Game::e_PlayerProduction::kGameOver)
	{
		m_pAnim->Initialize(kAnimInfoFilename, m_modelH, kAnimGameOver);
		MV1SetRotationXYZ(m_modelH, VGet(0.0f, 30.0f, 0.0f));

	}
	else if (playerScene == Game::e_PlayerProduction::kGameClear)
	{
		m_pAnim->Initialize(kAnimInfoFilename, m_modelH, kAnimGameClear);
		MV1SetRotationXYZ(m_modelH, VGet(0.0f, kRotationY, 0.0f));

	}

	//プレイヤーの初期位置設定
	//m_pos = VGet(kPosX, kPosY, kPosZ);



	m_playerProduction = playerScene;
	m_updateFunc = &PlayerProduction::IdleUpdate;
}

void PlayerProduction::Update()
{
	//アップデート
	(this->*m_updateFunc)();

	//アニメーションの更新処理
	m_pAnim->UpdateAnim();

	MV1SetPosition(m_modelH, m_pos);
}

void PlayerProduction::Draw()
{
	MV1DrawModel(m_modelH);

}

void PlayerProduction::IdleUpdate()
{
	if (m_playerProduction == Game::e_PlayerProduction::kTitle)
	{
		OnSit();
	}
	else if (m_playerProduction == Game::e_PlayerProduction::kGameOver)
	{
		OnGameOver();
	}
	else if (m_playerProduction == Game::e_PlayerProduction::kGameClear)
	{
		OnGameClear();
	}
}

void PlayerProduction::TitleIdleUpdate()
{
	if (Pad::IsTrigger(PAD_INPUT_1))
	{
		OnDash();
	}

	if (Pad::IsTrigger(PAD_INPUT_2))
	{
		OnSitDown();
	}
}

void PlayerProduction::DashUpdate()
{
	m_pos.x += 0.5f;
	m_pos.z -= 0.5f;
}

void PlayerProduction::SitUpdate()
{
	if (Pad::IsTrigger(PAD_INPUT_1))
	{
		OnSitUp();
	}
}

void PlayerProduction::SitUpUpdate()
{
	if (m_pAnim->IsLoop())
	{
		OnTitleIdle();
	}
}

void PlayerProduction::SitDownUpdate()
{
	if (m_pAnim->IsLoop())
	{
		OnSit();
	}
}

void PlayerProduction::GameOverUpdate()
{
	if (Pad::IsTrigger(PAD_INPUT_1))
	{
		OnRetry();
	}
}

void PlayerProduction::RetryUpdate()
{

}

void PlayerProduction::GameClearUpdate()
{

}

void PlayerProduction::OnIdle()
{
	//タイプによってアニメーションを変える
	m_pAnim->ChangeAnim(kAnimIdle);
	m_updateFunc = &PlayerProduction::IdleUpdate;
}

void PlayerProduction::OnTitleIdle()
{
	//タイプによってアニメーションを変える
	m_pAnim->ChangeAnim(kAnimIdle);
	m_updateFunc = &PlayerProduction::TitleIdleUpdate;
}

void PlayerProduction::OnDash()
{
	//タイプによってアニメーションを変える
	m_pAnim->ChangeAnim(kAnimDash);
	m_updateFunc = &PlayerProduction::DashUpdate;
}

void PlayerProduction::OnSit()
{
	//タイプによってアニメーションを変える
	m_pAnim->ChangeAnim(kAnimSit);
	m_updateFunc = &PlayerProduction::SitUpdate;
}

void PlayerProduction::OnSitDown()
{
	//タイプによってアニメーションを変える
	m_pAnim->ChangeAnim(kAnimSitDown, false, true, true);
	m_updateFunc = &PlayerProduction::SitDownUpdate;
}

void PlayerProduction::OnSitUp()
{
	//タイプによってアニメーションを変える
	m_pAnim->ChangeAnim(kAnimSitUp, false, true, true);
	m_updateFunc = &PlayerProduction::SitUpUpdate;
}

void PlayerProduction::OnGameOver()
{
	//タイプによってアニメーションを変える
	m_pAnim->ChangeAnim(kAnimGameOver, false, true, true);
	m_updateFunc = &PlayerProduction::GameOverUpdate;
}

void PlayerProduction::OnRetry()
{
	//タイプによってアニメーションを変える
	m_pAnim->ChangeAnim(kAnimRetry, false, true, true);
	m_updateFunc = &PlayerProduction::RetryUpdate;
}

void PlayerProduction::OnGameClear()
{
	//タイプによってアニメーションを変える
	m_pAnim->ChangeAnim(kAnimGameClear);
	m_updateFunc = &PlayerProduction::GameClearUpdate;
}

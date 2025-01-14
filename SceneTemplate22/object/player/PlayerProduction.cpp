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
	const char* const kAnimTitle = "Title";
	const char* const kAnimTitleStart = "TitleStart";
	const char* const kAnimGameOver = "GameOver";
	const char* const kAnimRetry = "Retry";
	const char* const kAnimGameClear = "GameClear";

}

PlayerProduction::PlayerProduction():
	m_pos(VGet(0,0,0)),
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

}

void PlayerProduction::Initialize(Game::e_PlayerProduction playerScene)
{
	MV1SetScale(m_modelH, VGet(kModelScale, kModelScale, kModelScale));

	m_playerProduction = playerScene;
	m_updateFunc = &PlayerProduction::IdleUpdate;
}

void PlayerProduction::Update()
{
	//アップデート
	(this->*m_updateFunc)();

	//アニメーションの更新処理
	m_pAnim->UpdateAnim();

}

void PlayerProduction::Draw()
{

}

void PlayerProduction::IdleUpdate()
{
	if (m_isTitle)
	{
		OnTitle();
	}
	else if (m_isGameOver)
	{
		OnGameOver();
	}
	else if (m_isGameClear)
	{
		OnGameClear();
	}
}

void PlayerProduction::TitleUpdate()
{
	//if (Pad::IsTrigger(PAD_INPUT_1))
	//{
	//	
	//}
}

void PlayerProduction::TitleStartUpdate()
{

}

void PlayerProduction::GameOverUpdate()
{

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
	m_pAnim->ChangeAnim(kAnimTitle);
	m_updateFunc = &PlayerProduction::IdleUpdate;
}

void PlayerProduction::OnTitle()
{
	//タイプによってアニメーションを変える
	m_pAnim->ChangeAnim(kAnimTitle);
	m_updateFunc = &PlayerProduction::TitleUpdate;
}

void PlayerProduction::OnTitleStart()
{
	//タイプによってアニメーションを変える
	m_pAnim->ChangeAnim(kAnimTitleStart);
	m_updateFunc = &PlayerProduction::TitleStartUpdate;
}

void PlayerProduction::OnGameOver()
{
	//タイプによってアニメーションを変える
	m_pAnim->ChangeAnim(kAnimGameOver);
	m_updateFunc = &PlayerProduction::GameOverUpdate;
}

void PlayerProduction::OnRetry()
{
	//タイプによってアニメーションを変える
	m_pAnim->ChangeAnim(kAnimRetry);
	m_updateFunc = &PlayerProduction::RetryUpdate;
}

void PlayerProduction::OnGameClear()
{
	//タイプによってアニメーションを変える
	m_pAnim->ChangeAnim(kAnimGameClear);
	m_updateFunc = &PlayerProduction::GameClearUpdate;
}

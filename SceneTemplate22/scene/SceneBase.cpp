#include "SceneBase.h"
#include "util/Game.h"

#include "util/SoundManager.h"
#include "util/EffectManager.h"

namespace
{
	//フェード速度
	constexpr int kFadeSpeed = 5;

	constexpr int kFadeColor = 0xffffff;

	const char* kFontPath = "Data/Font/Dela-Gothic-One.ttf";

	constexpr float kCursorSpeed = 0.2f;


}


SceneBase::SceneBase(SceneManager& manager):
	m_pManager(manager)
{
	//初期設定はフェードアウト状態
	m_fadeColor = 0x000000;
	m_fadeBright = 255;
	m_fadeSpeed = -kFadeSpeed;
	m_isToNextScene = false;
	m_isFadeColor = false;
	m_fadeGraphSelectTime = 0;
	m_fadeGraphTitleTime = 0;

	/*サウンドのロード*/
	//BGM
	SoundManager::GetInstance().Load("titleBgm", "Data/Sound/Bgm/TitleBGM.mp3", true);
	SoundManager::GetInstance().Load("selectBgm", "Data/Sound/Bgm/SelectBGM.mp3", true);
	SoundManager::GetInstance().Load("battleBgm", "Data/Sound/Bgm/BattleBGM.mp3", true);
	SoundManager::GetInstance().Load("stageClearBgm", "Data/Sound/Bgm/StageClearClearBGM.mp3", true);
	SoundManager::GetInstance().Load("gameOverBgm", "Data/Sound/Bgm/GameOver.mp3", true);
	SoundManager::GetInstance().Load("gameClearBgm", "Data/Sound/Bgm/GameClearBGM.mp3", true);
	
	//カーソル用SE
	SoundManager::GetInstance().Load("selectSe", "Data/Sound/Se/Select.mp3", false);
	SoundManager::GetInstance().Load("dectionSe", "Data/Sound/Se/Dection.mp3", false);
	SoundManager::GetInstance().Load("backSe", "Data/Sound/Se/Back.mp3", false);
	SoundManager::GetInstance().Load("pouseSe", "Data/Sound/Se/PouseOpen.mp3", false);
	SoundManager::GetInstance().Load("pouseSe", "Data/Sound/Se/PouseOpen.mp3", false);

	//シーン遷移用SE
	SoundManager::GetInstance().Load("selectTransSe", "Data/Sound/Se/SelectSceneTrans.mp3", false);
	SoundManager::GetInstance().Load("gamePlayTransSe", "Data/Sound/Se/GamePlaySceneTrans.mp3", false);
	SoundManager::GetInstance().Load("pouseSe", "Data/Sound/Se/PouseOpen.mp3", false);
	SoundManager::GetInstance().Load("pouseSe", "Data/Sound/Se/PouseOpen.mp3", false);
	SoundManager::GetInstance().Load("faceSelectSe", "Data/Sound/Se/FaceSelect.mp3", false);


	//プレイヤー用SE
	SoundManager::GetInstance().Load("healHpSe", "Data/Sound/Se/HealHp.mp3", false);
	SoundManager::GetInstance().Load("healMpSe", "Data/Sound/Se/HealMp.mp3", false);
	SoundManager::GetInstance().Load("footstepsSe", "Data/Sound/Se/FootSteps.mp3", false);
	SoundManager::GetInstance().Load("jumpSe", "Data/Sound/Se/Jump.mp3", false);
	SoundManager::GetInstance().Load("attackFirstSe", "Data/Sound/Se/AttackXActionFirst.mp3", false);
	SoundManager::GetInstance().Load("attackSecondSe", "Data/Sound/Se/AttackXActionSecond.mp3", false);
	SoundManager::GetInstance().Load("attackThirdSe", "Data/Sound/Se/AttackXActionThird.mp3", false);
	SoundManager::GetInstance().Load("attackYSe", "Data/Sound/Se/AttackYAction.mp3", false);
	SoundManager::GetInstance().Load("attackChargeSe", "Data/Sound/Se/Charge.mp3", false);
	SoundManager::GetInstance().Load("damageSe", "Data/Sound/Se/Damage.mp3", false);
	SoundManager::GetInstance().Load("deadSe", "Data/Sound/Se/Dead.mp3", false);
	SoundManager::GetInstance().Load("faceUseSe", "Data/Sound/Se/FaceUse.mp3", false);
	SoundManager::GetInstance().Load("lockOnSe", "Data/Sound/Se/LockOn.mp3", false);
	SoundManager::GetInstance().Load("cameraResetSe", "Data/Sound/Se/CameraReset.mp3", false);

	//ボス用SE
	SoundManager::GetInstance().Load("bossFootStepsSe", "Data/Sound/Se/Back.mp3", false);
	SoundManager::GetInstance().Load("bossFootStepsSe", "Data/Sound/Se/Back.mp3", false);
	SoundManager::GetInstance().Load("bossFootStepsSe", "Data/Sound/Se/Back.mp3", false);
	SoundManager::GetInstance().Load("bossFootStepsSe", "Data/Sound/Se/Back.mp3", false);
	//SoundManager::GetInstance().Load("backSe", "Data/Sound/Se/Back.mp3", false);
	//SoundManager::GetInstance().Load("backSe", "Data/Sound/Se/Back.mp3", false);

	/*エフェクトのロード*/
	EffectManager::GetInstance().Load("playerHitEffect", "Data/Effect/PlayerHit.efkefc", 120, 6.0f);
	EffectManager::GetInstance().Load("bossHitEffect", "Data/Effect/BossHit.efkefc", 120, 2.0f);
	EffectManager::GetInstance().Load("hpHitEffect", "Data/Effect/HpHeal.efkefc", 120, 5.0f);
	EffectManager::GetInstance().Load("mpHitEffect", "Data/Effect/MpHeal.efkefc", 120 , 5.0f);
	EffectManager::GetInstance().Load("shotEffect", "Data/Effect/Charge.efkefc", 120, 1.0f);
	EffectManager::GetInstance().Load("moveEffect", "Data/Effect/Move.efkefc", 120, 1.0f);
	EffectManager::GetInstance().Load("jumpEffect", "Data/Effect/Jump.efkefc", 120, 2.0f);
	EffectManager::GetInstance().Load("gameClearEffect", "Data/Effect/StageClear01.efkefc", 200, 4.0f);
	EffectManager::GetInstance().Load("gameOverEffect", "Data/Effect/PlayerHit.efkefc", 120, 1.0f);
	EffectManager::GetInstance().Load("attackChargeEffect", "Data/Effect/AttackCharge.efkefc", 60, 8.0f);
	EffectManager::GetInstance().Load("attackChargeFinishEffect", "Data/Effect/ChargeFinish.efkefc", 120, 2.0f);
	EffectManager::GetInstance().Load("attackYEffect", "Data/Effect/PlayerShockAttack.efkefc", 40, 5.0f);
	EffectManager::GetInstance().Load("playerShockEffect", "Data/Effect/PlayerShockAttack.efkefc", 120, 1.0f);
	EffectManager::GetInstance().Load("bossShockEffect", "Data/Effect/BossShockAttack.efkefc", 160, 4.0f);
	EffectManager::GetInstance().Load("backGroundEffect", "Data/Effect/PlayerHit.efkefc", 120, 1.0f);
	EffectManager::GetInstance().Load("faceUseEffect", "Data/Effect/FaceUse01.efkefc", 120, 1.5f);
	EffectManager::GetInstance().Load("powerPreliminaryActionEffect", "Data/Effect/PowerPreliminaryAction.efkefc", 120, 3.0f);
	EffectManager::GetInstance().Load("speedPreliminaryActionEffect", "Data/Effect/SpeedPreliminaryAction.efkefc", 120, 3.0f);
	EffectManager::GetInstance().Load("shotPreliminaryActionEffect", "Data/Effect/ShotPreliminaryAction.efkefc", 120, 3.0f);
	EffectManager::GetInstance().Load("rassPreliminaryActionEffect", "Data/Effect/RassPreliminaryAction.efkefc", 120, 3.0f);
	EffectManager::GetInstance().Load("shotBossAttackEffect", "Data/Effect/ShotBossAttack.efkefc", 120, 5.5f);
	EffectManager::GetInstance().Load("shotPlayerAttackXEffect", "Data/Effect/ShotPlayerAttackX.efkefc", 120, 4.5f);
	EffectManager::GetInstance().Load("shotPlayerAttackYEffect", "Data/Effect/ShotPlayerAttackY2.efkefc", 120, 4.0f);
	EffectManager::GetInstance().Load("speedPlayerAttackYEffect", "Data/Effect/AttackY.efkefc", 120, 4.0f);
	EffectManager::GetInstance().Load("BossAttackShockEffect", "Data/Effect/BossShockAttack.efkefc", 120, 0.5f);

}

void SceneBase::UpdateFade()
{

	m_fadeBright += m_fadeSpeed;

	if (m_fadeBright >= 255)
	{
		m_fadeBright = 255;
		if (m_fadeSpeed > 0)
		{
			m_fadeSpeed = 0;
		}
	}

	if (m_fadeBright <= 0)
	{
		m_fadeBright = 0;
		if (m_fadeSpeed < 0)
		{
			m_fadeSpeed = 0;
		}

	}
}

void SceneBase::DrawFade(int fadeColor) const
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeBright);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, fadeColor, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void SceneBase::StartFadeOut()
{
	m_fadeSpeed = kFadeSpeed;
}

void SceneBase::StartFadeIn()
{
	m_fadeSpeed = -kFadeSpeed;
}

bool SceneBase::IsFadingIn() const
{
	if (m_fadeSpeed < 0) return true;
	return false;
}

bool SceneBase::IsFadingOut() const
{
	if (m_fadeSpeed > 0) return true;
	return false;
}

bool SceneBase::IsFinishFadeOut() const
{
	if (m_fadeBright == 255 && m_fadeSpeed == 0)
	{
		return true;
	}

	return false;
}

void SceneBase::FadeInSkip()
{
	m_fadeBright = 0;
	m_fadeSpeed = -kFadeSpeed;
}

void SceneBase::FadeOutSkip()
{
	m_fadeBright = 255;
	m_fadeSpeed = kFadeSpeed;
}

void SceneBase::UpdateFadeSelectGraph()
{
	// スタート指示を点滅させる
	if (m_fadeGraphSelectTime == 120) 
	{
		m_fadeGraphSelectTime++;
	}
	else if (m_fadeGraphSelectTime == 1) 
	{
		m_fadeGraphSelectTime--;
	}
	else if (m_fadeGraphSelectTime % 2 == 0) 
	{
		m_fadeGraphSelectTime += 2;
	}
	else 
	{
		m_fadeGraphSelectTime -= 2;
	}
}

void SceneBase::DrawFadeSelectGraph(int graphHandle, Vec2 graphPos)
{
	// フェードしながら描画
	int alpha = static_cast<int>(255 * ((float)m_fadeGraphSelectTime / 120));
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	// 画像の描画
	DrawGraph(graphPos.x, graphPos.y, graphHandle, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void SceneBase::FadeGraphSelectReset()
{
	m_fadeGraphSelectTime = 120;
}

void SceneBase::UpdateFadeGraphTitleLogo()
{
	// スタート指示を点滅させる
	if (m_fadeGraphTitleTime == 120)
	{
		m_fadeGraphTitleTime++;
	}
	else if (m_fadeGraphTitleTime % 2 == 0)
	{
		m_fadeGraphTitleTime += 2;
	}

}

void SceneBase::DrawFadeGraphTitleLogo(int graphHandle, Vec2 graphPos)
{
	// フェードしながら描画
	int alpha = static_cast<int>(255 * ((float)m_fadeGraphTitleTime / 120));
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	// 画像の描画
	DrawGraph(graphPos.x, graphPos.y, graphHandle, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void SceneBase::FadeGraphTitleLogoReset()
{
	m_fadeGraphTitleTime = 0;
}





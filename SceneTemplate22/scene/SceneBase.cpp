#include "SceneBase.h"
#include "util/Game.h"

#include "util/SoundManager.h"
#include "util/EffectManager.h"

namespace
{
	//フェード速度
	constexpr int kFadeSpeed = 5;

	constexpr int kFadeColor = 0xffffff;
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

	/*サウンドのロード*/
	//BGM
	SoundManager::GetInstance().Load("titleBgm", "Data/Sound/Bgm/Select.mp3", true);
	SoundManager::GetInstance().Load("selectBgm", "Data/Sound/Bgm/Select.mp3", true);
	SoundManager::GetInstance().Load("battleBgm", "Data/Sound/Bgm/Select.mp3", true);
	
	//カーソル用SE
	SoundManager::GetInstance().Load("selectSe", "Data/Sound/Se/Select.mp3", false);
	SoundManager::GetInstance().Load("dectionSe", "Data/Sound/Se/Dection.mp3", false);
	SoundManager::GetInstance().Load("backSe", "Data/Sound/Se/Back.mp3", false);
	SoundManager::GetInstance().Load("pouseSe", "Data/Sound/Se/PouseOpen.mp3", false);

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

	//ボス用SE
	SoundManager::GetInstance().Load("bossFootStepsSe", "Data/Sound/Se/Back.mp3", false);
	//SoundManager::GetInstance().Load("backSe", "Data/Sound/Se/Back.mp3", false);
	//SoundManager::GetInstance().Load("backSe", "Data/Sound/Se/Back.mp3", false);



	/*エフェクトのロード*/
	EffectManager::GetInstance().Load("hitEffect", "Data/Effect/player_hit.efk", 1200.0f , 10.0f);


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

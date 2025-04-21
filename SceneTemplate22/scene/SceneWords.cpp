#include "SceneWords.h"

#include "scene/SceneManager.h"
#include "scene/ScenePause.h"
#include "scene/SceneTitle.h"
#include "scene/SceneSelect.h"
#include "scene/SceneOption.h"

#include "util/Game.h"
#include "util/Pad.h"
#include "util/Font.h"
#include "util/SoundManager.h"

namespace
{
	//フォントのパス
	const char* kFontPath = "Data/Font/Dela-Gothic-One.ttf";

	//フォントサイズ	
	constexpr int kFontSize = 38;

	//エフェクトを出すフレーム数
	constexpr int kEffectFrame = 190;
	//チュートリアルのフレーム数
	constexpr int kTutorialFrame = 120;
	constexpr int kTutorialBossFrame = 180;

	//初期位置
	constexpr VECTOR kInitPos = { 400.0f,-35.0f,740.0f };
	constexpr VECTOR kLookPos = { -100.0f,30.0f,500.0f };

	//チュートリアルの説明を行う位置
	constexpr float kTutorialJumpPosZ = 620.0f;
	constexpr float kTutorialJumpClearPosZ = 506.0f;
	constexpr float kTutorialDashJumpPosZ = 317.0f;
	constexpr float kTutorialDashJumpClearPosZ = 88.0f;
	constexpr float kTutorialAttackXPosZ = -9.0f;
	constexpr float kTutorialAttackYPosZ = -242.0f;
	constexpr float kTutorialBossBattlePosZ = -455.0f;

	//ボスに行く部屋に表示される画像の座標
	const Vec2 kHitPos = { 440.0f , 480.0f };
	const Vec2 kHitBossPos = { 340.0f , 30.0f };
	const Vec2 kHitTextPos = { 740.0f , 230.0f };
	const Vec2 kHitText2Pos = { 743.0f , 230.0f };
	const Vec2 kHitItemPos = { 790.0f , 160.0f };
	const Vec2 kHitStarPos = { 790.0f , 220.0f };

	//調整する座標
	const Vec2 kHitAdjustmentPos = { 25.0f , 40.0f };
	const Vec2 kHitAdjustmentUpPos = { 25.0f , 80.0f };

	//プレイヤーの最初の位置
	constexpr VECTOR kPlayerPos = { 383.0f,-410.0f,670.0f };
	constexpr VECTOR kSelectPlayerPos = { 400.0f,-35.0f,740.0f };

	constexpr int kShadowMapSize = 16384;								// ステージのシャドウマップサイズ
	const VECTOR kShadowAreaMinPos = { -10000.0f, -500.0f, -10000.0f };		// シャドウマップに描画する最小範囲
	const VECTOR kShadowAreaMaxPos = { 10000.0f, 0.0f, 10000.0f };	// シャドウマップに描画する最大範囲
	const VECTOR kShadowDir = { 0.0f, -5.0f, 0.0f };					// ライト方向

	constexpr float kShadowColor = 0.7f;
	constexpr float kShadowAlpha = 0.3f;
}

SceneWords::SceneWords(SceneManager& manager):
	SceneBase(manager)

{
	m_handles.push_back(LoadGraph("Data/Image/TutorialSerihu.png"));

	m_fontH = Font::GetInstance().GetFontHandle(kFontPath, "Dela Gothic One", 32);


}

SceneWords::~SceneWords()
{
	//画像の削除
	for (int i = 0; i < m_handles.size(); i++)
	{
		DeleteGraph(m_handles[i]);
	}
	m_handles.clear();

}

void SceneWords::Update()
{
	Pad::Update();

	//スタートボタンを押した場合
	if (Pad::IsTrigger(PAD_INPUT_8))
	{
		//バックSEを入れる
		SoundManager::GetInstance().PlaySe("backSe");
		m_pManager.PopScene();
	}
}

void SceneWords::Draw()
{
	DrawGraph(130, 50, m_handles[0], true);
	//DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0xffffff, true);

	DrawFormatStringToHandle(300, 100, 0xffffff, m_fontH, "TEST");

}

#include "DxLib.h"

#include "SceneManager.h"
#include "SceneGamePlay.h"
#include "ScenePause.h"

#include "object/player/PlayerBase.h"
#include "object/SkyDome.h"

#include "util/Pad.h"

namespace
{
	constexpr float kCameraDist = 96;
	constexpr float kCameraHeight = 32;

	constexpr float kMaxSpeed = 0.9f;

	constexpr float kAnalogRangeMin = 0.1f;
	constexpr float kAnalogRangeMax = 0.8f;

	constexpr float kAnalogInputMax = 1000.0f;
}

SceneGamePlay::SceneGamePlay(SceneManager& manager) :
	SceneBase(manager),
#ifdef _DEBUG
	m_analogX(0),
	m_analogZ(0),
	m_radius(5),
	m_angle(0.0f),
#endif 
	m_cameraAngle(0.0f)
{
	m_pPlayer = std::make_shared<PlayerBase>();
	m_pSkyDome = std::make_shared<SkyDome>();

	m_playerPos = VGet(0, 0, 0);
	m_cameraPos = VGet(0, 0, 0);

	m_pPlayer->Initialize();
}

SceneGamePlay::~SceneGamePlay()
{
	
}


void SceneGamePlay::Update()
{
	UpdateFade();
	Pad::Update();

	if (!IsFading())
	{
		if (Pad::IsTrigger(PAD_INPUT_8))
		{
			m_pManager.PushScene(std::make_shared<ScenePause>(m_pManager));
		}
	}

	m_pSkyDome->Update();
	m_pPlayer->Update();

#ifdef _DEBUG

//	/*プレイヤーの移動*/
//
//	GetJoypadAnalogInput(&m_analogX, &m_analogZ, DX_INPUT_PAD1);
//	VECTOR move = VGet(m_analogX, 0.0f, -m_analogZ);
//	float len = VSize(move);
//	float rate = len / kAnalogInputMax;
//
//	//アナログスティック無効な範囲を除外する
//	rate = (rate - kAnalogRangeMin) / (kAnalogRangeMax - kAnalogRangeMin);
//	rate = min(rate, 1.0f);
//	rate = max(rate, 0.0f);
//
//	//速度が決定できるので移動ベクトルに反映する
//	move = VNorm(move);
//	float speed = kMaxSpeed * rate;
//	move = VScale(move, speed);
//
//	m_playerPos = VAdd(m_playerPos, move);
//
//	/*テスト中のカメラ移動*/
//
//	VECTOR aimPos = VGet(m_playerPos.x, m_playerPos.y, m_playerPos.z);
//	//ベクトルの方向(注視点-カメラのポジション)
//	VECTOR posToAim = VSub(aimPos, m_cameraPos);
//
//	VECTOR scalePosToAim = VScale(posToAim, 0.1f);
//
//	//アナログスティックを使って移動
//	int analogCameraX = 0;
//	int analogCameraY = 0;
//
//	GetJoypadAnalogInputRight(&analogCameraX, &analogCameraY, DX_INPUT_PAD1);
//
//	if (analogCameraX >= 10) m_cameraAngle -= 0.05f;
//	if (analogCameraX <= -10) m_cameraAngle += 0.05f;
//
//	m_cameraPos.x += cosf(m_cameraAngle) * 36;
//	m_cameraPos.y += 0;
//	m_cameraPos.z += sinf(m_cameraAngle) * 36;
//
//
//	m_cameraPos = VAdd(m_cameraPos, posToAim);
//
//
//
//
	if ((GetJoypadInputState(DX_INPUT_KEY_PAD1) & PAD_INPUT_5))
	{
		m_cameraAngle += 0.05f;
	}
	if ((GetJoypadInputState(DX_INPUT_KEY_PAD1) & PAD_INPUT_6))
	{
		m_cameraAngle -= 0.05f;
	}

	SetCameraNearFar(0.1f, 580.0f);
	VECTOR cameraPos;
	cameraPos.x = cosf(m_cameraAngle) * kCameraDist;
	cameraPos.y = kCameraHeight;
	cameraPos.z = sinf(m_cameraAngle) * kCameraDist;
	SetCameraPositionAndTarget_UpVecY(cameraPos, VGet(0, 0, 0));
//

#endif 
	

}

void SceneGamePlay::Draw()
{
	DrawString(0, 0, "Scene Game Play", 0xffffff, false);

	m_pSkyDome->Draw();
	m_pPlayer->Draw();

	//DrawSphere3D(m_playerPos, m_radius, 8, 0xff00ff, 0xffffff, false);
	//DrawSphere3D(m_cameraPos, m_radius, 8, 0x00ffff, 0x00ffff, false);

	DrawGrid();
	DrawFade();
}

void SceneGamePlay::DrawGrid()
{
	for (int x = -350; x <= 350; x += 5)
	{
		DrawLine3D(VGet(static_cast<float>(x), 0, -350), VGet(static_cast<float>(x), 0, 350), 0xffff00);
	}
	for (int z = -350; z <= 350; z += 5)
	{
		DrawLine3D(VGet(-350, 0, static_cast<float>(z)), VGet(350, 0, static_cast<float>(z)), 0xff0000);
	}

	// X+-,Z+-の方向が分かりやすいように表示を追加する
	VECTOR dispPos = ConvWorldPosToScreenPos(VGet(50, 0, 0));
	DrawStringF(dispPos.x, dispPos.y, "X+", 0xffffff);
	dispPos = ConvWorldPosToScreenPos(VGet(-50, 0, 0));
	DrawStringF(dispPos.x, dispPos.y, "X-", 0xffffff);
	dispPos = ConvWorldPosToScreenPos(VGet(0, 0, 50));
	DrawStringF(dispPos.x, dispPos.y, "Z+", 0xffffff);
	dispPos = ConvWorldPosToScreenPos(VGet(0, 0, -50));
	DrawStringF(dispPos.x, dispPos.y, "Z-", 0xffffff);
}
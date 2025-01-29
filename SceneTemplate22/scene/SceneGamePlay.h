#pragma once
#include "SceneBase.h"
#include "myLib/Physics.h"
#include "util/Game.h"

#include <vector>
#include <memory>
#include <List>

class Player;
class PlayerWeapon;
class Camera;
class Camera2;
class HpBar;
class PlayerBarUi;
class FaceUi;
class FaceFrameUi;
class ButtonUi;
class BossPower;
class BossSpeed;
class BossShot;
class BossRast;
class Tomb;
class Field;
class MyLib::Physics;

class SceneGamePlay :
	public SceneBase
{
public:
	SceneGamePlay(SceneManager& manager , Game::e_BossKind bosskind, Game::e_StageKind stageKind);
	virtual ~SceneGamePlay();

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update() override final;	//毎フレーム行う更新処理

	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw() override final;	//毎フレーム行う描画処理

	//void StartFadeOut();

private:

//#ifdef _DEBUG

	//スマートポインタ
	std::shared_ptr<Player> m_pPlayer;
	std::shared_ptr<PlayerWeapon> m_pPlayerWeapon;
	std::shared_ptr<Camera> m_pCamera;
	std::shared_ptr<Camera2> m_pCamera2;
	
	std::shared_ptr<BossPower> m_pBossPower;
	std::shared_ptr<BossSpeed> m_pBossSpeed;
	std::shared_ptr<BossShot> m_pBossShot;
	std::shared_ptr<BossRast> m_pBossRast;
	
	//std::shared_ptr<GamePlayUi> m_pGamePlayUi;

	std::shared_ptr<HpBar> m_pHpBarUi;
	std::shared_ptr<PlayerBarUi> m_pPlayerBarUi;
	std::shared_ptr<FaceFrameUi> m_pFaceFrameUi;
	std::shared_ptr<FaceUi> m_pFaceUi;
	std::shared_ptr<ButtonUi> m_pButtonUi;

	std::shared_ptr<Tomb> m_pTomb;
	std::shared_ptr<Field> m_pField;

	std::shared_ptr<MyLib::Physics> m_pPhysics;



	int m_lightHandle;

	//
	VECTOR m_playerPos;
	VECTOR m_cameraPos;

	int m_analogX;
	int m_analogZ;

	int m_gameOverTime;
	int m_gameClearTime;
	int m_selectTime;

	int m_radius;
	float m_angle;

	//画像ハンドルこれで全ての画像をロードする
	std::vector<int> m_handles;

	bool m_isFadingOut;
	float m_cameraAngle;

	//ゲームプレイからセレクトに戻るためのフラグ
	bool m_isPowerTriangl;
	bool m_isSpeedTriangl;
	bool m_isShotTriangl;

	//当たり判定フラグ
	bool m_isHpHit;
	bool m_isMpHit;

	bool m_isCameraLockOn;

	Game::e_BossKind m_bossKind;

};


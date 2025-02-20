#pragma once
#include "scene/SceneBase.h"
#include "myLib/Physics.h"
#include "util/Game.h"

#include <vector>
#include <memory>
#include <List>

class Player;
class PlayerWeapon;
class Camera;
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

private:

	//スマートポインタ
	std::shared_ptr<Player> m_pPlayer;
	std::shared_ptr<PlayerWeapon> m_pPlayerWeapon;
	std::shared_ptr<Camera> m_pCamera;
	std::shared_ptr<BossPower> m_pBossPower;
	std::shared_ptr<BossSpeed> m_pBossSpeed;
	std::shared_ptr<BossShot> m_pBossShot;
	std::shared_ptr<BossRast> m_pBossRast;
	std::shared_ptr<HpBar> m_pHpBarUi;
	std::shared_ptr<PlayerBarUi> m_pPlayerBarUi;
	std::shared_ptr<FaceFrameUi> m_pFaceFrameUi;
	std::shared_ptr<FaceUi> m_pFaceUi;
	std::shared_ptr<ButtonUi> m_pButtonUi;
	std::shared_ptr<Tomb> m_pTomb;
	std::shared_ptr<Field> m_pField;
	std::shared_ptr<MyLib::Physics> m_pPhysics;

	//ライトハンドル
	int m_lightHandle;

	int m_gameOverTime;
	int m_gameClearTime;
	int m_selectTime;

	//画像ハンドルこれで全ての画像をロードする
	std::vector<int> m_handles;

	//ゲームプレイからセレクトに戻るためのフラグ
	bool m_isPowerTriangl;
	bool m_isSpeedTriangl;
	bool m_isShotTriangl;

	//カメラがロックオンしているかどうか
	bool m_isCameraLockOn;

	//ボスの種類
	Game::e_BossKind m_bossKind;

};


#pragma once
#include "SceneBase.h"
#include "Camera.h"

class Camera;
class Player;
class Enemy;
class GimmickObj;
class Stage;
class SkyDome;
class ScenePlaying :
    public SceneBase
{
public:
    ScenePlaying();
    virtual ~ScenePlaying();

	virtual void Init();	//シーンに入るときの初期化処理
	virtual std::shared_ptr<SceneBase> Update();	//毎フレーム行う更新処理
	virtual void Draw();	//毎フレーム行う描画処理
	virtual void End();	//シーンを抜けるときの処理

private:
	std::shared_ptr<Camera> m_pCamera;
	std::shared_ptr<Player> m_pPlayer;
	std::shared_ptr<Enemy> m_pEnemy;
	std::shared_ptr<GimmickObj> m_pGimmick;
	std::shared_ptr<Stage> m_pStage;
	std::shared_ptr<SkyDome> m_pSkyDome;

	//フレーム関係
	int m_frameScene;

	//フラグ
	bool m_isInterval;

	//当たり判定
	bool m_isPlayerHit;
	bool m_isAttackHit;
	bool m_isGimmickHit;
};


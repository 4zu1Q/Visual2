#pragma once
#include "SceneBase.h"
#include "Camera.h"

class Camera;
class Player;
class Enemy;
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

	//カーソル選択
	enum Select
	{
		kRestart,		//スタート
		kOption,	//オプション
		kTitle,		//タイトル移動
	};

	std::shared_ptr<Camera> m_pCamera;
	std::shared_ptr<Player> m_pPlayer;
	std::shared_ptr<Enemy> m_pEnemy;
	std::shared_ptr<Stage> m_pStage;
	std::shared_ptr<SkyDome> m_pSkyDome;

	//ハンドル変数
	int m_restartH;
	int m_optionH;
	int m_titleH;
	int m_selectH;

	//フレーム関係
	int m_frameScene;
	int m_frameHit;
	int m_frameDamage;

	//フェードフラグ
	bool m_isInterval;
	bool m_isTitle;

	//当たり判定
	bool m_isPlayerHit;	//プレイヤーと敵があった場合
	bool m_isAttackHit;	//プレイヤーの攻撃が当たった場合
	bool m_isDamageHit;	//敵の攻撃が当たった場合

	float m_selectAnimation;

	//enum変数
	Select m_select;
	int m_selectPosY;

	//メニュー
	bool m_isMenu;

	bool m_isDamageCount;
	bool m_isHitCount;
	bool m_isCommand;

};


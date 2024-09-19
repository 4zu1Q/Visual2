#pragma once
#include "SceneBase.h"
#include "Camera.h"
#include <vector>
#include <memory>

class Camera;
class Player;
class Enemy;
class Stage;
class SkyDome;
class ItemBase;
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

	void CreateItemHp(VECTOR pos);	//HPを回復するアイテムを生成

	void Knockback(VECTOR pos ,VECTOR move ,float length);

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
	std::vector<ItemBase*> m_pItem;

	//ハンドル変数
	int m_restartH;
	int m_optionH;
	int m_titleH;
	int m_selectH;
	int m_operatorH;

	int m_hitSH;
	int m_itemSH;
	int	m_hitPH;
	int	m_itemPH;

	//モデル変数
	int m_modelHeartH;

	//フレーム関係
	int m_frameScene;
	int m_fadeFrameScene;
	int m_playerAttackHitFrame;
	int m_playerSkillHitFrame;
	int m_playerFrame;
	int m_enemyAttackHitFrame;
	int m_enemySkillHitFrame;
	int m_enemyFrame;

	

	//フェードフラグ
	bool m_isWin;
	bool m_isLose;
	bool m_isInterval;
	bool m_isTitle;

	//当たり判定
	bool m_isPlayerHit;	//プレイヤーと敵があった場合
	bool m_isPlayerAttackHit;	//プレイヤーの攻撃が当たった場合
	bool m_isPlayerSkillHit;	//プレイヤーのスキルが当たった場合
	bool m_isEnemyAttackHit;	//敵の攻撃が当たった場合
	bool m_isEnemySkillHit;	//敵の攻撃が当たった場合
	bool m_isItemHit;	//アイテムとプレイヤーが当たった場合

	//死んだ判定
	bool m_isEnemyDeath;
	bool m_isPlayerDeath;

	bool m_isEnemySearch;
	bool m_isEnemyStop;

	float m_selectAnimation;

	//enum変数
	Select m_select;
	int m_selectPosY;

	//メニュー
	bool m_isMenu;

	bool m_isEnemyAttackHitCount;
	bool m_isEnemySkillHitCount;
	bool m_isPlayerAttackHitCount;
	bool m_isPlayerSkillHitCount;
	bool m_isCommand;
	bool m_isOption;

	int m_soundBgmH;
	int m_soundSelectH;
	int m_soundCancelH;
	int m_soundDecsionH;

	int m_soundPlayerDamageH;
	int m_soundADamageH;
	int m_soundSDamageH;
	int m_soundItemH;

};


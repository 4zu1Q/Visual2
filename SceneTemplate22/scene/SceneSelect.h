#pragma once
#include "SceneBase.h"

#include <vector>
#include <memory>

class Player;
class Camera;
class SkyDome;
class HpBar;
class FaceUi;
class ItemBase;
class ItemMp;
class ItemHp;
class Tomb;

class SceneSelect :
	public SceneBase
{
public:
	SceneSelect(SceneManager& manager);
	virtual ~SceneSelect();

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
	std::shared_ptr<Camera> m_pCamera;
	std::shared_ptr<SkyDome> m_pSkyDome;
	std::shared_ptr<HpBar> m_pHpBar;
	std::shared_ptr<FaceUi> m_pFaceUi;
	
	std::shared_ptr<ItemBase> m_pItem;	//後々消す

	std::shared_ptr<ItemMp> m_pItemMp;
	std::shared_ptr<ItemHp> m_pItemHp;
	std::shared_ptr<Tomb> m_pTomb;

	//遷移先
	enum class e_SceneTrans : int
	{
		kPowerTypeBoss,		//パワータイプのボス
		kSpeedTypeBoss,		//スピードタイプのボス
		kShooterTypeBoss,	//遠距離タイプのボス
		kRastBoss,			//ラスボス
	};

	e_SceneTrans m_sceneTrans;

	int m_frameCount;

	VECTOR m_playerPos;
	VECTOR m_cameraPos;

	int m_analogX;
	int m_analogZ;

	int m_radius;
	float m_angle;

	bool m_isFadingOut;
	float m_cameraAngle;


	/* 当たり判定フラグ */

	//アイテムの当たり判定フラグ
	bool m_isHpHit;
	bool m_isMpHit;

	//墓の当たり判定フラグ
	bool m_isTombHitP;
	bool m_isTombHitS;
	bool m_isTombHitR;


};


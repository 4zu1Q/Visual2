#pragma once
#include "SceneBase.h"
#include "myLib/Physics.h"


#include <vector>
#include <memory>

class Player;
class Camera;
class SkyDome;
class HpBar;
class FaceUi;
class FaceFrameUi;
class ButtonUi;
class ItemMp;
class ItemHp;
class Tomb;
class Field;
class MyLib::Physics;

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

	std::shared_ptr<HpBar> m_pHpBarUi;
	std::shared_ptr<FaceUi> m_pFaceUi;
	std::shared_ptr<FaceFrameUi> m_pFaceFrameUi;
	std::shared_ptr<ButtonUi> m_pButtonUi;
	
	std::shared_ptr<ItemMp> m_pItemMp;
	std::shared_ptr<ItemHp> m_pItemHp;
	std::shared_ptr<Tomb> m_pTomb;
	std::shared_ptr<Field> m_pField;


	std::shared_ptr<MyLib::Physics> m_pPhysics;

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

};


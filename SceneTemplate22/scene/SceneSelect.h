#pragma once
#include "scene/SceneBase.h"
#include "myLib/Physics.h"

#include "util/Game.h"

#include <vector>
#include <memory>

class Player;
class PlayerWeapon;
class Camera;
class FaceUi;
class FaceFrameUi;
class ButtonUi;
class PlayerBarUi;
class ItemMp;
class ItemHp;
class SkyDome;
class Field;
class Tomb;
class MyLib::Physics;

class SceneSelect :
	public SceneBase
{
public:
	SceneSelect(SceneManager& manager, Game::e_StageKind stageKind, VECTOR playerPos);
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
	std::shared_ptr<PlayerWeapon> m_pPlayerWeapon;
	std::shared_ptr<Camera> m_pCamera;
	
	std::shared_ptr<PlayerBarUi> m_pPlayerBarUi;
	std::shared_ptr<FaceUi> m_pFaceUi;
	std::shared_ptr<FaceFrameUi> m_pFaceFrameUi;
	std::shared_ptr<ButtonUi> m_pButtonUi;

	std::shared_ptr<ItemMp> m_pItemMp;
	std::shared_ptr<ItemHp> m_pItemHp;

	std::shared_ptr<Field> m_pField;
	std::shared_ptr<Tomb> m_pTomb;
	std::shared_ptr<SkyDome> m_pSkyDome;

	std::shared_ptr<MyLib::Physics> m_pPhysics;

	//遷移先
	enum class e_SceneTrans : int
	{
		kPowerTypeBoss,		//パワータイプのボス
		kSpeedTypeBoss,		//スピードタイプのボス
		kShooterTypeBoss,	//遠距離タイプのボス
		kRastTypeBoss,		//ラスボス
	};

	//enum変数
	e_SceneTrans m_sceneTrans;

	//画像ハンドルこれで全ての画像をロードする
	std::vector<int> m_handles;
	
	int m_fontH;	//フォントハンドル
	int m_shadowH;	//シャドウマップハンドル

	int m_effectFrame;	//エフェクトのフレーム

	//ステージに移動する当たり判定フラグ
	bool m_isPowerStage;
	bool m_isSpeedStage;
	bool m_isShotStage;
	bool m_isRastStage;

};


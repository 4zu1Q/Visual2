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
class SkyDome;
class Field;
class MyLib::Physics;

class SceneTutorial : public SceneBase
{
public:

	SceneTutorial(SceneManager& manager);
	virtual ~SceneTutorial();

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


	//ステージ移動する当たり判定フラグ
	bool m_isSelectStage;

	//


};


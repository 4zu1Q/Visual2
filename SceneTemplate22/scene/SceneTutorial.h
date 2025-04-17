#pragma once
#include "scene/SceneBase.h"
#include "myLib/Physics.h"

#include "util/Game.h"

#include <vector>
#include <memory>

class Player;
class PlayerWeapon;
class EnemyNormal;
class EnemySpecial;
class BossTutorial;
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

	SceneTutorial(SceneManager& manager, Game::e_StageKind stageKind);
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
	std::shared_ptr<EnemyNormal> m_pEnemyNormal;
	std::shared_ptr<EnemySpecial> m_pEnemySpecial;

	std::shared_ptr<BossTutorial> m_pBossTutorial;

	std::shared_ptr<PlayerBarUi> m_pPlayerBarUi;
	std::shared_ptr<FaceUi> m_pFaceUi;
	std::shared_ptr<FaceFrameUi> m_pFaceFrameUi;
	std::shared_ptr<ButtonUi> m_pButtonUi;

	std::shared_ptr<Field> m_pField;
	std::shared_ptr<SkyDome> m_pSkyDome;

	std::shared_ptr<MyLib::Physics> m_pPhysics;


	//画像ハンドルこれで全ての画像をロードする
	std::vector<int> m_handles;

	//チュートリアルの時に使う
	std::vector<bool> m_isTutorial;

	int m_fontH;	//フォントハンドル
	int m_shadowMap;	//シャドウマップハンドル

	int m_effectFrame;	//エフェクトのフレーム

	//ステージ移動する当たり判定フラグ
	bool m_isSelectStage;

	//


};


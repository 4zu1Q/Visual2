#pragma once
#include "DxLib.h"
#include <memory>


//class Player;
class Camera
{
public:

	Camera();
	virtual ~Camera();

	void Initialize();
	void Finalize();

	//void Update(Player& player);
	void Draw();

	void Update(VECTOR playerPos/*, bool isLockOn*/);

	void ResetCamera();

	//デバッグ用のアップデート
	void DebugUpdate();


	const VECTOR& GetPos() const { return m_nextPos; }


	//カメラのアングルを取得
	const float GetAngle() const { return m_angle; }
	
	/// <summary>
	/// カメラのX角度の取得
	/// </summary>
	/// <returns></returns>
	float GetCameraAngleX() { return m_angleH; }


private:
	
	/// <summary>
	/// カメラの座標アップデート処理
	/// </summary>
	void CameraPosUpdate();

	/// <summary>
	/// 通常時の更新処理
	/// </summary>
	/// <param name="targetPos">注視点</param>
	void NormalUpdate(VECTOR targetPos);

	/// <summary>
	/// 角度のアップデート処理
	/// </summary>
	/// <param name="isLookOn">ロックオンするかどうか</param>
	void AngleUpdate(/*bool isLookOn*/);
	
	/// <summary>
	/// ロックオン状態のときの更新
	/// </summary>
	/// <param name="targetPos"></param>
	void LockOnUpdate(VECTOR targetPos);


private:

	//スマートポインタ
	//std::shared_ptr<Player> m_pPlayer;



	
	// MyLibのやつ継承すると思われ
	VECTOR m_pos;				//座標
	VECTOR m_prevPos;			//前フレーム座標
	VECTOR m_nextPos;			//移動する予定の座標



	VECTOR m_targetPos;			//注視点座標
	VECTOR m_cameraangle;		//カメラのアングル

	MATRIX m_rotX;
	MATRIX m_rotY;

	float m_angle;	//カメラ角度

	float m_angleH;		//水平角度 弧度法
	float m_angleV;		//垂直角度 弧度法

	float m_cameraToTargetLength;	//カメラからプレイヤーの距離

};


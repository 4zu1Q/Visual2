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

	void Update(int stageHandle, VECTOR targetPos);
	void Draw();


	void ResetCamera();

	//デバッグ用のアップデート
	void DebugUpdate(VECTOR playerPos);


	const VECTOR& GetNextPos() const { return m_nextPos; }


	//カメラのアングルを取得
	const float GetAngle() const { return m_angle; }
	
	/// <summary>
	/// カメラのX角度の取得
	/// </summary>
	/// <returns></returns>
	float GetCameraAngleX() { return m_angleH; }


	void SetPlayerPos(VECTOR playerPos) { m_playerPos = playerPos; }

	const VECTOR GetDirection();

	bool GetIsDash() const { return m_isDash; }
	void SetIsDash(bool isDash) { m_isDash = isDash; }

	void CameraProcess(VECTOR playerPos);

private:
	
	/// <summary>
	/// カメラの座標アップデート処理
	/// </summary>
	void CameraPosUpdate();

	/// <summary>
	/// 通常時の更新処理
	/// </summary>
	/// <param name="targetPos">注視点</param>
	//void NormalUpdate(VECTOR targetPos);

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

	void NormalUpdate(VECTOR targetPos);
	void DashUpdate(VECTOR targetPos);
	void TargetUpdate(VECTOR targetPos);
	void MovieUpdate(VECTOR targetPos);
	void ClearUpdate(VECTOR targetPos);

	void OnNormal();
	void OnDash();
	void OnTarget();
	void OnMovie();
	void OnClear();


private:

	//メンバ関数ポインタ
	using UpdateFunc_t = void(Camera::*)(VECTOR targetPos);
	UpdateFunc_t m_updateFunc;

	//スマートポインタ
	//std::shared_ptr<Player> m_pPlayer;

	//////////////////今やってるカメラの変数

	//カメラの角度
	float m_cameraAngleX;
	float m_cameraAngleY;

	VECTOR m_cameraPos;		//カメラ座標
	VECTOR m_aimPos;		//カメラが向いているベクトル
	VECTOR m_playerPos;		//プレイヤーの座標

	VECTOR m_testPositon;

	VECTOR m_targetPos;			//注視点座標

	int m_lightHandle;		//ライトハンドル
	float m_angleMoveScale;	//移動の大きさ

	float m_cameraDist;

	MV1_COLL_RESULT_POLY_DIM m_hitDim{};	//当たり判定結果構造体


	//フラグ
	bool m_isDash;
	bool m_isTarget;
	bool m_isMovie;
	bool m_isClear;


	/////////別のカメラのやり方↓

	// MyLibのやつ継承すると思われ
	VECTOR m_pos;				//座標
	VECTOR m_prevPos;			//前フレーム座標
	VECTOR m_nextPos;			//移動する予定の座標



	//VECTOR m_targetPos;			//注視点座標
	VECTOR m_cameraangle;		//カメラのアングル

	MATRIX m_rotX;
	MATRIX m_rotY;

	float m_angle;	//カメラ角度

	float m_angleH;		//水平角度 弧度法
	float m_angleV;		//垂直角度 弧度法

	float m_cameraToTargetLength;	//カメラからプレイヤーの距離

	VECTOR m_setEye;
	VECTOR m_setTarget;


};


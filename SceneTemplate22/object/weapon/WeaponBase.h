#pragma once
#include "object/ObjectBase.h"


class WeaponBase : public ObjectBase
{
public:

	WeaponBase(Collidable::e_Priority priority, e_GameObjectTag tag, MyLib::ColliderData::e_Kind kind, bool isTrigger);
	virtual ~WeaponBase();

	void Initialize(int modelH, int modelRightFrame, int modelLeftFrame);
	void Finalize();



	/// <summary>
	/// 右手に武器を持たせたいとき用の関数
	/// </summary>
	/// <param name="modelH"></param>
	void OneHandUpdate(int weaponModelH);

	/// <summary>
	/// 両手に武器を持たせたいとき用の関数
	/// </summary>
	/// <param name="modelH"></param>
	void BothHandsUpdate(int rightWeaponModelH, int leftWeaponModelH);

	/// <summary>
	/// 片手だけ描画
	/// </summary>
	void OneHandDraw(int weaponModelH);

	/// <summary>
	/// 両手だけ描画
	/// </summary>
	void BothHandsDraw(int rightWeaponModelH, int leftWeaponModelH);


protected:

	//モデルハンドル
	int m_modelH;

	int m_modelRightFrame;	//アタッチするモデルの右手のフレーム
	int m_modelLeftFrame;	//アタッチするモデルの左手のフレーム

	VECTOR m_attachFramePos;	//武器をアタッチする座標

};


#pragma once
#include "object/ObjectBase.h"


class WeaponBase : public ObjectBase
{
public:

	//WeaponBase(Collidable::e_Priority priority, e_GameObjectTag tag, MyLib::ColliderData::e_Kind kind, bool isTrigger);
	WeaponBase();
	virtual ~WeaponBase();

	void Initialize(int modelH, int ownerModelH , const TCHAR* frameLeftName, const TCHAR* frameRightName, float modelSize);
	void Finalize();


	void Update(VECTOR moveVec);
	void Draw();


	/// <summary>
	/// 右手に武器を持たせたいとき用の関数
	/// </summary>
	/// <param name="modelH"></param>
	void OneHandUpdate(VECTOR moveVec);

	/// <summary>
	/// 両手に武器を持たせたいとき用の関数
	/// </summary>
	/// <param name="modelH"></param>
	void BothHandsUpdate(VECTOR moveVec);

	/// <summary>
	/// 片手だけ描画
	/// </summary>
	void OneHandDraw();

	/// <summary>
	/// 両手だけ描画
	/// </summary>
	void BothHandsDraw(int rightWeaponModelH, int leftWeaponModelH);

private:

	void SetAttachFrame(const TCHAR* frameLeftName,const TCHAR* frameRightName);

protected:

	void SetModelSize(float modelSize);

private:

	//モデルハンドル
	int m_weaponModelH;
	int m_ownerModelH;

	int m_weaponRightAttachFrame;	//アタッチするモデルの右手のフレーム
	int m_weaponlLeftAttachFrame;	//アタッチするモデルの左手のフレーム

	float m_modelSize;

	VECTOR m_attachFramePos;	//武器をアタッチする座標


	MATRIX m_mat;
};


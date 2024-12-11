#pragma once
#include "DxLib.h"
#include <memory>


class PlayerWeapon 
{

public:
	PlayerWeapon();
	virtual ~PlayerWeapon();

	/// <summary>
	/// モデルのロード用
	/// </summary>
	void Load();

	void Initialize(int modelHint, int modelRightFrame, int modelLeftFrame);
	void Finalize();

	//void InitializeCol(std::shared_ptr<MyLib::Physics> physics);
	//void FinalizeCol(std::shared_ptr<MyLib::Physics> physics);

	/// <summary>
	/// 苦肉の策
	/// 武器をモデルのフレームにアタッチするための関数
	/// </summary>
	void SwordUpdate();
	void AxeUpdate();
	void DaggerUpdate();
	void MagicWandUpdate();
	void LongSwordUpdate();
	
	void SwordDraw();
	void AxeDraw();
	void DaggerDraw();
	void MagicWandDraw();
	void LongSwordDraw();

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


private:

	//武器のモデルハンドル
	int m_swordModelH;
	int m_axeModelH;
	int m_daggerRightModelH;
	int m_daggerLeftModelH;
	int m_magicWandModelH;
	int m_longSwordModelH;

	//アタッチするモデル
	int m_modelH;	

	//武器に関する変数
	VECTOR m_attachFramePos;

	int m_modelRightFrame;	//アタッチするモデルの右手のフレーム
	int m_modelLeftFrame;	//アタッチするモデルの左手のフレーム



};


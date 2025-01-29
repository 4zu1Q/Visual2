
#include "PlayerWeapon.h"
#include "Player.h"

namespace
{
	//武器のファイル名
	const char* const kSwordModelFileName = "Data/Model/Weapon/Player_Sword.mv1";
	const char* const kAxeModelFileName = "Data/Model/Weapon/Player_Ax1e.mv1";
	const char* const kDaggerModelFileName = "Data/Model/Weapon/Player_Dagge1r.mv1";
	const char* const kMagicWandModelFileName = "Data/Model/Weapon/Player_MagicWan1d.mv1";
	const char* const kLongSwordModelFileName = "Data/Model/Weapon/Player_LongSwor1d.mv1";

	//モデルのスケール値
	constexpr float kModelScale = 1.0f;
}

PlayerWeapon::PlayerWeapon():
	m_modelH(-1),
	m_swordModelH(-1),
	m_axeModelH(-1),
	m_daggerRightModelH(-1),
	m_daggerLeftModelH(-1),
	m_magicWandModelH(-1),
	m_longSwordModelH(-1),
	m_modelRightFrame(0),
	m_modelLeftFrame(0),
	m_attachFramePos(VGet(0, 0, 0))
{

}

PlayerWeapon::~PlayerWeapon()
{

}

void PlayerWeapon::Load()
{
	//モデルのロード
	m_swordModelH = MV1LoadModel(kSwordModelFileName);
	m_axeModelH = MV1LoadModel(kAxeModelFileName);
	m_daggerRightModelH = MV1LoadModel(kDaggerModelFileName);
	m_daggerLeftModelH = MV1LoadModel(kDaggerModelFileName);
	m_magicWandModelH = MV1LoadModel(kMagicWandModelFileName);
	m_longSwordModelH = MV1LoadModel(kLongSwordModelFileName);

}

void PlayerWeapon::Initialize(int modelH, int modelRightFrame, int modelLeftFrame)
{
	//代入
	m_modelH = modelH;
	m_modelRightFrame = modelRightFrame;
	m_modelLeftFrame = modelLeftFrame;

}

void PlayerWeapon::Finalize()
{

	//モデルのデリート
	MV1DeleteModel(m_swordModelH);
	MV1DeleteModel(m_axeModelH);
	MV1DeleteModel(m_daggerRightModelH);
	MV1DeleteModel(m_daggerLeftModelH);
	MV1DeleteModel(m_magicWandModelH);
	MV1DeleteModel(m_longSwordModelH);

	m_swordModelH = -1;
	m_axeModelH = -1;
	m_daggerRightModelH = -1;
	m_daggerLeftModelH = -1;
	m_magicWandModelH = -1;
	m_longSwordModelH = -1;
}



void PlayerWeapon::SwordUpdate()
{
	OneHandUpdate(m_swordModelH);
}

void PlayerWeapon::AxeUpdate()
{
	OneHandUpdate(m_axeModelH);
}

void PlayerWeapon::DaggerUpdate()
{
	BothHandsUpdate(m_daggerRightModelH, m_daggerLeftModelH);
}

void PlayerWeapon::MagicWandUpdate()
{
	OneHandUpdate(m_magicWandModelH);
}

void PlayerWeapon::LongSwordUpdate()
{
	OneHandUpdate(m_longSwordModelH);
}


void PlayerWeapon::SwordDraw()
{
	OneHandDraw(m_swordModelH);
}

void PlayerWeapon::AxeDraw()
{
	OneHandDraw(m_axeModelH);
}

void PlayerWeapon::DaggerDraw()
{
	BothHandsDraw(m_daggerRightModelH,m_daggerLeftModelH);
}

void PlayerWeapon::MagicWandDraw()
{
	OneHandDraw(m_magicWandModelH);
}

void PlayerWeapon::LongSwordDraw()
{
	OneHandDraw(m_longSwordModelH);
}

void PlayerWeapon::OneHandUpdate(int weaponModelH)
{
	//武器のアタッチ
	MATRIX transMat;
	MATRIX attachFrameMat;
	MATRIX mixMatrix;


	//アタッチするモデルのMV1SetMatrixの設定を無効にする
	MV1SetMatrix(weaponModelH, MGetIdent());

	m_attachFramePos = MV1GetFramePosition(weaponModelH, 0);

	transMat = MGetTranslate(VScale(m_attachFramePos, -1.0f));

	attachFrameMat = MV1GetFrameLocalWorldMatrix(m_modelH, m_modelRightFrame);

	mixMatrix = MMult(transMat, attachFrameMat);

	MV1SetMatrix(weaponModelH, mixMatrix);
}

void PlayerWeapon::BothHandsUpdate(int rightWeaponModelH,int leftWeaponModelH)
{
	//武器のアタッチ
	MATRIX transMat;

	MATRIX rightAttachFrameMat;
	MATRIX leftAttachFrameMat;

	MATRIX rightMixMatrix;
	MATRIX leftMixMatrix;

	//アタッチするモデルのMV1SetMatrixの設定を無効にする
	MV1SetMatrix(rightWeaponModelH, MGetIdent());
	MV1SetMatrix(leftWeaponModelH, MGetIdent());

	m_attachFramePos = MV1GetFramePosition(rightWeaponModelH, 0);
	m_attachFramePos = MV1GetFramePosition(leftWeaponModelH, 0);

	transMat = MGetTranslate(VScale(m_attachFramePos, -1.0f));

	rightAttachFrameMat = MV1GetFrameLocalWorldMatrix(m_modelH, m_modelRightFrame);
	leftAttachFrameMat = MV1GetFrameLocalWorldMatrix(m_modelH, m_modelLeftFrame);

	rightMixMatrix = MMult(transMat, rightAttachFrameMat);
	leftMixMatrix = MMult(transMat, leftAttachFrameMat);

	MV1SetMatrix(rightWeaponModelH, rightMixMatrix);
	MV1SetMatrix(leftWeaponModelH, leftMixMatrix);
}

void PlayerWeapon::OneHandDraw(int weaponModelH)
{
	MV1DrawModel(weaponModelH);
}

void PlayerWeapon::BothHandsDraw(int rightWeaponModelH, int leftWeaponModelH)
{
	MV1DrawModel(rightWeaponModelH);
	MV1DrawModel(leftWeaponModelH);
}

#include "PlayerWeapon.h"

namespace
{
	//武器のファイル名
	const char* const kSwordModelFileName = "Data/Model/Weapon/Player_Sword.mv1";
	const char* const kLongSwordModelFileName = "Data/Model/Weapon/Player_LongSword.mv1";
	const char* const kAxeModelFileName = "Data/Model/Weapon/Player_Axe.mv1";
	const char* const kMagicWandModelFileName = "Data/Model/Weapon/Player_MagicWand.mv1";
	const char* const kDaggerModelFileName = "Data/Model/Weapon/Player_Dagger.mv1";
	const char* const kShiledModelFileName = "Data/Model/Weapon/Player_Shiled.mv1";

	//プレイヤーの手のフレーム
	constexpr int kPlayerRightModelFrame = 14;
	constexpr int kPlayerLeftModelFrame = 9;

	//モデルのスケール値
	constexpr float kModelScale = 1.0f;
}

PlayerWeapon::PlayerWeapon():
	m_attachFramePos(VGet(0, 0, 0))
{
	//モデルのロード
	m_rightModelH = MV1LoadModel(kDaggerModelFileName);
	m_leftModelH = MV1LoadModel(kDaggerModelFileName);
}

PlayerWeapon::~PlayerWeapon()
{
	Finalize();
}

void PlayerWeapon::Initialize()
{
	//武器のモデルのスケール値を設定
	MV1SetScale(m_rightModelH, VGet(kModelScale, kModelScale, kModelScale));
	MV1SetScale(m_leftModelH, VGet(kModelScale, kModelScale, kModelScale));
}

void PlayerWeapon::Finalize()
{
	//モデルのデリート
	MV1DeleteModel(m_rightModelH);
	MV1DeleteModel(m_leftModelH);

	m_rightModelH = -1;
	m_leftModelH = -1;
}

void PlayerWeapon::OneHandUpdate(int modelH)
{
	//武器のアタッチ
	MATRIX transMat;
	MATRIX attachFrameMat;
	MATRIX mixMatrix;

	//アタッチするモデルのMV1SetMatrixの設定を無効にする
	MV1SetMatrix(m_rightModelH, MGetIdent());

	m_attachFramePos = MV1GetFramePosition(m_rightModelH, 0);

	transMat = MGetTranslate(VScale(m_attachFramePos, -1.0f));

	attachFrameMat = MV1GetFrameLocalWorldMatrix(modelH, kPlayerRightModelFrame);

	mixMatrix = MMult(transMat, attachFrameMat);

	MV1SetMatrix(m_rightModelH, mixMatrix);
}

void PlayerWeapon::BothHandsUpdate(int modelH)
{
	//武器のアタッチ
	MATRIX transMat;

	MATRIX rightAttachFrameMat;
	MATRIX leftAttachFrameMat;

	MATRIX rightMixMatrix;
	MATRIX leftMixMatrix;

	//アタッチするモデルのMV1SetMatrixの設定を無効にする
	MV1SetMatrix(m_rightModelH, MGetIdent());

	m_attachFramePos = MV1GetFramePosition(m_rightModelH, 0);

	transMat = MGetTranslate(VScale(m_attachFramePos, -1.0f));

	rightAttachFrameMat = MV1GetFrameLocalWorldMatrix(modelH, kPlayerRightModelFrame);
	leftAttachFrameMat = MV1GetFrameLocalWorldMatrix(modelH, kPlayerLeftModelFrame);

	rightMixMatrix = MMult(transMat, rightAttachFrameMat);
	leftMixMatrix = MMult(transMat, leftAttachFrameMat);

	MV1SetMatrix(m_rightModelH, rightMixMatrix);
	MV1SetMatrix(m_leftModelH, leftMixMatrix);
}

void PlayerWeapon::OneHandDraw()
{
	MV1DrawModel(m_rightModelH);
}

void PlayerWeapon::BothHandsDraw()
{
	MV1DrawModel(m_rightModelH);
	MV1DrawModel(m_leftModelH);
}

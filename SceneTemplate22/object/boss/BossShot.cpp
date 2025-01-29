#include "BossShot.h"
#include "object/player/Player.h"

#include "util/AnimController.h"
#include "util/ActionTime.h"
#include "util/Pad.h"

#include <cmath>
#include <cassert>

namespace
{
	//プレイヤーのモデルファイル名
	const char* const kModelFilename = "Data/Model/Boss/BossShot.mv1";
	const char* const kWeaponModelFilename = "Data/Model/Weapon/Player_MagicWand.mv1";
	//モデルのスケール値
	constexpr float kModelScale = 10.0f;

	constexpr float kWalkSpeed = 0.4f;
	constexpr float kDashSpeed = 0.7f;


	constexpr float kAvoidSpeed = 3.0f;


	//初期位置
	constexpr VECTOR kInitPos = { 0.0f,20.0f,200.0f };

	//カプセルの上の座標
	constexpr VECTOR kUpPos = { 0.0f,18.0f,0.0f };

	/*ボスのアニメーションの種類*/
	const char* const kAnimBossInfoFilename = "Data/Master/AnimBossShotMaster.csv";

	const char* const kAnimIdle = "Idle";
	const char* const kAnimWalk = "Walk";
	const char* const kAnimDash = "Dash";

	const char* const kAnimAttack1 = "Attack1";
	const char* const kAnimAttack2 = "Attack2";
	const char* const kAnimAttack3 = "Attack3";

	const char* const kAnimAvoid = "Avoid";

	const char* const kAnimCoolTime = "CoolTime";

	const char* const kAnimHit = "Hit";

	const char* const kAnimDown = "Down";
	const char* const kAnimDead = "Dead";

	//HPの最大値
	constexpr float kMaxHp = 400.0f;

	//次の状態に遷移するまでの時間
	constexpr float kIdleToAttackTime = 60.0f;
	constexpr float kIdleToDashTime = 70.0f;
	constexpr float kCoolTimeToIdleTime = 120.0f;
	constexpr float kAvoidToIdleTime = 29.0f;

	//次の状態に遷移するまでのプレイヤーとの長さ
	constexpr float kIdleToAttackLength = 100.0f;
	constexpr float kIdleToAvoidLength = 100.0f;
	constexpr float kWalkToIdleLength = 20.0f;

	//攻撃の種類
	constexpr int kActionKind = 3;
	constexpr int kAttackKind = 3;
	constexpr int kAvoidKind = 2;

}

BossShot::BossShot() :
	BossBase(Collidable::e_Priority::kStatic, Game::e_GameObjectTag::kBoss, MyLib::ColliderData::e_Kind::kSphere, false),
	m_pos(kInitPos),
	m_posUp(kInitPos),
	m_direction(VGet(0, 0, 0)),
	m_velocity(VGet(0, 0, 0)),
	m_playerPos(VGet(0, 0, 0)),
	m_angle(0.0f),
	m_nextAngle(0.0f),
	m_length(0.0f),
	m_actionTime(0),
	m_isAttack(false),
	m_isAvoid(false),
	m_actionKind(0),
	m_dashFrontLength(0.0f),
	m_dashBackLength(0.0f),
	m_dashRightLength(0.0f),
	m_avoidLeftLength(0.0f),
	m_hp(350.0f),
	m_attachFramePos({0,0,0}),
	m_modelRightFrame(54)
{
	m_isClear = false;

	m_avoidKind = e_AvoidKind::kFront;

	//回避先の座標
	m_dashFrontPos = { 0.0f, 0.0f, 200.0f };
	m_dashBackPos = { 0.0f, 0.0f, -200.0f };
	m_dashRightPos = { 200.0f, 0.0f, 0.0f };
	m_dashLeftPos = { -200.0f, 0.0f, 0.0f };

	m_modelH = MV1LoadModel(kModelFilename);
	m_weaponModelH = MV1LoadModel(kWeaponModelFilename);

	m_pAnim = std::make_shared<AnimController>();

	m_pPlayer = std::make_shared<Player>();

	m_pColliderData = std::make_shared<MyLib::ColliderDataSphere>(false);

	//auto circleColliderData = dynamic_cast<MyLib::ColliderDataSphere*>();
	auto circleColliderData = std::dynamic_pointer_cast<MyLib::ColliderDataSphere>(m_pColliderData);
	circleColliderData->m_radius = 6.5f;

	//m_pColliderData = std::make_shared<MyLib::ColliderDataCapsule>(false);

	//auto circleColliderData = std::dynamic_pointer_cast<MyLib::ColliderDataCapsule>(m_pColliderData);
	//circleColliderData->m_radius = 5.0f;
	//circleColliderData->m_posDown = VGet(0.0f, 0.0f, 0.0f);
	//circleColliderData->m_posUp = VGet(0.0f, 0.0f, 0.0f);


}

BossShot::~BossShot()
{

}

void BossShot::Initialize(std::shared_ptr<MyLib::Physics> physics)
{
	//
	Collidable::Initialize(physics);

	// 物理挙動の初期化
	m_rigidbody.Initialize(true);
	m_rigidbody.SetPos(kInitPos);
	//m_speed = 0.1f;

	//初期位置を代入
	m_pos = VGet(0, 15, 100);

	//モデルのスケールを決める
	MV1SetScale(m_modelH, VGet(kModelScale, kModelScale, kModelScale));

	//アニメーションの初期化
	m_pAnim->Initialize(kAnimBossInfoFilename, m_modelH, kAnimIdle);

	// メンバ関数ポインタの初期化
	m_updateFunc = &BossShot::IdleUpdate;

}

void BossShot::Finalize(std::shared_ptr<MyLib::Physics> physics)
{
	MV1DeleteModel(m_modelH);
	MV1DeleteModel(m_weaponModelH);
	m_modelH = -1;
	m_weaponModelH = -1;

	Collidable::Finalize(physics);
}

void BossShot::Update(std::shared_ptr<MyLib::Physics> physics, Player& player)
{
	//アニメーションの更新処理
	m_pAnim->UpdateAnim();

	if (Pad::IsPress PAD_INPUT_1 && Pad::IsPress PAD_INPUT_2)
	{
		m_hp -= 40;
	}

	//アップデート
	(this->*m_updateFunc)();

	/*武器のアタッチ*/
	MATRIX transMat;
	MATRIX attachFrameMat;
	MATRIX mixMatrix;

	//アタッチするモデルのMV1SetMatrixの設定を無効にする
	MV1SetMatrix(m_weaponModelH, MGetIdent());

	m_attachFramePos = MV1GetFramePosition(m_weaponModelH, 0);

	transMat = MGetTranslate(VScale(m_attachFramePos, -1.0f));

	attachFrameMat = MV1GetFrameLocalWorldMatrix(m_modelH, m_modelRightFrame);

	mixMatrix = MMult(transMat, attachFrameMat);

	MV1SetMatrix(m_weaponModelH, mixMatrix);


	/*座標の取得*/
	m_playerPos = player.GetPos();
	m_pos = m_rigidbody.GetPos();

	m_dashFrontPos.y = m_pos.y;
	m_dashBackPos.y = m_pos.y;
	m_dashRightPos.y = m_pos.y;
	m_dashLeftPos.y = m_pos.y;

	//プレイヤーとボスの距離を求める
	VECTOR toPlayer = VSub(m_playerPos, m_pos);
	m_length = VSize(toPlayer);

	//ボスとボスが移動する場所の距離を求める
	VECTOR toAvoidFront = VSub(m_dashFrontPos, m_pos);
	m_dashFrontLength = VSize(toAvoidFront);

	VECTOR toAvoidBack = VSub(m_dashBackPos, m_pos);
	m_dashBackLength = VSize(toAvoidBack);

	VECTOR toAvoidRight = VSub(m_dashRightPos, m_pos);
	m_dashRightLength = VSize(toAvoidRight);

	VECTOR toAvoidLeft = VSub(m_dashLeftPos, m_pos);
	m_avoidLeftLength = VSize(toAvoidLeft);

	if (m_length > 60)
	{
		m_isAvoid = true;
	}
	else
	{
		m_isAvoid = false;
	}

	//HPがゼロより下にいった場合
	if (m_hp <= 0)
	{
		m_hp = 0;

		//死亡状態へ遷移
		OnDead();
	}

	//auto pos = m_rigidbody.GetPos();

	//モデルのポジションを合わせるよう
	//VECTOR modelPos = VGet(pos.x, pos.y, pos.z);

	m_posUp = VGet(m_pos.x, m_pos.y + kUpPos.y, m_pos.z);

	//DrawSphere3D(m_pos, 32, 16, 0xffffff, 0xffffff, false);

	//モデルに座標をセットする
	MV1SetPosition(m_modelH, m_pos);
	MV1SetRotationXYZ(m_modelH, VGet(0.0f, m_angle + DX_PI_F, 0.0f));

}

void BossShot::Draw()
{
	MV1DrawModel(m_modelH);
	MV1DrawModel(m_weaponModelH);

	DrawFormatString(0, 280, 0xff0fff, "ShotBossPos:%f,%f,%f", m_pos.x, m_pos.y, m_pos.z);
	DrawFormatString(0, 388, 0xff0fff, "ShotBossToPlayer:%f", m_length);

	DrawFormatString(0, 408, 0xff0fff, "F:%f", m_dashFrontLength);
	DrawFormatString(0, 428, 0xff0fff, "B:%f", m_dashBackLength);
	DrawFormatString(0, 448, 0xff0fff, "R:%f", m_dashRightLength);
	DrawFormatString(0, 468, 0xff0fff, "L:%f", m_avoidLeftLength);


	//DrawCapsule3D(m_posDown, m_posUp, m_radius, 32, 0xffffff, 0xffffff, false);
}

const VECTOR& BossShot::GetPosUp() const
{
	auto pos = VAdd(m_rigidbody.GetPos(), VGet(0.0f, 10.0f, 0.0f));
	return pos;
}

const VECTOR& BossShot::GetPosDown() const
{
	return m_rigidbody.GetPos();
}

void BossShot::SetPosDown(const VECTOR pos)
{
	m_rigidbody.SetPos(pos);
}

void BossShot::IdleUpdate()
{
	m_actionTime++;

	//auto pos = m_rigidbody.GetPos();

	//プレイヤーへの向きを取得
	m_direction = VSub(m_playerPos, m_pos);
	m_direction = VNorm(m_direction);

	//m_angle = atan2f(m_direction.x, m_direction.z);

	//プレイヤーと離れていた場合歩き状態に移動 && タイマー
	if (m_actionTime > kIdleToAttackTime && m_length > kIdleToAttackLength)
	{
		m_actionKind = GetRand(kAttackKind);

		//ランダムで攻撃を行う
		switch (m_actionKind)
		{
		case 0:
			OnAttack1();
			break;
		case 1:
			OnAttack2();
			break;
		case 2:
			OnAttack3();
			break;
		case 3:
			OnAttack1();
			break;
		default:
			break;
		}
	}
	//プレイヤーと十分な距離の場合 && タイマー
	else if (m_actionTime > kIdleToDashTime && m_length < kIdleToAttackLength )
	{
		m_actionKind = GetRand(kAvoidKind);



		if (m_avoidKind == e_AvoidKind::kFront)
		{

			//ランダムで回避を行う
			switch (m_actionKind)
			{
			case 0:
				OnDashBack();
				break;
			case 1:
				OnDashRight();
				break;
			case 2:
				OnDashLeft();
				break;
			default:
				break;
			}
		}
		else if (m_avoidKind == e_AvoidKind::kBack)
		{

			//ランダムで回避を行う
			switch (m_actionKind)
			{
			case 0:
				OnDashFront();
				break;
			case 1:
				OnDashRight();
				break;
			case 2:
				OnDashLeft();
				break;
			default:
				break;
			}
		}
		else if (m_avoidKind == e_AvoidKind::kRight)
		{

			//ランダムで回避を行う
			switch (m_actionKind)
			{
			case 0:
				OnDashFront();
				break;
			case 1:
				OnDashBack();
				break;
			case 2:
				OnDashLeft();
				break;
			default:
				break;
			}
		}
		else if (m_avoidKind == e_AvoidKind::kLeft)
		{

			//ランダムで回避を行う
			switch (m_actionKind)
			{
			case 0:
				OnDashFront();
				break;
			case 1:
				OnDashBack();
				break;
			case 2:
				OnDashRight();
				break;
			default:
				break;
			}
		}

	}




	m_nextAngle = atan2(m_direction.x, m_direction.z);

	SmoothAngle(m_angle, m_nextAngle);

	VECTOR move;
	move.y = m_rigidbody.GetVelocity().y;
	m_rigidbody.SetVelocity(VGet(0, move.y, 0));

}

void BossShot::Attack1Update()
{
	//アニメーションが終わったらアイドル状態に戻る
	if (m_pAnim->IsLoop())
	{
		OnAttackCoolTime();
	}

	m_rigidbody.SetVelocity(VGet(0.0f, 0.0f, 0.0f));
}

void BossShot::Attack2Update()
{
	//アニメーションが終わったらアイドル状態に戻る
	if (m_pAnim->IsLoop())
	{
		OnAttackCoolTime();
	}
	m_rigidbody.SetVelocity(VGet(0.0f, 0.0f, 0.0f));
}

void BossShot::Attack3Update()
{
	//アニメーションが終わったらクールタイム状態に入る
	if (m_pAnim->IsLoop())
	{
		OnAttackCoolTime();
	}
	m_rigidbody.SetVelocity(VGet(0.0f, 0.0f, 0.0f));
}

void BossShot::DashFrontUpdate()
{
	m_actionTime++;

	//プレイヤーへの向きを取得
	m_direction = VSub(m_dashFrontPos, m_pos);
	//正規化
	m_direction = VNorm(m_direction);
	//モデルの角度
	m_angle = atan2f(m_direction.x, m_direction.z);

	//移動処理
	VECTOR length = VSub(m_dashFrontPos, m_pos);
	length = VNorm(length);
	m_velocity = VScale(length, kAvoidSpeed);

	//敵の移動
	m_rigidbody.SetVelocity(m_velocity);

	//ちゃんと座標までいけたら
	if (m_dashFrontLength <= 40 && m_actionTime > kAvoidToIdleTime)
	{
		OnIdle();
	}
	//else if (m_dashFrontLength > 40 && m_actionTime > kAvoidToIdleTime)
	//{
	//	//OnAvoidCoolTime();
	//	OnDashFront();
	//}
}

void BossShot::DashBackUpdate()
{
	m_actionTime++;

	//プレイヤーへの向きを取得
	m_direction = VSub(m_dashBackPos, m_pos);
	//正規化
	m_direction = VNorm(m_direction);
	//モデルの角度
	m_angle = atan2f(m_direction.x, m_direction.z);
	
	//移動処理
	VECTOR length = VSub(m_dashBackPos, m_pos);
	length = VNorm(length);
	m_velocity = VScale(length, kAvoidSpeed);

	//敵の移動
	m_rigidbody.SetVelocity(m_velocity);

	//ちゃんと座標までいけたら
	if (m_dashBackLength <= 40 && m_actionTime > kAvoidToIdleTime)
	{
		OnIdle();
	}
	//else if (m_dashBackLength > 40 && m_actionTime > kAvoidToIdleTime)
	//{
	//	//OnAvoidCoolTime();
	//	OnDashBack();
	//}
}

void BossShot::DashRightUpdate()
{
	m_actionTime++;

	//移動先の向きを取得
	m_direction = VSub(m_dashRightPos, m_pos);
	//正規化
	m_direction = VNorm(m_direction);
	//モデルの角度
	m_angle = atan2f(m_direction.x, m_direction.z);

	//移動処理
	VECTOR length = VSub(m_dashRightPos, m_pos);
	length = VNorm(length);
	m_velocity = VScale(length, kAvoidSpeed);

	//敵の移動
	m_rigidbody.SetVelocity(m_velocity);

	//ちゃんと座標までいけたら
	if (m_dashRightLength <= 40 && m_actionTime > kAvoidToIdleTime)
	{
		OnIdle();
	}
	//else if (m_dashRightLength > 40 && m_actionTime > kAvoidToIdleTime)
	//{
	//	//OnAvoidCoolTime();
	//	OnDashRight();
	//}

}

void BossShot::DashLeftUpdate()
{
	m_actionTime++;

	//プレイヤーへの向きを取得
	m_direction = VSub(m_dashLeftPos, m_pos);
	//正規化
	m_direction = VNorm(m_direction);
	//モデルの角度
	m_angle = atan2f(m_direction.x, m_direction.z);

	//移動処理
	VECTOR length = VSub(m_dashLeftPos, m_pos);
	length = VNorm(length);
	m_velocity = VScale(length, kAvoidSpeed);

	//敵の移動
	m_rigidbody.SetVelocity(m_velocity);

	//ちゃんと座標までいけたら
	if (m_avoidLeftLength <= 40 && m_actionTime > kAvoidToIdleTime)
	{
		OnIdle();
	}
	//else if(m_avoidLeftLength > 40 && m_actionTime > kAvoidToIdleTime)
	//{
	//	//OnAvoidCoolTime();
	//	OnDashLeft();
	//}
}

void BossShot::AttackCoolTimeUpdate()
{
	m_actionTime++;

	m_rigidbody.SetVelocity(VGet(0.0f, 0.0f, 0.0f));

	if (m_actionTime > kCoolTimeToIdleTime)
	{
		OnIdle();
	}
}

void BossShot::DownUpdate()
{
	//アニメーションが終わったらアイドル状態に戻る
	if (m_pAnim->IsLoop())
	{
		OnIdle();
	}
}

void BossShot::DeadUpdate()
{
	//ワープアイテムが出現するフラグをおいておく
	m_isClear = true;

}


void BossShot::OnIdle()
{
	m_rigidbody.SetVelocity(VGet(0.0f, 0.0f, 0.0f));
	m_actionKind = 0;
	m_actionTime = 0;
	m_pAnim->ChangeAnim(kAnimIdle);
	m_updateFunc = &BossShot::IdleUpdate;
}

void BossShot::OnAttack1()
{
	m_rigidbody.SetVelocity(VGet(0.0f, 0.0f, 0.0f));
	m_actionKind = 0;
	m_actionTime = 0;
	m_pAnim->ChangeAnim(kAnimAttack1, true, true, false);
	m_updateFunc = &BossShot::Attack1Update;
}

void BossShot::OnAttack2()
{
	m_rigidbody.SetVelocity(VGet(0.0f, 0.0f, 0.0f));
	m_actionKind = 0;
	m_actionTime = 0;
	m_pAnim->ChangeAnim(kAnimAttack2, true, true, false);
	m_updateFunc = &BossShot::Attack2Update;
}

void BossShot::OnAttack3()
{
	m_rigidbody.SetVelocity(VGet(0.0f, 0.0f, 0.0f));
	m_actionKind = 0;
	m_actionTime = 0;
	m_pAnim->ChangeAnim(kAnimAttack3, true, true, false);
	m_updateFunc = &BossShot::Attack3Update;
}

void BossShot::OnDashFront()
{
	m_avoidKind = e_AvoidKind::kFront;
	m_actionTime = 0;
	m_pAnim->ChangeAnim(kAnimDash);
	m_updateFunc = &BossShot::DashFrontUpdate;
}

void BossShot::OnDashBack()
{
	m_avoidKind = e_AvoidKind::kBack;
	m_actionTime = 0;
	m_pAnim->ChangeAnim(kAnimDash);
	m_updateFunc = &BossShot::DashBackUpdate;
}

void BossShot::OnDashRight()
{
	m_avoidKind = e_AvoidKind::kRight;
	m_actionTime = 0;
	m_pAnim->ChangeAnim(kAnimDash);
	m_updateFunc = &BossShot::DashRightUpdate;
}

void BossShot::OnDashLeft()
{
	m_avoidKind = e_AvoidKind::kLeft;
	m_actionTime = 0;
	m_pAnim->ChangeAnim(kAnimDash);
	m_updateFunc = &BossShot::DashLeftUpdate;
}

void BossShot::OnAttackCoolTime()
{
	m_rigidbody.SetVelocity(VGet(0.0f, 0.0f, 0.0f));
	m_actionKind = 0;
	m_actionTime = 0;
	m_pAnim->ChangeAnim(kAnimCoolTime);
	m_updateFunc = &BossShot::AttackCoolTimeUpdate;
}

void BossShot::OnDown()
{
	m_rigidbody.SetVelocity(VGet(0, 0, 0));

	m_actionKind = 0;
	m_actionTime = 0;
	m_pAnim->ChangeAnim(kAnimDown);
	m_updateFunc = &BossShot::DownUpdate;
}

void BossShot::OnDead()
{
	m_rigidbody.SetVelocity(VGet(0, 0, 0));

	m_actionKind = 0;
	m_actionTime = 0;
	m_pAnim->ChangeAnim(kAnimDead, false, true, true);
	m_updateFunc = &BossShot::DeadUpdate;
}

#include "BossPower.h"
#include "object/player/Player.h"

#include "util/AnimController.h"
#include "util/ActionTime.h"

#include <cmath>
#include <cassert>

namespace
{
	//プレイヤーのモデルファイル名
	const char* const kModelFilename = "Data/Model/Boss/BossPower.mv1";
	//モデルのスケール値
	constexpr float kModelScale = 10.0f;

	constexpr float kWalkSpeed = 0.4f;
	constexpr float kDashSpeed = 0.7f;


	//初期位置
	constexpr VECTOR kInitPos = { 0.0f,0.0f,200.0f };

	//カプセルの上の座標
	constexpr VECTOR kUpPos = { 0.0f,18.0f,0.0f };

	/*ボスのアニメーションの種類*/
	const char* const kAnimInfoFilename = "Data/Master/AnimBossPowerMaster.csv";

	const char* const kAnimIdle = "Idle";
	const char* const kAnimWalk = "Walk";
	const char* const kAnimDash = "Dash";

	const char* const kAnimAttack1 = "Attack1";
	const char* const kAnimAttack2 = "Attack2";
	const char* const kAnimAttack3 = "Attack3";

	const char* const kAnimCoolTime = "CoolTime";

	const char* const kAnimHit = "Hit";

	const char* const kAnimDown = "Down";
	const char* const kAnimDead = "Dead";

	//HPの最大値
	constexpr float kMaxHp = 400.0f;

	//次の状態に遷移するまでの時間
	constexpr float kIdleToWalkTime = 40.0f;
	constexpr float kIdleToAttackTime = 90.0f;
	constexpr float kCoolTimeToIdleTime = 120.0f;

	//次の状態に遷移するまでのプレイヤーとの長さ
	constexpr float kIdleToWalkLength = 20.0f;
	constexpr float kIdleToAttackLength = 20.0f;
	constexpr float kWalkToIdleLength = 20.0f;

	constexpr float kWalkToDashLength = 100.0f;
	constexpr float kDashToWalkLength = 80.0f;

	//攻撃の種類
	constexpr int kAttackKind = 2;

}

BossPower::BossPower():
	BossBase(Collidable::e_Priority::kStatic, Game::e_GameObjectTag::kBoss, MyLib::ColliderData::e_Kind::kSphere, false),
	m_pos(kInitPos),
	m_posUp(kInitPos),
	m_direction(VGet(0,0,0)),
	m_velocity(VGet(0,0,0)),
	m_playerPos(VGet(0, 0, 0)),
	m_angle(0.0f),
	m_nextAngle(0.0f),
	m_length(0.0f),
	m_attackCoolTime(0),
	m_actionTime(0),
	m_isAttack(false),
	m_attackKind(0),
	m_hp(350.0f)
{

	m_modelH = MV1LoadModel(kModelFilename);

	m_pAnim = std::make_shared<AnimController>();

	//時間のタイマーセット
	//m_pOnIdleTime = std::make_shared<ActionTime>(120);
	//m_pOnWlakTime = std::make_shared<ActionTime>(300);
	//m_pOnDashTime = std::make_shared<ActionTime>(120);
	//m_pOnDownTime = std::make_shared<ActionTime>(240);
	//m_pOnAttackTime = std::make_shared<ActionTime>(120);

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

BossPower::~BossPower()
{

}

void BossPower::Initialize(std::shared_ptr<MyLib::Physics> physics)
{
	//
	Collidable::Initialize(physics);

	// 物理挙動の初期化
	m_rigidbody.Initialize(true);
	m_rigidbody.SetPos(kInitPos);
	//m_speed = 0.1f;

	//初期位置を代入
	m_pos = VGet(0, 0, 100);

	//モデルのスケールを決める
	MV1SetScale(m_modelH, VGet(kModelScale, kModelScale, kModelScale));

	//アニメーションの初期化
	m_pAnim->Initialize(kAnimInfoFilename, m_modelH, kAnimIdle);

	// メンバ関数ポインタの初期化
	m_updateFunc = &BossPower::IdleUpdate;

}

void BossPower::Finalize(std::shared_ptr<MyLib::Physics> physics)
{
	MV1DeleteModel(m_modelH);
	m_modelH = -1;

	Collidable::Finalize(physics);
}

void BossPower::Update(std::shared_ptr<MyLib::Physics> physics, Player& player)
{
	//アップデート
	(this->*m_updateFunc)();

	//プレイヤーとボスの距離を距離を求める
	VECTOR toPlayer = VSub(m_playerPos, m_pos);
	m_length = VSize(toPlayer);

	//m_pos = m_pPlayer->GetPosDown();

	m_playerPos = player.GetPos();
	m_pos = m_rigidbody.GetPos();

	//アニメーションの更新処理
	m_pAnim->UpdateAnim();
	
	//auto pos = m_rigidbody.GetPos();

	//モデルのポジションを合わせるよう
	//VECTOR modelPos = VGet(pos.x, pos.y, pos.z);

	m_posUp = VGet(m_pos.x, m_pos.y + kUpPos.y, m_pos.z);

	//DrawSphere3D(m_pos, 32, 16, 0xffffff, 0xffffff, false);

	//モデルに座標をセットする
	MV1SetPosition(m_modelH, m_pos);
	MV1SetRotationXYZ(m_modelH, VGet(0.0f, m_angle + DX_PI_F, 0.0f));

}

void BossPower::Draw()
{
	MV1DrawModel(m_modelH);

	DrawFormatString(0, 248, 0xff0fff, "BossPos:%f,%f,%f", m_pos.x, m_pos.y, m_pos.z);
	DrawFormatString(0, 348, 0xff0fff, "BossToPlayer:%f", m_length);
	
	//DrawCapsule3D(m_posDown, m_posUp, m_radius, 32, 0xffffff, 0xffffff, false);
}

const VECTOR& BossPower::GetPosDown() const
{
	return m_rigidbody.GetPos();
}

void BossPower::SetPosDown(const VECTOR pos)
{
	m_rigidbody.SetPos(pos);
}

void BossPower::IdleUpdate()
{
	m_actionTime++;

	//auto pos = m_rigidbody.GetPos();

	//プレイヤーへの向きを取得
	m_direction = VSub(m_playerPos, m_pos);
	m_direction = VNorm(m_direction);

	//m_angle = atan2f(m_direction.x, m_direction.z);

	//プレイヤーと離れていた場合歩き状態に移動 && タイマー
	if (m_actionTime > kIdleToWalkTime && m_length > kIdleToWalkLength)
	{
		OnWalk();
	}
	//プレイヤーと十分な距離の場合 && タイマー
	else if (m_actionTime > kIdleToAttackTime && m_length < kIdleToAttackLength)
	{
		//ランダム関数かなんか使ってやる

		m_attackKind = GetRand(kAttackKind);
		
		//ランダムで攻撃を行う
		switch (m_attackKind)
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
			OnAvoid();
			break;
		default:
			break;
		}

	}
	m_nextAngle = atan2(m_direction.x, m_direction.z);

	SmoothAngle(m_angle, m_nextAngle);

	VECTOR move;
	move.y = m_rigidbody.GetVelocity().y;
	m_rigidbody.SetVelocity(VGet(0, move.y, 0));

}

void BossPower::WalkUpdate()
{
	//VECTOR pos = m_rigidbody.GetPos();

	//プレイヤーへの向きを取得
	m_direction = VSub(m_playerPos, m_pos);

	VECTOR length = VSub(m_pos, m_playerPos);
	float size = VSize(length);

	m_direction = VNorm(m_direction);

	m_angle = atan2f(m_direction.x, m_direction.z);

	//ベクトルを、正規化し、向きだけを保存させる
	m_velocity = VScale(m_direction, kWalkSpeed);

	//敵の移動
	m_rigidbody.SetVelocity(m_velocity);

	//プレイヤーとの距離が指定されている距離未満なら
	if (size < kWalkToIdleLength)
	{
		OnIdle();
	}

	//距離が遠かった場合ダッシュ状態に遷移
	if (m_length > kWalkToDashLength)
	{
		OnDash();
	}

}

void BossPower::DashUpdate()
{
	//プレイヤーへの向きを取得
	m_direction = VSub(m_playerPos, m_pos);

	VECTOR length = VSub(m_pos, m_playerPos);
	float size = VSize(length);

	m_direction = VNorm(m_direction);

	m_angle = atan2f(m_direction.x, m_direction.z);

	//ベクトルを、正規化し、向きだけを保存させる
	m_velocity = VScale(m_direction, kDashSpeed);

	//敵の移動
	m_rigidbody.SetVelocity(m_velocity);

	//距離が近くなっていったら歩きに状態に遷移
	if(m_length < kDashToWalkLength)
	{
		OnWalk();
	}
}

void BossPower::Attack1Update()
{
	//アニメーションが終わったらアイドル状態に戻る
	if (m_pAnim->IsLoop())
	{
		OnIdle();
	}
}

void BossPower::Attack2Update()
{
	//アニメーションが終わったらアイドル状態に戻る
	if (m_pAnim->IsLoop())
	{
		OnIdle();
	}
}

void BossPower::Attack3Update()
{
	//アニメーションが終わったらクールタイム状態に入る
	if (m_pAnim->IsLoop())
	{
		OnAttackCoolTime();
	}
}

void BossPower::AvoidUpdate()
{
	//アニメーションが終わったらクールタイム状態に入る
	if (m_pAnim->IsLoop())
	{
		OnIdle();
	}
}

void BossPower::AttackCoolTimeUpdate()
{
	m_attackCoolTime++;

	if (m_attackCoolTime > kCoolTimeToIdleTime)
	{
		OnIdle();
	}
}

void BossPower::DownUpdate()
{
	//アニメーションが終わったらアイドル状態に戻る
	if (m_pAnim->IsLoop())
	{
		OnIdle();
	}
}

void BossPower::DeadUpdate()
{
	//ワープアイテムが出現するフラグをおいておく

}


void BossPower::OnIdle()
{
	m_attackCoolTime = 0;
	m_pAnim->ChangeAnim(kAnimIdle);
	m_updateFunc = &BossPower::IdleUpdate;
}

void BossPower::OnWalk()
{
	m_actionTime = 0;
	m_pAnim->ChangeAnim(kAnimWalk);
	m_updateFunc = &BossPower::WalkUpdate;
}

void BossPower::OnDash()
{
	m_actionTime = 0;
	m_pAnim->ChangeAnim(kAnimDash);
	m_updateFunc = &BossPower::DashUpdate;
}

void BossPower::OnAttack1()
{
	m_attackKind = 0;
	m_actionTime = 0;
	m_pAnim->ChangeAnim(kAnimAttack1, true, true, false);
	m_updateFunc = &BossPower::Attack1Update;
}

void BossPower::OnAttack2()
{
	m_attackKind = 0;
	m_actionTime = 0;
	m_pAnim->ChangeAnim(kAnimAttack2, true, true, false);
	m_updateFunc = &BossPower::Attack2Update;
}

void BossPower::OnAttack3()
{
	m_attackKind = 0;
	m_actionTime = 0;
	m_pAnim->ChangeAnim(kAnimAttack3, true, true, false);
	m_updateFunc = &BossPower::Attack3Update;
}

void BossPower::OnAvoid()
{
	m_attackKind = 0;
	m_actionTime = 0;
	m_pAnim->ChangeAnim(kAnimAttack3, true, true, false);
	m_updateFunc = &BossPower::AvoidUpdate;
}

void BossPower::OnAttackCoolTime()
{
	m_pAnim->ChangeAnim(kAnimCoolTime);
	m_updateFunc = &BossPower::AttackCoolTimeUpdate;
}

void BossPower::OnDown()
{
	m_pAnim->ChangeAnim(kAnimDown);
	m_updateFunc = &BossPower::DownUpdate;
}

void BossPower::OnDead()
{
	m_pAnim->ChangeAnim(kAnimDead, false, true, true);
	m_updateFunc = &BossPower::DeadUpdate;
}

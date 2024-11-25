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
	constexpr float kModelScale = 8.0f;

	constexpr float kSpeed = 1.0f;


	//初期位置
	constexpr VECTOR kInitPos = { 0.0f,0.0f,200.0f };

	//カプセルの上の座標
	constexpr VECTOR kUpPos = { 0.0f,18.0f,0.0f };

	/*プレイヤーのアニメーションの種類*/
	const char* const kAnimInfoFilename = "Data/Master/AnimBossPowerMaster.csv";

	const char* const kAnimNormalIdle = "Idle";
	const char* const kAnimNormalWalk = "Walk";
	const char* const kAnimNormalDash = "Dash";

	const char* const kAnimAttack1 = "Attack1";
	const char* const kAnimAttack2 = "Attack2";
	const char* const kAnimAttack3 = "Attack3";

	const char* const kAnimDown = "Down";
	const char* const kAnimDead = "Dead";

}

BossPower::BossPower():
	BossBase(Collidable::e_Priority::kStatic, Game::e_GameObjectTag::kBoss, MyLib::ColliderData::e_Kind::kSphere, false),
	m_posDown(kInitPos),
	m_posUp(kInitPos),
	m_direction(VGet(0,0,0)),
	m_velocity(VGet(0,0,0)),
	m_angle(0.0f)
{

	m_modelH = MV1LoadModel(kModelFilename);

	m_pAnim = std::make_shared<AnimController>();

	//時間のタイマーセット
	m_pOnIdleTime = std::make_shared<ActionTime>(120);
	m_pOnWlakTime = std::make_shared<ActionTime>(300);
	m_pOnDashTime = std::make_shared<ActionTime>(120);
	m_pOnDownTime = std::make_shared<ActionTime>(240);
	m_pOnAttackTime = std::make_shared<ActionTime>(120);

	m_pPlayer = std::make_shared<Player>();

	m_pColliderData = std::make_shared<MyLib::ColliderDataSphere>(false);

	//auto circleColliderData = dynamic_cast<MyLib::ColliderDataSphere*>();
	auto circleColliderData = std::dynamic_pointer_cast<MyLib::ColliderDataSphere>(m_pColliderData);
	circleColliderData->m_radius = 5.0f;

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
	m_pAnim->Initialize(kAnimInfoFilename, m_modelH, kAnimNormalIdle);

	// メンバ関数ポインタの初期化
	m_updaFunc = &BossPower::IdleUpdate;

}

void BossPower::Finalize(std::shared_ptr<MyLib::Physics> physics)
{
	MV1DeleteModel(m_modelH);
	m_modelH = -1;

	Collidable::Finalize(physics);
}

void BossPower::Update(std::shared_ptr<MyLib::Physics> physics)
{
	//アップデート
	(this->*m_updaFunc)();

	//m_pos = m_pPlayer->GetPosDown();

	//アニメーションの更新処理
	m_pAnim->UpdateAnim();
	
	auto pos = m_rigidbody.GetPos();

	//モデルのポジションを合わせるよう
	VECTOR modelPos = VGet(pos.x, pos.y, pos.z);

	m_posUp = VGet(pos.x, pos.y + kUpPos.y, pos.z);

	DrawSphere3D(pos, 32, 16, 0xffffff, 0xffffff, false);

	//モデルに座標をセットする
	MV1SetPosition(m_modelH, modelPos);
	MV1SetRotationXYZ(m_modelH, VGet(0.0f, m_angle + DX_PI_F, 0.0f));

}

void BossPower::Draw()
{
	MV1DrawModel(m_modelH);

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

	//プレイヤーと離れていた場合歩き状態に移動

	if (m_pOnWlakTime->IsUpdate())
	{
		m_pOnWlakTime->Reset();
		OnWalk();
	}

	//プレイヤーと距離が離れていない場合攻撃状態に移動
	//ランダム関数などを使用して三つの攻撃を使う

	
	//プレイヤーへの向きを取得
	//m_direction = VSub(m_pos, m_posDown);
	//m_direction = VNorm(m_direction);

	//m_angle = atan2f(m_direction.x, m_direction.z);

	//VECTOR move;
	//move.y = m_rigidbody.GetVelocity().y;
	//m_rigidbody.SetVelocity(VGet(0, move.y, 0));
}

void BossPower::WalkUpdate()
{


	//プレイヤーと距離が離れていない場合攻撃状態に移動
	//ランダム関数などを使用して三つの攻撃を使う

	if (m_pOnAttackTime->IsUpdate())
	{
		m_pOnWlakTime->Reset();
		OnAttack3();
	}

	//プレイヤーへの向きを取得
	m_direction = VSub(m_pos, m_posDown);
	m_direction = VNorm(m_direction);

	m_angle = atan2f(m_direction.x, m_direction.z);

	//ベクトルを、正規化し、向きだけを保存させる
	m_velocity = VScale(m_direction, kSpeed);

	auto pos = m_rigidbody.GetPos();

	//敵の移動
	pos = VAdd(pos, m_velocity);
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

	//アニメーションが終わったらアイドル状態に戻る
	if (m_pAnim->IsLoop())
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
	m_pAnim->ChangeAnim(kAnimNormalIdle);
	m_updaFunc = &BossPower::IdleUpdate;
}

void BossPower::OnWalk()
{
	m_pAnim->ChangeAnim(kAnimNormalWalk);
	m_updaFunc = &BossPower::WalkUpdate;
}

void BossPower::OnAttack1()
{
	m_pAnim->ChangeAnim(kAnimAttack1, true, true, false);
	m_updaFunc = &BossPower::Attack1Update;
}

void BossPower::OnAttack2()
{
	m_pAnim->ChangeAnim(kAnimAttack2, true, true, false);
	m_updaFunc = &BossPower::Attack2Update;
}

void BossPower::OnAttack3()
{
	m_pAnim->ChangeAnim(kAnimAttack3, true, true, false);
	m_updaFunc = &BossPower::Attack3Update;
}

void BossPower::OnDown()
{
	m_pAnim->ChangeAnim(kAnimDown);
	m_updaFunc = &BossPower::DownUpdate;
}

void BossPower::OnDead()
{
	m_pAnim->ChangeAnim(kAnimDead, false, true, true);
	m_updaFunc = &BossPower::DeadUpdate;
}

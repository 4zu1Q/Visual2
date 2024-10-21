#include "PlayerBase.h"
#include "PlayerWeapon.h"

#include "util/Pad.h"

#include <cmath>
#include <cassert>

namespace
{
	//プレイヤーのモデルファイル名
	const char* const kModelFilename = "Data/Model/Player/Player.mv1";

	//モデルのスケール値
	constexpr float kModelScale = 5.0f;

	//プレイヤーの手のフレーム番号
	constexpr int kRightModelFrameNo = 14;
	constexpr int kLeftModelFrameNo = 9;

	//アニメーションの切り替えにかかるフレーム数
	constexpr float kAnimChangeFrame = 10.0f;
	constexpr float kAnimChangeRateSpeed = 1.0f / kAnimChangeFrame;

	//アニメーションブレンド率の最大
	constexpr float kAnimBlendRateMax = 1.0f;

	constexpr float kAimAnimStartFrame = 9.0f;

	//アナログスティック関連
	constexpr float kAnalogRangeMin = 0.1f;		//アナログスティックの入力判定範囲
	constexpr float kAnalogRangeMax = 0.8f;
	constexpr float kAnalogInputMax = 1000.0f;	//アナログスティックから入力されるベクトルの最大

	//移動速度：ノーマル & ストロンゲスト & マジック
	constexpr float kMaxSpeedN = 2.0f;
	constexpr float kMinSpeedN = 1.0f;

	//移動速度：パワー
	constexpr float kMaxSpeedP = 2.0f;
	constexpr float kMinSpeedP = 1.0f;

	//移動速度：スピード
	constexpr float kMaxSpeedS = 3.0f;
	constexpr float kMinSpeedS = 1.5f;

	//ジャンプ力
	constexpr float kJumpPower = 10.0f;

}

PlayerBase::PlayerBase() :
	m_modelH(-1),
	m_radius(5),
	m_pos(VGet(0, 0, 0)),
	m_move(VGet(0, 0, 0)),
	m_attackPos(VGet(0, 0, 0)),
	m_attackDir(VGet(0, 0, 0)),
	m_avoid(VGet(0, 0, 0)),
	m_analogX(0),
	m_analogZ(0),
	m_hp(10),
	m_isDead(false)
{

	/*マスク関連の初期化*/
	m_isFaceUse = false;

#ifdef _DEBUG
	m_isPowerFace = true;
	m_isSpeedFace = true;
	m_isShotFace = false;
	m_isStrongestFace = false;
#else

	m_isPowerFace = false;
	m_isSpeedFace = false;
	m_isShotFace = false;
	m_isStrongestFace = false;

#endif

	/*アニメーション情報初期化*/
	//m_equipAnimNo = -1;
	m_currentAnimNo = -1;
	m_prevAnimNo = -1;

	m_animSpeed = 0.5f;
	m_animBlendRate = 1.0f;
	m_isAnimationFinish = false;


	m_modelH = MV1LoadModel(kModelFilename);
	assert(m_modelH > -1);

	m_playerKind = e_PlayerKind::kPowerPlayer;
	m_nowAnimIndex = e_AnimIndex::kIdle;

	m_pWeapon = std::make_shared<PlayerWeapon>();
	m_pWeapon->Load();
	
}

PlayerBase::~PlayerBase()
{

	Finalize();

}

void PlayerBase::Initialize()
{
	//モデルのスケールを決める
	MV1SetScale(m_modelH, VGet(kModelScale, kModelScale, kModelScale));

	m_pWeapon->Initialize(m_modelH, kRightModelFrameNo, kLeftModelFrameNo);

	//攻撃するアニメーションを設定
	//m_equipAnimNo = MV1AttachAnim(m_modelH, e_AnimIndex::kNormalAttack);

	//待機アニメーションを設定
	m_currentAnimNo = MV1AttachAnim(m_modelH, e_AnimIndex::kIdle);

}

void PlayerBase::Finalize()
{
	//モデルをデリートする
	MV1DeleteModel(m_modelH);
	m_modelH = -1;

	m_pWeapon->Finalize();
}

void PlayerBase::Update()
{

	/*アニメーションの切り替え*/
	if (m_prevAnimNo != -1)
	{
		//フレームでアニメーションを切り替える
		m_animBlendRate += kAnimChangeRateSpeed;
		if (m_animBlendRate >= kAnimBlendRateMax)
		{
			m_animBlendRate = kAnimBlendRateMax;
		}

		//アニメーションのブレンド率を設定する
		MV1SetAttachAnimBlendRate(m_modelH, m_prevAnimNo, kAnimBlendRateMax - m_animBlendRate);
		MV1SetAttachAnimBlendRate(m_modelH, m_currentAnimNo, m_animBlendRate);
	}

	m_isAnimationFinish = IsUpdateAnim(m_currentAnimNo);
	if (m_isAnimationFinish)
	{
		ChangeAnim(m_nowAnimIndex);
	}
	IsUpdateAnim(m_prevAnimNo);

	/*フレームにアタッチするための更新処理*/
	m_pWeapon->SwordUpdate();
	m_pWeapon->AxeUpdate();
	m_pWeapon->DaggerUpdate();
	m_pWeapon->MagicWandUpdate();
	m_pWeapon->LongSwordUpdate();

	//仮重力
	m_pos.y -= 2.0f;

	//仮地面
	if (m_pos.y < 0)
	{
		m_pos.y = 0;
	}

	//移動関数
	Move();
	//顔を選択する関数
	FaceSelect();

	//Aボタンを押した場合
	if (Pad::IsTrigger(PAD_INPUT_1))
	{
		ChangeAnim(e_AnimIndex::kJump);

		//ジャンプする関数を呼び出す
		Jump();
	}

	//Bボタンを押した場合
	if (Pad::IsTrigger(PAD_INPUT_2))
	{
		ChangeAnim(e_AnimIndex::kAvoid);

		//回避する関数を呼び出す
		Avoid();
	}

	//Xボタンを押した場合
	if (Pad::IsTrigger(PAD_INPUT_3))
	{
		ChangeAnim(e_AnimIndex::kNormalAttack);

		//攻撃する関数を呼び出す
		Attack();
	}

	//Yボタンを押した場合
	if (Pad::IsTrigger(PAD_INPUT_4))
	{
		ChangeAnim(e_AnimIndex::kNormalSkill);

		//特殊攻撃する関数を呼び出す
		FaceAttack();
	}


	MV1SetPosition(m_modelH, m_pos);
	MV1SetRotationXYZ(m_modelH, VGet(0, m_angle, 0));
}

void PlayerBase::Draw()
{

	//顔を付けている場合
	if (m_playerKind == e_PlayerKind::kPowerPlayer && m_isFaceUse )
	{
		//武器をアタッチする描画関数
		m_pWeapon->AxeDraw();
	}
	else if (m_playerKind == e_PlayerKind::kSpeedPlayer && m_isFaceUse )
	{
		//武器をアタッチする描画関数
		m_pWeapon->DaggerDraw();
	}
	else if (m_playerKind == e_PlayerKind::kShotPlayer && m_isFaceUse )
	{
		//武器をアタッチする描画関数
		m_pWeapon->MagicWandDraw();
	}
	else if (m_playerKind == e_PlayerKind::kStrongestPlayer && m_isFaceUse )
	{
		//武器をアタッチする描画関数
		m_pWeapon->LongSwordDraw();
	}


	//顔を付けていない場合
	if (!m_isFaceUse)
	{
		//武器をアタッチする描画関数
		m_pWeapon->SwordDraw();
	}

	//モデルの描画
	MV1DrawModel(m_modelH);

#ifdef _DEBUG

	DrawSphere3D(m_attackPos, m_radius, 8, 0xff00ff, 0xffffff, false);
	DrawSphere3D(m_pos, m_radius, 8, 0xff00ff, 0xffffff, false);

	DrawFormatString(0, 48, 0xff0fff, "playerPos:%f,%f,%f", m_attackPos.x, m_attackPos.y, m_attackPos.z);
	DrawFormatString(0, 64, 0xff0fff, "playerAttackPos:%f,%f,%f", m_attackPos.x, m_attackPos.y, m_attackPos.z);

	DrawFormatString(0, 80, 0x000fff, " PlayerKind : %d ", m_playerKind);
	DrawFormatString(0, 280, 0x000fff, " PlayerHp : %d ", m_hp);

#endif

}

void PlayerBase::Move()
{
	/*プレイヤーの移動*/
	GetJoypadAnalogInput(&m_analogX, &m_analogZ, DX_INPUT_PAD1);
	VECTOR move = VGet(m_analogX, 0.0f, -m_analogZ);
	float len = VSize(move);
	float rate = len / kAnalogInputMax;

	//アナログスティック無効な範囲を除外する
	//あってもなくてもどっちでもよい
	//rate = (rate - kAnalogRangeMin) / (kAnalogRangeMax - kAnalogRangeMin);
	//rate = min(rate, 1.0f);
	//rate = max(rate, 0.0f);

	//速度が決定できるので移動ベクトルに反映する
	move = VNorm(move);

	
	//スティックの押し加減でプレイヤーのスピードを変える
	//歩き
	if (rate <= 0.8f && rate >= 0.0f);
	{
		float speed = kMinSpeedN * rate;
		move = VScale(move, speed);
	}
	//走り
	if (rate >= 0.8f)
	{
		float speed = kMaxSpeedN * rate;
		move = VScale(move, speed);
	}

	m_pos = VAdd(m_pos, move);

	m_move = move;

	//動いている間
	if (VSquareSize(move) > 0.0f)
	{
		m_angle = -atan2f(move.z, move.x) - DX_PI_F / 2;
		m_attackDir = VNorm(move);
		m_avoid = VNorm(move);

	}

	VECTOR attackMove = VScale(m_attackDir, 10.0f);

	m_attackPos = VAdd(m_pos, attackMove);


}

void PlayerBase::Attack()
{
	//攻撃
}

void PlayerBase::FaceAttack()
{

	//特殊1
	if (m_playerKind == e_PlayerKind::kPowerPlayer && m_isFaceUse)
	{
		//ここにパワーアタック用の処理を入れる予定
		printfDx("kPowerPlayer");
	}
	//特殊2
	else if (m_playerKind == e_PlayerKind::kSpeedPlayer && m_isFaceUse)
	{
		//ここにスピードアタック用の処理を入れる予定
		printfDx("kSpeedPlayer");
	}
	//特殊3
	else if (m_playerKind == e_PlayerKind::kShotPlayer && m_isFaceUse)
	{
		//ここにショットアタック用の処理を入れる予定
		printfDx("kShotPlayer");
	}
	//特殊4
	else if (m_playerKind == e_PlayerKind::kStrongestPlayer && m_isFaceUse)
	{
		//ここにストロンゲストアタック用の処理を入れる予定
		printfDx("kStrongestPlayer");
	}

}

void PlayerBase::Avoid()
{

	VECTOR avoid = VScale(m_avoid, 10.0f);
	m_move = VAdd(m_move, avoid);

	m_pos = VAdd(m_pos, m_move);

}

void PlayerBase::Jump()
{
	m_pos.y = kJumpPower;
}

void PlayerBase::FaceSelect()
{

	//顔を選択
	if (Pad::IsTrigger(PAD_INPUT_6) && !m_isFaceUse)
	{
		if (m_playerKind != e_PlayerKind::kStrongestPlayer)
		{
			m_playerKind = static_cast<e_PlayerKind>(static_cast<int>(m_playerKind) + 1);
		}
	}
	if (Pad::IsTrigger(PAD_INPUT_5) && !m_isFaceUse)
	{
		if (m_playerKind != e_PlayerKind::kPowerPlayer)
		{
			m_playerKind = static_cast<e_PlayerKind>(static_cast<int>(m_playerKind) - 1);
		}
	}

	//顔を決定する	ここはZRで決定にする
	if (Pad::IsTrigger(PAD_INPUT_4))
	{

		//if (m_playerKind == e_PlayerKind::kPowerPlayer && m_isPowerFace)
		//{
		//	m_isFaceUse = true;

		//}
		//else if (m_playerKind == e_PlayerKind::kSpeedPlayer && m_isSpeedFace)
		//{
		//	m_isFaceUse = true;

		//}
		//else if (m_playerKind == e_PlayerKind::kShotPlayer && m_isShotFace)
		//{
		//	m_isFaceUse = true;

		//}
		//else if (m_playerKind == e_PlayerKind::kStrongestPlayer && m_isStrongestFace)
		//{
		//	m_isFaceUse = true;

		//}

		m_isFaceUse = !m_isFaceUse;
	}

	//if (Pad::IsTrigger(PAD_INPUT_4) && m_isFaceUse)
	//{
	//	m_isFaceUse = false;
	//}


}

bool PlayerBase::IsUpdateAnim(int attachNo, float startTime)
{
	//アニメーションが設定されていないので終了
	if (attachNo == -1) return false;

	//アニメーションを進行させる
	float nowFrame = MV1GetAttachAnimTime(m_modelH, attachNo);	//現在の再生カウントを取得
	nowFrame += m_animSpeed;	// アニメーションを進める

	//現在再生中のアニメーションの総カウントを取得する
	float totalAnimFrame = MV1GetAttachAnimTotalTime(m_modelH, attachNo);
	bool isLoop = false;

	//総フレーム分引いても総フレームより大きいかもしれないからwhileで大きい間引き続ける
	while (nowFrame >= totalAnimFrame)
	{
		//nowFrameを0にリセットするとアニメーションフレームの飛びがでてくるから総フレーム分引く
		nowFrame -= totalAnimFrame;
		nowFrame += startTime;	// アニメーションを進める
		isLoop = true;
	}

	//進めた時間に設定
	MV1SetAttachAnimTime(m_modelH, attachNo, nowFrame);

	return isLoop;
}

void PlayerBase::ChangeAnim(int animIndex, float animSpeed)
{
	//さらに古いアニメーションがアタッチされている場合はこの時点で削除しておく
	if (m_prevAnimNo != -1)
	{
		MV1DetachAnim(m_modelH, m_prevAnimNo);
	}

	//現在再生中の待機アニメーションは変更前のアニメーション扱いに
	m_prevAnimNo = m_currentAnimNo;

	//変更後のアニメーションとして攻撃アニメーションを改めて設定する
	m_currentAnimNo = MV1AttachAnim(m_modelH, animIndex);

	//切り替えの瞬間は変更前のアニメーションが再生される状態にする
	m_animBlendRate = 0.0f;

	m_animSpeed = animSpeed;

	//変更前のアニメーション100%
	MV1SetAttachAnimBlendRate(m_modelH, m_prevAnimNo, 1.0f - m_animBlendRate);

	//変更後のアニメーション0%
	MV1SetAttachAnimBlendRate(m_modelH, m_currentAnimNo, m_animBlendRate);

}



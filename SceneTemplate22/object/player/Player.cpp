#include "Player.h"
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

	//アニメーションをスタートさせた時のフレーム
	constexpr float kAimAnimStartFrame = 9.0f;

	//アニメーションの速度
	constexpr float kAnimSpeed = 0.1f;


	//アナログスティック関連
	constexpr float kAnalogRangeMin = 0.1f;		//アナログスティックの入力判定範囲
	constexpr float kAnalogRangeMax = 0.8f;
	constexpr float kAnalogInputMax = 1000.0f;	//アナログスティックから入力されるベクトルの最大

	//移動速度
	constexpr float kMaxSpeedN = 1.0f;
	constexpr float kMinSpeedN = 0.6f;

	//ダッシュスピード
	constexpr float kDashSpeed = 1.5f;

	//ジャンプ力
	constexpr float kJumpPower = 10.0f;

	//初期位置
	constexpr VECTOR kInitPos = { 0.0f,0.0f,0.0f };

	//カプセルの上の座標
	constexpr VECTOR kUpPos = { 0.0f,8.0f,0.0f };

}

Player::Player() :
	m_modelH(-1),
	m_radius(2),
	m_posDown(kInitPos),
	m_posUp(kInitPos),
	m_move(VGet(0, 0, 0)),
	m_attackPos(VGet(0, 0, 0)),
	m_attackDir(VGet(0, 0, 0)),
	m_avoid(VGet(0, 0, 0)),
	m_analogX(0),
	m_analogZ(0),
	m_hp(10),
	m_cameraAngle(0.0f),
	m_isDead(false),
	m_isMove(false)
{

	/*マスク関連の初期化*/
	m_isFaceUse = false;

#ifdef _DEBUG
	m_isPowerFace = true;
	m_isSpeedFace = true;
	m_isShotFace = true;
	m_isStrongestFace = true;
#else

	m_isPowerFace = false;
	m_isSpeedFace = false;
	m_isShotFace = false;
	m_isStrongestFace = false;

#endif

	/*アニメーション情報初期化*/
	m_currentAnimNo = -1;
	m_prevAnimNo = -1;

	m_animSpeed = 0.8f;
	m_animBlendRate = 1.0f;
	m_isAnimationFinish = false;

	m_animIndex = e_AnimIndex::kIdle;

	m_isAnimIdle = false;
	m_isAnimWalk = false;
	m_isAnimDash = false;
	m_isAnimAttackX = false;
	m_isAnimAttackY = false;
	m_isAnimDamage = false;
	m_isAnimDown = false;
	m_isAnimJump = false;

	//モデルのロード
	m_modelH = MV1LoadModel(kModelFilename);
	assert(m_modelH > -1);


	m_playerKind = e_PlayerKind::kPowerPlayer;

	m_pWeapon = std::make_shared<PlayerWeapon>();



}

Player::~Player()
{
	Finalize();
}

void Player::Initialize(VECTOR pos)
{
	//初期位置を代入
	m_posDown = pos;

	//モデルのスケールを決める
	MV1SetScale(m_modelH, VGet(kModelScale, kModelScale, kModelScale));

	//武器の初期化
	m_pWeapon->Load();
	m_pWeapon->Initialize(m_modelH, kRightModelFrameNo, kLeftModelFrameNo);

	//待機アニメーションを設定
	m_currentAnimNo = MV1AttachAnim(m_modelH, e_AnimIndex::kIdle, -1, false);
	m_prevAnimNo = -1;
	m_animBlendRate = 1.0f;

	// メンバ関数ポインタ = &クラス名::入れたい関数
	m_updaFunc = &Player::Move;
}

void Player::Finalize()
{
	//モデルをデリートする
	MV1DeleteModel(m_modelH);
	m_modelH = -1;

	m_pWeapon->Finalize();
}

void Player::Update()
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
	//アニメーションを進める
	if (m_isAnimationFinish)
	{
		ChangeAnim(m_animIndex);
	}
	IsUpdateAnim(m_prevAnimNo);


	//仮重力
	m_posDown.y -= 1.2f;


	//仮地面
	if (m_posDown.y < 0)
	{
		m_posDown.y = 0;
	}
	
	//顔を選択する関数
	FaceSelect();

	/*フレームにアタッチするための更新処理*/
	m_pWeapon->SwordUpdate();
	m_pWeapon->AxeUpdate();
	m_pWeapon->DaggerUpdate();
	m_pWeapon->MagicWandUpdate();
	m_pWeapon->LongSwordUpdate();

	//アップデート
	//(this->*m_updaFunc)(/*引数入れる*/);
	
	if (!m_isAnimAttackX && !m_isAnimAttackY /*&& !m_isAnimAvoid*/ && !m_isAnimDamage)
	{
		//攻撃X
		if (Pad::IsTrigger(PAD_INPUT_3) && !m_isAnimJump)
		{
			//m_updaFunc = &Player::AttackX;
			//ノーマル時のX攻撃
			ChangeAnim(e_AnimIndex::kNormalAttack);
			m_isAnimAttackX = true;
			m_isMove = true;
		}
		else
		{
			if (m_isMove)
			{
				Move();
			}
		}

		//攻撃Y
		if (Pad::IsTrigger(PAD_INPUT_4) && !m_isAnimJump)
		{

			//m_updaFunc = &Player::AttackY;
			//ノーマル時のY攻撃
			ChangeAnim(e_AnimIndex::kNormalSkill);
			m_isAnimAttackY = true;
			m_isMove = false;

		}
		else
		{
			if (!m_isMove)
			{
				Move();

			}
		}

	}
	else
	{
		//攻撃アニメーションが終了したら待機アニメーションを再生する
		if (m_isAnimationFinish)
		{
			m_isAnimAttackX = false;
			m_isAnimAttackY = false;
			m_isAnimDash = false;
		}
	}

	//Bボタンを押した場合
	if (Pad::IsTrigger(PAD_INPUT_2))
	{
		ChangeAnim(e_AnimIndex::kJump);
		m_isAnimJump = true;
		//Jump();
		Avoid();

		//回避する関数を呼び出す
		//m_updaFunc = &Player::Avoid;
	}
	else
	{
		//アニメーションが終わったら
		if (m_isAnimationFinish)
		{
			m_isAnimJump = false;
		}
	}


	//ダメージを受けた時のアニメーション
	if (m_isAnimDamage)
	{
		ChangeAnim(e_AnimIndex::kJump);
		m_isAnimAvoid = true;
	}
	else
	{
		//アニメーションが終わったら
		if (m_isAnimationFinish)
		{
			m_isAnimDamage = false;
		}
	}

	//カプセル用のポジション
	m_posUp = VGet(m_posDown.x, m_posDown.y + 8.0f, m_posDown.z);

	//モデルに座標をセットする
	MV1SetPosition(m_modelH, m_posDown);
	//モデルに回転をセットする
	MV1SetRotationXYZ(m_modelH, VGet(0, m_angle, 0));
}

void Player::Draw()
{

	//武器の描画
	WeaponDraw();

	//モデルの描画
	MV1DrawModel(m_modelH);

#ifdef _DEBUG

	DrawSphere3D(m_attackPos, m_radius, 8, 0xff00ff, 0xffffff, false);
	DrawSphere3D(m_posDown, m_radius, 8, 0xff00ff, 0xffffff, false);

	DrawFormatString(0, 48, 0xff0fff, "playerPos:%f,%f,%f", m_attackPos.x, m_attackPos.y, m_attackPos.z);
	DrawFormatString(0, 64, 0xff0fff, "playerAttackPos:%f,%f,%f", m_attackPos.x, m_attackPos.y, m_attackPos.z);

	DrawFormatString(0, 80, 0x000fff, " PlayerKind : %d ", m_playerKind);
	DrawFormatString(0, 280, 0x000fff, " PlayerHp : %d ", m_hp);

#endif

}

void Player::WeaponDraw()
{
	//顔を付けている場合
	if (m_playerKind == e_PlayerKind::kPowerPlayer && m_isFaceUse)
	{
		//武器をアタッチする描画関数
		m_pWeapon->AxeDraw();
	}
	else if (m_playerKind == e_PlayerKind::kSpeedPlayer && m_isFaceUse)
	{
		//武器をアタッチする描画関数
		m_pWeapon->DaggerDraw();
	}
	else if (m_playerKind == e_PlayerKind::kShotPlayer && m_isFaceUse)
	{
		//武器をアタッチする描画関数
		m_pWeapon->MagicWandDraw();
	}
	else if (m_playerKind == e_PlayerKind::kStrongestPlayer && m_isFaceUse)
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
}

void Player::Move()
{
	/*プレイヤーの移動*/
	GetJoypadAnalogInput(&m_analogX, &m_analogZ, DX_INPUT_PAD1);
	VECTOR move = VGet(m_analogX, 0.0f, -m_analogZ);
	float len = VSize(move);
	float rate = len / kAnalogInputMax;

	//速度が決定できるので移動ベクトルに反映する
	move = VNorm(move);

	//アナログスティック無効な範囲を除外する
	//あってもなくてもどっちでもよい
	rate = (rate - kAnalogRangeMin) / (kAnalogRangeMax - kAnalogRangeMin);
	rate = min(rate, 1.0f);
	rate = max(rate, 0.0f);

	//スティックの押し加減でプレイヤーのスピードを変える
	if (rate <= 0.6f && rate > 0.0f && !m_isFaceUse);
	{
		float speed = kMinSpeedN * rate;
		move = VScale(move, speed);
	}
	if (rate >= 0.6f && !m_isFaceUse)
	{
		float speed = kMaxSpeedN * rate;
		move = VScale(move, speed);
	}


	//歩きのアニメーション
	if (VSize(move) != 0.0f)
	{
		if (!m_isAnimWalk && !m_isAnimJump)
		{
			ChangeAnim(e_AnimIndex::kWalk);
			m_animIndex = e_AnimIndex::kWalk;
		}
		m_isAnimWalk = true;
	}
	else
	{
		m_isAnimWalk = false;
	}

	if (Pad::IsPress(PAD_INPUT_1) /*&& m_stamina >= 0.0f*/ && VSize(move) != 0.0f )
	{
		float speed = kDashSpeed * rate;
		move = VScale(move, speed);

		if (!m_isAnimDash && !m_isAnimJump)
		{
			ChangeAnim(e_AnimIndex::kDash);
			m_animIndex = e_AnimIndex::kDash;
		}
		m_isAnimDash = true;

	}
	else
	{
		if (m_isAnimDash && m_isAnimWalk && !m_isAnimJump)
		{
			ChangeAnim(e_AnimIndex::kWalk);
			m_animIndex = e_AnimIndex::kWalk;
		}
		m_isAnimDash = false;
	}

	//カメラのいる場所(角度)から
	//コントローラーによる移動方向を決定する
	MATRIX mtx = MGetRotY(-m_cameraAngle - DX_PI_F / 2);
	move = VTransform(move, mtx);

	m_posDown = VAdd(m_posDown, move);
	m_move = move;

	//動いている間
	if (VSquareSize(move) > 0.0f)
	{
		m_angle = -atan2f(move.z, move.x) - DX_PI_F / 2;
		m_attackDir = VNorm(move);
		m_avoid = VNorm(move);

	}

	VECTOR attackMove = VScale(m_attackDir, 10.0f);

	m_attackPos = VAdd(m_posDown, attackMove);

	//キャラクターが移動していない場合
	if (!m_isAnimDash && !m_isAnimWalk && !m_isAnimIdle)
	{
		ChangeAnim(e_AnimIndex::kIdle);
		m_animIndex = e_AnimIndex::kIdle;
		m_isAnimIdle = true;
	}
	else
	{
		if (m_isAnimationFinish)
		{
			m_isAnimIdle = false;
		}
	}

	//HPがゼロより下にいった場合
	if (m_hp <= 0)
	{
		m_hp = 0;
	}

}

void Player::AttackX()
{

}

void Player::AttackY()
{

}

void Player::Avoid()
{

	//VECTOR avoid = VScale(m_avoid, 10.0f);
	//m_move = VAdd(m_move, avoid);

	////m_pos = VAdd(m_pos, m_move);

	//ChangeAnim(e_AnimIndex::kAvoid);

	//VECTOR avoid = VScale(m_avoid, 10.0f);
	//m_move = VAdd(m_move, avoid);

	//m_posDown = VAdd(m_posDown, m_move);
}

void Player::Jump()
{
	m_posDown = VAdd(m_posDown, VGet(0, kJumpPower, 0));
	//m_posDown.y = kJumpPower;
	//ChangeAnim(e_AnimIndex::kJump);
}

void Player::FaceSelect()
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
		m_isFaceUse = !m_isFaceUse;
	}

}

bool Player::IsUpdateAnim(int attachNo, float startTime)
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

void Player::ChangeAnim(int animIndex)
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

	//m_animSpeed = animSpeed;

	//変更前のアニメーション100%
	MV1SetAttachAnimBlendRate(m_modelH, m_prevAnimNo, 1.0f - m_animBlendRate);

	//変更後のアニメーション0%
	MV1SetAttachAnimBlendRate(m_modelH, m_currentAnimNo, m_animBlendRate);

}
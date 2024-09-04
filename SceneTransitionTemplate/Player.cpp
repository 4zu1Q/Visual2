#include "Player.h"
#include "Pad.h"
#include "Game.h"
#include <cmath>
#include <cassert>

/// <summary>
/// 定数
/// </summary>
namespace
{

	//モデルのファイル名
	const char* const kPlayerModelFilename = "data/model/player/barbarian.mv1";

	//モデル用の定数
	constexpr float kScale = 10.0f;

	//HPのファイル名
	const char* const kHpFilename = "data/image/Hp.png";
	const char* const kLostHpFilename = "data/image/LostHp.png";


	//シェーダのファイル名
	const char* const kOutlinePsFilename = "./OutlinePs.pso";
	const char* const kOutlineVsFilename = "./OutlineVs.vso";

	//アニメーション
	constexpr int kIdleAnimIndex = 1;		//待機
	constexpr int kWalkAnimIndex = 2;		//歩き
	constexpr int kRunAnimIndex = 7;		//走り
	constexpr int kAttackAnimIndex = 30;	//攻撃
	constexpr int kDamageAnimIndex = 25;	//ダメージ
	constexpr int kFallAnimIndex = 26;		//倒れる
	constexpr int kFallingAnimIndex = 27;	//倒れ中

	//アニメーションの切り替えにかかるフレーム数
	constexpr float kAnimChangeFrame = 8.0f;
	constexpr float kAnimChangeRateSpeed = 1.0f / kAnimChangeFrame;

	//ダメージ
	constexpr int kDamageCount = 120;



	//アナログスティックによる移動関連
	constexpr float kMaxSpeed = 0.5f;		//プレイヤーの最大移動速度
	constexpr float kAnalogRangeMin = 0.1;	//アナログスティックの入力判定範囲
	constexpr float kAnalogRangeMax = 0.8;
	constexpr float kAnalogInputMax = 1000.0f;	//アナログスティックから入力されるベクトルの最大値

	//ハートのアニメーション
	constexpr float kHpSinSpeed = 0.05f;
	constexpr float kHpAnimationSize = 3.0f;


	//壁
	constexpr float kWall = 200;
}

/// <summary>
/// コンストラクタ
/// </summary>
Player::Player() :
	m_modelH(-1),
	m_pos(VGet(0,0,0)),
	m_attackPos(VGet(0,0,0)),
	m_attackDir(VGet(0,0,0)),
	m_hpH(LoadGraph(kHpFilename)),
	m_losthpH(LoadGraph(kLostHpFilename)),
	m_outlinePsH(-1),
	m_outlineVsH(-1),
	m_currentAnimNo(-1),
	m_prevAnimNo(-1),
	m_animBlendRate(0.0f),
	m_cameraAngle(0.0f),
	m_angle(0.0f),
	m_radius(6.0f),
	m_isAttack(false),
	m_isWalk(false),
	m_isJump(false),
	m_isDamage(false),
	m_damageFrame(0),
	m_hp(10),
	m_losthp(10),
	m_hpAnimationHeight(0.0f),
	m_animIndex(AnimKind::kNone),
	m_animationIndex(-1)
{

}

/// <summary>
/// デストラクタ
/// </summary>
Player::~Player()
{
	Delete();
}

/// <summary>
/// ロード
/// </summary>
void Player::Load()
{
	//モデルのロード
	m_modelH = MV1LoadModel(kPlayerModelFilename);
	assert(m_modelH > -1);

	//アニメーションのブレンド率を初期化
	m_animBlendRate = 1.0f;

	//初期状態ではアニメーションはアタッチされていないにする
	m_currentPlayAnim = -1;
	m_prevPlayAnim = -1;

	//アニメーションを再生


	//シェーダのロード
	//ShaderLoad();
}

/// <summary>
/// デリート
/// </summary>
void Player::Delete()
{
	MV1DeleteModel(m_modelH);
	m_modelH = -1;
}

/// <summary>
/// 初期化
/// </summary>
void Player::Init()
{

	//待機アニメーションを設定
	m_currentAnimNo = MV1AttachAnim(m_modelH, kIdleAnimIndex, -1, false);
	m_prevAnimNo = -1;
	m_animBlendRate = 1.0f;

	//プレイヤーの初期位置設定
	m_pos = VGet(0.0f, 0.0f, -30.0f);
	m_attackPos = VGet(m_pos.x, m_pos.y, m_pos.z - 10);

	MV1SetPosition(m_modelH, m_pos);
	MV1SetScale(m_modelH, VGet(kScale, kScale, kScale));

	m_isAttack = false;
}

/// <summary>
/// アップデート
/// </summary>
void Player::Update()
{
	Pad::Update();

	//アニメーション
	if (m_prevAnimNo != -1)
	{
		// test 8フレーム切り替え
		m_animBlendRate += kAnimChangeRateSpeed;
		if (m_animBlendRate >= 1.0f) m_animBlendRate = 1.0f;

		//変更後のアニメーション割合を設定する
		MV1SetAttachAnimBlendRate(m_modelH, m_prevAnimNo, 1.0f - m_animBlendRate);
		MV1SetAttachAnimBlendRate(m_modelH, m_currentAnimNo, m_animBlendRate);
	}

	//アニメーションを進める
	bool isLoop = UpdateAnim(m_currentAnimNo);
	UpdateAnim(m_prevAnimNo);

	//ボタンを押したら攻撃アニメーションを再生する
	if (!m_isAttack)
	{

		if (Pad::IsTrigger PAD_INPUT_3)
		{
			m_isAttack = true;
			ChangeAnim(kAttackAnimIndex);
		}

			m_isWalk = true;

			//アナログスティックを使って移動
			int analogX = 0;
			int analogZ = 0;

			GetJoypadAnalogInput(&analogX, &analogZ, DX_INPUT_PAD1);
			//アナログスティックの入力の10% ~ 80%の範囲を使用する

			//ベクトルの長さが最大で1000になる
			//プレイヤーの最大移動速度は0.01f / frame

			VECTOR move = VGet(analogX, 0.0f, -analogZ);	//ベクトルの長さは0～1000

			//0.00 ~ 0.01の長さにする
			//ベクトルの長さを取得する
			float len = VSize(move);
			//ベクトルの長さを0.0~1.0の割合に変換する
			float rate = len / kAnalogInputMax;

			//アナログスティック無効な範囲を除外する
			rate = (rate - kAnalogRangeMin) / (kAnalogRangeMax - kAnalogRangeMin);
			rate = min(rate, 1.0f);
			rate = max(rate, 0.0f);

			//速度が決定できるので移動ベクトルに反映する
			move = VNorm(move);
			float speed = kMaxSpeed * rate;
			move = VScale(move, speed);

			//カメラのいる場所(角度)から
			//コントローラーによる移動方向を決定する
			MATRIX mtx = MGetRotY(-m_cameraAngle - DX_PI_F / 2);
			move = VTransform(move, mtx);

			//移動方向からプレイヤーの向く方向を決定する
			//移動していない場合(ゼロベクトル)の場合は変更しない
			if (VSquareSize(move) > 0.0f)
			{
				m_angle = -atan2f(move.z, move.x) - DX_PI_F / 2;
				m_attackDir = VNorm(move);
			}

			m_pos = VAdd(m_pos, move);

			//Bボタンを押している間
			if (Pad::IsPress(PAD_INPUT_1))
			{
				//動くスピードを1.5倍
				move = VScale(move, 1.5f);
				//動きを反映
				m_pos = VAdd(m_pos, move);
			}
			
			VECTOR attackMove = VScale(m_attackDir, 12.0f);
			m_attackPos = VAdd(m_pos,attackMove);


	}
	else
	{
		//攻撃アニメーションが終了したら待機アニメーションを再生する
		if (isLoop)
		{
			m_isAttack = false;
			ChangeAnim(kIdleAnimIndex);
		}
		
			

	}

	MV1SetPosition(m_modelH, m_pos);
	MV1SetRotationXYZ(m_modelH, VGet(0, m_angle, 0));
	

	//ShaderUpdate();

	//Hpのアニメーション
	static float SinCount = 0;
	SinCount += kHpSinSpeed;
	m_hpAnimationHeight = sinf(SinCount) * kHpAnimationSize;


	//移動範囲
	if (m_pos.x >= 195) m_pos.x = 195;
	if (m_pos.x <= -195) m_pos.x = -195;
	if (m_pos.z >= 195) m_pos.z = 195;
	if (m_pos.z <= -195) m_pos.z = -195;
	
	//ダメージ点滅時間
	if (m_isDamage)
	{
		m_damageFrame++;
		if (m_damageFrame > kDamageCount)
		{
			m_isDamage = false;
			m_damageFrame = 0;
		}
	}
}

/// <summary>
/// 描画
/// </summary>
void Player::Draw()
{


	// 半透明にしてメニュー背景の四角
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
	DrawFillBox(10, 10, 500, 50, 0x000000);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	// 不透明に戻して白い枠
	DrawLineBox(10, 10, 500, 50, 0x00ffff);

	for (int i = 1; i <= m_losthp; i++)
	{
		DrawExtendGraph(35 * i, 16 + m_hpAnimationHeight, 35 * i + 32, 48, m_losthpH, true);

	}

	for (int i = 1; i <= m_hp; i++)
	{
		DrawExtendGraph(35 * i, 16 + m_hpAnimationHeight, 35 * i + 32, 48, m_hpH, true);
	}

#ifdef _DEBUG

	DrawSphere3D(VAdd(m_pos, VGet(0, 8, 0)), m_radius, 8, 0xffffff, 0xffffff, false);
	DrawSphere3D(VAdd(m_attackPos, VGet(0, 8, 0)), m_radius, 8, 0xff00ff, 0xff00ff, false);

	DrawFormatString(0, 16, 0xffffff, "Player(x:%f,y:%f,z:%f)", m_pos.x, m_pos.y, m_pos.z);
	DrawFormatString(800, 16, 0xffffff, "Player(x:%f,y:%f,z:%f)", m_attackPos.x, m_attackPos.y, m_attackPos.z);

	DrawFormatString(400, 16, 0xffffff, "PlayerHp:%d", m_hp);

#endif
	// ダメージ演出  2フレーム間隔で表示非表示切り替え
	// 0: 表示される
	// 1: 表示される
	// 2: 非表示
	// 3: 非表示
	// 4: 表示される	...
	// % 4 することで012301230123... に変換する
	if (m_damageFrame % 4 >= 2) return;

	MV1DrawModel(m_modelH);

}

/// <summary>
/// ギミックに当たった時のアップデート処理
/// </summary>
void Player::OnGimmickHitUpdate()
{
	Pad::Update();

	int frame = 0;
	m_isJump = true;


}

/// <summary>
/// アニメーションのアップデート処理
/// </summary>
/// <param name="attachNo"></param>
/// <returns></returns>
bool Player::UpdateAnim(int attachNo)
{
	//アニメーションが設定されていないので終了
	if (attachNo == -1) return false;

	//アニメーションを進行させる
	float now = MV1GetAttachAnimTime(m_modelH, attachNo);	//現在の再生カウントを取得
	bool isLoop = false;
	now += 0.5f;	//アニメーションを進める

	//現在再生中のアニメーションの総カウントを取得する
	float total = MV1GetAttachAnimTotalTime(m_modelH, attachNo);

	while (now >= total)
	{
		now -= total;
		isLoop = true;
	}

	//進めた時間に設定
	MV1SetAttachAnimTime(m_modelH, attachNo, now);

	return isLoop;
}

/// <summary>
/// アニメーションを変更する関数
/// </summary>
/// <param name="animIndex"></param>
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
	m_currentAnimNo = MV1AttachAnim(m_modelH, animIndex, -1, false);

	//切り替えの瞬間は変更前のアニメーションが再生される状態にする
	m_animBlendRate = 0.0f;

	//変更前のアニメーション100%
	MV1SetAttachAnimBlendRate(m_modelH, m_prevAnimNo, 1.0f - m_animBlendRate);

	//変更後のアニメーション0%
	MV1SetAttachAnimBlendRate(m_modelH, m_currentAnimNo, m_animBlendRate);
}

/// <summary>
/// シェーダをロードする関数
/// </summary>
void Player::ShaderLoad()
{
	/*シェーダ*/
	//作ったシェーダを呼び出すか呼び出さないか
	MV1SetUseOrigShader(true);

	//シェーダのロード
	m_outlineVsH = LoadVertexShader(kOutlineVsFilename);
	m_outlinePsH = LoadPixelShader(kOutlinePsFilename);
}

/// <summary>
/// シェーダのアップデート処理
/// </summary>
void Player::ShaderUpdate()
{
	//メッシュの数を取得
	auto m_meshNum = MV1GetMeshNum(m_modelH);

	/*反転法*/
	SetUseVertexShader(m_outlineVsH);
	SetUsePixelShader(m_outlinePsH);
	MV1DrawModel(m_modelH);


}

void Player::PlayAnim(AnimKind playAnim)
{
	if (m_prevPlayAnim != -1)
	{
		MV1DetachAnim(m_modelH, m_prevPlayAnim);
		m_prevPlayAnim = -1;
	}

	//今まで再生中のモーションだったものの情報をPrevに移動する
	m_prevPlayAnim = m_currentPlayAnim;
	m_prevAnimCount = m_currentAnimCount;

	//新たに指定のモーションをモデルにアタッチして、アタッチ番号を保存する
	m_currentPlayAnim = MV1AttachAnim(m_modelH, static_cast<int>(playAnim));
	m_currentAnimCount = 0.0f;

	//ブレンド率はPrevが有効ではない場合は1.0f(現在モーションが100%の状態)にする
	m_animBlendRate = m_prevPlayAnim == -1 ? 1.0f : 0.0f;

}

void Player::AttackCol(VECTOR pos)
{
}
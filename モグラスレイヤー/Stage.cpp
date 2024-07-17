#include "Stage.h"
#include "DxLib.h"

/// <summary>
/// 定数
/// </summary>
namespace
{
	//モデルのファイル名
	const char* const kPlayerModelFileName = "data/model/ground/ground.mv1";
}

/// <summary>
/// コンストラクタ
/// </summary>
Stage::Stage() :
	m_modelHandle(-1),
	m_pos(VGet(0.0f, -10.0f, 0.0f))
{
}

/// <summary>
/// デストラクタ
/// </summary>
Stage::~Stage()
{
}

/// <summary>
/// ロード
/// </summary>
void Stage::Load()
{
	m_modelHandle = MV1LoadModel(kPlayerModelFileName);
}

/// <summary>
/// デリート
/// </summary>
void Stage::Delete()
{
	MV1DeleteModel(m_modelHandle);
	m_modelHandle = -1;
}

/// <summary>
/// 初期化
/// </summary>
void Stage::Init()
{
	//3Dモデルのスケールを2倍する
	MV1SetScale(m_modelHandle, VGet(3.0f, 3.0f, 3.0f));

	MV1SetPosition(m_modelHandle, m_pos);

}

/// <summary>
/// アップデート
/// </summary>
void Stage::Update()
{

	MV1SetPosition(m_modelHandle, m_pos);
}

/// <summary>
/// 描画
/// </summary>
void Stage::Draw()
{
	MV1DrawModel(m_modelHandle);

}

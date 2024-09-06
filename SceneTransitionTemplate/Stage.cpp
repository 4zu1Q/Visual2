#include "Stage.h"

#include "Stage.h"
#include "DxLib.h"
#include <cassert>

/// <summary>
/// 定数
/// </summary>
namespace
{
	//モデルのファイル名
	const char* const kPlayerModelFileName = "data/model/stage/stage.mv1";
}

/// <summary>
/// コンストラクタ
/// </summary>
Stage::Stage() :
	m_modelHandle(-1),
	m_pos(VGet(0.0f, 129, 0.0f)),
	m_light(-1)
{
	
}

/// <summary>
/// デストラクタ
/// </summary>
Stage::~Stage()
{
	Delete();
}

/// <summary>
/// ロード
/// </summary>
void Stage::Load()
{
	m_modelHandle = MV1LoadModel(kPlayerModelFileName);
	//assert(m_modelHandle >= 0);

	

}

/// <summary>
/// デリート
/// </summary>
void Stage::Delete()
{
	MV1DeleteModel(m_modelHandle);
	m_modelHandle = -1;

	DeleteLightHandle(m_light);
}

/// <summary>
/// 初期化
/// </summary>
void Stage::Init()
{
	ChangeLightTypeDir(VGet(-1.0f, 0.0f, 0.0f));
	m_light = CreateDirLightHandle(VGet(1.0f, 0.0f, 0.0f));

	//3Dモデルのスケールを2倍する
	MV1SetScale(m_modelHandle, VGet(0.4f, 0.4f, 0.4f));

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

#ifdef _DEBUG

	DrawFormatString(0, 96, 0xffffff, "Stage(x:%f,y:%f,z:%f)", m_pos.x, m_pos.y, m_pos.z);

#endif
}

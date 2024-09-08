#include "Stage.h"

#include "Stage.h"
#include "DxLib.h"
#include <cassert>

/// <summary>
/// �萔
/// </summary>
namespace
{
	//���f���̃t�@�C����
	const char* const kPlayerModelFileName = "data/model/stage/Stage10.mv1";
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
Stage::Stage() :
	m_modelHandle(-1),
	m_pos(VGet(0.0f, 129, 0.0f)),
	m_light(-1)
{
	
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Stage::~Stage()
{
	Delete();
}

/// <summary>
/// ���[�h
/// </summary>
void Stage::Load()
{
	m_modelHandle = MV1LoadModel(kPlayerModelFileName);
	//assert(m_modelHandle >= 0);

	

}

/// <summary>
/// �f���[�g
/// </summary>
void Stage::Delete()
{
	MV1DeleteModel(m_modelHandle);
	m_modelHandle = -1;

	DeleteLightHandle(m_light);
}

/// <summary>
/// ������
/// </summary>
void Stage::Init()
{
	ChangeLightTypeDir(VGet(-1.0f, 0.0f, 0.0f));
	m_light = CreateDirLightHandle(VGet(1.0f, 0.0f, 0.0f));

	//3D���f���̃X�P�[����2�{����
	MV1SetScale(m_modelHandle, VGet(0.4f, 0.4f, 0.4f));

	MV1SetPosition(m_modelHandle, m_pos);

}

/// <summary>
/// �A�b�v�f�[�g
/// </summary>
void Stage::Update()
{

	MV1SetPosition(m_modelHandle, m_pos);
}

/// <summary>
/// �`��
/// </summary>
void Stage::Draw()
{
	MV1DrawModel(m_modelHandle);

#ifdef _DEBUG

	DrawFormatString(0, 96, 0xffffff, "Stage(x:%f,y:%f,z:%f)", m_pos.x, m_pos.y, m_pos.z);

#endif
}

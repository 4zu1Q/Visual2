#include "Field.h"
#include "DxLib.h"

namespace 
{

	const char* const kFieldModelFilename = "Data/Model/Stage/Field10.mv1";
	const char* const kTestModelFilename = "Data/Model/Weapon/Boss_Shield.mv1";

	const char* const kFileName[static_cast<int>(Game::e_StageKind::kStageNum)] =
	{
		"Data/Model/Stage/Field10.mv1",
		"Data/Model/Stage/Field07.mv1",
	};

	//戦う時のスケール値
	constexpr VECTOR kSelectModelScale = { 1.0f , 1.0f , 1.0f };
	constexpr VECTOR kGamePlayModelScale = { 0.4f , 0.4f , 0.4f };

	constexpr VECTOR kSelectModelPosition = { 0.0f, 0.0f, 0.0f };
	constexpr VECTOR kGamePlayModelPosition = { 0.0f, 150.0f, 0.0f };
}

Field::Field(Game::e_StageKind stageKind):
	m_modelH(-1),
	m_pos(VGet(0,0,0))
{
	m_modelH = MV1LoadModel(kFileName[static_cast<int>(stageKind)]);

	if (stageKind == Game::e_StageKind::kSelect)
	{
		MV1SetScale(m_modelH, kSelectModelScale);
	}
	else if (stageKind == Game::e_StageKind::kGamePlay)
	{
		MV1SetScale(m_modelH, kGamePlayModelScale);
	}

	if (stageKind == Game::e_StageKind::kSelect)
	{
		m_pos = kSelectModelPosition;
	}
	else if (stageKind == Game::e_StageKind::kGamePlay)
	{
		m_pos = kGamePlayModelPosition;
	}

	//モデルに座標をセットする
	MV1SetPosition(m_modelH, m_pos);
}

Field::~Field()
{
	MV1DeleteModel(m_modelH);
	m_modelH = -1;
}

void Field::Initialize()
{
}

//void Field::Update()
//{
//
//}

void Field::Draw()
{
	MV1DrawModel(m_modelH);
}

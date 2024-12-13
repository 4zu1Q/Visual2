#include "Field.h"
#include "DxLib.h"

namespace 
{

	const char* const kFieldModelFilename = "Data/Model/Stage/Field10.mv1";
	const char* const kTestModelFilename = "Data/Model/Weapon/Boss_Shield.mv1";

	//戦う時のスケール値
	//constexpr VECTOR kModelScale = { 0.4f , 0.4f , 0.4f };
	constexpr VECTOR kModelScale = { 1.0f , 1.0f , 1.0f };

	constexpr VECTOR kModelPosition = { 0.0f, 0.0f, 0.0f };
}

Field::Field():
	m_modelH(-1),
	m_pos(VGet(0,0,0))
{
	m_modelH = MV1LoadModel(kFieldModelFilename);
	MV1SetScale(m_modelH, kModelScale);

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

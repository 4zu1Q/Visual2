#include "Field.h"
#include "DxLib.h"

namespace 
{

	const char* const kFieldModelFilename = "Data/Model/Stage/Field03.mv1";
	const char* const kTestModelFilename = "Data/Model/Weapon/Boss_Shield.mv1";

	constexpr VECTOR kModelScale = { 0.45f , 0.45f , 0.45f };

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

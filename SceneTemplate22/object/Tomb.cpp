#include "Tomb.h"

#include "object/player/Player.h"

namespace
{
	constexpr float kRotSpeed = 0.009f;

	const char* const kItemHpModelFilename = "Data/Model/Obj/Potion.mv1";

	constexpr float kSinSpeed = 0.01f;
	constexpr float kHeightSize = 0.01f;

	constexpr float kAddPosY = 8.0f;

	constexpr VECTOR kModelScale = { 0.2f , 0.2f , 0.2f };

}

Tomb::Tomb()
{
}

Tomb::~Tomb()
{
}

void Tomb::Initialize()
{
}

void Tomb::Finalize()
{
}

void Tomb::Update()
{
}

void Tomb::Draw()
{
}

bool Tomb::TombHit(std::shared_ptr<Player> pPlayer)
{
	return false;
}

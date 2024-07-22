#include "SceneBase.h"

void SceneBase::Init()
{
}

std::shared_ptr<SceneBase> SceneBase::Update()
{
	return shared_from_this();
}

void SceneBase::Draw()
{

}

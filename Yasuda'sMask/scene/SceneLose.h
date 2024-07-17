#pragma once
#include "SceneBase.h"
class SceneLose :
    public SceneBase
{
public:
	SceneLose();
	virtual ~SceneLose();

	virtual void Init();	//シーンに入るときの初期化処理
	virtual std::shared_ptr<SceneBase> Update();	//毎フレーム行う更新処理
	virtual void Draw();	//毎フレーム行う描画処理
	virtual void End();	//シーンを抜けるときの処理

private:

#ifdef _DEBUG



#endif

};


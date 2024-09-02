#pragma once
#include "SceneBase.h"
class SceneDebug :
    public SceneBase
{
public:
	SceneDebug();
	virtual ~SceneDebug();

	virtual void Init();	//シーンに入るときの初期化処理
	virtual std::shared_ptr<SceneBase> Update();	//毎フレーム行う更新処理
	virtual void Draw();	//毎フレーム行う描画処理
	virtual void End();	//シーンを抜けるときの処理

private:

	//カーソル選択

	enum Select
	{
		kSceneDebug,
		kSceneTitle,
		kScenePlaying,
		kSceneWin,
		kSceneLose,
	};

	Select m_select;	//選択変数
};


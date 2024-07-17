#pragma once
#include "SceneBase.h"
#include <memory>

class SceneTitle :
    public SceneBase
{
public:
    SceneTitle();
    virtual ~SceneTitle();

	virtual void Init();	//シーンに入るときの初期化処理
	virtual std::shared_ptr<SceneBase> Update();	//毎フレーム行う更新処理
	virtual void Draw();	//毎フレーム行う描画処理
	virtual void End();	//シーンを抜けるときの処理
    
private:

#ifdef _DEBUG

	enum Select
	{
		kSceneSelect,
		kScenePlaying,
		kSceneWin,
		kSceneLose,
	};
	Select m_select;	//選択変数

#endif

	int m_frameCount;


};


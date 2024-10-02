#pragma once
#include "SceneBase.h"

class SceneTitle :
    public SceneBase
{
public:
    SceneTitle();
    virtual ~SceneTitle();

	virtual void Init()  override final;	//シーンに入るときの初期化処理
	virtual void End() override final;	//シーンを抜けるときの処理

	virtual std::shared_ptr<SceneBase> Update() override final;	//毎フレーム行う更新処理
	virtual void Draw() override final;	//毎フレーム行う描画処理
    
private:
	int m_frameCount;

	

};


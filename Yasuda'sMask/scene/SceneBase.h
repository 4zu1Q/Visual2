#pragma once
#include <memory>


class SceneBase :public std::enable_shared_from_this<SceneBase>
{
public:
	SceneBase() {}
	virtual ~SceneBase() {}

	
	virtual void Init() = 0;	//シーンに入るときの初期化処理

														//毎フレーム行う更新処理
	virtual std::shared_ptr<SceneBase> Update() = 0;	//シーンを変更する場合は遷移先のシーンのポインタ
														//シーン変更しない場合は自身のポインタ

	virtual void Draw() = 0;	//毎フレーム行う描画処理
	virtual void End() = 0;	//シーンを抜けるときの処理

};


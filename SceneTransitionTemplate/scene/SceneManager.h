#pragma once
#include <memory>

class SceneBase;
class SceneManager
{
public:
	SceneManager();
	virtual ~SceneManager();

	virtual void Init();	//シーンに入るときの初期化処理
	virtual void Update();	//毎フレーム行う更新処理
	virtual void Draw();	//毎フレーム行う描画処理
	virtual void End();	//シーンを抜けるときの処理

	//ゲーム終了のフラグ取得する
	virtual bool& GetClose() { return m_isClose; }
	virtual void SetClose(bool close) { m_isClose = close; }


private:
	std::shared_ptr<SceneBase> m_pScene;

	//ゲームを終了させるフラグ
	bool m_isClose = false;

};


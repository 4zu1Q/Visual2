#pragma once
#include <memory>
#include "SceneBase.h"

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

	/// <summary>
	/// メインwhile文を終了させる
	/// </summary>
	/// <returns>ゲームを終了させるか true:終了 false:終了しない</returns>
	bool Close() const { return m_isClose; };

	// 終了フラグを変更する(終了するか)
	void SetCloseFlag(const bool flag) { m_isClose = flag; }

private:
	std::shared_ptr<SceneBase> m_pScene;

	bool m_isClose = false;		//終了フラグ

};


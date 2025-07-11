#pragma once
#include <list>
#include <memory>
#include <windows.h>

class SceneBase;

class SceneManager
{
public:
	SceneManager();
	virtual ~SceneManager();

	/// <summary>
	/// 持っているシーンのUpdate関数を呼び出す
	/// </summary>
	void Update();	//毎フレーム行う更新処理

	/// <summary>
	/// 持っているシーンのDraw関数を呼び出す
	/// </summary>
	void Draw();	//毎フレーム行う描画処理

	/// <summary>
	/// 実行中のシーンを引数で指定したシーンに切り替える
	/// </summary>
	/// <param name="nextScene"></param>
	void ChangeScene(std::shared_ptr<SceneBase> nextScene);

	/// <summary>
	/// Sceneをクリアしてシーンを切り替える
	/// </summary>
	/// <param name="nextScene"></param>
	void ChangeAndClearScene(std::shared_ptr<SceneBase> nextScene);

	/// <summary>
	/// 現在先頭で実行中のシーンの上にシーンを乗っける
	/// </summary>
	/// <param name="scene"></param>
	void PushScene(std::shared_ptr<SceneBase> scene);

	/// <summary>
	/// 現在の先頭シーンを削除する
	/// </summary>
	void PopScene(bool isToTitle = false);

private:

	/// <summary>
	/// デバッグ用の描画
	/// </summary>
	void DrawDebug();

private:

	std::list<std::shared_ptr<SceneBase>> m_scenes;

	//シーンのスマートポインタ
	std::shared_ptr<SceneBase> m_pScene;

	float m_updateTime;	//更新にかかる時間
	float m_drawTime;	//描画にかかる時間
};


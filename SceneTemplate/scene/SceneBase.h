#pragma once
#include <memory>


class SceneBase :public std::enable_shared_from_this<SceneBase>
{
public:
	SceneBase() {}
	virtual ~SceneBase() {}

	
	virtual void Init() = 0;	//シーンに入るときの初期化処理
	virtual void End() = 0;		//シーンを抜けるときの処理

														//毎フレーム行う更新処理
	virtual std::shared_ptr<SceneBase> Update() = 0;	//シーンを変更する場合は遷移先のシーンのポインタ
														//シーン変更しない場合は自身のポインタ
										
	virtual void Draw() = 0;	//毎フレーム行う描画処理

	/*フェード関連*/
	//void UpdateFade();
	//void DrawFade() const;


	//シーンの種類
	enum SceneKind
	{
		kSceneDebug,		//デバッグシーン
		kSceneTitle,		//タイトルシーン
		kSceneSelect,		//セレクトシーン
		kSceneGamePlay,		//ゲームプレイシーン
		kSceneGameOver,		//ゲームオーバーシーン
		kSceneGameClear,	//ゲームクリアシーン
	};

	SceneKind m_sceneKind;

private:

	//フェード関連処理
	int m_fadeColor;
	int m_fadeBright;
	int m_fadeSpeed;

};


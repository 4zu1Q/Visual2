#pragma once

// ゲームシーン基底クラス
class SceneBase
{
public:
	SceneBase();
	virtual ~SceneBase() {}

public:
	// 派生クラスに継承する必要のある処理
	virtual void startLoad() abstract;	// リソースのロード開始
	virtual bool isLoaded() const abstract;	// リソースのロード完了判定

	virtual void init() abstract;	// 初期化	リソースのロード完了後に呼ばれる
	virtual void end() abstract;		// シーン終了時に呼ばれる	リソースの解放はここ

	virtual void update() abstract;	// リソースのロード完了以降呼ばれる	フェード中も呼ばれる
	virtual void draw() abstract;

	// フェードよりも前面に表示
	virtual void drawFront() {}
#ifdef _DEBUG
	// デバッグ表示(最前面表示)
	virtual void drawDebug() {}
#endif
	// 現在のロード進捗を取得	0.0~1.0	不明の場合は負の値を返す
	virtual float getLoadProgress() const { return -1.0f; }

protected:
	// 派生クラス作成に利用する処理

	// 初期化終了判定
	bool isInitialized() const { return m_isInit; }

	// ロード中表示を行うかどうか設定
	// ロードに1秒程度以上時間がかかる場合はtrue推奨
	void setDispLoading(bool isDisp) { m_isDispLoading = isDisp; }

	// フェードにかかるフレーム数を設定
	// 設定を行わなかった場合はデフォルトのフレーム数になる
	// フェードインはデフォルトスピードにして、シーン終了前に別のフレーム数を設定することも可能
	void setFadeFrame(int frame) { m_fadeTotalFrame = frame; }

	// フェード中判定
	bool isFadingIn() const;	// フェードイン中
	bool isFadingOut() const;	// フェードアウト中
	bool isFading() const { return isFadingIn() || isFadingOut(); }	// フェードインorアウト中

	// 現在のフェード度合い	0.0:完全に画面見えている	1.0:画面見えていない
	float getFadeProgress() const;

public:
	// 次のシーンに遷移する
	// 同時にフェードイン開始する
	// フェード中には呼ばないように
	// nullptr指定でゲーム終了
	void setNextScene(SceneBase* pNext);
public:
	// 継承を行わない処理	SceneManagerから呼び出すのはこっち
	void initAll();	// シーン個別のinit()の他共通で必要な初期化を行う	
					// これはロード完了後の初回update()で呼ぶのでSceneManager()からは呼ばない
	void endAll();	// シーン個別のend()の他共通で必要な初期化を行う

	void updateAll();	// シーン個別のupdate()の他共通で必要な初期化を行う
	void drawAll();		// シーン個別のdraw()の他共通で必要な初期化を行う

	bool isSceneEnd();	// 現在のシーンを終了して次のシーンに遷移する
	SceneBase* getNextScene() { return m_pNext; }	// 終了後の遷移先シーン取得	nullptrでゲーム終了

private:
	// フェード関連
	void updateFade();
	void drawFade() const;

	// ロード中表示
	void drawLoading() const;

	// マウスカーソル表示
	void drawCursor() const;

	void startFadeOut();	// フェードアウト開始

	// マウスカーソル表示設定
	void setDispCursor(bool isDisp) { m_isDispCursor = isDisp; }

private:
	// 初期化処理終了判定
	bool m_isInit;

	// フェード関連処理
	int m_fadeFrame;	// 0:フェードイン完了	=m_fadeTotalFrame:画面真っ暗
	int m_fadeTotalFrame;
	int m_fadeDir;

	// ロード中表示を行うか
	bool m_isDispLoading;
	// ロードフレーム数カウント
	int m_loadFrame;

	// カーソル表示
	bool m_isDispCursor;

	// シーン遷移
	bool m_isEnd;
	SceneBase* m_pNext;
};
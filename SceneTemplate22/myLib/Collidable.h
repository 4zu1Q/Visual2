#pragma once

#include <list> 

#include "myLib/ColliderData.h"

#include "myLib/Rigidbody.h"
#include "util/Game.h"

namespace MyLib {

class Rigidbody;
class Physics;
class ColliderData;

/// <summary>
/// 衝突できるもの。物理・衝突判定をする場合はこれを継承させる
/// </summary>
class Collidable abstract
{
public:

	// 位置補正の優先度の判別に使う
	enum class e_Priority : int
	{
		Low,		// 低
		High,		// 高
		Static,		// 動かない（最高）
	};

	Collidable(e_Priority priority, Game::e_GameObjectTag tag, ColliderData::e_Kind colliderKind, bool isTrigger);	// コンストラクタ

	virtual ~Collidable();													// デストラクタ
	virtual void Initialize(MyLib::Physics* physics);
	virtual void Finalize(MyLib::Physics* physics);

	virtual void OnCollide(const Collidable& colider) abstract;				// 衝突したとき

	Game::e_GameObjectTag GetTag()	const { return tag; }					// タグ情報
	e_Priority GetPriority()			const { return priority; }				// 優先度
	Rigidbody GetRigidbody()		const { return rigidbody; }				// 優先度

	// 当たり判定を無視（スルー）するタグの追加/削除
	void AddThroughTag(Game::e_GameObjectTag tag);
	void RemoveThroughTag(Game::e_GameObjectTag tag);

	// 当たり判定を無視（スルー）する対象かどうか
	bool IsThroughTarget(const Collidable* target) const;

protected:
	Rigidbody		rigidbody;		// 物理データ
	ColliderData* colliderData;	// 当たり判定データ

private:
	ColliderData* CreateColliderData(ColliderData::e_Kind kind, bool isTrigger);

	Game::e_GameObjectTag	tag;
	e_Priority		priority;

	// 当たり判定を無視（スルー）するタグのリスト
	// HACK: Unityみたいにレイヤーの追加の方がスマート
	// FIXME: 本来ColliderData側じゃない？
	std::list<Game::e_GameObjectTag>	throughTags;

	// PhysicsがCollidableを自由に管理するためにフレンド化
	friend Physics;

	// 以下、フレンドであるPhisicsのみが扱う型や変数

private:

	// FIXME: これはPhysicsが持つ方が望ましい
	VECTOR		nextPos;
};

}


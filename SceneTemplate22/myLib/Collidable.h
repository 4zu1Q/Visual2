#pragma once

#include <memory>
#include <List>

#include "myLib/ColliderData.h"

#include "myLib/Rigidbody.h"
#include "util/Game.h"

namespace MyLib 
{

	class Rigidbody;
	class Physics;
	class ColliderData;

	/// <summary>
	/// 衝突できるもの。物理・衝突判定をする場合はこれを継承させる
	/// </summary>
	class Collidable abstract : std::enable_shared_from_this<Collidable>
	{
	public:

		// 位置補正の優先度の判別に使う
		enum class e_Priority : int
		{
			kLow,		// 低
			kMidle,		// 中
			kHigh,		// 高
			kStatic,		// 動かない（最高）
		};

		Collidable(e_Priority priority, Game::e_GameObjectTag tag, ColliderData::e_Kind colliderKind, bool isTrigger);	// コンストラクタ

		virtual ~Collidable();													// デストラクタ
		virtual void Initialize(std::shared_ptr<MyLib::Physics> physics/*MyLib::Physics* physics*/);
		virtual void Finalize(std::shared_ptr<MyLib::Physics> physics/*MyLib::Physics* physics*/);

		virtual void OnCollide(const Collidable& colider) abstract;				// 衝突したとき

		Game::e_GameObjectTag GetTag() const { return m_tag; }					// タグ情報
		e_Priority GetPriority() const { return m_priority; }				// 優先度
		Rigidbody GetRigidbody() const { return m_rigidbody; }				// 優先度

		// 当たり判定を無視（スルー）するタグの追加/削除
		void AddThroughTag(Game::e_GameObjectTag tag);
		void RemoveThroughTag(Game::e_GameObjectTag tag);

		// 当たり判定を無視（スルー）する対象かどうか
		bool IsThroughTarget(const Collidable* target) const;

	private:

		std::shared_ptr<ColliderData> CreateColliderData(ColliderData::e_Kind kind, bool isTrigger);

	protected:
		Rigidbody m_rigidbody;		// 物理データ
		std::shared_ptr<ColliderData> m_pColliderData;	// 当たり判定データ
		//std::list<std::shared_ptr<ColliderData>> m_colliders;

		//std::list<Collidable*> m_collidables;	// 登録されたCollidableのリスト


		Game::e_GameObjectTag m_tag;
		e_Priority m_priority;

		// 当たり判定を無視（スルー）するタグのリスト
		std::list<Game::e_GameObjectTag> m_throughTags;

		// PhysicsがCollidableを自由に管理するためにフレンド化
		friend Physics;

	private:

		VECTOR m_nextPos;
	};

}


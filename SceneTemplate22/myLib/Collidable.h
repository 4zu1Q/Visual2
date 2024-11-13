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
	class Collidable abstract : public std::enable_shared_from_this<Collidable>
	{

		// PhysicsがCollidableを自由に管理するためにフレンド化
		friend Physics;

	public:

		// 位置補正の優先度の判別に使う
		enum class e_Priority : int
		{
			kLow,		// 低
			kMidle,		// 中
			kHigh,		// 高
			kStatic,	// 動かない（最高）
		};

		// コンストラクタ
		Collidable(e_Priority priority, Game::e_GameObjectTag tag, ColliderData::e_Kind colliderKind, bool isTrigger);
		//デストラクタ
		virtual ~Collidable();	

		//初期化
		virtual void Initialize(std::shared_ptr<MyLib::Physics> physics);
		//終了化
		virtual void Finalize(std::shared_ptr<MyLib::Physics> physics);

		//// 衝突したとき
		//virtual void OnCollide(const Collidable& colider) abstract;

		// タグ情報
		Game::e_GameObjectTag GetTag() const { return m_tag; }
		// 優先度
		e_Priority GetPriority() const { return m_priority; }		
		// 物理
		Rigidbody GetRigidbody() const { return m_rigidbody; }				

		// 当たり判定を無視（スルー）するタグの追加/削除
		void AddThroughTag(Game::e_GameObjectTag tag);
		void RemoveThroughTag(Game::e_GameObjectTag tag);

		// 当たり判定を無視（スルー）する対象かどうか
		bool IsThroughTarget(const Collidable* target) const;


		//virtual void OnCollide(const std::shared_ptr<Collidable>& colider);

	private:

		//当たり判定データの作成
		std::shared_ptr<ColliderData> CreateColliderData(ColliderData::e_Kind kind, bool isTrigger);

	protected:

		// 物理データ
		Rigidbody m_rigidbody;		
		// 当たり判定データ
		std::shared_ptr<ColliderData> m_pColliderData;	

		//タグ
		Game::e_GameObjectTag m_tag;
		//優先度
		e_Priority m_priority;

		// 当たり判定を無視（スルー）するタグのリスト
		std::list<Game::e_GameObjectTag> m_throughTags;

	};

}


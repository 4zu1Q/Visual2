// 2024 Takeru Yui All Rights Reserved.
#include <cassert> 
#include "DxLib.h"
#include "MyLib.h"

using namespace MyLib;

/// <summary>
/// コンストラクタ
/// </summary>
Collidable::Collidable(e_Priority priority, Game::e_GameObjectTag tag, ColliderData::e_Kind colliderKind, bool isTrigger):
	m_priority(priority),
	m_tag(tag),
	m_colliderData(nullptr),
	m_nextPos(VGet(0,0,0))
{
	CreateColliderData(colliderKind, isTrigger);
}

/// <summary>
/// デストラクタ
/// </summary>
Collidable::~Collidable()
{
	if (m_colliderData != nullptr)
	{
		delete m_colliderData;
		m_colliderData = nullptr;
	}
}

/// <summary>
/// 初期化
/// </summary>
void Collidable::Initialize(MyLib::Physics* physics)
{
	physics->Entry(this);	// 物理情報に自身を登録
}

/// <summary>
/// 終了
/// </summary>
void Collidable::Finalize(MyLib::Physics* physics)
{
	physics->Exit(this);	// 物理情報登録解除
}

/// <summary>
/// 当たり判定を無視（スルー）するタグの追加
/// </summary>
void Collidable::AddThroughTag(Game::e_GameObjectTag tag)
{

	//当たり判定を無視するタグのリストに追加予定のタグが存在するかを確認
	bool found = (std::find(m_throughTags.begin(), m_throughTags.end(), tag) != m_throughTags.end());

	//リストの中に既に存在していたら警告を出す
	if (found)
	{
		assert(0 && "指定タグは既に追加されています");
	}
	//存在していなかったら追加する
	else
	{
		m_throughTags.emplace_back(tag);
	}
}

/// <summary>
/// 当たり判定を無視（スルー）するタグの削除
/// </summary>
void Collidable::RemoveThroughTag(Game::e_GameObjectTag tag)
{
	//当たり判定を無視するタグのリストに追加予定のタグが存在するかを確認
	bool found = (std::find(m_throughTags.begin(), m_throughTags.end(), tag) != m_throughTags.end());

	//存在していなかったら警告を出す
	if (!found)
	{
		assert(0 && "指定タグは存在しません");
	}
	//存在していたら削除する
	else
	{
		m_throughTags.remove(tag);
	}
}

// 当たり判定を無視（スルー）する対象かどうか
bool Collidable::IsThroughTarget(const Collidable* target) const
{
	//確認したい当たり判定のタグが無視するタグのリストに含まれているかどうか調べる
	bool found = (std::find(m_throughTags.begin(), m_throughTags.end(), target->GetTag()) != m_throughTags.end());
	return found;
}

/// <summary>
/// 当たり判定データの作成
/// </summary>
ColliderData* Collidable::CreateColliderData(ColliderData::e_Kind kind, bool isTrigger)
{
	if (m_colliderData != nullptr)
	{
		assert(0 && "colliderDataは既に作られています。");
		return m_colliderData;
	}
	switch (kind)
	{
	case ColliderData::e_Kind::kSphere:
		m_colliderData = new ColliderDataSphere(isTrigger);
		break;
	case ColliderData::e_Kind::kCapsule:
		m_colliderData = new ColliderDataCapsule(isTrigger);
		break;
	case ColliderData::e_Kind::kLine:
		m_colliderData = new ColliderDataLine(isTrigger);
		break;
	default:
		assert(0 && "colliderData作成に失敗。");
		break;
	}
	return m_colliderData;
}

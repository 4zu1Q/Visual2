#pragma once
#include "DxLib.h"
#include <memory>

class Player;
class BossBase;

class AttackCol
{
public:
    AttackCol();
    ~AttackCol();

    void Initialize(float radius, VECTOR pos);
    void Update(VECTOR pos);
    bool CheckCollisionWithPlayer(const std::shared_ptr<Player>& player);
    bool CheckCollisionWithBoss(const std::shared_ptr<BossBase>& boss);
    void Draw();

private:

    float m_radius;
    VECTOR m_pos;

};


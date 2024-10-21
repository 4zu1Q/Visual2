#pragma once
#include "BossBase.h"
class BossShot :
    public BossBase
{
public:
    BossShot();
    virtual ~BossShot();

    void Initialize();
    void Finalize();

    void Update();
    void Draw();

private:



};


//
// Created by leafnsand on 2015/5/23.
//

#ifndef XGAME2D_TESTSPRITE_H
#define XGAME2D_TESTSPRITE_H

#include <xGame2D.h>

using namespace xGame2D;

class TestSprite : public Sprite
{
constructor:
    TestSprite();
    virtual ~TestSprite();

public:
    virtual bool init() override;
};


#endif //XGAME2D_TESTSPRITE_H

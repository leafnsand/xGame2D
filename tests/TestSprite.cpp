//
// Created by leafnsand on 2015/5/23.
//

#include "TestSprite.h"

using namespace xGame2D;

TestSprite::TestSprite()
{
}

TestSprite::~TestSprite()
{
}

bool TestSprite::init()
{
    if (Sprite::init())
    {
        std::string name = "resources/mario.png";
        auto image = Object::create<Image>(name);
        addChild(image);
        return true;
    }
    return false;
}

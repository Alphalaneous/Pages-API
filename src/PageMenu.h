#pragma once

#ifndef MENUPAGE_H
#define MENUPAGE_H

#include <Geode/Geode.hpp>
using namespace geode::prelude;

class PageMenu : public CCMenu {

private:
    int page = 0;
    CCMenuItemSpriteExtra* nextButton;
    CCMenuItemSpriteExtra* prevButton;
    CCArray* pages;

public:
    ~PageMenu();
    static PageMenu* create(CCMenu* menu, int elementCount, Layout* layout,  CCSize innerSize, bool forceContentSize);
    bool init(CCMenu* menu, int elementCount, Layout* layout,  CCSize innerSize, bool forceContentSize);
    void goRight(CCObject* obj);
    void goLeft(CCObject* obj);
    void setPageVisible();
};

#endif
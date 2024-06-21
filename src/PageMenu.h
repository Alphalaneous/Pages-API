#pragma once

#ifndef MENUPAGE_H
#define MENUPAGE_H

#include <Geode/Geode.hpp>
using namespace geode::prelude;

class PageMenu : public CCMenu {

private:
    int m_page = 0;
    CCMenuItemSpriteExtra* m_nextButton;
    CCMenuItemSpriteExtra* m_prevButton;
    CCArray* m_pages;

public:
    ~PageMenu();
    static PageMenu* create(CCMenu* menu, int elementCount, Layout* layout, bool forceContentSize = false);
    bool init(CCMenu* menu, int elementCount, Layout* layout, bool forceContentSize);
    void goRight(CCObject* obj);
    void goLeft(CCObject* obj);
    void setPageVisible();
    void scaleAtMax();
};

#endif
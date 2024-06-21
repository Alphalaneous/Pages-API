#pragma once

#ifndef MENUPAGE_H
#define MENUPAGE_H

#include <Geode/Geode.hpp>
using namespace geode::prelude;

enum PageOrientation{ VERTICAL, HORIZONTAL };

class PageMenu : public CCMenu {

private:
    int m_page = 0;
    int m_maxCount = 0;
    float m_navGap = 15;
    CCMenu* m_originalMenu;
    PageOrientation m_pageOrientation = PageOrientation::HORIZONTAL;

public:

    Layout* m_layout;
    CCNode* m_innerNode;
    CCMenuItemSpriteExtra* m_nextButton;
    CCMenuItemSpriteExtra* m_prevButton;
    CCSprite* m_nextSprite;
    CCSprite* m_prevSprite;
    CCArray* m_pages;

    ~PageMenu();
    static PageMenu* create(CCMenu* menu, int elementCount, bool forceContentSize = false);
    bool init(CCMenu* menu, int elementCount, bool forceContentSize);
    int getPageCount();
    float getNavGap();
    PageOrientation getPageOrientation();
    void goRight(CCObject* obj);
    void goLeft(CCObject* obj);
    void setPageVisible();
    void scaleWhenFull();
    void setNavGap(float gapSize);
    void setOrientation(PageOrientation orientation);
    void setPageLayout(Layout* layout);
    void addPagedChild(CCNode* child);
    CCMenu* createPage();
};

#endif
#pragma once

#ifndef MENUPAGE_H
#define MENUPAGE_H

#include <Geode/Geode.hpp>

#ifdef GEODE_IS_WINDOWS
    #ifdef ALPHALANEOUS_PAGES_API_EXPORTING
        #define PAGES_API_DLL __declspec(dllexport)
    #else
        #define PAGES_API_DLL __declspec(dllimport)
    #endif
    #else
        #define PAGES_API_DLL __attribute__((visibility("default")))
#endif

using namespace geode::prelude;

enum PAGES_API_DLL PageOrientation{ VERTICAL, HORIZONTAL };

class PAGES_API_DLL PageMenu : public CCMenu {

protected:
    void checkMenu(float dt);
    void checkAttributes(float dt);
private:
    int m_page = 0;
    int m_maxCount = 0;
    int m_lastVisibleCount;
    float m_navGap = 15;
    bool m_isEditing = false;
    bool m_isPage = true;
    bool m_forceContentSize = false;
    bool m_isUniformScale = false;
    CCMenu* m_originalMenu;
    CCMenu* m_navMenu;
    CCArray* m_children;
    PageOrientation m_pageOrientation = PageOrientation::HORIZONTAL;
    CCMenuItem* m_doneButton;
    CCScale9Sprite* m_buttonBG;
    CCLabelBMFont* m_doneLabel;
    bool m_finishedInit = false;
    bool m_forceScale = false;
    bool m_scaleWhenFull = false;
    float m_forcedScale = 0;
    CCDictionary* m_lastAttributes;
public:

    Layout* m_layout;
    CCNode* m_innerNode;
    CCMenuItemSpriteExtra* m_nextButton;
    CCMenuItemSpriteExtra* m_prevButton;
    CCSprite* m_nextSprite;
    CCSprite* m_prevSprite;
    CCScale9Sprite* m_background;
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
    void startShakeChildren();
    void stopShakeChildren();
    void startEditing();
    void stopEditing(CCObject* obj);
    void setUniformScale(bool isUniform);
    void updatePage();
    void setForceScale(bool force, float scale);
    CCMenu* createPage();
};

#endif
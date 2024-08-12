#pragma once

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

enum PageOrientation{ VERTICAL, HORIZONTAL };

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
    Ref<CCMenu> m_originalMenu;
    Ref<CCMenu> m_navMenu;
    Ref<CCArray> m_children;
    PageOrientation m_pageOrientation = PageOrientation::HORIZONTAL;
    Ref<CCMenuItem> m_doneButton;
    Ref<CCScale9Sprite> m_buttonBG;
    Ref<CCLabelBMFont> m_doneLabel;
    bool m_finishedInit = false;
    bool m_forceScale = false;
    bool m_scaleWhenFull = false;
    float m_forcedScale = 0;
    Ref<CCDictionary> m_lastAttributes;
public:

    Ref<Layout> m_layout;
    Ref<CCNode> m_innerNode;
    Ref<CCMenuItemSpriteExtra> m_nextButton;
    Ref<CCMenuItemSpriteExtra> m_prevButton;
    Ref<CCSprite> m_nextSprite;
    Ref<CCSprite> m_prevSprite;
    Ref<CCScale9Sprite> m_background;
    Ref<CCArray> m_pages;

    ~PageMenu();
    static PageMenu* create(CCMenu* menu, Layout* layout, int elementCount, bool forceContentSize = false);
    bool init(CCMenu* menu, Layout* layout, int elementCount, bool forceContentSize);
    int getPageCount();
    float getNavGap();
    CCArray* getPagedChildren();
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
    void setElementCount(int count);
    void setArrowScale(float scale);
    float getTotalWidth();
    CCMenu* createPage();
};
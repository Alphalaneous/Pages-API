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

enum PageOrientation{ VERTICAL, HORIZONTAL };

class PAGES_API_DLL PageMenu : public cocos2d::CCMenu {

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
    cocos2d::CCMenu* m_originalMenu; //should always be in the scene tree, NEVER REMOVE unless you want crashes
    geode::Ref<cocos2d::CCMenu> m_navMenu;
    geode::Ref<cocos2d::CCArray> m_children;
    PageOrientation m_pageOrientation = PageOrientation::HORIZONTAL;
    geode::Ref<cocos2d::CCMenuItem> m_doneButton;
    geode::Ref<cocos2d::extension::CCScale9Sprite> m_buttonBG;
    geode::Ref<cocos2d::CCLabelBMFont> m_doneLabel;
    bool m_finishedInit = false;
    bool m_forceScale = false;
    bool m_scaleWhenFull = false;
    bool m_origAutoScale = true;
    bool m_origIgnoreInvisible = true;
    float m_forcedScale = 0;
    geode::Ref<cocos2d::CCDictionary> m_lastAttributes;
public:

    geode::Ref<cocos2d::Layout> m_layout;
    geode::Ref<cocos2d::CCNode> m_innerNode;
    geode::Ref<CCMenuItemSpriteExtra> m_nextButton;
    geode::Ref<CCMenuItemSpriteExtra> m_prevButton;
    geode::Ref<cocos2d::CCSprite> m_nextSprite;
    geode::Ref<cocos2d::CCSprite> m_prevSprite;
    geode::Ref<cocos2d::extension::CCScale9Sprite> m_background;
    geode::Ref<cocos2d::CCArray> m_pages;

    ~PageMenu();
    static PageMenu* create(cocos2d::CCMenu* menu, cocos2d::Layout* layout, int elementCount, bool forceContentSize = false);
    bool init(cocos2d::CCMenu* menu, cocos2d::Layout* layout, int elementCount, bool forceContentSize);
    int getPageCount();
    float getNavGap();
    cocos2d::CCArray* getPagedChildren();
    PageOrientation getPageOrientation();
    void goRight(cocos2d::CCObject* obj);
    void goLeft(cocos2d::CCObject* obj);
    void setPageVisible();
    void scaleWhenFull();
    void setNavGap(float gapSize);
    void setOrientation(PageOrientation orientation);
    void setPageLayout(cocos2d::Layout* layout);
    void addPagedChild(cocos2d::CCNode* child);
    void startShakeChildren();
    void stopShakeChildren();
    void startEditing();
    void stopEditing(cocos2d::CCObject* obj);
    void setUniformScale(bool isUniform);
    void updatePage();
    void setForceScale(bool force, float scale);
    void setElementCount(int count);
    void setArrowScale(float scale);
    void disablePages(cocos2d::CCObject* obj);
    float getTotalWidth();
    cocos2d::CCMenu* createPage();
};
#pragma once

#include <Geode/cocos/cocoa/CCObject.h>
#include <Geode/cocos/menu_nodes/CCMenu.h>
#include <Geode/utils/cocos.hpp>
#include <Geode/modify/CCMenu.hpp>
#include "PagesAPI.h"

#ifdef GEODE_IS_WINDOWS
    #ifdef ALPHALANEOUS_PAGES_API_EXPORTING
        #define PAGES_API_DLL __declspec(dllexport)
    #else
        #define PAGES_API_DLL __declspec(dllimport)
    #endif
#else
    #define PAGES_API_DLL __attribute__((visibility("default")))
#endif

struct PAGES_API_DLL PageMenu : public geode::Modify<PageMenu, cocos2d::CCMenu> {

public:
    struct Fields {
        int m_elementCount = 0;
        int m_currentPage = 0;
        int m_pageCount = 1;
        geode::Ref<cocos2d::CCArray> m_pages;
        PageOrientation m_orientation;
        cocos2d::CCMenu* m_arrowsMenu;
        bool m_paged = false;
        float m_max;
        float m_padding;
        float m_buttonScale = 0.75;
        float m_lastChildrenCount;
        float m_buttonWidth;

        std::function<void(CCObject*)> m_nextCallback = [](CCObject* obj) {};
        std::function<void(CCObject*)> m_prevCallback = [](CCObject* obj) {};
    };
    void setPaged(int count, PageOrientation orientation, float max, float padding = 4);
    void setPage(int pageNum);
    void checkChildren(float dt);
    void setElementCount(int count);
    void setOrientation(PageOrientation orientation);
    void setMax(float max);
    void enablePages(bool enable);
    void setButtonScale(float scale);
    void setFixed(float max);

    void setNextCallback(std::function<void(CCObject*)> callback);
    void setPrevCallback(std::function<void(CCObject*)> callback);

private:
    void nextPage(cocos2d::CCObject* obj);
    void prevPage(cocos2d::CCObject* obj);

protected:
    void addArrowButtons();
};

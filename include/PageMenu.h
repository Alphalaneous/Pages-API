#pragma once

#include <Geode/Geode.hpp>
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
    };
    void setPaged(int count, PageOrientation orientation, float max, float padding = 4);
    void setPage(int pageNum);
    void addChild(cocos2d::CCNode* child, int zOrder, int tag);
    void removeChild(cocos2d::CCNode* child, bool cleanup);
    void setElementCount(int count);
    void setOrientation(PageOrientation orientation);
    void setMax(float max);
    void enablePages(bool enable);
    void setButtonScale(float scale);

private:
    void nextPage(cocos2d::CCObject* obj);
    void prevPage(cocos2d::CCObject* obj);

protected:
    void addArrowButtons();
};
#pragma once

#include <Geode/cocos/cocoa/CCObject.h>
#include <Geode/cocos/cocoa/CCInteger.h>
#include <Geode/cocos/cocoa/CCFloat.h>
#include <Geode/cocos/actions/CCActionInstant.h>
#include <Geode/cocos/menu_nodes/CCMenu.h>

enum PageOrientation{ VERTICAL, HORIZONTAL };

namespace PagesAPI {

    inline bool isLoaded() {
        return geode::Loader::get()->isModLoaded("alphalaneous.pages_api");
    }

    inline void setElementCount(cocos2d::CCMenu* menu, int count) {
        if (cocos2d::CCActionInstant* func = static_cast<cocos2d::CCActionInstant*>(menu->getUserObject("alphalaneous.pages_api/element-count-callback"))) {
            menu->setUserObject("alphalaneous.pages_api/element-count", cocos2d::CCInteger::create(count));
            func->update(0);
        }
    }

    inline void setOrientation(cocos2d::CCMenu* menu, PageOrientation orientation) {
        if (cocos2d::CCActionInstant* func = static_cast<cocos2d::CCActionInstant*>(menu->getUserObject("alphalaneous.pages_api/orientation-callback"))) {
            menu->setUserObject("alphalaneous.pages_api/orientation", cocos2d::CCInteger::create((int) orientation));
            func->update(0);
        }
    }

    inline void setMax(cocos2d::CCMenu* menu, float max) {
        if (cocos2d::CCActionInstant* func = static_cast<cocos2d::CCActionInstant*>(menu->getUserObject("alphalaneous.pages_api/max-callback"))) {
            menu->setUserObject("alphalaneous.pages_api/max", cocos2d::CCFloat::create(max));
            func->update(0);
        }
    }

    inline void setButtonScale(cocos2d::CCMenu* menu, float scale) {
        if (cocos2d::CCActionInstant* func = static_cast<cocos2d::CCActionInstant*>(menu->getUserObject("alphalaneous.pages_api/button-scale-callback"))) {
            menu->setUserObject("alphalaneous.pages_api/button-scale", cocos2d::CCFloat::create(scale));
            func->update(0);
        }
    }

    inline void enablePages(cocos2d::CCMenu* menu, bool enable) {
        if (cocos2d::CCActionInstant* func = static_cast<cocos2d::CCActionInstant*>(menu->getUserObject("alphalaneous.pages_api/enable-callback"))) {
            menu->setUserObject("alphalaneous.pages_api/enable", cocos2d::CCBool::create(enable));
            func->update(0);
        }
    }
}
#pragma once

#include <Geode/Geode.hpp>

enum PageOrientation{ VERTICAL, HORIZONTAL };

namespace PagesAPI {

    inline bool isLoaded() {
        return geode::Loader::get()->isModLoaded("alphalaneous.pages_api");
    }

    inline void setElementCount(cocos2d::CCMenu* menu, int count) {
        if (cocos2d::CCActionInstant* func = static_cast<cocos2d::CCActionInstant*>(menu->getUserObject("element-count-callback"_spr))) {
            menu->setUserObject("element-count"_spr, cocos2d::CCInteger::create(count));
            func->update(0);
        }
    }

    inline void setOrientation(cocos2d::CCMenu* menu, PageOrientation orientation) {
        if (cocos2d::CCActionInstant* func = static_cast<cocos2d::CCActionInstant*>(menu->getUserObject("orientation-callback"_spr))) {
            menu->setUserObject("orientation"_spr, cocos2d::CCInteger::create((int) orientation));
            func->update(0);
        }
    }

    inline void setMax(cocos2d::CCMenu* menu, float max) {
        if (cocos2d::CCActionInstant* func = static_cast<cocos2d::CCActionInstant*>(menu->getUserObject("max-callback"_spr))) {
            menu->setUserObject("max"_spr, cocos2d::CCFloat::create(max));
            func->update(0);
        }
    }

    inline void setButtonScale(cocos2d::CCMenu* menu, float scale) {
        if (cocos2d::CCActionInstant* func = static_cast<cocos2d::CCActionInstant*>(menu->getUserObject("button-scale-callback"_spr))) {
            menu->setUserObject("button-scale"_spr, cocos2d::CCFloat::create(scale));
            func->update(0);
        }
    }

    inline void enablePages(cocos2d::CCMenu* menu, bool enable) {
        if (cocos2d::CCActionInstant* func = static_cast<cocos2d::CCActionInstant*>(menu->getUserObject("enable-callback"_spr))) {
            menu->setUserObject("enable"_spr, cocos2d::CCBool::create(enable));
            func->update(0);
        }
    }
}
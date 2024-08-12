#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include "../../include/PageMenu.h"
#include "../Macros.h"

using namespace geode::prelude;

class $modify(PageMenuLayer, MenuLayer) {

    LATE_MODIFY(MenuLayer::init);

    bool init(){
        if (!MenuLayer::init()) {
            return false;
        }

        if(Mod::get()->getSettingValue<bool>("menulayer-bottom-menu")){
            auto bottomMenu = getChildByID("bottom-menu");

            PageMenu* menuPage = PageMenu::create(typeinfo_cast<CCMenu*>(bottomMenu), bottomMenu->getLayout(), 6);
            menuPage->scaleWhenFull();
            menuPage->setUniformScale(true);
            addChild(menuPage);
        }

        if(Mod::get()->getSettingValue<bool>("menulayer-right-menu")){
            auto rightMenu = getChildByID("right-side-menu");

            PageMenu* menuPageR = PageMenu::create(typeinfo_cast<CCMenu*>(rightMenu), rightMenu->getLayout(), 3);
            menuPageR->setNavGap(2);
            menuPageR->setOrientation(PageOrientation::VERTICAL);
            menuPageR->scaleWhenFull();

            addChild(menuPageR);
        }

        return true;
    }
};
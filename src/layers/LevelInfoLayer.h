#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include "../../include/PageMenu.h"
#include "../Macros.h"

using namespace geode::prelude;

class $modify(PageLevelInfoLayer, LevelInfoLayer) {

    LATE_MODIFY(LevelInfoLayer::init);

    struct Fields {
        PageMenu* m_leftSidePage;
    };

    bool init(GJGameLevel* level, bool challenge){
        if (!LevelInfoLayer::init(level, challenge)) {
            return false;
        }

        if(Mod::get()->getSettingValue<bool>("levelinfolayer-left-menu")){
            auto leftSideMenu = getChildByID("left-side-menu");
            leftSideMenu->setContentSize({50, 150});
            leftSideMenu->setPosition({leftSideMenu->getPosition().x, leftSideMenu->getPosition().y + 20});

            m_fields->m_leftSidePage = PageMenu::create(typeinfo_cast<CCMenu*>(leftSideMenu), leftSideMenu->getLayout(), 3, false);
            m_fields->m_leftSidePage->setOrientation(PageOrientation::VERTICAL);
            m_fields->m_leftSidePage->setNavGap(0);
            m_fields->m_leftSidePage->setUniformScale(true);
            m_fields->m_leftSidePage->setForceScale(true, 0.78f);
            addChild(m_fields->m_leftSidePage);
        }

        return true;
    }
};
#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>
#include "../../include/PageMenu.h"
#include "../Macros.h"

using namespace geode::prelude;

class $modify(PageEditorUI, EditorUI) {

    LATE_MODIFY(EditorUI::init);

    bool init(LevelEditorLayer* editorLayer){
        if (!EditorUI::init(editorLayer)) {
            return false;
        }

        if(Mod::get()->getSettingValue<bool>("editor-build-tabs-menu")){
            auto buildTabsMenu = getChildByID("build-tabs-menu");
            auto toolbarTogglesMenu = getChildByID("toolbar-toggles-menu");
            buildTabsMenu->setPositionY(toolbarTogglesMenu->getScaledContentSize().height);
            buildTabsMenu->setContentSize({480, 17.5});
            buildTabsMenu->setAnchorPoint({0.5, 0});
            CCSize winSize = CCDirector::get()->getWinSize();

            PageMenu* menuPage = PageMenu::create(typeinfo_cast<CCMenu*>(buildTabsMenu), buildTabsMenu->getLayout(), 14, true);
            
            float scaleFactor = winSize.width / menuPage->getTotalWidth();

            if(scaleFactor < 1) {
                buildTabsMenu->setScale(scaleFactor);
            }

            menuPage->setArrowScale(0.70f);
            addChild(menuPage);
        }

        return true;
    }
};

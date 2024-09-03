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
            createPageForTabs("build-tabs-menu");
            createPageForTabs("edit-tabs-menu");
            createPageForTabs("delete-tabs-menu");
        }

        return true;
    }

    void createPageForTabs(std::string id) {
        if (auto tabsMenu = getChildByID(id.c_str())) {
            auto toolbarTogglesMenu = getChildByID("toolbar-toggles-menu");
            tabsMenu->setPositionY(toolbarTogglesMenu->getScaledContentSize().height);
            tabsMenu->setAnchorPoint({0.5, 0});
            CCSize winSize = CCDirector::get()->getWinSize();

            float scale = tabsMenu->getScale();

            float newWidth = 480 / scale;

            tabsMenu->setContentSize({newWidth, 17.5});

            PageMenu* menuPage = PageMenu::create(typeinfo_cast<CCMenu*>(tabsMenu), tabsMenu->getLayout(), std::ceil(14 / scale), true);
            
            float scaleFactor = winSize.width / menuPage->getTotalWidth();

            if(scaleFactor < 1) {
                tabsMenu->setScale(scaleFactor);
            }

            menuPage->setArrowScale(0.70f);
            addChild(menuPage);
        }
    }
};

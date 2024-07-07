#pragma once

#ifndef PAGE_LEVELINFOLAYER_H
#define PAGE_LEVELINFOLAYER_H

#include <Geode/Geode.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include "../../include/PageMenu.h"
#include "../Macros.h"

using namespace geode::prelude;

class $modify(PageLevelInfoLayer, LevelInfoLayer) {

	LATE_MODIFY(LevelInfoLayer::init);

	bool init(GJGameLevel* level, bool challenge){
		if (!LevelInfoLayer::init(level, challenge)) {
			return false;
		}

		if(Mod::get()->getSettingValue<bool>("levelinfolayer-left-menu")){
			auto leftSideMenu = getChildByID("left-side-menu");
			leftSideMenu->setContentSize({40, 120});
			PageMenu* menuPage = PageMenu::create(typeinfo_cast<CCMenu*>(leftSideMenu), 3, false);
            menuPage->setOrientation(PageOrientation::VERTICAL);
            menuPage->setUniformScale();
			menuPage->setPosition({menuPage->getPosition().x, menuPage->getPosition().y + 20});
			addChild(menuPage);
		}

		return true;
	}
};

#endif
#pragma once

#ifndef PAGE_CREATORLAYER_H
#define PAGE_CREATORLAYER_H

#include <Geode/Geode.hpp>
#include <Geode/modify/CreatorLayer.hpp>
#include "../../include/PageMenu.h"
#include "../Macros.h"

using namespace geode::prelude;

class $modify(PageCreatorLayer, CreatorLayer) {

	LATE_MODIFY(CreatorLayer::init);

	bool init(){
		if (!CreatorLayer::init()) {
			return false;
		}

		if(Mod::get()->getSettingValue<bool>("creator-layer-menu")){
			auto creatorButtonsMenu = getChildByID("creator-buttons-menu");

			PageMenu* menuPage = PageMenu::create(typeinfo_cast<CCMenu*>(creatorButtonsMenu), 15, true);
			addChild(menuPage);
		}

		return true;
	}
};

#endif
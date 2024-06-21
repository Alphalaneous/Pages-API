#pragma once

#ifndef PAGE_LEVELSEARCHLAYER_H
#define PAGE_LEVELSEARCHLAYER_H

#include <Geode/Geode.hpp>
#include <Geode/modify/LevelSearchLayer.hpp>
#include "../PageMenu.h"
#include "../Macros.h"

using namespace geode::prelude;

class $modify(PageLevelSearchLayer, LevelSearchLayer) {

	LATE_MODIFY(LevelSearchLayer::init);

	bool init(int isLists) {
		if (!LevelSearchLayer::init(isLists)) {
			return false;
		}
		
		RowLayout* layout = RowLayout::create();
        layout->setGrowCrossAxis(true);
        layout->setCrossAxisOverflow(false);
        layout->setAxisAlignment(AxisAlignment::Center);
        layout->setCrossAxisAlignment(AxisAlignment::Center);
		layout->ignoreInvisibleChildren(true);

		auto quickSearchMenu = getChildByID("quick-search-menu");
		quickSearchMenu->setContentSize({365, 116});
		quickSearchMenu->ignoreAnchorPointForPosition(false);
		quickSearchMenu->setPosition({quickSearchMenu->getPosition().x, quickSearchMenu->getPosition().y + 28});

		PageMenu* menuPage = PageMenu::create(typeinfo_cast<CCMenu*>(quickSearchMenu), 9, layout);

		addChild(menuPage);

		return true;
	}
};

#endif
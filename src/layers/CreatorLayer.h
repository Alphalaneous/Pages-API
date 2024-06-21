#pragma once

#ifndef PAGE_CREATORLAYER_H
#define PAGE_CREATORLAYER_H

#include <Geode/Geode.hpp>
#include <Geode/modify/CreatorLayer.hpp>
#include "../PageMenu.h"
#include "../Macros.h"

using namespace geode::prelude;

class $modify(PageCreatorLayer, CreatorLayer) {

	LATE_MODIFY(CreatorLayer::init);

	bool init(){
		if (!CreatorLayer::init()) {
			return false;
		}

		auto creatorButtonsMenu = getChildByID("creator-buttons-menu");

		RowLayout* layout = RowLayout::create();
        layout->setGrowCrossAxis(true);
        layout->setCrossAxisOverflow(false);
        layout->setAxisAlignment(AxisAlignment::Center);
        layout->setCrossAxisAlignment(AxisAlignment::Center);
        layout->setCrossAxisLineAlignment(AxisAlignment::Center);
        layout->setCrossAxisReverse(true);
		layout->setGap(6);
		layout->setAutoScale(true);
		layout->ignoreInvisibleChildren(true);

		PageMenu* menuPage = PageMenu::create(typeinfo_cast<CCMenu*>(creatorButtonsMenu), 15, layout, true);
		addChild(menuPage);

		return true;
	}
};

#endif
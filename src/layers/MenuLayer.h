#pragma once

#ifndef PAGE_MENULAYER_H
#define PAGE_MENULAYER_H

#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include "PageMenu.h"
#include "Macros.h"

using namespace geode::prelude;

class $modify(PageMenuLayer, MenuLayer) {

	LATE_MODIFY(MenuLayer::init);

	bool init(){
		if (!MenuLayer::init()) {
			return false;
		}

		auto bottomMenu = getChildByID("bottom-menu");

		int childrenCount = bottomMenu->getChildrenCount();

		RowLayout* layout = RowLayout::create();
        layout->setGrowCrossAxis(false);
        layout->setCrossAxisOverflow(true);
        layout->setAxisAlignment(AxisAlignment::Center);
        layout->setCrossAxisAlignment(AxisAlignment::Center);
        layout->setCrossAxisLineAlignment(AxisAlignment::Center);
        layout->setCrossAxisReverse(false);
		layout->setGap(5);
		layout->setAutoScale(true);
		layout->ignoreInvisibleChildren(true);

		PageMenu* menuPage = PageMenu::create(typeinfo_cast<CCMenu*>(bottomMenu), 6, layout);
		menuPage->setAnchorPoint({0.5, 0});
		menuPage->scaleAtMax();

		addChild(menuPage);

		return true;
	}
};

#endif
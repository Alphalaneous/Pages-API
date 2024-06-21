#pragma once

#ifndef PAGE_PAUSELAYER_H
#define PAGE_PAUSELAYER_H

#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include "../../include/PageMenu.h"
#include "../Macros.h"

using namespace geode::prelude;

class $modify(PagePauseLayer, PauseLayer) {

	LATE_MODIFY(PauseLayer::customSetup);

	void customSetup(){
		PauseLayer::customSetup();

		auto centerMenu = getChildByID("center-button-menu");
		centerMenu->ignoreAnchorPointForPosition(false);
		centerMenu->setContentSize({450, 85});

		int childrenCount = centerMenu->getChildrenCount();

		RowLayout* layout = RowLayout::create();
        layout->setGrowCrossAxis(true);
        layout->setCrossAxisOverflow(false);
        layout->setAxisAlignment(AxisAlignment::Center);
        layout->setCrossAxisAlignment(AxisAlignment::Center);
        layout->setCrossAxisLineAlignment(AxisAlignment::Center);
		layout->setGap(10);
		layout->ignoreInvisibleChildren(true);

		PageMenu* menuPage = PageMenu::create(typeinfo_cast<CCMenu*>(centerMenu), 6);
		menuPage->scaleWhenFull();
		menuPage->setPageLayout(layout);
		addChild(menuPage);
	}
};

#endif
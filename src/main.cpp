#include <Geode/Geode.hpp>
#include <Geode/modify/LevelSearchLayer.hpp>
#include <Geode/modify/CreatorLayer.hpp>
#include <Geode/utils/NodeIDs.hpp>
#include "PageMenu.h"

using namespace geode::prelude;
class $modify(LevelSearchLayer) {

	static void onModify(auto& self) {
        (void) self.setHookPriority("LevelSearchLayer::init", INT_MIN);
    }

	bool init(int isLists) {
		if (!LevelSearchLayer::init(isLists)) {
			return false;
		}
		
		RowLayout* layout = RowLayout::create();
        layout->setGrowCrossAxis(true);
        layout->setCrossAxisOverflow(false);
        layout->setAxisAlignment(AxisAlignment::Center);
        layout->setCrossAxisAlignment(AxisAlignment::Center);

		auto quickSearchMenu = getChildByID("quick-search-menu");
		quickSearchMenu->setContentSize({365, 116});
		quickSearchMenu->ignoreAnchorPointForPosition(false);
		quickSearchMenu->setPosition({quickSearchMenu->getPosition().x, quickSearchMenu->getPosition().y + 28});

		PageMenu* menuPage = PageMenu::create(typeinfo_cast<CCMenu*>(quickSearchMenu), 9, layout, {348, 102});

		addChild(menuPage);

		return true;
	}
};

class $modify(CreatorLayer) {

	static void onModify(auto& self) {
        (void) self.setHookPriority("CreatorLayer::init", INT_MIN);
    }

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

		PageMenu* menuPage = PageMenu::create(typeinfo_cast<CCMenu*>(creatorButtonsMenu), 15, layout, {450, 310});
		addChild(menuPage);

		return true;
	}
};
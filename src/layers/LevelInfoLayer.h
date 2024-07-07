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

	struct Fields {
		PageMenu* m_leftSidePage;
	};

	bool init(GJGameLevel* level, bool challenge){
		if (!LevelInfoLayer::init(level, challenge)) {
			return false;
		}

		if(Mod::get()->getSettingValue<bool>("levelinfolayer-left-menu")){
			auto leftSideMenu = getChildByID("left-side-menu");
			leftSideMenu->setContentSize({40, 120});
			m_fields->m_leftSidePage = PageMenu::create(typeinfo_cast<CCMenu*>(leftSideMenu), 3, false);
            m_fields->m_leftSidePage->setOrientation(PageOrientation::VERTICAL);
            m_fields->m_leftSidePage->setUniformScale(true);
			m_fields->m_leftSidePage->setPosition({m_fields->m_leftSidePage->getPosition().x, m_fields->m_leftSidePage->getPosition().y + 20});
			addChild(m_fields->m_leftSidePage);
		}

		return true;
	}

	void levelDownloadFinished(GJGameLevel* p0){
		LevelInfoLayer::levelDownloadFinished(p0);
		m_fields->m_leftSidePage->updatePage();
	}
};

#endif
#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/CCMenuItemSpriteExtra.hpp>
#include "../include/PageMenu.h"
#include "Macros.h"

using namespace geode::prelude;

class $modify(PageCCMenuItemSpriteExtra, CCMenuItemSpriteExtra) {

    LATE_MODIFY(CCMenuItemSpriteExtra::init);

    struct Fields {
        SEL_SCHEDULE m_holdSchedule;
        bool m_runActivation = true;
        float m_origRotation;
    };

    bool init(cocos2d::CCNode* p0, cocos2d::CCNode* p1, cocos2d::CCObject* p2, cocos2d::SEL_MenuHandler p3){
        if(!CCMenuItemSpriteExtra::init(p0, p1, p2, p3)) return false;

        m_fields->m_origRotation = getRotation();

        return true;
    }

    #ifdef IN_PROGRESS

    void activate() {
        if(m_fields->m_runActivation){
            CCMenuItemSpriteExtra::activate();
        }
    }

    void selected() {
        CCMenuItemSpriteExtra::selected();

        if(getUserObject("page-menu")){
            m_fields->m_holdSchedule = schedule_selector(PageCCMenuItemSpriteExtra::showMenuEditor);
            scheduleOnce(m_fields->m_holdSchedule, 1);
        }

    }

    #endif

    void unselected() {
        CCMenuItemSpriteExtra::unselected();

        if(getUserObject("page-menu")){
            if(m_fields->m_holdSchedule){
                unschedule(m_fields->m_holdSchedule);
            }
        }
    }

    void showMenuEditor(float dt) {
        if(PageMenu* menu = typeinfo_cast<PageMenu*>(getParent()->getParent())){
            menu->startEditing();
        }
    }

    void startShakeAnimation(){
        
        #ifdef IN_PROGRESS

        m_fields->m_runActivation = false;
        
        float rotationDegrees = 10.0f;

        CCRotateBy* rotate20left = CCRotateBy::create(0.1f, -rotationDegrees);
        CCRotateBy* rotate20Right = CCRotateBy::create(0.1f, rotationDegrees);

        setRotation(getRotation() + rotationDegrees/4);

        CCSequence* sequence = CCSequence::createWithTwoActions(rotate20left, rotate20Right);

        CCRepeatForever* repeat = CCRepeatForever::create(sequence);
        repeat->setTag(69);

        runAction(repeat);

        #endif
    }

    void stopShakeAnimation() {

        #ifdef IN_PROGRESS

        m_fields->m_runActivation = true;
        stopActionByTag(69);
        setRotation(m_fields->m_origRotation);

        #endif
    }
};

#include <Geode/Geode.hpp>
#include "../include/PageMenu.h"

using namespace geode::prelude;

void PageMenu::checkChildren(float dt) {
    auto fields = m_fields.self();
    if (!fields->m_paged) return;
    if (fields->m_lastChildrenCount != getChildrenCount()) {
        setPaged(fields->m_elementCount, fields->m_orientation, fields->m_max, fields->m_padding);
    }
    if (fields->m_paged && fields->m_arrowsMenu) {
        fields->m_arrowsMenu->setPosition(getPosition());
        fields->m_arrowsMenu->setVisible(isVisible());
        fields->m_arrowsMenu->ignoreAnchorPointForPosition(false);
        fields->m_arrowsMenu->setAnchorPoint(getAnchorPoint());
    }
}

void PageMenu::setElementCount(int count) {
    auto fields = m_fields.self();
    setPaged(count, fields->m_orientation, fields->m_max, fields->m_padding);
}

void PageMenu::setOrientation(PageOrientation orientation) {
    auto fields = m_fields.self();
    setPaged(fields->m_elementCount, orientation, fields->m_max, fields->m_padding);
}

void PageMenu::setMax(float max) {
    auto fields = m_fields.self();
    setPaged(fields->m_elementCount, fields->m_orientation, max, fields->m_padding);
}

void PageMenu::enablePages(bool enabled) {
    auto fields = m_fields.self();

    fields->m_paged = enabled;
    if (enabled) {
        setPaged(fields->m_elementCount, fields->m_orientation, fields->m_max, fields->m_padding);
    }
    else {
        for (CCNode* child : CCArrayExt<CCNode*>(getChildren())) {
            child->setVisible(true);
        }
        if (fields->m_arrowsMenu) {
            fields->m_arrowsMenu->removeFromParent();
        }
    }
}

void PageMenu::setButtonScale(float scale) {
    auto fields = m_fields.self();

    fields->m_buttonScale = scale;
    setPaged(fields->m_elementCount, fields->m_orientation, fields->m_max, fields->m_padding);
}

#define MAKE_CALLBACK(type, var, name, method)\
auto var = CallFuncExt::create([this]() {\
    if (type* val = static_cast<type*>(getUserObject(std::string(Mod::get()->expandSpriteName(name))))) {\
        method(val->getValue());\
    }\
});\
setUserObject(std::string(Mod::get()->expandSpriteName(name "-callback")), var);

void PageMenu::setPaged(int count, PageOrientation orientation, float max, float padding) {

    auto fields = m_fields.self();

    MAKE_CALLBACK(CCInteger, countCallback, "element-count", setElementCount);
    MAKE_CALLBACK(CCFloat, maxCallback, "max", setMax);
    MAKE_CALLBACK(CCFloat, buttonScaleCallback, "button-scale", setButtonScale);
    MAKE_CALLBACK(CCBool, enableCallback, "enable", enablePages);

    auto orientationCallback = CallFuncExt::create([this]() {
        if (CCInteger* orientation = static_cast<CCInteger*>(getUserObject("orientation"_spr))) {
            setOrientation((PageOrientation)orientation->getValue());
        }
    });
    setUserObject("orientation-callback"_spr, orientationCallback);

    if (fields->m_pages) {
        fields->m_pages->removeAllObjects();
    }
    else {
        fields->m_pages = CCArray::create();
    }
    fields->m_paged = true;
    fields->m_lastChildrenCount = getChildrenCount();
    fields->m_max = max;
    fields->m_padding = padding;
    fields->m_elementCount = count;
    fields->m_orientation = orientation;
    fields->m_pageCount = std::ceil(getChildrenCount()/(double)count);

    schedule(schedule_selector(PageMenu::checkChildren));

    for (int i = 0; i < fields->m_pageCount; i++) {
        CCArray* page = CCArray::create();
        int counter = count;
        for (int j = 0; j < counter; j++) {
            int pos = j + (i * count);
            if (pos >= getChildrenCount()) break;
            CCNode* node = static_cast<CCNode*>(getChildren()->objectAtIndex(pos));
            page->addObject(node);
        }
        fields->m_pages->addObject(page);
    }

    setPage(fields->m_currentPage);
    if (Layout* layout = getLayout()) {
        layout->ignoreInvisibleChildren(true);
    }
    updateLayout();
    addArrowButtons();
}

void PageMenu::addArrowButtons() {
    auto fields = m_fields.self();

    if (fields->m_pageCount <= 1) return;

    CCNode* parent = getParent();
    if (fields->m_arrowsMenu) {
        fields->m_arrowsMenu->removeFromParent();
    }

    fields->m_arrowsMenu = CCMenu::create();

    CCSprite* m_nextSprite = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
    m_nextSprite->setFlipX(true);
    m_nextSprite->setScale(fields->m_buttonScale);
    CCMenuItemSpriteExtra* m_nextButton = CCMenuItemSpriteExtra::create(m_nextSprite, this, menu_selector(PageMenu::nextPage));
    m_nextButton->setID("next-button");

    float buttonWidth = m_nextButton->getScaledContentWidth();

    CCSprite* m_prevSprite = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
    m_prevSprite->setScale(fields->m_buttonScale);
    CCMenuItemSpriteExtra* m_prevButton = CCMenuItemSpriteExtra::create(m_prevSprite, this, menu_selector(PageMenu::prevPage));
    m_prevButton->setID("prev-button");
    
    fields->m_arrowsMenu->setID(fmt::format("{}-navigation-menu", getID()));
    fields->m_arrowsMenu->ignoreAnchorPointForPosition(false);
    fields->m_arrowsMenu->setPosition(getPosition());
    fields->m_arrowsMenu->setScale(getScale());
    fields->m_arrowsMenu->setAnchorPoint(getAnchorPoint());
    fields->m_arrowsMenu->addChild(m_nextButton);
    fields->m_arrowsMenu->addChild(m_prevButton);

    if (fields->m_orientation == PageOrientation::HORIZONTAL) {

        fields->m_arrowsMenu->setContentSize({getContentWidth() + buttonWidth * 2 + fields->m_padding * 2, getContentHeight()});

        m_prevButton->setPositionX(buttonWidth/2);
        m_prevButton->setPositionY(fields->m_arrowsMenu->getContentHeight()/2);

        m_nextButton->setPositionX(fields->m_arrowsMenu->getContentWidth() - buttonWidth/2);
        m_nextButton->setPositionY(fields->m_arrowsMenu->getContentHeight()/2);

        if (fields->m_arrowsMenu->getScaledContentWidth() > fields->m_max) {
            float scaleFactor = fields->m_max/fields->m_arrowsMenu->getScaledContentWidth();
            fields->m_arrowsMenu->setScale(scaleFactor);
            setScale(scaleFactor * getScale());
        }
    }
    else {
        fields->m_arrowsMenu->setContentSize({getContentWidth(), getContentHeight() + buttonWidth * 2 + fields->m_padding * 2});

        m_prevButton->setRotation(90);
        m_nextButton->setRotation(90);

        m_nextButton->setPositionX(fields->m_arrowsMenu->getContentWidth()/2);
        m_nextButton->setPositionY(buttonWidth/2);

        m_prevButton->setPositionX(fields->m_arrowsMenu->getContentWidth()/2);
        m_prevButton->setPositionY(fields->m_arrowsMenu->getContentHeight() - buttonWidth/2);

        if (fields->m_arrowsMenu->getScaledContentHeight() > fields->m_max) {
            float scaleFactor = fields->m_max/fields->m_arrowsMenu->getScaledContentHeight() ;
            fields->m_arrowsMenu->setScale(scaleFactor);
            setScale(scaleFactor * getScale());
        }
    }

    parent->addChild(fields->m_arrowsMenu);
}

void PageMenu::setPage(int pageNum) {
    auto fields = m_fields.self();
    int iter = 0;

    if (pageNum >= fields->m_pageCount) {
        pageNum = fields->m_pageCount - 1;
    }

    for (CCArray* page : CCArrayExt<CCArray*>(fields->m_pages)) {
        for (CCNode* node : CCArrayExt<CCNode*>(page)) {
            node->setVisible(iter == pageNum);
        }
        iter++;
    }
    updateLayout();
}

void PageMenu::nextPage(CCObject* obj) {
    auto fields = m_fields.self();
    fields->m_currentPage++;
    if (fields->m_currentPage >= fields->m_pageCount) fields->m_currentPage = 0;
    setPage(fields->m_currentPage);
}

void PageMenu::prevPage(CCObject* obj) {
    auto fields = m_fields.self();
    fields->m_currentPage--;
    if (fields->m_currentPage < 0) fields->m_currentPage = fields->m_pageCount - 1;
    setPage(fields->m_currentPage);
}
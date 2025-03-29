#include <Geode/Geode.hpp>
#include "../include/PageMenu.h"

using namespace geode::prelude;

CCPoint getNodeCenter(CCNode* node) {
    CCPoint pos = node->getPosition();
    CCSize size = node->getScaledContentSize();
    CCPoint anchor = node->getAnchorPoint();

    return {pos.x + size.width * (0.5f - anchor.x),
               pos.y + size.height * (0.5f - anchor.y)};
}

void PageMenu::checkChildren(float dt) {
    auto fields = m_fields.self();
    if (!fields->m_paged) return;
    if (fields->m_lastChildrenCount != getChildrenCount()) {
        setPaged(fields->m_elementCount, fields->m_orientation, fields->m_max, fields->m_padding);
    }
    if (fields->m_paged && fields->m_arrowsMenu) {
        fields->m_arrowsMenu->setPosition(getNodeCenter(this));
        fields->m_arrowsMenu->setVisible(isVisible());
        fields->m_arrowsMenu->ignoreAnchorPointForPosition(false);
        fields->m_arrowsMenu->setZOrder(getZOrder());
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

void PageMenu::setFixed(float max) {
    auto fields = m_fields.self();
    if (AxisLayout* axisLayout = typeinfo_cast<AxisLayout*>(getLayout())) {
        axisLayout->setCrossAxisOverflow(false);
    }
    if (fields->m_orientation == PageOrientation::VERTICAL) {
        setContentWidth(max);
    }
    else {
        setContentHeight(max);
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

    CCArray* validChildren = CCArray::create();
    for (CCNode* child : CCArrayExt<CCNode*>(getChildren())) {
        if (child->isVisible() || child->getUserObject("was-visible"_spr)) {
            validChildren->addObject(child);
            child->setUserObject("was-visible"_spr, CCBool::create(true));
        }
    }

    fields->m_pageCount = std::ceil(validChildren->count()/(double)count);

    schedule(schedule_selector(PageMenu::checkChildren));

    for (int i = 0; i < fields->m_pageCount; i++) {
        CCArray* page = CCArray::create();
        int counter = count;
        for (int j = 0; j < counter; j++) {
            int pos = j + (i * count);
            if (pos >= validChildren->count()) break;
            CCNode* node = static_cast<CCNode*>(validChildren->objectAtIndex(pos));
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

    CCSprite* nextSprite = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
    nextSprite->setFlipX(true);
    nextSprite->setScale(fields->m_buttonScale);
    CCMenuItemSpriteExtra* nextButton = CCMenuItemSpriteExtra::create(nextSprite, this, menu_selector(PageMenu::nextPage));
    nextButton->setID("next-button");

    fields->m_buttonWidth = nextButton->getScaledContentWidth();

    CCSprite* prevSprite = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
    prevSprite->setScale(fields->m_buttonScale);
    CCMenuItemSpriteExtra* prevButton = CCMenuItemSpriteExtra::create(prevSprite, this, menu_selector(PageMenu::prevPage));
    prevButton->setID("prev-button");
    
    fields->m_arrowsMenu->setID(fmt::format("{}-navigation-menu", getID()));
    fields->m_arrowsMenu->ignoreAnchorPointForPosition(false);
    fields->m_arrowsMenu->setPosition(getNodeCenter(this));
    fields->m_arrowsMenu->setScale(getScale());
    fields->m_arrowsMenu->addChild(nextButton);
    fields->m_arrowsMenu->addChild(prevButton);

    if (fields->m_orientation == PageOrientation::HORIZONTAL) {

        fields->m_arrowsMenu->setContentSize({getContentWidth() + fields->m_buttonWidth * 2 + fields->m_padding * 2, getContentHeight()});

        prevButton->setPositionX(fields->m_buttonWidth/2);
        prevButton->setPositionY(fields->m_arrowsMenu->getContentHeight()/2);

        nextButton->setPositionX(fields->m_arrowsMenu->getContentWidth() - fields->m_buttonWidth/2);
        nextButton->setPositionY(fields->m_arrowsMenu->getContentHeight()/2);

        if (fields->m_arrowsMenu->getScaledContentWidth() > fields->m_max) {
            float scaleFactor = fields->m_max/fields->m_arrowsMenu->getScaledContentWidth();
            fields->m_arrowsMenu->setScale(scaleFactor);
            setScale(scaleFactor * getScale());
        }
    }
    else {
        fields->m_arrowsMenu->setContentSize({getContentWidth(), getContentHeight() + fields->m_buttonWidth * 2 + fields->m_padding * 2});

        prevButton->setRotation(90);
        nextButton->setRotation(90);

        nextButton->setPositionX(fields->m_arrowsMenu->getContentWidth()/2);
        nextButton->setPositionY(fields->m_buttonWidth/2);

        prevButton->setPositionX(fields->m_arrowsMenu->getContentWidth()/2);
        prevButton->setPositionY(fields->m_arrowsMenu->getContentHeight() - fields->m_buttonWidth/2);

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

    m_fields->m_nextCallback(obj);
}

void PageMenu::prevPage(CCObject* obj) {
    auto fields = m_fields.self();
    fields->m_currentPage--;
    if (fields->m_currentPage < 0) fields->m_currentPage = fields->m_pageCount - 1;
    setPage(fields->m_currentPage);

    m_fields->m_prevCallback(obj);
}

void setNextCallback(std::function<void(CCObject*)> callback) {m_fields->m_nextCallback = callback;}
void setPrevCallback(std::function<void(CCObject*)> callback) {m_fields->m_prevCallback = callback;}
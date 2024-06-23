#include "../include/PageMenu.h"

PageMenu::~PageMenu() {
	m_pages->release();
}

PageMenu* PageMenu::create(CCMenu* menu, int elementCount, bool forceContentSize) {
    auto node = new PageMenu();
    if (!node->init(menu, elementCount, forceContentSize)) {
        CC_SAFE_DELETE(node);
        return nullptr;
    }
    node->autorelease();
    return node;
}

bool PageMenu::init(CCMenu* menu, int elementCount, bool forceContentSize) {
    CCArray* children = menu->getChildren();

    CCSize winSize = CCDirector::get()->getWinSize();

    m_maxCount = elementCount;
    m_originalMenu = menu;
    m_innerNode = CCNode::create();
    m_innerNode->setContentSize(menu->getContentSize());
    //m_innerNode->setPosition(menu->getPosition());
    m_innerNode->ignoreAnchorPointForPosition(true);
    m_innerNode->setAnchorPoint(menu->getAnchorPoint());
    m_innerNode->setID("pages");
    m_innerNode->setScale(menu->getScale());

    ignoreAnchorPointForPosition(false);
    setContentSize(menu->getContentSize());
    setPosition(menu->getPosition());
    setID(menu->getID());
    m_layout = menu->getLayout();
    if (m_layout) {
	    m_layout->ignoreInvisibleChildren(true);
    }

    CCSize contentSize = typeinfo_cast<CCNode*>(children->objectAtIndex(0))->getContentSize();

    int idx = 0;

    int pageCount = std::ceil(children->count()/(float)elementCount);

    m_pages = CCArray::create();
    m_pages->retain();

    for (int i = 0; i < pageCount; i++) {

        CCMenu* searchPage = createPage();
        for (int j = 0; j < elementCount; j++) {
            if (children->count() == 0) {
                break;
            }
            CCNode* child = typeinfo_cast<CCNode*>(children->objectAtIndex(0));
            menu->removeChild(child, false);
            if (!child->isVisible()) elementCount++;

            if (forceContentSize) {
                child->setContentSize(contentSize);
                if(CCSprite* spr = getChildOfType<CCSprite>(child, 0)){
                    spr->setAnchorPoint({1, 0});
                    spr->setPosition({contentSize.width, 0});
                }
            }
            searchPage->addChild(child);
        }
        if (searchPage->getChildrenCount() > 0) {
            searchPage->updateLayout();
            m_pages->addObject(searchPage);
        }
    }

    for (CCMenu* page : CCArrayExt<CCMenu*>(m_pages)) {
        m_innerNode->addChild(page);
    }

    if (m_pages->count() > 0) {
        typeinfo_cast<CCNode*>(m_pages->objectAtIndex(0))->setVisible(true);
    }

    float btnScale = 0.6f;
    
    m_nextSprite = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
    m_nextSprite->setFlipX(true);
    m_nextSprite->setScale(btnScale);
    m_nextButton = CCMenuItemSpriteExtra::create(m_nextSprite, this, menu_selector(PageMenu::goRight));

    m_prevSprite = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
    m_prevSprite->setScale(btnScale);
    m_prevButton = CCMenuItemSpriteExtra::create(m_prevSprite, this, menu_selector(PageMenu::goLeft));

    setNavGap(m_navGap);
    setOrientation(PageOrientation::HORIZONTAL);
    CCMenu* pageButtons = CCMenu::create();
    pageButtons->setID("page-navigation-menu");
    pageButtons->setContentSize(getContentSize());
    pageButtons->setPosition(m_innerNode->getPosition());
    pageButtons->ignoreAnchorPointForPosition(true);
    pageButtons->addChild(m_nextButton);
    pageButtons->addChild(m_prevButton);

    addChild(pageButtons);

    if (m_pages->count() <= 1) {
        m_prevButton->setVisible(false);
        m_nextButton->setVisible(false);
    }

    menu->removeFromParentAndCleanup(false);
    addChild(m_innerNode);

    return true;
}

CCMenu* PageMenu::createPage() {
    CCMenu* searchPage = CCMenu::create();
    CCSize menuSize = m_originalMenu->getContentSize();
    searchPage->setPosition({menuSize.width/2, menuSize.height/2});
    searchPage->setScale(m_originalMenu->getScale());
    searchPage->setContentSize(m_originalMenu->getContentSize());
    if (m_layout) {
        searchPage->setLayout(m_layout);
        searchPage->updateLayout();
    }
    searchPage->setVisible(false);
    return searchPage;
}

void PageMenu::addPagedChild(CCNode* child) {

    bool childWasAdded = false;
    if (m_maxCount == 0) return;

    for (CCMenu* page : CCArrayExt<CCMenu*>(m_pages)) {
        if (page->getChildrenCount() < m_maxCount) {
            page->addChild(child);
            page->updateLayout();
            childWasAdded = true;
            break;
        }
    }

    if (!childWasAdded) {
        CCMenu* newPage = createPage();
        m_pages->addObject(newPage);
        m_innerNode->addChild(newPage);
        addPagedChild(child);
    }
}

void PageMenu::setPageLayout(Layout* layout) {
    m_layout = layout;
    for (CCMenu* page : CCArrayExt<CCMenu*>(m_pages)) {
        page->setLayout(layout);
        page->updateLayout();
    }
}

void PageMenu::setOrientation(PageOrientation orientation) {

    if(m_originalMenu->getUserObject("orientation")){
        orientation = (PageOrientation)typeinfo_cast<CCInteger*>(m_originalMenu->getUserObject("orientation"))->getValue();
    }

    if (orientation == PageOrientation::HORIZONTAL) {
        m_nextSprite->setRotation(0);
        m_prevSprite->setRotation(0);
    }
    else {
        m_nextSprite->setRotation(-90);
        m_prevSprite->setRotation(-90);
    }

    m_pageOrientation = orientation;
    setNavGap(m_navGap);
}

void PageMenu::setNavGap(float gapSize) {

    if(m_originalMenu->getUserObject("gap")){
        gapSize = typeinfo_cast<CCFloat*>(m_originalMenu->getUserObject("gap"))->getValue();
    }

    m_navGap = gapSize;

    float yPosNext;
    float yPosPrev;
    float xPosNext;
    float xPosPrev;

    if (m_pageOrientation == PageOrientation::VERTICAL) {
        xPosNext = getContentSize().width/2;
        xPosPrev = getContentSize().width/2;
        yPosNext = getContentSize().height + gapSize;
        yPosPrev = -gapSize;
    }
    else {
        yPosNext = getContentSize().height/2;
        yPosPrev = getContentSize().height/2;
        xPosNext = getContentSize().width + gapSize;
        xPosPrev = -gapSize;
    }

    m_nextButton->setPosition({xPosNext, yPosNext});
    m_prevButton->setPosition({xPosPrev, yPosPrev});
}

void PageMenu::goLeft(CCObject* obj) {
		
    m_page--;

    if (m_page < 0) {
        m_page = m_pages->count()-1;
    }
    
    setPageVisible();
}

void PageMenu::goRight(CCObject* obj) {
		
    m_page++;
    m_prevButton->setVisible(true);

    if (m_page > m_pages->count()-1) {
        m_page = 0;
    }
    setPageVisible();
}

void PageMenu::setPageVisible() {
    for (CCMenu* page : CCArrayExt<CCMenu*>(m_pages)) {
        page->setVisible(false);
    }

    if (m_pages->count() > 0) {
        typeinfo_cast<CCNode*>(m_pages->objectAtIndex(m_page))->setVisible(true);
    }
}

void PageMenu::scaleWhenFull() {
    if (m_pages->count() > 1) {
        setScale(0.9f);
    }
}

int PageMenu::getPageCount() {
    return m_pages->count();
}

float PageMenu::getNavGap() {
    return m_navGap;
}

PageOrientation PageMenu::getPageOrientation() {
    return m_pageOrientation;
}
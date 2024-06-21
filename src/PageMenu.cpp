#include "PageMenu.h"

PageMenu::~PageMenu() {
	m_pages->release();
}

PageMenu* PageMenu::create(CCMenu* menu, int elementCount, Layout* layout, bool forceContentSize){
    auto node = new PageMenu();
    if (!node->init(menu, elementCount, layout, forceContentSize)) {
        CC_SAFE_DELETE(node);
        return nullptr;
    }
    node->autorelease();
    return node;
}

bool PageMenu::init(CCMenu* menu, int elementCount, Layout* layout, bool forceContentSize){
    CCArray* children = menu->getChildren();

    CCSize winSize = CCDirector::get()->getWinSize();

    CCNode* innerNode = CCNode::create();
    innerNode->setContentSize(menu->getContentSize());
    innerNode->setPosition(menu->getPosition());
    innerNode->ignoreAnchorPointForPosition(menu->isIgnoreAnchorPointForPosition());
    innerNode->setAnchorPoint(menu->getAnchorPoint());
    innerNode->setID("pages");
    innerNode->setScale(menu->getScale());

    setContentSize(winSize);
    setID(menu->getID());
    
    int idx = 0;

    int pageCount = std::ceil(children->count()/(float)elementCount);

    m_pages = CCArray::create();
    m_pages->retain();

    CCSize contentSize = typeinfo_cast<CCNode*>(children->objectAtIndex(0))->getContentSize();

    for(int i = 0; i < pageCount; i++){
        CCMenu* searchPage = CCMenu::create();
        CCSize menuSize = menu->getContentSize();
        searchPage->setPosition({menuSize.width/2, menuSize.height/2});
        searchPage->setScale(menu->getScale());
        searchPage->setContentSize(menu->getContentSize());

        for(int j = 0; j < elementCount; j++){

            if(children->count() == 0) {
                break;
            }
            CCNode* child = typeinfo_cast<CCNode*>(children->objectAtIndex(0));
            menu->removeChild(child, false);
            if(!child->isVisible()) elementCount++;

            if(forceContentSize){
                child->setContentSize(contentSize);
                if(CCSprite* spr = getChildOfType<CCSprite>(child, 0)){
                    spr->setAnchorPoint({1, 0});
                    spr->setPosition({contentSize.width, 0});
                }
            }

            searchPage->addChild(child);
        }
        if(searchPage->getChildrenCount() > 0){
            searchPage->setLayout(layout);
            searchPage->updateLayout();
            searchPage->setVisible(false);
            m_pages->addObject(searchPage);
        }
    }

    for(CCMenu* page : CCArrayExt<CCMenu*>(m_pages)){
        innerNode->addChild(page);
    }

    if(m_pages->count() > 0){
        typeinfo_cast<CCNode*>(m_pages->objectAtIndex(0))->setVisible(true);
    }

    float btnDistance = 15.0f;
    float btnScale = 0.6f;
    float yPos = innerNode->getContentSize().height/2;

    CCSprite* nextSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
    nextSpr->setFlipX(true);
    nextSpr->setScale(btnScale);
    m_nextButton = CCMenuItemSpriteExtra::create(nextSpr, this, menu_selector(PageMenu::goRight));

    float xPosNext = innerNode->getContentSize().width + btnDistance;
    m_nextButton->setPosition({xPosNext, yPos});


    CCSprite* prevSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
    prevSpr->setScale(btnScale);
    m_prevButton = CCMenuItemSpriteExtra::create(prevSpr, this, menu_selector(PageMenu::goLeft));

    float xPosPrev = -btnDistance;
    m_prevButton->setPosition({xPosPrev, yPos});

    CCMenu* pageButtons = CCMenu::create();
    pageButtons->setID("page-navigation-menu");
    pageButtons->setContentSize(innerNode->getContentSize());
    pageButtons->setPosition(innerNode->getPosition());
    pageButtons->ignoreAnchorPointForPosition(false);
    pageButtons->addChild(m_nextButton);
    pageButtons->addChild(m_prevButton);

    addChild(pageButtons);

    if(m_pages->count() <= 1){
        m_prevButton->setVisible(false);
        m_nextButton->setVisible(false);
    }

    menu->removeFromParentAndCleanup(false);
    addChild(innerNode);

    return true;
}

void PageMenu::goLeft(CCObject* obj){
		
    m_page--;

    if(m_page < 0){
        m_page = m_pages->count()-1;
    }
    
    setPageVisible();
}

void PageMenu::goRight(CCObject* obj){
		
    m_page++;
    m_prevButton->setVisible(true);

    if(m_page > m_pages->count()-1){
        m_page = 0;
    }
    setPageVisible();
}

void PageMenu::setPageVisible(){
    for(CCMenu* page : CCArrayExt<CCMenu*>(m_pages)){
        page->setVisible(false);
    }

    if(m_pages->count() > 0){
        typeinfo_cast<CCNode*>(m_pages->objectAtIndex(m_page))->setVisible(true);
    }
}

void PageMenu::scaleAtMax(){
    if(m_pages->count() > 1){
        setScale(0.9f);
    }
}

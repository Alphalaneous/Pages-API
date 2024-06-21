#include "PageMenu.h"

PageMenu::~PageMenu() {
	pages->release();
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

    pages = CCArray::create();
    pages->retain();

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
            pages->addObject(searchPage);
        }
    }

    for(CCMenu* page : CCArrayExt<CCMenu*>(pages)){
        innerNode->addChild(page);
    }

    if(pages->count() > 0){
        typeinfo_cast<CCNode*>(pages->objectAtIndex(0))->setVisible(true);
    }

    float btnDistance = 15.0f;
    float btnScale = 0.6f;
    float yPos = innerNode->getContentSize().height/2;

    CCSprite* nextSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
    nextSpr->setFlipX(true);
    nextSpr->setScale(btnScale);
    nextButton = CCMenuItemSpriteExtra::create(nextSpr, this, menu_selector(PageMenu::goRight));

    float xPosNext = innerNode->getContentSize().width + btnDistance;
    nextButton->setPosition({xPosNext, yPos});


    CCSprite* prevSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
    prevSpr->setScale(btnScale);
    prevButton = CCMenuItemSpriteExtra::create(prevSpr, this, menu_selector(PageMenu::goLeft));

    float xPosPrev = -btnDistance;
    prevButton->setPosition({xPosPrev, yPos});

    CCMenu* pageButtons = CCMenu::create();
    pageButtons->setID("page-navigation-menu");
    pageButtons->setContentSize(innerNode->getContentSize());
    pageButtons->setPosition(innerNode->getPosition());
    pageButtons->ignoreAnchorPointForPosition(false);
    pageButtons->addChild(nextButton);
    pageButtons->addChild(prevButton);

    addChild(pageButtons);

    if(pages->count() <= 1){
        prevButton->setVisible(false);
        nextButton->setVisible(false);
    }

    menu->removeFromParentAndCleanup(false);
    addChild(innerNode);

    return true;
}

void PageMenu::goLeft(CCObject* obj){
		
    page--;

    if(page < 0){
        page = pages->count()-1;
    }
    
    setPageVisible();
}

void PageMenu::goRight(CCObject* obj){
		
    page++;
    prevButton->setVisible(true);

    if(page > pages->count()-1){
        page = 0;
    }
    setPageVisible();
}

void PageMenu::setPageVisible(){
    for(CCMenu* page : CCArrayExt<CCMenu*>(pages)){
        page->setVisible(false);
    }

    if(pages->count() > 0){
        typeinfo_cast<CCNode*>(pages->objectAtIndex(page))->setVisible(true);
    }
}
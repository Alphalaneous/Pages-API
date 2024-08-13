#include "../include/PageMenu.h"
#include "CCMenuItemSpriteExtra.h"

PageMenu::~PageMenu() {
}

PageMenu* PageMenu::create(CCMenu* menu, Layout* layout, int elementCount, bool forceContentSize) {
    auto node = new PageMenu();
    if (!node->init(menu, layout, elementCount, forceContentSize)) {
        CC_SAFE_DELETE(node);
        return nullptr;
    }
    node->autorelease();
    return node;
}

bool PageMenu::init(CCMenu* menu, Layout* layout, int elementCount, bool forceContentSize) {

    CCMenu::init();
    if(CCObject* obj = menu->getUserObject("disable-pages")){
        if (CCBool* disable = typeinfo_cast<CCBool*>(obj)){
            if(disable->getValue()) {
                m_isPage = false;
                return true;
            }
        }
    }
    if(CCObject* obj = menu->getUserObject("element-count")){
        if (CCInteger* count = typeinfo_cast<CCInteger*>(obj)){
            elementCount = count->getValue();
        }
    }

    m_forceContentSize = forceContentSize;
    m_children = CCArray::create();

    m_lastAttributes = CCDictionary::create();

    CCSize winSize = CCDirector::get()->getWinSize();

    m_maxCount = elementCount;
    m_originalMenu = menu;

    m_innerNode = CCNode::create();
    m_innerNode->setContentSize(menu->getScaledContentSize());
    m_innerNode->ignoreAnchorPointForPosition(true);
    m_innerNode->setAnchorPoint(menu->getAnchorPoint());
    m_innerNode->setID("pages");

    float scaleFactor = 1.2f;

    m_background = CCScale9Sprite::create("square02_001.png");
    m_background->setContentSize({(m_innerNode->getContentSize().width + 10) * scaleFactor, (m_innerNode->getContentSize().height + 10) * scaleFactor});
    m_background->setPosition({m_innerNode->getContentSize().width/2, m_innerNode->getContentSize().height/2});
    m_background->setOpacity(0.0f);
    m_background->setScale(1/scaleFactor);
    addChild(m_background);

    float buttonScaleFactor = 3.5f;

    CCSize buttonSize = {30, 16};

    #ifdef IN_PROGRESS

    m_doneButton = CCMenuItem::create(this, menu_selector(PageMenu::stopEditing));
    m_buttonBG = CCScale9Sprite::create("square02_001.png");
    m_buttonBG->setPosition({buttonSize.width/2, buttonSize.height/2});
    m_buttonBG->setContentSize({buttonSize.width * buttonScaleFactor, buttonSize.height * buttonScaleFactor});
    m_buttonBG->setScale(1/buttonScaleFactor);

    m_doneLabel = CCLabelBMFont::create("Done", "chatFont.fnt");
    m_doneLabel->setPosition({buttonSize.width/2, buttonSize.height/2});
    m_doneLabel->setScale(0.70f);
    m_doneLabel->setZOrder(1);
    m_doneButton->setPosition({(m_innerNode->getContentSize().width + 10/2) - buttonSize.width/2, (m_innerNode->getContentSize().height + 10/2) + buttonSize.height/2 + 1.5f});
    m_doneButton->setContentSize({buttonSize.width, buttonSize.height});
    m_doneButton->addChild(m_buttonBG);
    m_doneButton->addChild(m_doneLabel);


    m_doneLabel->setOpacity(0);
    m_buttonBG->setOpacity(0);

    #endif

    ignoreAnchorPointForPosition(false);
    setContentSize(menu->getContentSize());
    setPosition(menu->getPosition());
    setID(fmt::format("paged-{}", menu->getID()));
    if(layout){
        m_layout = layout;
        m_layout->ignoreInvisibleChildren(true);
    }
    else {
        return false;
    }

    int idx = 0;

    int pageCount = std::ceil(m_children->count()/(float)elementCount);

    m_pages = CCArray::create();

    float btnScale = 0.6f;

    m_nextSprite = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
    m_nextSprite->setFlipX(true);
    m_nextSprite->setScale(btnScale);
    m_nextButton = CCMenuItemSpriteExtra::create(m_nextSprite, this, menu_selector(PageMenu::goRight));

    m_prevSprite = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
    m_prevSprite->setScale(btnScale);
    m_prevButton = CCMenuItemSpriteExtra::create(m_prevSprite, this, menu_selector(PageMenu::goLeft));

    m_navMenu = CCMenu::create();
    m_navMenu->setID("page-navigation-menu");
    m_navMenu->setContentSize(getContentSize());
    m_navMenu->setPosition(m_innerNode->getPosition());
    m_navMenu->ignoreAnchorPointForPosition(true);
    m_navMenu->addChild(m_nextButton);
    m_navMenu->addChild(m_prevButton);
    m_finishedInit = true;

    updatePage();
    setNavGap(m_navGap);
    setOrientation(PageOrientation::HORIZONTAL);

    #ifdef IN_PROGRESS

    m_navMenu->addChild(m_doneButton);

    #endif

    addChild(m_navMenu);
    addChild(m_innerNode);

    setUserObject("real-scale", CCFloat::create(1));

    schedule(schedule_selector(PageMenu::checkMenu));
    schedule(schedule_selector(PageMenu::checkAttributes));

    return true;
}

void PageMenu::checkAttributes(float dt){

    float lastScale = getScale();

    attributeListen(PositionX);
    attributeListen(PositionY);
    attributeListen(ScaleX);
    attributeListen(ScaleY);
    attributeListen(SkewX);
    attributeListen(SkewY);
    attributeListen(RotationX);
    attributeListen(RotationY);
    attributeListen(ZOrder);
    attributeListen(ContentWidth);
    attributeListen(ContentHeight);

    if(lastScale != getScale() && m_scaleWhenFull){
        setUserObject("real-scale", CCFloat::create(m_originalMenu->getScale()));
        scaleWhenFull();
    }

    if(CCBool* obj = typeinfo_cast<CCBool*>(m_lastAttributes->objectForKey("Visible"))){
        if(obj->getValue() != m_originalMenu->isVisible()){
            setVisible(m_originalMenu->isVisible());
        }
    }
    else {
        setVisible(m_originalMenu->isVisible());
    }
    m_lastAttributes->setObject(CCBool::create(m_originalMenu->isVisible()), "Visible");

    if(CCBool* obj = typeinfo_cast<CCBool*>(m_lastAttributes->objectForKey("IgnoreAnchorPointForPosition"))){
        if(obj->getValue() != m_originalMenu->isIgnoreAnchorPointForPosition()){
            ignoreAnchorPointForPosition(m_originalMenu->isIgnoreAnchorPointForPosition());
        }
    }
    else {
        ignoreAnchorPointForPosition(m_originalMenu->isIgnoreAnchorPointForPosition());
    }
    m_lastAttributes->setObject(CCBool::create(m_originalMenu->isIgnoreAnchorPointForPosition()), "IgnoreAnchorPointForPosition");

    if(CCFloat* obj = typeinfo_cast<CCFloat*>(m_lastAttributes->objectForKey("AnchorPointX"))){
        if(obj->getValue() != m_originalMenu->getAnchorPoint().x){
            setAnchorPoint({m_originalMenu->getAnchorPoint().x, getAnchorPoint().y});
        }
    }
    else {
        setAnchorPoint({m_originalMenu->getAnchorPoint().x, getAnchorPoint().y});
    }
    m_lastAttributes->setObject(CCFloat::create(m_originalMenu->isIgnoreAnchorPointForPosition()), "AnchorPointX");

    if(CCFloat* obj = typeinfo_cast<CCFloat*>(m_lastAttributes->objectForKey("AnchorPointY"))){
        if(obj->getValue() != m_originalMenu->getAnchorPoint().y){
            setAnchorPoint({getAnchorPoint().x, m_originalMenu->getAnchorPoint().y});
        }
    }
    else {
        setAnchorPoint({getAnchorPoint().x, m_originalMenu->getAnchorPoint().y});
    }
    m_lastAttributes->setObject(CCFloat::create(m_originalMenu->isIgnoreAnchorPointForPosition()), "AnchorPointY");

}

void PageMenu::checkMenu(float dt){

    if(!m_isPage || !m_finishedInit || !m_children || !m_originalMenu) return;

    if(m_originalMenu->getChildrenCount() > 0){
        updatePage();
        return;
    }

    int visibleCount = 0;

    for(CCNode* child : CCArrayExt<CCNode*>(m_children)){
        if(child->isVisible()) visibleCount++;
        if(visibleCount > m_lastVisibleCount){
            updatePage();
            break;
        }
    }

    if(visibleCount < m_lastVisibleCount){
        updatePage();
    }

    m_lastVisibleCount = visibleCount;
}

void PageMenu::setElementCount(int count){

    if(CCObject* obj = m_originalMenu->getUserObject("element-count")){
        if (CCInteger* count = typeinfo_cast<CCInteger*>(obj)){
            m_maxCount = count->getValue();
        }
    }
    else {
        m_maxCount = count;
    }

    if(!m_isPage || !m_finishedInit || !m_children || !m_originalMenu) return;

    updatePage();
}

CCArray* PageMenu::getPagedChildren(){
    return m_children;
}

float PageMenu::getTotalWidth(){
    return getContentSize().width + m_navGap * 2 + m_nextButton->getScaledContentSize().width * 2;
}

void PageMenu::setArrowScale(float scale){
    m_nextButton->setScale(scale);
    m_nextButton->m_baseScale = scale;
    m_prevButton->setScale(scale);
    m_prevButton->m_baseScale = scale;
}

void PageMenu::updatePage() {

    if(!m_isPage || !m_finishedInit || !m_children || !m_originalMenu) return;

    for(CCNode* node : CCArrayExt<CCNode*>(m_originalMenu->getChildren())){
        m_children->addObject(node);
    }

    m_originalMenu->removeAllChildrenWithCleanup(false);

    if(m_children->count() == 0) return;

    m_innerNode->removeAllChildrenWithCleanup(false);
    m_pages->removeAllObjects();
    
    CCSize contentSize = {0, 0};

    if(CCNode* node = typeinfo_cast<CCNode*>(m_children->objectAtIndex(0))){
        contentSize = node->getContentSize();
    }

    int pageCount = std::ceil(m_children->count()/(float)m_maxCount);
    int elementCount = m_maxCount;
    int childrenCount = m_children->count();
    int pos = 0;

    for (int i = 0; i < pageCount; i++) {

        CCMenu* searchPage = createPage();
        for (int j = 0; j < elementCount; j++) {

            if (childrenCount == 0) {
                break;
            }

            if(CCNode* child =  typeinfo_cast<CCNode*>(m_children->objectAtIndex(pos))){
                child->setUserObject("page-menu", CCBool::create(true));
                childrenCount--;
                if (!child->isVisible()) elementCount++;

                if (m_forceContentSize) {
                    child->setContentSize(contentSize);
                    if(CCSprite* spr = getChildOfType<CCSprite>(child, 0)){
                        spr->setAnchorPoint({1, 0});
                        spr->setPosition({contentSize.width, 0});
                    }
                }
                pos++;
                searchPage->addChild(child);
            }
        }
        if (searchPage->getChildrenCount() > 0) {
            searchPage->updateLayout();
            searchPage->setScale(1);
            m_pages->addObject(searchPage);
        }
    }

    for (CCMenu* page : CCArrayExt<CCMenu*>(m_pages)) {
        if(page){
            m_innerNode->addChild(page);
        }
    }

    if (m_pages->count() > 0) {
        
        int page = 0;
        if(m_pages->count() > m_page){
            page = m_page;
        }

        if(CCNode* node = typeinfo_cast<CCNode*>(m_pages->objectAtIndex(m_page))){
            node->setVisible(true);
        }
    }
    if(m_navMenu){
        m_navMenu->setVisible(m_pages->count() > 1);
    }
    setUniformScale(m_isUniformScale);

    if(m_scaleWhenFull){
        scaleWhenFull();
    }
}

CCMenu* PageMenu::createPage() {
    CCMenu* searchPage = CCMenu::create();
    if(m_originalMenu){
        CCSize menuSize = m_originalMenu->getContentSize();
        searchPage->setPosition({menuSize.width/2, menuSize.height/2});
        searchPage->setScale(m_originalMenu->getScale());
        searchPage->setContentSize(m_originalMenu->getContentSize());
        if (m_layout) {
            searchPage->setLayout(m_layout);
            searchPage->updateLayout();
        }
        searchPage->setVisible(false);
    }
    return searchPage;
}

void PageMenu::addPagedChild(CCNode* child) {

    if(!m_isPage) return;

    bool childWasAdded = false;
    if (m_maxCount == 0) return;
    child->setUserObject("page-menu", CCBool::create(true));
    m_children->addObject(child);

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

    if(!m_isPage) return;

    m_layout = layout;
    for (CCMenu* page : CCArrayExt<CCMenu*>(m_pages)) {
        page->setLayout(layout);
        page->updateLayout();
    }
}

void PageMenu::setOrientation(PageOrientation orientation) {

    if(!m_isPage) return;

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

    if(!m_isPage) return;

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

void PageMenu::setUniformScale(bool isUniform){

    if(!m_isPage) return;
    
    m_isUniformScale = isUniform;

    if(!m_children) return;
    if(m_children->count() == 0) return;
    if(m_pages->count() <= 1) return;

    if(isUniform){

        if(m_forceScale){
            for (CCNode* node : CCArrayExt<CCNode*>(m_children)) {
                if(CCMenuItemSpriteExtra* button = typeinfo_cast<CCMenuItemSpriteExtra*>(node)){
                    button->m_baseScale = m_forcedScale;
                }
                node->setScale(m_forcedScale);
            }
        }
        else{
            if(CCNode* child = typeinfo_cast<CCNode*>(m_children->objectAtIndex(0))){
                float scale = child->getScale();
                for (CCNode* node : CCArrayExt<CCNode*>(m_children)) {
                    if(CCMenuItemSpriteExtra* button = typeinfo_cast<CCMenuItemSpriteExtra*>(node)){
                        button->m_baseScale = scale;
                    }
                    node->setScale(scale);
                }
            }
        }

        if(AxisLayout* layout = typeinfo_cast<AxisLayout*>(m_layout.data())){
            layout->setAutoScale(false);
        }

        for (CCMenu* page : CCArrayExt<CCMenu*>(m_pages)) {
            page->updateLayout();
        }
    }
}

void PageMenu::setForceScale(bool force, float scale){
    m_forceScale = force;
    m_forcedScale = scale;
    
    setUniformScale(m_isUniformScale);
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

    if(!m_isPage) return;

    for (CCMenu* page : CCArrayExt<CCMenu*>(m_pages)) {
        page->setVisible(false);
    }

    if (m_pages->count() > 0) {
        typeinfo_cast<CCNode*>(m_pages->objectAtIndex(m_page))->setVisible(true);
    }
}

void PageMenu::scaleWhenFull() {
    
    if(!m_isPage) return;

    m_scaleWhenFull = true;

    bool doShrink = true;

    if(CCObject* obj = m_originalMenu->getUserObject("shrink-when-full")){
        if (CCBool* shrink = typeinfo_cast<CCBool*>(obj)){
            doShrink = shrink->getValue();
        }
    }

    if (doShrink && m_pages->count() > 1) {
        setScale(typeinfo_cast<CCFloat*>(getUserObject("real-scale"))->getValue() * 0.85f);
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

void PageMenu::startShakeChildren() {
    for(PageCCMenuItemSpriteExtra* child : CCArrayExt<PageCCMenuItemSpriteExtra*>(m_children)){
        child->startShakeAnimation();
    }
}

void PageMenu::stopShakeChildren() {
    for(PageCCMenuItemSpriteExtra* child : CCArrayExt<PageCCMenuItemSpriteExtra*>(m_children)){
        child->stopShakeAnimation();
    }
}

void PageMenu::startEditing() {
    if(!m_isEditing){
        m_isEditing = true;
        m_background->runAction(CCFadeTo::create(0.3, 125));
        m_buttonBG->runAction(CCFadeTo::create(0.3, 125));
        m_doneLabel->runAction(CCFadeTo::create(0.3, 255));
        
        startShakeChildren();
    }
}

void PageMenu::stopEditing(CCObject* obj) {
    if(m_isEditing){
        m_isEditing = false;
        m_background->runAction(CCFadeTo::create(0.3, 0));
        m_buttonBG->runAction(CCFadeTo::create(0.3, 0));
        m_doneLabel->runAction(CCFadeTo::create(0.3, 0));
        stopShakeChildren();
    }
}
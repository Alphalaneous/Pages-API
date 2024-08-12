#include "layers/CreatorLayer.h"
#include "layers/LevelSearchLayer.h"
#include "layers/MenuLayer.h"
#include "layers/PauseLayer.h"
#include "layers/LevelInfoLayer.h"
#include "layers/EditorUI.h"

#include "CCMenuItemSpriteExtra.h"

//#define IN_PROGRESS

CCNode* CCNode_getChildByID(CCNode* self, std::string const& id) {

    if (!self) return nullptr; //cuz people are dumb, I gotta check if they're calling the method on a nullptr

    if (CCNode* parent = self->getParent()) {
        if (PageMenu* page = typeinfo_cast<PageMenu*>(parent->getChildByID(fmt::format("paged-{}", self->getID())))) {
            for (CCNode* child : CCArrayExt<CCNode*>(page->getPagedChildren())) {
                if (child->getID() == id) {
                    return child;
                }
            }
            return nullptr;
        }
    }

    return self->getChildByID(id);
}

$execute {
    (void) Mod::get()->hook(
        reinterpret_cast<void*>(
            geode::addresser::getNonVirtual(
                geode::modifier::Resolve<std::string const&>::func(&CCNode::getChildByID)
            )
        ),
        &CCNode_getChildByID,
        "CCNode::getChildByID",
        tulip::hook::TulipConvention::Thiscall
    );
}
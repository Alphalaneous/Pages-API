#include "layers/CreatorLayer.h"
#include "layers/LevelSearchLayer.h"
#include "layers/MenuLayer.h"
#include "layers/PauseLayer.h"
#include "layers/LevelInfoLayer.h"

#include "CCMenuItemSpriteExtra.h"

//#define IN_PROGRESS

CCNode* CCNode_getChildByID_NOT_PAGES_APIS_FAULT_IF_YOU_CALL_GETCHILDBYID_ON_A_NULLPTR_OBJECT_PLEASE_DOUBLE_CHECK_YOUR_CODE_THANK_YOU(CCNode* self, std::string const& id) {

    if (!self) return nullptr;

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
        &CCNode_getChildByID_NOT_PAGES_APIS_FAULT_IF_YOU_CALL_GETCHILDBYID_ON_A_NULLPTR_OBJECT_PLEASE_DOUBLE_CHECK_YOUR_CODE_THANK_YOU,
        "CCNode::getChildByID",
        tulip::hook::TulipConvention::Thiscall
    );
}
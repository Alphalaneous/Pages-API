#define LATE_MODIFY(method_name)\
static void onModify(auto& self) {\
    (void) self.setHookPriority(#method_name, INT_MIN/2);\
}

#define attributeListen(attribute)\
if(CCFloat* obj = typeinfo_cast<CCFloat*>(m_lastAttributes->objectForKey(#attribute))){\
    if(obj->getValue() != m_originalMenu->get##attribute()){\
        set##attribute(m_originalMenu->get##attribute());\
    }\
}\
else {\
    set##attribute(m_originalMenu->get##attribute());\
}\
m_lastAttributes->setObject(CCFloat::create(m_originalMenu->get##attribute()), #attribute);
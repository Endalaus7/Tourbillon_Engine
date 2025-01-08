#include "dynamicReflect.h"

void* DynamicReflect::getData(ReflectName* name)
{
    if(members.find(name)==members.end())
        return nullptr;
    DynamicTypeInfo& info = members[name];
    std::byte* ptr = m_data + info.offset;

    return (void*)ptr;
}

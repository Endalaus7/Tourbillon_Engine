#include "dynamicReflect.h"

std::unordered_map<ReflectName, ConverterBase*> g_typeConverter;

void registerDynType()
{
    DYNAMIC_REGISTER_TYPE(char);
    DYNAMIC_REGISTER_TYPE(int);
    DYNAMIC_REGISTER_TYPE(float);
    DYNAMIC_REGISTER_TYPE(double);
}

void DynamicReflect::BeginRegister(const ReflectName& name)
{
    class_name = name;
    m_tail = nullptr;
    //register_members.clear();
}

void DynamicReflect::RegisterMember(const ReflectName& name, const ReflectName& type_name)
{
    DynamicTypeInfo mem;

    ConverterBase* conver = g_typeConverter["int"];
    auto value = conver->get(0);

    size_t curr_offset = m_tail ? m_tail->offset : 0;
    size_t curr_aligment = m_tail ? m_tail->alignment : 0;

    size_t size = sizeof(int);
    size_t align = alignof(int);

    mem.type_name = type_name;
    mem.size = size;
    curr_aligment = curr_aligment > align ? curr_aligment : align;//std::max(curr_aligment, align);
    mem.offset = curr_offset;//(offset + alignof(T) - 1) & ~(alignof(T) - 1);
    mem.alignment = curr_aligment;
    curr_offset += mem.size;

    members[name] = mem;
    m_tail = &members[name];
}


void DynamicReflect::EndRegister(DynamicReflect& refl)
{
    size_t size = (curr_offset + curr_aligment - 1) & ~(curr_aligment - 1);
    std::vector<char> buffer(size);
    //
    std::pmr::monotonic_buffer_resource resource(buffer.data(), size);
    refl.m_data = (std::byte*)resource.allocate(size);

    //register_members.clear();
}

void* DynamicReflect::getData(const ReflectName& name)
{
    if(members.find(name)==members.end())
        return nullptr;
    auto& info = members[name];
    std::byte* ptr = m_data + info.offset;

    return (void*)ptr;
}


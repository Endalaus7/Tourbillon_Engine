#pragma once
#include "metaMarcos.hpp"
#include "common.hpp"
#include <memory_resource>
#include <unordered_map>
#include <memory>
#include <functional>

//////////////////////////////////////
//动态反射：可以根据配置调整参数
// 存储的类型
//////////////////////////////////////

// 类型注册表：保存字符串到类型的映射

class ConverterBase
{
public:
    virtual void* get(void* data) = 0;
};


static std::unordered_map<ReflectName, ConverterBase*> g_typeConverter;//类型转换器

static void registerDynType();

template <typename T>
class Convert : public ConverterBase
{
public:
    virtual void* get(void* data)override
    {
        return static_cast<void*>(data);
    }
};

// 宏用于注册类型
#define DYNAMIC_REGISTER_TYPE(type)\
    g_typeConverter[STR(type)] = new Convert<type>();


struct DynamicTypeInfo
{
    ReflectName type_name;//
    size_t size;       // 成员的大小
    size_t offset;       // 
    size_t alignment;     // 对齐
};

class DynamicReflect
{
private:
    std::byte* m_data;//首地址
    ReflectName class_name;
public:
    void BeginRegister(const ReflectName& class_name);
    void RegisterMember(const ReflectName& name, const ReflectName& type_name);
    void EndRegister(DynamicReflect& refl);

    void* getData(const ReflectName& name);
private:
    std::unordered_map<ReflectName, DynamicTypeInfo> members;
    DynamicTypeInfo* m_tail;
};



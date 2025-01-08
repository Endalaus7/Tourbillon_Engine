#pragma once
#include "metaMarcos.hpp"
#include "common.hpp"
#include <memory_resource>
#include <unordered_map>
#include <memory>
#include <functional>

//////////////////////////////////////
//动态反射：可以根据配置调整参数
//////////////////////////////////////

// 类型注册表：保存字符串到类型的映射


// 宏用于注册类型
#define DYNAMIC_REGISTER_TYPE(type)\
    g_typeRegistry[STR(type)] = []() -> void* { return new type; };

// 宏用于根据字符串获取对应类型的实例
// new的方式后续替换，会产生大量内存碎片
#define GET_DYNAMIC_TYPE_INSTANCE(type_name, value) \
    static_cast<std::remove_pointer_t<decltype(g_typeRegistry[type_name]())>*>(value)


void DynReflManager::registerDynType()
{
    DYNAMIC_REGISTER_TYPE(char);
    DYNAMIC_REGISTER_TYPE(int);
    DYNAMIC_REGISTER_TYPE(float);
    DYNAMIC_REGISTER_TYPE(double);
}

struct DynamicTypeInfo
{
    ReflectName type_name;//
    size_t size;       // 成员的大小
    size_t offset;       // 
    size_t alignment;     // 对齐
};

class DynamicReflect
{
    friend class DynReflManager;
public:
    std::byte* m_data;//首地址
    ReflectName classname;
    

    void* getData(ReflectName* name);
private:
    std::pmr::unordered_map<ReflectName, DynamicTypeInfo> members;
};

//管理类
class DynReflManager :public TourBillon::Singleton<DynReflManager>
{
    DynReflManager()
    {
        registerDynType();
    }
public:
    void BeginRegister(DynamicReflect& refl, const ReflectName& classname)
    {
        refl.classname = classname;
        register_members.clear();
        curr_offset = 0;
        curr_aligment = 0;
    }

    template <typename T>
    void registerMemType(DynamicReflect& refl,const T& obj, const ReflectName& name, const ReflectName& type_name)
    {
        DynamicTypeInfo mem;
       
        size_t size = sizeof(obj);
        size_t align = alignof(obj);

        auto ptr = GET_DYNAMIC_TYPE_INSTANCE(type_name, obj);

        mem.type_name = type_name;
        mem.size = size;
        curr_aligment = std::max(curr_aligment, align);
        mem.offset = curr_offset;//(offset + alignof(T) - 1) & ~(alignof(T) - 1);
        mem.alignment = curr_aligment;
        register_members.insert({ type_name, mem });
        curr_offset += mem.size;

        refl.members[name] = mem;

    }
    void EndRegister(DynamicReflect& refl)
    {
        size_t size = (curr_offset + curr_aligment - 1) & ~(curr_aligment - 1);
        std::vector<char> buffer(size);
        //
        std::pmr::monotonic_buffer_resource resource(buffer.data(), size);
        refl.m_data = (std::byte*)resource.allocate(size);

        register_members.clear();
    }
    
    static void registerDynType();
private:
    static std::unordered_map<ReflectName, std::function<void* ()>> g_typeRegistry;//注册类型

    std::unordered_map<ReflectName, DynamicTypeInfo> register_members;//注册时的暂时变量，多线程不兼容
    size_t curr_offset = 0;//多线程不兼容
    size_t curr_aligment = 0;//多线程不兼容
};

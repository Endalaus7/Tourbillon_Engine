#pragma once
#include "metaMarcos.hpp"
#include "common.hpp"
#include <memory_resource>
#include <unordered_map>

namespace TourBillon
{
    // 基础类型的反射信息
    struct DynMemberInfo {
        ReflectPath name;  // 成员名称
        size_t offset;     // 成员的偏移量
        size_t size;       // 成员的大小
    };


    struct DynamicStruct
    {
        std::string type;
    };

    class DynamicReflect
    {
    public:
        ReflectPath name;
        std::pmr::vector<DynamicStruct> members;
    };

    class DynReflManager :public Singleton<DynReflManager>
    {
    public:
        template <typename T>
        static void registerType(DynamicReflect& ,const T& obj)
        {
            DynamicStruct mem;
            mem.type = typeid(obj).name();
            members.push_back({ "Type", typeid(obj).name() });

        }

    };
}
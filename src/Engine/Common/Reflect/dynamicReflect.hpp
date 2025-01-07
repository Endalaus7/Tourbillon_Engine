#pragma once
#include "metaMarcos.hpp"
#include "common.hpp"
#include <memory_resource>
#include <unordered_map>

namespace TourBillon
{
    // �������͵ķ�����Ϣ
    struct DynMemberInfo {
        ReflectPath name;  // ��Ա����
        size_t offset;     // ��Ա��ƫ����
        size_t size;       // ��Ա�Ĵ�С
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
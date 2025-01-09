#pragma once
#include "metaMarcos.hpp"
#include "common.hpp"
#include <memory_resource>
#include <unordered_map>
#include <memory>
#include <functional>

//////////////////////////////////////
//��̬���䣺���Ը������õ�������
// �洢������
//////////////////////////////////////

// ����ע��������ַ��������͵�ӳ��

class ConverterBase
{
public:
    virtual void* get(void* data) = 0;
};


static std::unordered_map<ReflectName, ConverterBase*> g_typeConverter;//����ת����

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

// ������ע������
#define DYNAMIC_REGISTER_TYPE(type)\
    g_typeConverter[STR(type)] = new Convert<type>();


struct DynamicTypeInfo
{
    ReflectName type_name;//
    size_t size;       // ��Ա�Ĵ�С
    size_t offset;       // 
    size_t alignment;     // ����
};

class DynamicReflect
{
private:
    std::byte* m_data;//�׵�ַ
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



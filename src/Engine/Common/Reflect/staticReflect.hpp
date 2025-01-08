#pragma once
#include "metaMarcos.hpp"

#include <type_traits>
#include <utility>
#include <cstddef>
#include <functional>
#include <vector>


#include "common.hpp"

//非继承
#define STATIC_PROPERTY_CLASS_BEGIN()\
    public:                                  \
        template <typename, size_t> struct FIELD; \
        static constexpr size_t m_parent_field_count = 0;\
        static constexpr size_t m_global_begin_count = __COUNTER__;\

//继承,todo
#define STATIC_PROPERTY_CLASS_BEGIN_PARENT(argparent)\
    public:                                  \
        template <typename, size_t> struct FIELD; \
        static constexpr size_t m_parent_field_count = argparent::_field_count_;\
        static constexpr size_t m_global_begin_count = __COUNTER__;\


#define LOCAL_INDEX __COUNTER__ - m_global_begin_count + m_parent_field_count - 1
    
#define STATIC_PROPERTY_DEF(argtype, argname)\
    public:\
        argtype argname;     \
    private:\
        static constexpr size_t local_count_##argname = LOCAL_INDEX;\
    public:\
        template <typename T>                     \
        struct FIELD<T, local_count_##argname> {\
            T& obj;                               \
            FIELD(T& obj): obj(obj) {}            \
            auto value()->decltype(auto) {        \
                return (obj.STRIP(argname));          \
            }                                     \
            static constexpr const char* name() { \
                return STRING(STRIP(argname));        \
            }                                     \
        };    


#define STATIC_PROPERTY_DEF_ARRAY(argtype, argname)\
    public:\
        TBVector<argtype> argname;     \
    private:\
        static constexpr size_t local_count_##argname = LOCAL_INDEX;\
    public:\
        template <typename T>                     \
        struct FIELD<T, local_count_##argname> {\
            T& obj;                               \
            FIELD(T& obj): obj(obj) {}            \
            auto value()->decltype(auto) {        \
                return (obj.STRIP(argname));          \
            }                                     \
            static constexpr const char* name() { \
                return STRING(STRIP(argname));        \
            }                                     \
        };

#define STATIC_PROPERTY_CLASS_END()\
    public:static constexpr size_t _field_count_ = LOCAL_INDEX;
    



    
   
template<size_t index, typename T>
inline constexpr auto& get_property_value(T&& obj)
{
    using TDECAY = std::decay_t<T>;
    return TDECAY::template FIELD<T, index>(std::forward<T>(obj)).value();
}

template<size_t index, typename T>
inline constexpr const char* get_property_name()
{
    using TDECAY = std::decay_t<T>;
    return TDECAY::template FIELD<T, index>::name();
}

template<size_t index, typename T>
inline constexpr const char* get_property_name(T&& obj)
{
    using TDECAY = std::decay_t<T>;
    return TDECAY::template FIELD<T, index>::name();
}


template<typename T>
class IsReflected {
private:
    // 检查当前类是否包含 _field_count_
    template<typename U, typename = decltype(std::declval<U>()._field_count_)>
    static std::true_type test(int);

    // 如果没有这个成员则选择此重载
    template<typename>
    static std::false_type test(...);

    // 递归检查基类
    template<typename U>
    static auto check_base(int) -> decltype(IsReflected<typename std::remove_reference<decltype(std::declval<U>()._field_count_)>::type>::value, std::true_type());

    template<typename>
    static std::false_type check_base(...);

    // 检查是否为 std::vector
    template<typename U>
    static auto check_vector(int) -> decltype(IsReflected<typename U::value_type>::value, std::true_type());

    template<typename>
    static std::false_type check_vector(...);
public:
    static constexpr bool value = decltype(test<T>(0))::value || decltype(check_base<T>(0))::value;
};

template<typename T>
constexpr static bool IsReflected_t = IsReflected<T>::value;

#define IsReflected_v(var) IsReflected<std::remove_reference_t<decltype(var)>>::value

template <typename, template <typename...> class>
struct is_specialization : std::false_type {};

template <template <typename...> class Template, typename... Args>
struct is_specialization<Template<Args...>, Template> : std::true_type {};

template <typename T>
constexpr static bool IsArray_t = is_specialization<T, TBVector>::value;

#define IsArray_v(var) IsArray_t<std::remove_reference<decltype(var)>::type>

#define IsInt_v(var) IsType<int>(var)

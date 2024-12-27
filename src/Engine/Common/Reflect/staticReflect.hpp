#pragma once
#include "metaMarcos.hpp"

#include <type_traits>
#include <utility>
#include <cstddef>
#include <functional>
#include <vector>


#include "common.hpp"

//非继承
#define STATIC_PROPERTY_CLASS_BEGIN(argclass)\
    public:                                  \
        template <typename, size_t> struct FIELD; \
        static constexpr size_t m_parent_field_count = 0;\
        static constexpr size_t m_global_begin_count = __COUNTER__;\
        static constexpr const char* classname(){return STRING(STRIP(argclass));}\

//继承,todo
#define STATIC_PROPERTY_CLASS_BEGIN_PARENT(argclass, argparent)\
    public:                                  \
        template <typename, size_t> struct FIELD; \
        static constexpr size_t m_parent_field_count = argparent::_field_count_;\
        static constexpr size_t m_global_begin_count = __COUNTER__ + m_parent_field_count;\
        static constexpr const char* classname(){return STRING(STRIP(argclass));}\


#define LOCAL_INDEX __COUNTER__ - m_global_begin_count - m_parent_field_count - 1
    
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


template<typename T, typename = void>
struct IsReflected : std::false_type { };

template<typename T>
struct IsReflected<T, std::void_t<decltype(&T::_field_count_)>> : std::true_type { };

template<typename T>
constexpr static bool IsReflected_t = IsReflected<T>::value;

#define IsReflected_v(var) IsReflected<std::remove_reference_t<decltype(var)>>::value

#define IsArray_v(var) std::is_same_v<std::remove_reference<decltype(var)>::type, TBVector<typename std::remove_reference<decltype(var)>::type::value_type>>
#define IsInt_v(var) IsType<int>(var)

template<typename T, typename F, size_t... Is>
inline constexpr void forEachProperty(T&& obj, F&& f, std::index_sequence<Is...>) {
    using TDECAY = std::decay_t<T>;
    (void(f(typename TDECAY::template FIELD<T, Is>::name(),
        typename TDECAY::template FIELD<T, Is>(std::forward<T>(obj)).value())), ...);
}

template<typename T, typename F>
inline constexpr void forEachProperty(T&& obj, F&& f) {
    forEachProperty(std::forward<T>(obj),
        std::forward<F>(f),
        std::make_index_sequence<std::decay_t<T>::_field_count_>{});
}

// 递归回调，父级优先
template <typename T,typename F>
inline constexpr void forEachProperty_rec(T&& object, F&& f, size_t depth = 0)
{
    forEachProperty(std::forward<T>(object), [&](auto&& fieldName, auto&& value) {
        f(fieldName, std::forward<decltype(value)>(value), depth);
        if constexpr (IsReflected_v(value)) {
            forEachProperty_rec(value, f, depth + 1);
        }
        
        });
}

//递归回调，子级优先
template <typename T, typename F>
inline constexpr void forEachProperty_rev(T&& object, F&& f, size_t depth = 0)
{
	forEachProperty(std::forward<T>(object), [&](auto&& fieldName, auto&& value) {
		if constexpr (IsReflected_v(value)) {
            forEachProperty_rev(value, f, depth + 1);
		}
        f(fieldName, std::forward<decltype(value)>(value), depth);
		});
}


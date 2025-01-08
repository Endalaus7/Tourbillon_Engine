#pragma once
#include "dynamicReflect.hpp"
#include "staticReflect.hpp"



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

#define METHOD_PROPERTY(Listener) std::bind(&Listener, this, std::placeholders::_1,std::placeholders::_2,std::placeholders::_3)
#define FUNCTION_PROPERTY(Listener) std::bind(&Listener, std::placeholders::_1,std::placeholders::_2,std::placeholders::_3)

// 递归回调，父级优先
template <typename T, typename F>
inline constexpr void forEachProperty_rec(T&& object, F&& f, size_t depth = 0)
{
    forEachProperty(std::forward<T>(object), [&](auto&& fieldName, auto&& value) {
        f(fieldName, std::forward<decltype(value)>(value), depth);
        if constexpr (IsReflected_v(value)) {
            forEachProperty_rec(value, f, depth + 1);
            if constexpr (IsArray_v(value))
            {
                for (auto& itr : value)
                    forEachProperty_rec(itr, f, depth + 1);
            }
        }

        });
}

//递归回调，子级优先
template <typename T, typename F>
inline constexpr void forEachProperty_rev(T&& object, F&& f, size_t depth = 0)
{
    forEachProperty(std::forward<T>(object), [&](auto&& fieldName, auto&& value) {
        if constexpr (IsReflected_v(value)) {
            if (IsArray_v(value))
            {
                int a = 0;
                //for (auto& itr : value)
                //    forEachProperty_rev(itr, f, depth + 1);
            }
            forEachProperty_rev(value, f, depth + 1);
        }
        f(fieldName, std::forward<decltype(value)>(value), depth);
        });
}


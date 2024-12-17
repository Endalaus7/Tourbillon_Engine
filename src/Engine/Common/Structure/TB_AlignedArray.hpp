#pragma once

#include "common.hpp"
#include "log.hpp"
#include <memory>
#include <malloc.h> // std::aligned_alloc

#define DATA_TYPE typename std::aligned_storage<sizeof(T), alignof(T)>::type


#ifdef _DEBUG
static size_t debug_id_manager = 0;
#endif

template<typename T>
class Dll_Export TBAlignedArray
{
private:
	size_t _size = 0; 
	std::size_t _alignment; // 对齐字节数
    //DATA_TYPE* _data = nullptr;               // 内存块指针
    T* _data = nullptr;
#ifdef _DEBUG 
    size_t _debug_id = 0;
#endif
public:
	FORCE_INLINE size_t size() const noexcept { return _size; }
	FORCE_INLINE size_t alignment() const noexcept { return _alignment; }
    FORCE_INLINE T* data() noexcept { return reinterpret_cast<T*>(_data); }
    FORCE_INLINE const T* data() const noexcept { return reinterpret_cast<T*>(_data); }
    FORCE_INLINE const T* copy() const noexcept 
    {
        //DATA_TYPE* copy_data = static_cast<DATA_TYPE*>(_aligned_malloc(_size * sizeof(T), _alignment));
        T* copy_data = static_cast<T*>(_aligned_malloc(_size * sizeof(T), _alignment));
        std::copy(_data, _data + _size, copy_data);
        //return reinterpret_cast<T*>(_data);
    }
public:
    TBAlignedArray() :_size(0), _alignment(0), _data(nullptr) {}
    TBAlignedArray(const TBAlignedArray& obj)
    {
        _size = obj.size();
        apply(obj.size(), obj.alignment());
        std::copy(obj.data(), obj.data() + _size, _data);

    }
    TBAlignedArray(std::size_t size, std::size_t alignment = GPU_Aligned_Para)
        :_data(nullptr) 
    {
        apply(size, alignment);

    }
    ~TBAlignedArray() noexcept {
        if (!_data)
            return;

        for (size_t i = 0; i < _size; ++i) {
             //reinterpret_cast<T*>(&_data[i])->~T(); // 显式调用析构函数
        }
        _aligned_free(_data);
        _data = 0;
        _size = 0;

    }

    FORCE_INLINE void apply(std::size_t size, std::size_t alignment = GPU_Aligned_Para)noexcept
    {
        if (!size)return;
        
        //_data = static_cast<DATA_TYPE*>(_aligned_malloc(size * sizeof(T), alignment));
        _data = static_cast<T*>(_aligned_malloc(size * sizeof(T), alignment));
#ifdef _DEBUG 
        if (!_data) {
            LOG_ERROR("Aligned array alloc failed");
        }

        _debug_id = debug_id_manager;
        debug_id_manager++;
#endif
        for(size_t i = 0; i < size; ++i)
            new (&_data[i]) T();

        _size = size;
        _alignment = alignment;
    }


    inline T& operator[](std::size_t index) noexcept {
#ifdef _DEBUG 
        if (index >= _size) {
            LOG_ERROR("Aligned array Index out of range");
        }
#endif
        return reinterpret_cast<T*>(&_data[index])[0];
    }

    inline const T& operator[](std::size_t index) const noexcept {
#ifdef _DEBUG 
        if (index >= _size) {
            LOG_ERROR("Aligned array Index out of range");
        }
#endif
        return reinterpret_cast<T*>(&_data[index])[0];
    }

    void resize(std::size_t new_size, std::size_t new_alignment = GPU_Aligned_Para)noexcept {
        new_alignment = new_alignment ? new_alignment : _alignment;
        if (new_size == _size && new_alignment == _alignment) return;

        T* new_data = static_cast<T*>(_aligned_malloc(new_size * sizeof(T), _alignment));
#ifdef _DEBUG 
        if (!new_data) {
            LOG_ERROR("Aligned array resize failed");
        }
#endif

        // 如果新数组比旧数组大，拷贝旧数组数据
        std::size_t copy_size = std::min(_size, new_size);
        std::copy(_data, _data + copy_size, new_data);

        _aligned_free(_data);

        _data = new_data;
        _size = new_size;
        _alignment = new_alignment;
    }

    // 支持迭代器
    T* begin() { return reinterpret_cast<T*>(_data); }
    T* end() { return reinterpret_cast<T*>(_data + _size); }

    template<typename U>
    TBAlignedArray<U> convert()
    {
        TBAlignedArray<U> trans_array(_size);
		for (size_t i = 0; i < _size; ++i) {
            trans_array.data()[i] = static_cast<U>(_data[i]); 
		}
        return trans_array;
    }
};
#pragma once
#include <iostream>
#include <mutex>
#include <fstream>
#include <vector>

#define Real float
#define FORCE_INLINE __forceinline
#define Dll_Export __declspec(dllexport)
#define Dll_Import __declspec(dllimport)
#define GPU_Aligned_Para 8
#define M_PI 3.14159265358979

#define SAFE_DELETE(ptr) \
    do { if (ptr) { delete ptr; ptr = nullptr; } } while (0)

#define SAFE_DELETE_ARRAY(ptr) \
    do { if (ptr) { delete ptr[]; ptr = nullptr; } } while (0)

#define SAFE_CALL_DESTRUCTOR(ptr) \
    do { if (ptr) { ptr->~decltype(*ptr)();  ptr = nullptr; } } while (0)

#define TBVector std::vector
#define TBType uint32_t
#define TBIndex uint32_t

namespace TourBillon
{
    template <typename T>
    class Singleton {
    public:
        Singleton(const Singleton&) = delete;
        Singleton& operator=(const Singleton&) = delete;

        static T* Instance() {
            if (instance_ptr == nullptr)
                instance_ptr = new T();
            return instance_ptr;
        }

    protected:
        Singleton() = default;
        virtual ~Singleton() = default;

    private:
        static T* instance_ptr;
    };

    template <typename T>
    T* Singleton<T>::instance_ptr = nullptr;

}


#define GENERATE_PRIVATE_INTERFACE(memberType, memberName)                     \
private:                                                           \
    memberType memberName;                                             \
public:                                                            \
    const memberType& get##memberName() const {return memberName; }        \
    void set##memberName(const memberType& value) {memberName = value;}    
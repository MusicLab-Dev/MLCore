/**
 * @ Author: Matthieu Moinvaziri
 * @ Description: UniqueAlloc
 */

#pragma once

#include <memory>
#include <memory_resource>

namespace Core
{
    template<typename Type>
    class UniqueAlloc;
}

/** @brief This class provide instances of a types allocated within a shared static allocator */
template<typename Type, typename Allocator = std::pmr::polymorphic_allocator>
class Core::UniqueAlloc
{
public:
    /** @brief Deleter used to release instances */
    struct Deleter
    {
        void operator()(Type *data)
        {
            data->~Type();
            _Allocator.deallocate(data);
        }
    };


    /** @brief Default constructor */
    UniqueAlloc(void) noexcept = default;

    /** @brief Move constructor */
    UniqueAlloc(UniqueAlloc &&other) noexcept = default;

    /** @brief Allocate constructor */
    template<typename ...Args>
    UniqueAlloc(Args &&...args) noexcept_constructible(Type, Args...);

    /** @brief Destructor */
    ~UniqueAlloc(void) noexcept_destructible(Type) = default;

    /** @brief Move assignment */
    UniqueAlloc &operator=(UniqueAlloc &&other) noexcept = default;

    /** @brief Swap two instances */
    void swap(UniqueAlloc &other) noexcept { _data.swap(other._data); }


    /** @brief Instance getter */
    [[nodiscard]] Type *get(void) noexcept { return get(); }
    [[nodiscard]] const Type *get(void) const noexcept { return get(); }

    /** @brief Access operator */
    [[nodiscard]] Type *operator->(void) noexcept { return get(); }
    [[nodiscard]] const Type *operator->(void) const noexcept { return get(); }

    /** @brief Dereference operator */
    [[nodiscard]] Type &operator*(void) noexcept { return *get(); }
    [[nodiscard]] const Type &operator*(void) const noexcept { return *get(); }


    /** @brief Destroy and release the instance */
    void release(void) noexcept { _data.reset(); }

private:
    std::unique_ptr<Type, Deleter> _data {};

    static inline Allocator _Allocator {};
};
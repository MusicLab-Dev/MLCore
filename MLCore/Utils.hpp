/**
 * @ Author: Matthieu Moinvaziri
 * @ Description: A set of utility used globally in the framework
 */

#pragma once

#include <type_traits>
#include <cinttypes>

/** @brief Various exception helpers */
#define nothrow_constructible(Type, ...) std::is_nothrow_constructible_v<Type __VA_OPT__(,) __VA_ARGS__>
#define nothrow_copy_constructible(Type) std::is_nothrow_copy_constructible_v<Type>
#define nothrow_move_constructible(Type) std::is_nothrow_move_constructible_v<Type>
#define nothrow_forward_constructible(Type) (std::is_move_constructible_v<Type> ? nothrow_move_constructible(Type) : nothrow_copy_constructible(Type))
#define nothrow_copy_assignable(Type) std::is_nothrow_copy_assignable_v<Type>
#define nothrow_move_assignable(Type) std::is_nothrow_move_assignable_v<Type>
#define nothrow_forward_assignable(Type) (std::is_move_assignable_v<Type> ? nothrow_move_assignable(Type) : nothrow_copy_assignable(Type))
#define nothrow_destructible(Type) std::is_nothrow_destructible_v<Type>
#define nothrow_invokable(Function, ...) std::is_nothrow_invokable_v<Function __VA_OPT__(,) __VA_ARGS__>
#define nothrow_forward_iterator_constructible(Type) (Core::Utils::IsMoveIterator<Type>::Value ? nothrow_move_constructible(Type) : nothrow_copy_constructible(Type))
#define nothrow_convertible(From, To) std::is_nothrow_convertible_v<From, To>
#define nothrow_expr(Expression) noexcept(Expression)

/** @brief Various noexcept helpers */
#define noexcept_constructible(Type, ...) noexcept(nothrow_constructible(Type __VA_OPT__(,) __VA_ARGS__))
#define noexcept_copy_constructible(Type) noexcept(nothrow_copy_constructible(Type))
#define noexcept_move_constructible(Type) noexcept(nothrow_move_constructible(Type))
#define noexcept_forward_constructible(Type) noexcept(nothrow_forward_constructible(Type))
#define noexcept_copy_assignable(Type) noexcept(nothrow_copy_assignable(Type))
#define noexcept_move_assignable(Type) noexcept(nothrow_move_assignable(Type))
#define noexcept_forward_assignable(Type) noexcept(nothrow_forward_assignable(Type))
#define noexcept_destructible(Type) noexcept(nothrow_destructible(Type))
#define noexcept_invokable(Function, ...) noexcept(nothrow_invokable(Function __VA_OPT__(,) __VA_ARGS__))
#define noexcept_convertible(From, To) noexcept(nothrow_convertible(From, To))
#define noexcept_expr(Expression) noexcept(nothrow_expr(Expression))

/** @brief Align a variable / structure to cacheline size */
#define alignas_cacheline alignas(Core::CacheLineSize)
#define alignas_double_cacheline alignas(Core::CacheLineDoubleSize)
#define alignas_half_cacheline alignas(Core::CacheLineHalfSize)
#define alignas_quarter_cacheline alignas(Core::CacheLineQuarterSize)
#define alignas_eighth_cacheline alignas(Core::CacheLineEighthSize)


/** @brief Helpers used to assert alignment of a structure */
#define static_assert_alignof(Type, Alignment) static_assert(alignof(Type) == Alignment, #Type " must be aligned to " #Alignment)
#define static_assert_alignof_cacheline(Type) static_assert_alignof(Type, Core::CacheLineSize)
#define static_assert_alignof_double_cacheline(Type) static_assert_alignof(Type, Core::CacheLineDoubleSize)
#define static_assert_alignof_half_cacheline(Type) static_assert_alignof(Type, Core::CacheLineHalfSize)
#define static_assert_alignof_quarter_cacheline(Type) static_assert_alignof(Type, Core::CacheLineQuarterSize)
#define static_assert_alignof_eighth_cacheline(Type) static_assert_alignof(Type, Core::CacheLineEighthSize)

/** @brief Helpers used to assert size of a structure */
#define static_assert_sizeof(Type, Size) static_assert(sizeof(Type) == Size, #Type " must have a size of " #Size)
#define static_assert_sizeof_cacheline(Type) static_assert_sizeof(Type, Core::CacheLineSize)
#define static_assert_sizeof_double_cacheline(Type) static_assert_sizeof(Type, Core::CacheLineDoubleSize)
#define static_assert_sizeof_half_cacheline(Type) static_assert_sizeof(Type, Core::CacheLineHalfSize)
#define static_assert_sizeof_quarter_cacheline(Type) static_assert_sizeof(Type, Core::CacheLineQuarterSize)
#define static_assert_sizeof_eighth_cacheline(Type) static_assert_sizeof(Type, Core::CacheLineEighthSize)

/** @brief Helpers used to assert that the size and alignment of a structure are equal to themselves and a given value */
#define static_assert_fit(Type, Size) static_assert(sizeof(Type) == alignof(Type) && alignof(Type) == Size, #Type " must have a size of " #Size " and be aligned to " #Size)
#define static_assert_fit_cacheline(Type) static_assert_fit(Type, Core::CacheLineSize)
#define static_assert_fit_double_cacheline(Type) static_assert_fit(Type, Core::CacheLineDoubleSize)
#define static_assert_fit_half_cacheline(Type) static_assert_fit(Type, Core::CacheLineHalfSize)
#define static_assert_fit_quarter_cacheline(Type) static_assert_fit(Type, Core::CacheLineQuarterSize)
#define static_assert_fit_eighth_cacheline(Type) static_assert_fit(Type, Core::CacheLineEighthSize)

namespace Core
{
    /** @brief Theorical cacheline size */
    constexpr std::size_t CacheLineSize = sizeof(std::size_t) * 8;
    constexpr std::size_t CacheLineDoubleSize = CacheLineSize * 2;
    constexpr std::size_t CacheLineHalfSize = CacheLineSize / 2;
    constexpr std::size_t CacheLineQuarterSize = CacheLineSize / 4;
    constexpr std::size_t CacheLineEighthSize = CacheLineSize / 8;

    namespace Utils
    {
        /** @brief Helper to know if a given type is a std::move_iterator */
        template<typename Type>
        struct IsMoveIterator;

        /** @brief Helper that match non-move iterators */
        template<typename Type>
        struct IsMoveIterator
        {
            static constexpr bool Value = false;
        };

        /** @brief Helper that match move iterators */
        template<typename Iterator>
        struct IsMoveIterator<std::move_iterator<Iterator>> : public IsMoveIterator<Iterator>
        {
            static constexpr bool Value = true;
        };

        /** @brief Helper that match reverse iterators */
        template<typename Iterator>
        struct IsMoveIterator<std::reverse_iterator<Iterator>> : public IsMoveIterator<Iterator>
        {};

        static_assert(IsMoveIterator<std::move_iterator<void *>>::Value, "IsMoveIterator not working");
        static_assert(IsMoveIterator<std::reverse_iterator<std::move_iterator<void *>>>::Value, "IsMoveIterator not working");
        static_assert(!IsMoveIterator<std::reverse_iterator<void *>>::Value, "IsMoveIterator not working");
        static_assert(!IsMoveIterator<void *>::Value, "IsMoveIterator not working");


        /** @brief Default type used when a detection fails */
        struct NoneSuch {};

        /** @brief Detector detected invalid expression */
        template<typename Default, typename AlwaysVoid, template<typename...> typename _Op, typename... _Args>
        struct Detector
        {
            using Value = std::false_type;
            using Type = Default;
        };

        /** @brief Detector detected valid expression */
        template<typename Default, template<typename...> typename Op, typename... Args>
        struct Detector<Default, std::void_t<Op<Args...>>, Op, Args...>
        {
            using Value = std::true_type;
            using Type = Op<Args...>;
        };

        /** @brief Boolean that indicate if detector detected a valid expression or not */
        template<template<typename...> class Op, typename... Args>
        constexpr bool IsDetected = Detector<NoneSuch, void, Op, Args...>::Value::value;

        /** @brief Type of the detected expression, if the detection failed returns NoneSuch */
        template<template<typename...> class Op, typename... Args>
        using DetectedType = typename Detector<NoneSuch, void, Op, Args...>::Type;

        /** @brief Type of the detected expression, if the detection failed returns NoneSuch */
        template<typename Default, template<typename...> class Op, typename... Args>
        using DetectedOrType = typename Detector<Default, void, Op, Args...>::Type;

        /** @brief Check if the expression perfectly match a type */
        template<typename Expected, template<typename...> class Op, typename... Args>
        constexpr bool IsDetectedExact = std::is_same_v<Expected, DetectedType<Op, Args...>>;

        /** @brief Check if the expression is convertible to a type */
        template<typename Convertible, template<typename...> class Op, typename... Args>
        constexpr bool IsDetectedConvertible = std::is_convertible_v<Convertible, DetectedType<Op, Args...>>;
    }
}
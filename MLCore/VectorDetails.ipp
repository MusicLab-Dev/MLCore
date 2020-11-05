/**
 * @ Author: Matthieu Moinvaziri
 * @ Description: VectorDetails
 */

template<typename Base, typename Type, typename Range>
template<typename ...Args>
inline Type &Core::Internal::VectorDetails<Base, Type, Range>::push(Args &&...args)
    noexcept(std::is_nothrow_constructible_v<Type, Args...> && nothrow_destructible(Type))
{
    if (!data())
        reserve(2);
    else if (sizeUnsafe() == capacityUnsafe())
        grow();
    const auto currentSize = sizeUnsafe();
    Type * const elem = dataUnsafe() + currentSize;
    setSize(currentSize + 1);
    new (elem) Type(std::forward<Args>(args)...);
    return *elem;
}

template<typename Base, typename Type, typename Range>
inline void Core::Internal::VectorDetails<Base, Type, Range>::pop(void) noexcept_destructible(Type)
{
    const auto desiredSize = sizeUnsafe() - 1;

    dataUnsafe()[desiredSize].~Type();
    setSize(desiredSize);
}

template<typename Base, typename Type, typename Range>
template<typename InputIterator>
inline std::enable_if_t<std::is_constructible_v<Type, decltype(*std::declval<InputIterator>())>, typename Core::Internal::VectorDetails<Base, Type, Range>::Iterator>
    Core::Internal::VectorDetails<Base, Type, Range>::insert(const Iterator pos, const InputIterator from, const InputIterator to)
    noexcept(nothrow_forward_constructible(Type) && nothrow_destructible(Type))
{
    const std::size_t count = std::distance(from, to);
    std::size_t position;

    if (!count)
        return end();
    else if (pos == Iterator()) {
        reserve(count);
        position = 0;
    } else
        position = pos - beginUnsafe();
    const auto currentSize = sizeUnsafe();
    if (const auto currentCapacity = capacityUnsafe(), total = currentSize + count; total > currentCapacity) {
        const auto currentData = dataUnsafe();
        const auto desiredCapacity = currentCapacity + std::max(currentCapacity, count);
        const auto tmpData = allocate(desiredCapacity);
        std::uninitialized_move_n(currentData, position, tmpData);
        std::uninitialized_move_n(currentData + position, count, tmpData + position + count);
        std::copy(from, to, tmpData + position);
        deallocate(currentData);
        setData(tmpData);
        setSize(total);
        setCapacity(desiredCapacity);
        return tmpData + position;
    }
    const auto currentBegin = beginUnsafe();
    const auto currentEnd = endUnsafe();
    if (const auto after = currentSize - position; after > count) {
        std::uninitialized_move(currentEnd - count, currentEnd, currentEnd);
        std::copy(from, to, currentBegin + position);
    } else {
        auto mid = from;
        std::advance(mid, after);
        std::uninitialized_move(mid, to, currentEnd);
        std::uninitialized_move(currentBegin + position, currentEnd, currentEnd + count - after);
        std::copy(from, to, currentBegin + position);
    }
    setSize(currentSize + count);
    return currentBegin + position;
}

template<typename Base, typename Type, typename Range>
inline typename Core::Internal::VectorDetails<Base, Type, Range>::Iterator
    Core::Internal::VectorDetails<Base, Type, Range>::insert(const Iterator pos, const std::size_t count, const Type &value)
    noexcept(nothrow_copy_constructible(Type) && nothrow_destructible(Type))
{
    if (!count)
        return end();
    else if (pos == nullptr) {
        resize(count, value);
        return beginUnsafe();
    }
    std::size_t position = pos - beginUnsafe();
    const auto currentBegin = beginUnsafe();
    const auto currentEnd = endUnsafe();
    const auto currentSize = sizeUnsafe();
    if (const auto currentCapacity = capacityUnsafe(), total = currentSize + count; total > currentCapacity) {
        const auto desiredCapacity = currentCapacity + std::max(currentCapacity, count);
        const auto tmpData = allocate(desiredCapacity);
        std::uninitialized_move_n(currentBegin, position, tmpData);
        std::uninitialized_move(currentBegin + position, currentEnd, tmpData + position + count);
        std::fill_n(tmpData + position, count, value);
        deallocate(data());
        setData(tmpData);
        setSize(total);
        setCapacity(desiredCapacity);
        return tmpData + position;
    } else if (const auto after = sizeUnsafe() - position; after > count) {
        std::uninitialized_move(currentEnd - count, currentEnd, currentEnd);
        std::fill_n(currentBegin + position, count, value);
    } else {
        const auto mid = count - after;
        std::fill_n(currentEnd, mid, value);
        std::uninitialized_move(currentBegin + position, currentEnd, currentEnd + mid);
        std::fill_n(currentBegin + position, count - mid, value);
    }
    setSize(currentSize + count);
    return currentBegin + position;
}

template<typename Base, typename Type, typename Range>
inline void Core::Internal::VectorDetails<Base, Type, Range>::erase(const Iterator from, const Iterator to)
    noexcept(nothrow_forward_constructible(Type) && nothrow_destructible(Type))
{
    if (from == to)
        return;
    const auto end = endUnsafe();
    setSize(sizeUnsafe() - std::distance(from, to));
    if constexpr (std::is_move_assignable_v<Type> && !Utils::IsMoveIterator<Iterator>::Value)
        std::copy(std::make_move_iterator(to), std::make_move_iterator(end), from);
    else
        std::copy(to, end, from);
    std::destroy(to, end);
}

template<typename Base, typename Type, typename Range>
inline void Core::Internal::VectorDetails<Base, Type, Range>::resize(const std::size_t count)
    noexcept(std::is_nothrow_constructible_v<Type> && nothrow_destructible(Type))
{
    if (!count) {
        clear();
        return;
    } else if (!data())
        reserveUnsafe<false>(count);
    else if (capacityUnsafe() < count)
        reserveUnsafe<true>(count);
    else
        clearUnsafe();
    setSize(count);
    std::uninitialized_default_construct_n(data(), count);
}

template<typename Base, typename Type, typename Range>
inline void Core::Internal::VectorDetails<Base, Type, Range>::resize(const std::size_t count, const Type &value)
    noexcept(nothrow_copy_constructible(Type) && nothrow_destructible(Type))
{
    if (!count) {
        clear();
        return;
    } else if (!data())
        reserveUnsafe<false>(count);
    else if (capacityUnsafe() < count)
        reserveUnsafe<true>(count);
    else
        clearUnsafe();
    setSize(count);
    std::uninitialized_fill_n(data(), count, value);
}

template<typename Base, typename Type, typename Range>
template<typename InputIterator>
inline std::enable_if_t<std::is_constructible_v<Type, decltype(*std::declval<InputIterator>())>, void>
    Core::Internal::VectorDetails<Base, Type, Range>::resize(const InputIterator from, const InputIterator to)
    noexcept(nothrow_destructible(Type) && nothrow_forward_iterator_constructible(InputIterator))
{
    const std::size_t count = std::distance(from, to);

    if (!count) {
        clear();
        return;
    } else if (!data())
        reserveUnsafe<false>(count);
    else if (capacityUnsafe() < count)
        reserveUnsafe<true>(count);
    else
        clearUnsafe();
    setSize(count);
    std::uninitialized_copy(from, to, beginUnsafe());
}

template<typename Base, typename Type, typename Range>
inline void Core::Internal::VectorDetails<Base, Type, Range>::clear(void) noexcept_destructible(Type)
{
    if (data())
        clearUnsafe();
}

template<typename Base, typename Type, typename Range>
inline void Core::Internal::VectorDetails<Base, Type, Range>::clearUnsafe(void) noexcept_destructible(Type)
{
    std::destroy_n(dataUnsafe(), sizeUnsafe());
    setSize(0);
}

template<typename Base, typename Type, typename Range>
inline void Core::Internal::VectorDetails<Base, Type, Range>::release(void) noexcept_destructible(Type)
{
    if (data())
        releaseUnsafe();
}

template<typename Base, typename Type, typename Range>
inline void Core::Internal::VectorDetails<Base, Type, Range>::releaseUnsafe(void) noexcept_destructible(Type)
{
    clearUnsafe();
    deallocate(dataUnsafe());
    setCapacity(0);
    setData(nullptr);
}

template<typename Base, typename Type, typename Range>
inline bool Core::Internal::VectorDetails<Base, Type, Range>::reserve(const Range capacity)
    noexcept(nothrow_forward_constructible(Type) && nothrow_destructible(Type))
{
    if (data())
        return reserveUnsafe<true>(capacity);
    else
        return reserveUnsafe<false>(capacity);
}

template<typename Base, typename Type, typename Range>
template<bool IsSafe>
inline bool Core::Internal::VectorDetails<Base, Type, Range>::reserveUnsafe(const Range capacity)
    noexcept(nothrow_forward_constructible(Type) && nothrow_destructible(Type))
{
    if constexpr (IsSafe) {
        if (capacityUnsafe() >= capacity)
            return false;
        const auto currentSize = sizeUnsafe();
        const auto currentData = dataUnsafe();
        const auto tmpData = allocate(capacity);
        std::uninitialized_move_n(currentData, currentSize, tmpData);
        std::destroy_n(currentData, currentSize);
        deallocate(currentData);
        setData(tmpData);
        setSize(currentSize);
        setCapacity(capacity);
        return true;
    } else {
        setData(allocate(capacity));
        setSize(0);
        setCapacity(capacity);
        return true;
    }
}

template<typename Base, typename Type, typename Range>
inline void Core::Internal::VectorDetails<Base, Type, Range>::grow(const Range minimum)
    noexcept(nothrow_forward_constructible(Type) && nothrow_destructible(Type))
{
    const auto currentData = dataUnsafe();
    const auto currentSize = sizeUnsafe();
    const auto currentCapacity = capacityUnsafe();
    const auto desiredCapacity = currentCapacity + std::max(currentCapacity, minimum);
    const auto tmpData = allocate(desiredCapacity);

    std::uninitialized_move_n(currentData, currentSize, tmpData);
    std::destroy_n(currentData, currentSize);
    deallocate(currentData);
    setData(tmpData);
    setSize(currentSize);
    setCapacity(desiredCapacity);
}

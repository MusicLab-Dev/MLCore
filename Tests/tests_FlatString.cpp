/**
 * @ Author: Matthieu Moinvaziri
 * @ Description: Tests of the single consumer concurrent queue
 */

#include <gtest/gtest.h>

#include <MLCore/FlatString.hpp>

TEST(FlatString, Basics)
{
    constexpr const char *value = "hello world";
    char array[] = { 'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd' , '\0' };

    auto assertStringValue = [value, &array](const Core::FlatString &str) {
        ASSERT_EQ(str, value);
        ASSERT_EQ(str, array);
        ASSERT_EQ(str, std::string(value));
        ASSERT_EQ(str, std::string_view(value));
    };

    // Constructors
    Core::FlatString str(value);
    assertStringValue(str);
    str.release();
    new (&str) Core::FlatString(array);
    assertStringValue(str);
    str.release();
    new (&str) Core::FlatString(std::string(value));
    assertStringValue(str);
    str.release();
    new (&str) Core::FlatString(std::string_view(value));
    assertStringValue(str);

    // Assignments
    str = nullptr;
    ASSERT_EQ(str, nullptr);
    str = value;
    assertStringValue(str);
    str = array;
    assertStringValue(str);
    str = std::string(value);
    assertStringValue(str);
    str = std::string_view(value);
}
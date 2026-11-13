#include <cassert>
#include <cstdint>
#include <string>

#include "simple_variant.h"

int main() {
    const SimpleVariant empty;
    assert(empty.holds_alternative<void>());
    assert(!empty.holds_alternative<float>());

    const SimpleVariant integer{std::uint16_t{343}};
    assert(integer.holds_alternative<std::uint16_t>());
    assert(!integer.holds_alternative<float>());
    assert(integer.get<std::uint16_t>() == 343);
    assert(integer.get<float>() == 343.0f);

    const SimpleVariant decimal{34.3f};
    assert(decimal.holds_alternative<float>());
    assert(!decimal.holds_alternative<std::uint16_t>());
    assert(decimal.get<float>() == 34.3f);

    const SimpleVariant flag{true};
    assert(flag.holds_alternative<bool>() && flag.get<bool>());
    const SimpleVariant text{std::string{"Programm"}};
    assert(text.holds_alternative<std::string>() && text.get<std::string>() == "Programm");
    const SimpleVariant copy{text};
    assert(copy.holds_alternative<std::string>() && copy.get<std::string>() == "Programm");
}

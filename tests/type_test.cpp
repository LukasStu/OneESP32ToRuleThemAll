#include <cassert>
#include <string>

#include "type.h"

int main() {
    assert(GetRawByType(SimpleVariant(23.7f), Type::et_dec_val) == 237);
    assert(GetRawByType(SimpleVariant(-5.3f), Type::et_dec_val) == 0xffcb);
    assert(GetRawByType(SimpleVariant(1.234f), Type::et_dec_val) == 12);
    assert(GetRawByType(SimpleVariant(1.25f), Type::et_cent_val) == 125);
    assert(GetRawByType(SimpleVariant(-1.25f), Type::et_mil_val) == 0xfb1e);
    assert(GetRawByType(SimpleVariant(42.0f), Type::et_default) == 42);
    assert(GetRawByType(SimpleVariant(4660.0f), Type::et_little_endian) == 0x3412);
    assert(GetRawByType(SimpleVariant(true), Type::et_little_bool) == 0x0100);
    assert(GetRawByType(SimpleVariant(1.0f), Type::et_bool) == std::nullopt);
    assert(GetRawByType(SimpleVariant(std::string("12:34")), Type::et_zeit) == 0x0c22);
    assert(GetRawByType(SimpleVariant(std::string("invalid")), Type::et_zeit) == std::nullopt);
    assert(GetRawByType(SimpleVariant(std::string("Komfort")), Type::et_betriebsart) == 0x0300);
    assert(GetRawByType(SimpleVariant(std::string("Hochdruck")), Type::et_err_nr) == 0x0004);
    assert(GetValueByType(*GetRawByType(SimpleVariant(-5.3f), Type::et_dec_val), Type::et_dec_val).get<float>() == -5.3f);
}

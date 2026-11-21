#include <cassert>
#include <string>

#include "property.h"
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
    assert(GetValueByType(239, Property::kPUFFERSOLLTEMP.type).get<float>() == 23.9f);
    assert(GetValueByType(41, Property::kHEIZUNGSDRUCK_WPL.type).get<float>() == 0.41f);
    assert(GetValueByType(2479, Property::kVOLUMENSTROM_WPL.type).get<float>() == 24.79f);
    assert(GetValueByType(40, Property::kANLAGENFROST.type).get<float>() == 4.0f);
    assert(GetValueByType(55, Property::kSTEIGUNG_HK1.type).get<float>() == 0.55f);
    assert(GetValueByType(0x1654, Property::kHEIZPROG_1_MO_SO.type).get<std::string>() == "05:30-21:00");
    assert(GetValueByType(0x8000, Property::kHEIZPROG_1_MO_SO.type).get<std::string>() == "xx:xx-xx:xx?");
    assert(GetValueByType(491, Property::kRUECKLAUFTEMP_WP_VERMUTUNG.type).get<float>() == 49.1f);
    assert(GetValueByType(520, Property::kVORLAUFTEMP_VERMUTUNG.type).get<float>() == 52.0f);
    assert(GetValueByType(65336, Property::kEINSATZGRENZE_HZG_VERMUTUNG.type).get<float>() == -20.0f);
    assert(GetValueByType(466, Property::kSAUGGASTEMP_VERDICHTER.type).get<float>() == 46.6f);
    assert(GetValueByType(378, Property::kVERDAMPFEREINGANGSTEMP_VERMUTUNG.type).get<float>() == 37.8f);
    assert(GetValueByType(0x0100, Property::kSOMMERBETRIEB.type).get<bool>());
    assert(!GetValueByType(0x0000, Property::kSOMMERBETRIEB.type).get<bool>());
    assert(GetValueByType(0x0001, Property::kKUEHLEN.type).get<bool>());
    assert(!GetValueByType(0x0000, Property::kKUEHLEN.type).get<bool>());
    assert(Property(0x01D7).name == "SOLLTEMP_HK1");
}

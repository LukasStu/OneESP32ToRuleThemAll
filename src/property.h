#if !defined(PROPERTY_H)
#define PROPERTY_H

#include <cstdint>
#include <sstream>
#include <string>
#include <unordered_map>
#include "type.h"

namespace detail {
struct Property {
    std::string_view name;
    std::uint16_t id{0U};
    Type type{Type::et_default};

    constexpr Property(const std::string_view _name, const std::uint16_t _id, const Type _type)
        : name(_name), id(_id), type(_type) {}
    constexpr Property(const std::string_view _name, const std::uint16_t _id) : name(_name), id(_id) {}

    constexpr operator std::uint16_t() const { return id; }

    Property& operator=(const Property& p) {
        name = p.name;
        id = p.id;
        type = p.type;
        return *this;
    }

   protected:
    Property getProperty(const std::uint16_t id);
};

class Mapper {
   public:
    Mapper() = default;
    inline Mapper(const Property& other) { map.insert({other.id, other}); }

    Property getProperty(const std::uint16_t id);

   private:
    static inline std::unordered_map<std::uint16_t, Property> map;
};

}  // namespace detail

#define PROPERTY(NAME, VALUE, ...)                                          \
    static constexpr detail::Property k##NAME{#NAME, VALUE, ##__VA_ARGS__}; \
    static constexpr bool unique##VALUE{true};                              \
    static inline detail::Mapper k##NAME##_MAPPING {                        \
        k##NAME                                                             \
    }

struct Property : public detail::Property {
    constexpr Property(const Property& p) : detail::Property{p.name, p.id, p.type} {}
    constexpr Property(const detail::Property& p) : detail::Property{p.name, p.id, p.type} {}
    Property(const std::uint16_t _id) : detail::Property{getProperty(_id)} {}

    // Internal sentinel and reported error.
    PROPERTY(INDEX_NOT_FOUND, 0x0000);
    PROPERTY(FEHLERMELDUNG, 0x0001);

    // Room control and heating curve. FEK/FET labels follow the existing configuration.
    PROPERTY(KOMFORTTEMPERATUR, 0x0005, Type::et_dec_val);
    PROPERTY(ECOTEMPERATUR, 0x0008, Type::et_dec_val);
    PROPERTY(RAUMISTTEMP, 0x0011, Type::et_dec_val);
    PROPERTY(VERSTELLTE_RAUMSOLLTEMP, 0x0012, Type::et_dec_val);
    PROPERTY(RAUMFEUCHTE, 0x0075, Type::et_dec_val);
    PROPERTY(STEIGUNG_HK1, 0x010e, Type::et_cent_val);
    PROPERTY(RAUMEINFLUSS, 0x010f, Type::et_little_endian);
    PROPERTY(ANTEIL_VORLAUF_HK1, 0x059d);
    PROPERTY(FUSSPUNKT_HK1, 0x059e, Type::et_dec_val);
    PROPERTY(SOLLTEMP_FEK, 0x4e66, Type::et_dec_val);

    // Heating circuit and heat pump temperatures. Do not equate the WP return
    // with the heating-circuit return; the exact sensor assignments need verification.
    PROPERTY(VORLAUFSOLLTEMP, 0x0004, Type::et_dec_val);
    PROPERTY(AUSSENTEMP, 0x000c, Type::et_dec_val);
    PROPERTY(PUFFERISTTEMPERATUR, 0x0078, Type::et_dec_val);
    PROPERTY(PUFFERSOLLTEMP, 0x01d5, Type::et_dec_val);
    PROPERTY(VORLAUFTEMP, 0x01d6, Type::et_dec_val);
    PROPERTY(SOLLTEMP_HK1, 0x01d7, Type::et_dec_val);
    PROPERTY(RUECKLAUFISTTEMP, 0x02ca, Type::et_dec_val);
    PROPERTY(VORLAUFTEMP_06A2, 0x06a2, Type::et_dec_val);
    PROPERTY(RUECKLAUFTEMP_WP, 0x06ac, Type::et_dec_val);
    PROPERTY(VORLAUFISTTEMP, 0xfdf3, Type::et_dec_val);
    PROPERTY(VORLAUFISTTEMP_NHZ, 0xfdf5, Type::et_dec_val);

    // Warm water temperatures and settings.
    PROPERTY(SPEICHERSOLLTEMP, 0x0003, Type::et_dec_val);
    PROPERTY(SPEICHERISTTEMP, 0x000e, Type::et_dec_val);
    PROPERTY(KOMFORTTEMPERATUR_WW, 0x0013, Type::et_dec_val);
    PROPERTY(ECOTEMPERATUR_WW, 0x0a06, Type::et_dec_val);

    // Operating mode, limits and schedule. Status bit meanings are not confirmed.
    PROPERTY(PROGRAMMSCHALTER, 0x0112, Type::et_betriebsart);
    PROPERTY(BIVALENZTEMPERATUR_HZG, 0x01ac, Type::et_dec_val);
    PROPERTY(BIVALENZTEMPERATUR_WW, 0x01ad, Type::et_dec_val);
    PROPERTY(EINSATZGRENZE_HZG, 0x01ae, Type::et_dec_val);
    PROPERTY(EINSATZGRENZE_WW, 0x01af, Type::et_dec_val);
    PROPERTY(STATUS_0692, 0x0692);
    PROPERTY(KUEHLEN, 0x06a4, Type::et_bool);
    PROPERTY(ANLAGENFROST, 0x0a00, Type::et_dec_val);
    PROPERTY(HEIZPROG_1_MO_SO, 0x14a0, Type::et_time_domain);  // Encoding unconfirmed.
    PROPERTY(STATUS_4E5E, 0x4e5e);
    PROPERTY(STATUS_4E6A, 0x4e6a);
    PROPERTY(STATUS_4E7E, 0x4e7e);
    PROPERTY(BETRIEBS_STATUS, 0x4ecd);
    PROPERTY(SOMMERBETRIEB, 0xfdb4, Type::et_little_bool);
    PROPERTY(FROSTSCHUTZ, 0xfe07, Type::et_dec_val);

    // Date and time (little-endian CAN values).
    PROPERTY(WOCHENTAG, 0x0121, Type::et_little_endian);
    PROPERTY(TAG, 0x0122, Type::et_little_endian);
    PROPERTY(MONAT, 0x0123, Type::et_little_endian);
    PROPERTY(JAHR, 0x0124, Type::et_little_endian);
    PROPERTY(STUNDE, 0x0125, Type::et_little_endian);
    PROPERTY(MINUTE, 0x0126, Type::et_little_endian);

    // Hydraulic and refrigerant measurements. YAML applies additional scaling
    // to the refrigerant pressures; the WPL and generic flow IDs differ.
    PROPERTY(TAUPUNKTTEMP_KESSEL, 0x0264, Type::et_dec_val);
    PROPERTY(SAUGGASTEMP_VERDICHTER, 0x063f, Type::et_dec_val);
    PROPERTY(VOLUMENSTROM_WPL, 0x0673, Type::et_cent_val);
    PROPERTY(HEIZUNGSDRUCK_WPL, 0x0674, Type::et_cent_val);
    PROPERTY(DRUCK_HOCHDRUCK, 0x07a6, Type::et_dec_val);
    PROPERTY(DRUCK_NIEDERDRUCK, 0x07a7, Type::et_dec_val);
    PROPERTY(VERDAMPFERTEMP, 0x07a9, Type::et_dec_val);
    PROPERTY(VOLUMENSTROM, 0x4f47, Type::et_dec_val);
    PROPERTY(VERDAMPFEREINGANGSTEMP, 0xc2bf, Type::et_dec_val);

    // Compressor, fan and inverter measurements/counters.
    PROPERTY(ISTDREHZAHL_VERDICHTER_063D, 0x063d);
    PROPERTY(SPANNUNG_INVERTER, 0x06b1, Type::et_dec_val);
    PROPERTY(STROM_INVERTER, 0x06b2, Type::et_dec_val);
    PROPERTY(VD_HEIZBETRIEB, 0x07fc, Type::et_default);
    PROPERTY(VD_KUEHLEN, 0x07ff, Type::et_default);
    PROPERTY(VD_WW_BETRIEB, 0x0802, Type::et_default);
    PROPERTY(VD_ABTAUEN, 0x0808, Type::et_default);
    PROPERTY(VERDICHTER_STARTS_K, 0x4ef0);
    PROPERTY(VERDICHTER_STARTS, 0x4ef1);
    PROPERTY(LUEFTERDREHZAHL_C281, 0xc281);  // Fan assignment unconfirmed.
    PROPERTY(LUEFTERDREHZAHL_C283, 0xc283);  // Fan assignment unconfirmed.

    // Electrical energy and heat yield: separate low/high parts of counters.
    PROPERTY(EL_AUFNAHMELEISTUNG_WW_TAG_WH, 0x091a);
    PROPERTY(EL_AUFNAHMELEISTUNG_WW_TAG_KWH, 0x091b, Type::et_double_val);
    PROPERTY(EL_AUFNAHMELEISTUNG_HEIZ_TAG_WH, 0x091e);
    PROPERTY(EL_AUFNAHMELEISTUNG_HEIZ_TAG_KWH, 0x091f, Type::et_double_val);
    PROPERTY(WAERMEERTRAG_WW_TAG_WH, 0x092a);
    PROPERTY(WAERMEERTRAG_WW_TAG_KWH, 0x092b, Type::et_double_val);
    PROPERTY(WAERMEERTRAG_WW_SUM_KWH, 0x092c);
    PROPERTY(WAERMEERTRAG_WW_SUM_MWH, 0x092d, Type::et_double_val);
    PROPERTY(WAERMEERTRAG_HEIZ_TAG_WH, 0x092e);
    PROPERTY(WAERMEERTRAG_HEIZ_TAG_KWH, 0x092f, Type::et_double_val);
    PROPERTY(WAERMEERTRAG_HEIZ_SUM_KWH, 0x0930);
    PROPERTY(WAERMEERTRAG_HEIZ_SUM_MWH, 0x0931, Type::et_double_val);

    // Experimental temperature labels: the physical sensor assignments are unknown.
    PROPERTY(T_06, 0x0016, Type::et_dec_val);
    PROPERTY(T_03, 0x0199, Type::et_dec_val);
    PROPERTY(T_01, 0x0a36, Type::et_dec_val);
    PROPERTY(T_02, 0xfdf4, Type::et_dec_val);
    PROPERTY(T_04, 0xfe09, Type::et_dec_val);

    // Observed in the WPL13 CAN log, but meaning and encoding are unverified.
    // Keep the original unsigned raw value (et_default); IDs may include protocol
    // or status fields and do not imply a sensor or a usable setting.
    PROPERTY(UNKNOWN_0002, 0x0002);
    PROPERTY(UNKNOWN_0009, 0x0009);
    PROPERTY(UNKNOWN_000A, 0x000a);
    PROPERTY(UNKNOWN_000B, 0x000b);
    PROPERTY(UNKNOWN_000D, 0x000d);
    PROPERTY(UNKNOWN_000F, 0x000f);
    PROPERTY(UNKNOWN_0010, 0x0010);
    PROPERTY(UNKNOWN_0023, 0x0023);
    PROPERTY(UNKNOWN_0028, 0x0028);
    PROPERTY(UNKNOWN_0029, 0x0029);
    PROPERTY(UNKNOWN_002A, 0x002a);
    PROPERTY(UNKNOWN_002C, 0x002c);
    PROPERTY(UNKNOWN_0030, 0x0030);
    PROPERTY(UNKNOWN_0051, 0x0051);
    PROPERTY(UNKNOWN_0052, 0x0052);
    PROPERTY(UNKNOWN_0053, 0x0053);
    PROPERTY(UNKNOWN_0056, 0x0056);
    PROPERTY(UNKNOWN_0057, 0x0057);
    PROPERTY(UNKNOWN_0058, 0x0058);
    PROPERTY(UNKNOWN_005A, 0x005a);
    PROPERTY(UNKNOWN_005D, 0x005d);
    PROPERTY(UNKNOWN_005E, 0x005e);
    PROPERTY(UNKNOWN_005F, 0x005f);
    PROPERTY(UNKNOWN_0060, 0x0060);
    PROPERTY(UNKNOWN_0061, 0x0061);
    PROPERTY(UNKNOWN_0067, 0x0067);
    PROPERTY(UNKNOWN_006E, 0x006e);
    PROPERTY(UNKNOWN_0073, 0x0073);
    PROPERTY(UNKNOWN_0074, 0x0074);
    PROPERTY(UNKNOWN_00F8, 0x00f8);
    PROPERTY(UNKNOWN_00F9, 0x00f9);
    PROPERTY(UNKNOWN_00FD, 0x00fd);
    PROPERTY(UNKNOWN_00FE, 0x00fe);
    PROPERTY(UNKNOWN_019A, 0x019a);
    PROPERTY(UNKNOWN_01D2, 0x01d2);
    PROPERTY(UNKNOWN_01F2, 0x01f2);
    PROPERTY(UNKNOWN_0259, 0x0259);
    PROPERTY(UNKNOWN_025A, 0x025a);
    PROPERTY(UNKNOWN_02C2, 0x02c2);
    PROPERTY(UNKNOWN_0359, 0x0359);
    PROPERTY(UNKNOWN_03E7, 0x03e7);
    PROPERTY(UNKNOWN_05D8, 0x05d8);
    PROPERTY(UNKNOWN_05DC, 0x05dc);
    PROPERTY(UNKNOWN_05DF, 0x05df);
    PROPERTY(UNKNOWN_05E0, 0x05e0);
    PROPERTY(UNKNOWN_0639, 0x0639);
    PROPERTY(UNKNOWN_063B, 0x063b);
    PROPERTY(UNKNOWN_0669, 0x0669);
    PROPERTY(UNKNOWN_068D, 0x068d);
    PROPERTY(UNKNOWN_06AF, 0x06af);
    PROPERTY(UNKNOWN_06FF, 0x06ff);
    PROPERTY(UNKNOWN_079F, 0x079f);
    PROPERTY(UNKNOWN_07A0, 0x07a0);
    PROPERTY(UNKNOWN_07A1, 0x07a1);
    PROPERTY(UNKNOWN_07A2, 0x07a2);
    PROPERTY(UNKNOWN_07A8, 0x07a8);
    PROPERTY(UNKNOWN_07CB, 0x07cb);
    PROPERTY(UNKNOWN_07F2, 0x07f2);
    PROPERTY(UNKNOWN_07FD, 0x07fd);
    PROPERTY(UNKNOWN_07FE, 0x07fe);
    PROPERTY(UNKNOWN_0800, 0x0800);
    PROPERTY(UNKNOWN_0801, 0x0801);
    PROPERTY(UNKNOWN_0803, 0x0803);
    PROPERTY(UNKNOWN_0804, 0x0804);
    PROPERTY(UNKNOWN_0805, 0x0805);
    PROPERTY(UNKNOWN_0809, 0x0809);
    PROPERTY(UNKNOWN_080E, 0x080e);
    PROPERTY(UNKNOWN_081D, 0x081d);
    PROPERTY(UNKNOWN_0828, 0x0828);
    PROPERTY(UNKNOWN_0917, 0x0917);
    PROPERTY(UNKNOWN_0961, 0x0961);
    PROPERTY(UNKNOWN_0A20, 0x0a20);
    PROPERTY(UNKNOWN_101E, 0x101e);
    PROPERTY(UNKNOWN_1388, 0x1388);
    PROPERTY(UNKNOWN_138A, 0x138a);
    PROPERTY(UNKNOWN_4E30, 0x4e30);
    PROPERTY(UNKNOWN_4E7A, 0x4e7a);
    PROPERTY(UNKNOWN_4E8B, 0x4e8b);
    PROPERTY(UNKNOWN_4ED7, 0x4ed7);
    PROPERTY(UNKNOWN_4ED8, 0x4ed8);
    PROPERTY(UNKNOWN_4ED9, 0x4ed9);
    PROPERTY(UNKNOWN_4F13, 0x4f13);
    PROPERTY(UNKNOWN_795A, 0x795a);
    PROPERTY(UNKNOWN_C24C, 0xc24c);
    PROPERTY(UNKNOWN_C24D, 0xc24d);
    PROPERTY(UNKNOWN_C252, 0xc252);
    PROPERTY(UNKNOWN_C255, 0xc255);
    PROPERTY(UNKNOWN_C2C0, 0xc2c0);
    PROPERTY(UNKNOWN_C2C1, 0xc2c1);
    PROPERTY(UNKNOWN_C2C4, 0xc2c4);
    PROPERTY(UNKNOWN_C303, 0xc303);
    PROPERTY(UNKNOWN_FDAA, 0xfdaa);
    PROPERTY(UNKNOWN_FDAE, 0xfdae);
    PROPERTY(UNKNOWN_FE1B, 0xfe1b);
    PROPERTY(UNKNOWN_FE1D, 0xfe1d);
    PROPERTY(UNKNOWN_FE1E, 0xfe1e);
    PROPERTY(UNKNOWN_FE4C, 0xfe4c);
};
#endif
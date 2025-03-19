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

    PROPERTY(INDEX_NOT_FOUND, 0x0000);
    PROPERTY(FEHLERMELDUNG, 0x0001);
    PROPERTY(SPEICHERSOLLTEMP, 0x0003, Type::et_dec_val);
    PROPERTY(VORLAUFSOLLTEMP, 0x0004, Type::et_dec_val);
    PROPERTY(AUSSENTEMP, 0x000c, Type::et_dec_val);
    PROPERTY(SPEICHERISTTEMP, 0x000e, Type::et_dec_val);
    PROPERTY(VERSTELLTE_RAUMSOLLTEMP, 0x0012, Type::et_dec_val);
    PROPERTY(STEIGUNG_HK1, 0x010e, Type::et_cent_val);
    PROPERTY(WOCHENTAG, 0x0121, Type::et_little_endian);
    PROPERTY(TAG, 0x0122, Type::et_little_endian);
    PROPERTY(MONAT, 0x0123, Type::et_little_endian);
    PROPERTY(JAHR, 0x0124, Type::et_little_endian);
    PROPERTY(STUNDE, 0x0125, Type::et_little_endian);
    PROPERTY(MINUTE, 0x0126, Type::et_little_endian);
    PROPERTY(ANTEIL_VORLAUF_HK1, 0x059d);
    PROPERTY(FUSSPUNKT_HK1, 0x059e, Type::et_dec_val);
    PROPERTY(EL_AUFNAHMELEISTUNG_WW_TAG_WH, 0x091a);
    PROPERTY(EL_AUFNAHMELEISTUNG_WW_TAG_KWH, 0x091b, Type::et_double_val);
    PROPERTY(EL_AUFNAHMELEISTUNG_WW_SUM_KWH, 0x091c);
    PROPERTY(EL_AUFNAHMELEISTUNG_WW_SUM_MWH, 0x091d, Type::et_double_val);
    PROPERTY(EL_AUFNAHMELEISTUNG_HEIZ_TAG_WH, 0x091e);
    PROPERTY(EL_AUFNAHMELEISTUNG_HEIZ_TAG_KWH, 0x091f, Type::et_double_val);
    PROPERTY(EL_AUFNAHMELEISTUNG_HEIZ_SUM_KWH, 0x0920);
    PROPERTY(EL_AUFNAHMELEISTUNG_HEIZ_SUM_MWH, 0x0921, Type::et_double_val);
    PROPERTY(WAERMEERTRAG_WW_TAG_WH, 0x092a);
    PROPERTY(WAERMEERTRAG_WW_TAG_KWH, 0x092b, Type::et_double_val);
    PROPERTY(WAERMEERTRAG_WW_SUM_KWH, 0x092c);
    PROPERTY(WAERMEERTRAG_WW_SUM_MWH, 0x092d, Type::et_double_val);
    PROPERTY(WAERMEERTRAG_HEIZ_TAG_WH, 0x092e);
    PROPERTY(WAERMEERTRAG_HEIZ_TAG_KWH, 0x092f, Type::et_double_val);
    PROPERTY(WAERMEERTRAG_HEIZ_SUM_KWH, 0x0930);
    PROPERTY(WAERMEERTRAG_HEIZ_SUM_MWH, 0x0931, Type::et_double_val);
    PROPERTY(KOMFORTTEMPERATUR_WW, 0x0013, Type::et_dec_val);
    PROPERTY(ECOTEMPERATUR_WW, 0x0A06, Type::et_dec_val);
    PROPERTY(T_01, 0x0A36, Type::et_dec_val);
    PROPERTY(T_02, 0xfdf4, Type::et_dec_val);
    PROPERTY(T_03, 0x01D7, Type::et_dec_val);
    PROPERTY(T_04, 0xFE09, Type::et_dec_val);
    PROPERTY(PUFFERISTTEMPERATUR, 0x0078, Type::et_dec_val);
    PROPERTY(SOLLTEMP_HK1, 0x0199, Type::et_dec_val);
    PROPERTY(VORLAUFTEMP, 0x01d6, Type::et_dec_val);
    PROPERTY(MAXVORLAUFTEMP, 0x01e8, Type::et_dec_val);
    PROPERTY(HEISSGASTEMP, 0x0265, Type::et_dec_val);
    PROPERTY(EL_NACHERW_ANZ_STUFEN, 0x059f);
    PROPERTY(EL_NACHERW_VERZ_ZEIT, 0x05a0);
    PROPERTY(VERDICHTER_EINTRITTSTEMP, 0x06d9, Type::et_dec_val);
    PROPERTY(ISTDREHZAHL_VERDICHTER, 0x06eb);
    PROPERTY(SOLLDREHZAHL_VERDICHTER, 0x06ec);
    PROPERTY(VERDAMPFERTEMP, 0x07a9, Type::et_dec_val);
    PROPERTY(FROSTSCHUTZ, 0xfe07, Type::et_dec_val);
    PROPERTY(VERFLUESSIGERTEMP, 0x0a37, Type::et_dec_val);
    PROPERTY(OELSUMPFTEMP, 0x0a39, Type::et_dec_val);
    PROPERTY(WAERMEPUMPEN_STATUS, 0x02e2);
    PROPERTY(MINTEMP, 0x4ea4, Type::et_dec_val);
    PROPERTY(MAXTEMP, 0x4ea8, Type::et_dec_val);
    PROPERTY(MISCHERDYNAMIK, 0x4ea9);
    PROPERTY(ISTTEMPERATUR, 0x4eb4, Type::et_dec_val);
    PROPERTY(KOMFORTTEMPERATUR, 0x0005, Type::et_dec_val);
    PROPERTY(ECOTEMPERATUR, 0x0008, Type::et_dec_val);
    PROPERTY(RAUMISTTEMP, 0x0011, Type::et_dec_val);
    PROPERTY(RAUMSOLLTEMP, 0x0006, Type::et_dec_val);
    PROPERTY(RAUMFEUCHTE, 0x0075, Type::et_dec_val);
    PROPERTY(TAUPUNKTTEMP, 0x4ee0, Type::et_dec_val);
    PROPERTY(RAUMEINFLUSS, 0x4f33);
    PROPERTY(HEIZUNGSDRUCK, 0x4f46, et_cent_val);
    PROPERTY(VOLUMENSTROM, 0x4f47, Type::et_dec_val);
    PROPERTY(HEIZEN_EFFIZIENZ_TAG, 0x501d, Type::et_cent_val);
    PROPERTY(HEIZEN_EFFIZIENZ_JAHR, 0x501e, Type::et_cent_val);
    PROPERTY(RAUMSOLLTEMP_KUEHLEN, 0x4f04, Type::et_dec_val);
    PROPERTY(RUECKLAUFTEMP_QUELLE, 0x4fa6, Type::et_dec_val);
    PROPERTY(VORLAUFTEMP_QUELLE, 0x4fa7, Type::et_dec_val);
    PROPERTY(QUELLENDRUCK, 0x4fa8, Type::et_dec_val);
    PROPERTY(LEISTUNG_QUELLENPUMPE, 0x4fa9, Type::et_dec_val);
    PROPERTY(STROM_INVERTER, 0x06b2, Type::et_dec_val);
    PROPERTY(SPANNUNG_INVERTER, 0x06b1, Type::et_dec_val);
    PROPERTY(DRUCK_HOCHDRUCK, 0x07a6, Type::et_dec_val);
    PROPERTY(DRUCK_NIEDERDRUCK, 0x07a7, Type::et_dec_val);
    PROPERTY(VORLAUFISTTEMP, 0xfdf3, Type::et_dec_val);
    PROPERTY(BETRIEBS_STATUS, 0x4ecd);
    PROPERTY(VERDICHTER_STARTS, 0x4ef1);
    PROPERTY(VERDICHTER_STARTS_K, 0x4ef0);
    PROPERTY(HYSTERESE_VORLAUFTEMP_KUEHLEN, 0x4f00, Type::et_dec_val);
    PROPERTY(PROGRAMMSCHALTER, 0x0112, Type::et_betriebsart);
    PROPERTY(VORLAUFISTTEMP_NHZ, 0xfdf5, Type::et_dec_val);
    PROPERTY(RUECKLAUFISTTEMP_WP, 0x0016, Type::et_dec_val);
    PROPERTY(LEISTUNG_KUEHLEN, 0x7a40, Type::et_dec_val);
    PROPERTY(RUECKLAUFISTTEMP, 0x02CA, Type::et_dec_val);
    PROPERTY(VD_ABTAUEN, 0x0808, Type::et_zeit);

    PROPERTY(HEIZPROG_1_MO_SO, 0x14a0, Type::et_time_domain);
    PROPERTY(BIVALENZTEMPERATUR_HZG, 0x01ac, Type::et_dec_val);
    PROPERTY(BIVALENZTEMPERATUR_WW, 0x01ad, Type::et_dec_val);

    PROPERTY(LZ_VERD_1_HEIZBETRIEB, 0x07fc, Type::et_cent_val);
    PROPERTY(LZ_VERD_1_KUEHLBETRIEB, 0x07ff, Type::et_cent_val);
    PROPERTY(LZ_VERD_1_WW_BETRIEB, 0x0802, Type::et_cent_val);
};
#endif
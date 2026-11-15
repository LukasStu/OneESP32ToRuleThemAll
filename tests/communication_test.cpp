#include <cassert>
#include <cstdint>

#define ESPCLIENT_ID 0x6a2
#define MANAGER_ID 0x480
#define KESSEL_ID 0x180
#define HK1_ID 0x301
#define FET_ID 0x301
#define MUFG_ID 0x700
#define WPM_ID 0x480

#include "property.h"
#include "communication.h"

static_assert(getMessageType(0xc101) == MessageType::Read);
static_assert(getMessageType(0xc001) == MessageType::Write);
static_assert(getMessageType(0xc201) == MessageType::Response);
static_assert(getMessageType(0xc301) == MessageType::Response);

int main() {
    const std::vector<std::uint8_t> request{0xc1, 0x01, 0x11, 0x00, 0x00, 0x00, 0x00};
    assert(isRequest(request));
    assert(!isResponse(request));
    assert(processCanMessage(request, 0x301).first.id == Property::kINDEX_NOT_FOUND.id);

    const std::vector<std::uint8_t> temperature{0xc0, 0x01, 0x11, 0x00, 0xe6, 0x00, 0x00};
    const auto [property, value] = processCanMessage(temperature, 0x301);
    assert(property.id == Property::kRAUMISTTEMP.id);
    assert(value.get<float>() == 23.0f);

    const std::vector<std::uint8_t> zero{0xc0, 0x01, 0x11, 0x00, 0x00, 0x00, 0x00};
    const auto [zeroProperty, zeroValue] = processCanMessage(zero, 0x301);
    assert(zeroProperty.id == Property::kRAUMISTTEMP.id);
    assert(zeroValue.get<float>() == 0.0f);

    const std::vector<std::uint8_t> response{0xc2, 0x01, 0x11, 0x00, 0xe6, 0x00, 0x00};
    assert(isResponse(response));
    assert(!isRequest(response));
    assert(processCanMessage(response, 0x601).first.id == Property::kRAUMISTTEMP.id);

    const std::vector<std::uint8_t> responseWithBothBits{0xc3, 0x01, 0x11, 0x00, 0xe6, 0x00, 0x00};
    assert(isResponse(responseWithBothBits));
    assert(!isRequest(responseWithBothBits));
}

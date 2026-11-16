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

    const std::vector<std::uint8_t> statusResponse{0xd2, 0x22, 0xfa, 0x4e, 0x5e, 0x08, 0x00};
    const auto [statusProperty, statusValue] = processCanMessage(statusResponse, 0x480);
    assert(statusProperty.id == Property::kSTATUS_4E5E.id);
    assert(statusValue.get<float>() == 2048.0f);

    const std::vector<std::uint8_t> unavailableStatus{0xd2, 0x22, 0xfa, 0x4e, 0x5e, 0x80, 0x00};
    assert(processCanMessage(unavailableStatus, 0x480).second.get<float>() == 32768.0f);

    processNextTransmission();
    assert(test_canbus.sent_data.empty());
    queueTransmission(WPM, Property::kPROGRAMMSCHALTER, 0x1234);
    queueTransmission(Kessel, Property::kMINUTE, 0x5678);
    assert(test_canbus.sent_data.empty());
    assert(conditionalRequests.empty());

    processNextTransmission();
    assert(test_canbus.sent_data.size() == 1);
    assert(test_canbus.sent_data[0] == (std::vector<std::uint8_t>{
        static_cast<std::uint8_t>(WPM.getWriteId() >> 8U), static_cast<std::uint8_t>(WPM.getWriteId()),
        0xfa, static_cast<std::uint8_t>(Property::kPROGRAMMSCHALTER.id >> 8U),
        static_cast<std::uint8_t>(Property::kPROGRAMMSCHALTER.id), 0x12, 0x34}));
    assert(conditionalRequests.size() == 1);
    assert(conditionalRequests.front()._request.first.canId == WPM.canId);
    assert(conditionalRequests.front()._request.second.id == Property::kPROGRAMMSCHALTER.id);
    assert(!conditionalRequests.front()._condition());

    processNextTransmission();
    assert(test_canbus.sent_data.size() == 2);
    assert(test_canbus.sent_data[1] == (std::vector<std::uint8_t>{
        static_cast<std::uint8_t>(Kessel.getWriteId() >> 8U), static_cast<std::uint8_t>(Kessel.getWriteId()),
        0xfa, static_cast<std::uint8_t>(Property::kMINUTE.id >> 8U),
        static_cast<std::uint8_t>(Property::kMINUTE.id), 0x56, 0x78}));
    assert(conditionalRequests.size() == 2);
    assert(conditionalRequests.back()._request.second.id == Property::kMINUTE.id);
    assert(transmissions.empty());
    processNextTransmission();
    assert(test_canbus.sent_data.size() == 2);

    requestData(Manager, Property::kSTATUS_4E5E);
    assert(test_canbus.sent_data.back() == (std::vector<std::uint8_t>{
        static_cast<std::uint8_t>(Manager.getReadId() >> 8U), static_cast<std::uint8_t>(Manager.getReadId()),
        0xfa, 0x4e, 0x5e, 0x00, 0x00}));
}

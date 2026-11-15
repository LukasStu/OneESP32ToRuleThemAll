#include <algorithm>
#include <functional>
#include <optional>
#include <vector>

#define ESP_LOGD(...) ((void) 0)
#define ESP_LOGI(...) ((void) 0)
#define id(name) test_canbus

struct TestCanbus {
    std::vector<std::vector<std::uint8_t>> sent_data;
    void send_data(std::uint16_t, bool, const std::vector<std::uint8_t>& data) { sent_data.push_back(data); }
};

static TestCanbus test_canbus;

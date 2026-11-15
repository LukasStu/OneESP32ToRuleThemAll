#include <algorithm>
#include <functional>
#include <optional>
#include <vector>

#define ESP_LOGD(...) ((void) 0)
#define ESP_LOGI(...) ((void) 0)
#define id(name) test_canbus

struct TestCanbus {
    void send_data(std::uint16_t, bool, const std::vector<std::uint8_t>&) {}
};

static TestCanbus test_canbus;

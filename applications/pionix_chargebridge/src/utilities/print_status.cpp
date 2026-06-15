// SPDX-License-Identifier: Apache-2.0
// Copyright 2020 - 2025 Pionix GmbH and Contributors to EVerest

#include <algorithm>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

#include <charge_bridge/utilities/print_status.hpp>

namespace charge_bridge::utilities {

namespace ansi {
constexpr const char* reset = "\033[0m";
constexpr const char* green = "\033[32m";
constexpr const char* red = "\033[31m";
constexpr const char* gray = "\033[90m";
constexpr const char* bright_white = "\033[97m";
constexpr const char* bold_bright_white = "\033[1;97m";
constexpr const char* bold_bright_red = "\033[1;91m";
constexpr const char* bold_green = "\033[1;32m";
} // namespace ansi

struct Field {
    const char* name;
    std::optional<bool> value;
};

static std::string center(const std::string& text, std::size_t width) {
    if (text.size() >= width) {
        return text.substr(0, width);
    }

    const std::size_t left = (width - text.size()) / 2;
    const std::size_t right = width - text.size() - left;

    return std::string(left, ' ') + text + std::string(right, ' ');
}

static void print_border(std::ostream& os, std::size_t field_count, std::size_t width) {
    os << '+';

    for (std::size_t i = 0; i < field_count; ++i) {
        os << std::string(width, '-');
        os << '+';
    }

    os << '\n';
}

static std::string current_timestamp() {
    const auto now = std::chrono::system_clock::now();
    const std::time_t now_time = std::chrono::system_clock::to_time_t(now);

    std::tm tm{};
    localtime_r(&now_time, &tm); // Linux/POSIX

    std::ostringstream oss;
    oss << std::put_time(&tm, "%H:%M:%S");
    return oss.str();
}

void print_status(const chargebridge_status& s) {
    auto& os = std::cout;
    constexpr std::size_t W = 12;

    const std::vector<Field> fields{
        {"discovered", s.discovered}, {"connected", s.connected}, {"can0", s.can0}, {"serial1", s.serial1},
        {"serial2", s.serial2},       {"serial3", s.serial3},     {"plc", s.plc},   {"bsp", s.bsp},
        {"heartbeat", s.heartbeat},   {"gpio", s.gpio},
    };

    const std::size_t field_count = fields.size() + 2; // timestamp + cb_name

    print_border(os, field_count, W);

    // Top row: field names
    os << '|';
    os << ansi::bold_bright_white << center("timestamp", W) << ansi::reset << '|';
    os << ansi::bold_bright_white << center("cb_name", W) << ansi::reset << '|';

    for (const auto& f : fields) {
        os << ansi::bold_bright_white << center(f.name, W) << ansi::reset << '|';
    }
    os << '\n';

    const bool all_available_fields_good =
        std::all_of(fields.begin(), fields.end(), [](const Field& f) { return !f.value.has_value() || *f.value; });

    const char* cb_name_color = all_available_fields_good ? ansi::green : ansi::bold_bright_red;

    // Bottom row: timestamp / values
    os << '|';

    os << ansi::bold_bright_white << center(current_timestamp(), W) << ansi::reset << '|';

    os << cb_name_color << center(s.cb_name, W) << ansi::reset << '|';

    for (const auto& f : fields) {
        const char* color = ansi::gray;
        std::string status = "N/A";

        if (f.value.has_value()) {
            if (*f.value) {
                color = ansi::green;
                status = "OK";
            } else {
                color = ansi::bold_bright_red;
                status = "ERROR";
            }
        }

        const std::size_t left = (W - status.size()) / 2;
        const std::size_t right = W - status.size() - left;

        os << std::string(left, ' ') << color << status << ansi::reset << std::string(right, ' ') << '|';
    }
    os << '\n';

    print_border(os, field_count, W);
}
} // namespace charge_bridge::utilities

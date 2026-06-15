// SPDX-License-Identifier: Apache-2.0
// Copyright 2020 - 2025 Pionix GmbH and Contributors to EVerest
#pragma once

#include <optional>
#include <string>

namespace charge_bridge::utilities {

struct chargebridge_status {
    std::string cb_name;
    bool connected{false};
    bool discovered{false};
    std::optional<bool> can0;
    std::optional<bool> serial1;
    std::optional<bool> serial2;
    std::optional<bool> serial3;
    std::optional<bool> plc;
    std::optional<bool> bsp;
    std::optional<bool> heartbeat;
    std::optional<bool> gpio;
};

void print_status(const chargebridge_status& status);

} // namespace charge_bridge::utilities

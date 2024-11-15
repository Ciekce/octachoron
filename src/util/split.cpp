/*
 * Octachoron, a Pijersi engine
 * Copyright (C) 2024 Ciekce
 *
 * Octachoron is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Octachoron is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Octachoron. If not, see <https://www.gnu.org/licenses/>.
 */

#include "split.h"

#include <sstream>

namespace octachoron::util {
    auto split(std::string_view str, char delim) -> std::vector<std::string> {
        std::vector<std::string> result{};

        //TODO C++23: ispanstream
        std::istringstream stream{std::string{str}};

        for (std::string token{}; std::getline(stream, token, delim);) {
            if (token.empty()) {
                continue;
            }

            result.push_back(token);
        }

        return result;
    }
} // namespace octachoron::util

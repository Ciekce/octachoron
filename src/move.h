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

#pragma once

#include "types.h"

#include <iostream>

#include "core.h"

namespace octachoron {
    class Move {
    public:
        constexpr Move() = default;

        constexpr Move(const Move&) = default;
        constexpr Move(Move&&) = default;

        [[nodiscard]] constexpr Cell from() const {
            return Cell::fromRaw((m_move >> kFromShift) & kCellMask);
        }

        [[nodiscard]] constexpr Cell to() const {
            return Cell::fromRaw((m_move >> kToShift) & kCellMask);
        }

        [[nodiscard]] constexpr Cell to2() const {
            assert(isDouble());
            return Cell::fromRaw((m_move >> kTo2Shift) & kCellMask);
        }

        [[nodiscard]] constexpr bool isDouble() const {
            return (m_move >> kDoubleShift) & kDoubleMask;
        }

        [[nodiscard]] constexpr bool isSingleUnstack() const {
            return (m_move >> kSingleUnstackShift) & kSingleUnstackMask;
        }

        [[nodiscard]] constexpr bool isNull() const {
            return m_move == 0;
        }

        [[nodiscard]] constexpr bool operator==(const Move&) const = default;

        constexpr Move& operator=(const Move&) = default;
        constexpr Move& operator=(Move&&) = default;

        [[nodiscard]] static constexpr Move makeSingle(Cell from, Cell to) {
            assert(from != Cells::kNone);
            assert(to != Cells::kNone);

            return Move{static_cast<u32>((from.raw() << kFromShift) | (to.raw() << kToShift))};
        }

        [[nodiscard]] static constexpr Move makeSingleUnstack(Cell from, Cell to) {
            constexpr u32 kSingleUnstackFlag = 1 << kSingleUnstackShift;

            assert(from != Cells::kNone);
            assert(to != Cells::kNone);

            return Move{kSingleUnstackFlag | (from.raw() << kFromShift) | (to.raw() << kToShift)};
        }

        [[nodiscard]] static constexpr Move makeDouble(Cell from, Cell to, Cell to2) {
            constexpr u32 kDoubleFlag = 1 << kDoubleShift;

            assert(from != Cells::kNone);
            assert(to != Cells::kNone);
            assert(to2 != Cells::kNone);

            return Move{kDoubleFlag | (from.raw() << kFromShift) | (to.raw() << kToShift) | (to2.raw() << kTo2Shift)};
        }

    private:
        static constexpr i32 kFromShift = 0;
        static constexpr i32 kToShift = 6;
        static constexpr i32 kTo2Shift = 12;
        static constexpr i32 kSingleUnstackShift = 13;
        static constexpr i32 kDoubleShift = 14;

        static constexpr u32 kCellMask = 0b111111;
        static constexpr u32 kSingleUnstackMask = 0b1;
        static constexpr u32 kDoubleMask = 0b1;

        explicit constexpr Move(u32 move) :
                m_move{move} {}

        u32 m_move{};

        inline friend std::ostream& operator<<(std::ostream& stream, Move move) {
            if (move.isNull()) {
                stream << "0000";
                return stream;
            }

            stream << move.from();

            if (move.isSingleUnstack()) {
                stream << move.from();
            }

            stream << move.to();

            if (move.isDouble()) {
                stream << move.to2();
            }

            return stream;
        }
    };

    constexpr Move kNullMove{};
} // namespace octachoron

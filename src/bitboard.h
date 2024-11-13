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

#include "core.h"

#include "types.h"

namespace octachoron {
    namespace offsets {
        constexpr i32 kNorthWest = 6;
        constexpr i32 kNorthEast = 7;
        constexpr i32 kWest = -1;
        constexpr i32 kEast = 1;
        constexpr i32 kSouthWest = -7;
        constexpr i32 kSouthEast = -6;
    } // namespace offsets

    class Bitboard {
    public:
        constexpr Bitboard() = default;

        explicit constexpr Bitboard(u64 bb) :
                m_bb{bb} {}

        constexpr Bitboard(const Bitboard&) = default;
        constexpr Bitboard(Bitboard&&) = default;

        [[nodiscard]] constexpr Bitboard operator&(Bitboard rhs) const {
            return Bitboard{m_bb & rhs.raw()};
        }

        [[nodiscard]] constexpr Bitboard operator|(Bitboard rhs) const {
            return Bitboard{m_bb | rhs.raw()};
        }

        [[nodiscard]] constexpr Bitboard operator^(Bitboard rhs) const {
            return Bitboard{m_bb ^ rhs.raw()};
        }

        constexpr Bitboard& operator&=(Bitboard rhs) {
            m_bb &= rhs.raw();
            return *this;
        }

        constexpr Bitboard& operator|=(Bitboard rhs) {
            m_bb |= rhs.raw();
            return *this;
        }

        constexpr Bitboard& operator^=(Bitboard rhs) {
            m_bb ^= rhs.raw();
            return *this;
        }

        [[nodiscard]] constexpr Bitboard operator&(u64 rhs) const {
            return Bitboard{m_bb & rhs};
        }

        [[nodiscard]] constexpr Bitboard operator|(u64 rhs) const {
            return Bitboard{m_bb | rhs};
        }

        [[nodiscard]] constexpr Bitboard operator^(u64 rhs) const {
            return Bitboard{m_bb ^ rhs};
        }

        constexpr Bitboard& operator&=(u64 rhs) {
            m_bb &= rhs;
            return *this;
        }

        constexpr Bitboard& operator|=(u64 rhs) {
            m_bb |= rhs;
            return *this;
        }

        constexpr Bitboard& operator^=(u64 rhs) {
            m_bb ^= rhs;
            return *this;
        }

        [[nodiscard]] constexpr Bitboard operator&(i32 rhs) const {
            return Bitboard{m_bb & static_cast<u64>(rhs)};
        }

        [[nodiscard]] constexpr Bitboard operator|(i32 rhs) const {
            return Bitboard{m_bb | static_cast<u64>(rhs)};
        }

        [[nodiscard]] constexpr Bitboard operator^(i32 rhs) const {
            return Bitboard{m_bb ^ static_cast<u64>(rhs)};
        }

        constexpr Bitboard& operator&=(i32 rhs) {
            m_bb &= static_cast<u64>(rhs);
            return *this;
        }

        constexpr Bitboard& operator|=(i32 rhs) {
            m_bb |= static_cast<u64>(rhs);
            return *this;
        }

        constexpr Bitboard& operator^=(i32 rhs) {
            m_bb ^= static_cast<u64>(rhs);
            return *this;
        }

        [[nodiscard]] constexpr Bitboard operator~() const {
            return Bitboard{~m_bb};
        }

        [[nodiscard]] constexpr Bitboard operator<<(i32 rhs) const {
            return Bitboard{m_bb << rhs};
        }

        [[nodiscard]] constexpr Bitboard operator>>(i32 rhs) const {
            return Bitboard{m_bb >> rhs};
        }

        constexpr Bitboard& operator<<=(i32 rhs) {
            m_bb <<= rhs;
            return *this;
        }

        constexpr Bitboard& operator>>=(i32 rhs) {
            m_bb >>= rhs;
            return *this;
        }

        [[nodiscard]] constexpr Bitboard shiftNorthWest() const {
            return Bitboard{(m_bb & ~(kBdf1 | kRowG)) << offsets::kNorthWest};
        }

        [[nodiscard]] constexpr Bitboard shiftNorthEast() const {
            return Bitboard{(m_bb & ~(kBdf7 | kRowG)) << offsets::kNorthEast};
        }

        [[nodiscard]] constexpr Bitboard shiftWest() const {
            return Bitboard{(m_bb & ~kColumn1) >> -offsets::kWest};
        }

        [[nodiscard]] constexpr Bitboard shiftEast() const {
            return Bitboard{(m_bb & ~kEastBorder) << offsets::kEast};
        }

        [[nodiscard]] constexpr Bitboard shiftSouthWest() const {
            return Bitboard{(m_bb & ~kBdf1) >> -offsets::kSouthWest};
        }

        [[nodiscard]] constexpr Bitboard shiftSouthEast() const {
            return Bitboard{(m_bb & ~kBdf7) >> -offsets::kSouthEast};
        }

        [[nodiscard]] constexpr u64 raw() const {
            return m_bb;
        }

        [[nodiscard]] constexpr bool operator==(const Bitboard&) const = default;

        constexpr Bitboard& operator=(const Bitboard&) = default;
        constexpr Bitboard& operator=(Bitboard&&) = default;

        [[nodiscard]] static constexpr Bitboard fromCell(Cell cell) {
            return Bitboard{cell.bit()};
        }

        [[nodiscard]] static constexpr Bitboard fromCellOrZero(Cell cell) {
            if (cell == Cells::kNone)
                return Bitboard{};
            return Bitboard{cell.bit()};
        }

    private:
        u64 m_bb{};

    public:
        static constexpr u64 kRowA = UINT64_C(0x3f);
        static constexpr u64 kRowB = UINT64_C(0x1fc0);
        static constexpr u64 kRowC = UINT64_C(0x7e000);
        static constexpr u64 kRowD = UINT64_C(0x3f80000);
        static constexpr u64 kRowE = UINT64_C(0xfc000000);
        static constexpr u64 kRowF = UINT64_C(0x7f00000000);
        static constexpr u64 kRowG = UINT64_C(0x1f8000000000);

        static constexpr u64 kColumn1 = UINT64_C(0x8104082041);
        static constexpr u64 kColumn2 = UINT64_C(0x10208104082);
        static constexpr u64 kColumn3 = UINT64_C(0x20410208104);
        static constexpr u64 kColumn4 = UINT64_C(0x40820410208);
        static constexpr u64 kColumn5 = UINT64_C(0x81040820410);
        static constexpr u64 kColumn6 = UINT64_C(0x102081040820);
        static constexpr u64 kColumn7 = UINT64_C(0x4002001000);

        static constexpr u64 kEastBorder = UINT64_C(0x104082041020);

        static constexpr u64 kBdf1 = UINT64_C(0x100080040);
        static constexpr u64 kBdf7 = UINT64_C(0x4002001000);
    };
} // namespace octachoron

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

#include <cassert>

namespace octachoron {
    class Color {
    public:
        constexpr Color() = default;

        constexpr Color(const Color&) = default;
        constexpr Color(Color&&) = default;

        [[nodiscard]] constexpr u8 raw() const {
            return m_id;
        }

        [[nodiscard]] constexpr usize idx() const {
            return static_cast<usize>(m_id);
        }

        [[nodiscard]] constexpr Color flip() const {
            assert(m_id != kNoneId);
            return Color{static_cast<u8>(m_id ^ 1)};
        }

        [[nodiscard]] static constexpr Color fromRaw(u8 id) {
            assert(id <= kNoneId);
            return Color{id};
        }

        [[nodiscard]] constexpr explicit operator bool() const {
            return m_id != kNoneId;
        }

        [[nodiscard]] constexpr bool operator==(const Color&) const = default;

        constexpr Color& operator=(const Color&) = default;
        constexpr Color& operator=(Color&&) = default;

    private:
        explicit constexpr Color(u8 id) :
                m_id{id} {}

        u8 m_id{};

        enum : u8 {
            kWhiteId = 0,
            kBlackId,
            kNoneId,
        };

        friend struct Colors;
    };

    struct Colors {
        Colors() = delete;

        static constexpr Color kWhite{Color::kWhiteId};
        static constexpr Color kBlack{Color::kBlackId};
        static constexpr Color kNone{Color::kNoneId};

        static constexpr usize kCount = kNone.idx();
    };

    class Role {
    public:
        constexpr Role() = default;

        constexpr Role(const Role&) = default;
        constexpr Role(Role&&) = default;

        [[nodiscard]] constexpr u8 raw() const {
            return m_id;
        }

        [[nodiscard]] constexpr usize idx() const {
            return static_cast<usize>(m_id);
        }

        [[nodiscard]] static constexpr Role fromRaw(u8 id) {
            assert(id <= kNoneId);
            return Role{id};
        }

        [[nodiscard]] constexpr explicit operator bool() const {
            return m_id != kNoneId;
        }

        [[nodiscard]] constexpr bool operator==(const Role&) const = default;

        constexpr Role& operator=(const Role&) = default;
        constexpr Role& operator=(Role&&) = default;

    private:
        explicit constexpr Role(u8 id) :
                m_id{id} {}

        u8 m_id{};

        enum : u8 {
            kWiseId = 0,
            kRockId,
            kPaperId,
            kScissorsId,
            kNoneId,
        };

        friend struct Roles;
    };

    struct Roles {
        Roles() = delete;

        static constexpr Role kWise{Role::kWiseId};
        static constexpr Role kRock{Role::kRockId};
        static constexpr Role kPaper{Role::kPaperId};
        static constexpr Role kScissors{Role::kScissorsId};
        static constexpr Role kNone{Role::kNoneId};

        static constexpr usize kCount = kNone.idx();
    };

    class Piece;

    class PieceType {
    public:
        constexpr PieceType() = default;

        constexpr PieceType(const PieceType&) = default;
        constexpr PieceType(PieceType&&) = default;

        [[nodiscard]] constexpr u8 raw() const {
            return m_id;
        }

        [[nodiscard]] constexpr usize idx() const {
            return static_cast<usize>(m_id);
        }

        [[nodiscard]] constexpr Role role() const {
            return Role::fromRaw(m_id & 0b11);
        }

        [[nodiscard]] constexpr bool isStack() const {
            return m_id != kNoneId && m_id > kScissorsId;
        }

        [[nodiscard]] constexpr PieceType upper() const {
            assert(isStack());
            return fromRaw(m_id & 0b11);
        }

        [[nodiscard]] constexpr PieceType lower() const {
            assert(isStack());
            return fromRaw(m_id >> 2);
        }

        [[nodiscard]] constexpr Piece withColor(Color c) const;

        [[nodiscard]] constexpr PieceType stackedOn(PieceType other) const {
            assert(m_id != kNoneId);
            assert(m_id != kWiseId);
            assert(m_id <= kScissorsId);

            assert(other.m_id != kNoneId);
            assert(other.m_id <= kScissorsId);

            return fromRaw((other.m_id << 2) | m_id);
        }

        [[nodiscard]] static constexpr PieceType fromRaw(u8 id) {
            assert(id <= kNoneId);
            return PieceType{id};
        }

        [[nodiscard]] constexpr explicit operator bool() const {
            return m_id != kNoneId;
        }

        [[nodiscard]] constexpr bool operator==(const PieceType&) const = default;

        constexpr PieceType& operator=(const PieceType&) = default;
        constexpr PieceType& operator=(PieceType&&) = default;

    private:
        explicit constexpr PieceType(u8 id) :
                m_id{id} {}

        u8 m_id{};

        enum : u8 {
            kWiseId = 0,
            kRockId,
            kPaperId,
            kScissorsId,
            _kWiseOnRockId [[maybe_unused]],
            kRockOnRockId,
            kPaperOnRockId,
            kScissorsOnRockId,
            _kWiseOnPaperId [[maybe_unused]],
            kRockOnPaperId,
            kPaperOnPaperId,
            kScissorsOnPaperId,
            _kWiseOnScissorsId [[maybe_unused]],
            kRockOnScissorsId,
            kPaperOnScissorsId,
            kScissorsOnScissorsId,
            kWiseOnWiseId,
            kRockOnWiseId,
            kPaperOnWiseId,
            kScissorsOnWiseId,
            kNoneId,
        };

        friend struct PieceTypes;
    };

    struct PieceTypes {
        PieceTypes() = delete;

        static constexpr PieceType kWise{PieceType::kWiseId};
        static constexpr PieceType kRock{PieceType::kRockId};
        static constexpr PieceType kPaper{PieceType::kPaperId};
        static constexpr PieceType kScissors{PieceType::kScissorsId};
        static constexpr PieceType kRockOnRock{PieceType::kRockOnRockId};
        static constexpr PieceType kPaperOnRock{PieceType::kPaperOnRockId};
        static constexpr PieceType kScissorsOnRock{PieceType::kScissorsOnRockId};
        static constexpr PieceType kRockOnPaper{PieceType::kRockOnPaperId};
        static constexpr PieceType kPaperOnPaper{PieceType::kPaperOnPaperId};
        static constexpr PieceType kScissorsOnPaper{PieceType::kScissorsOnPaperId};
        static constexpr PieceType kRockOnScissors{PieceType::kRockOnScissorsId};
        static constexpr PieceType kPaperOnScissors{PieceType::kPaperOnScissorsId};
        static constexpr PieceType kScissorsOnScissors{PieceType::kScissorsOnScissorsId};
        static constexpr PieceType kWiseOnWise{PieceType::kWiseOnWiseId};
        static constexpr PieceType kRockOnWise{PieceType::kRockOnWiseId};
        static constexpr PieceType kPaperOnWise{PieceType::kPaperOnWiseId};
        static constexpr PieceType kScissorsOnWise{PieceType::kScissorsOnWiseId};
        static constexpr PieceType kNone{PieceType::kNoneId};

        static constexpr usize kCount = kNone.idx();
    };

    class Piece {
    public:
        constexpr Piece() = default;

        constexpr Piece(const Piece&) = default;
        constexpr Piece(Piece&&) = default;

        [[nodiscard]] constexpr u8 raw() const {
            return m_id;
        }

        [[nodiscard]] constexpr usize idx() const {
            return static_cast<usize>(m_id);
        }

        [[nodiscard]] constexpr PieceType type() const {
            assert(m_id != kNoneId);
            return PieceType::fromRaw(m_id >> 1);
        }

        [[nodiscard]] constexpr Role role() const {
            assert(m_id != kNoneId);
            return type().role();
        }

        [[nodiscard]] constexpr Color color() const {
            assert(m_id != kNoneId);
            return Color::fromRaw(m_id & 0b1);
        }

        [[nodiscard]] constexpr bool isStack() const {
            return m_id != kNoneId && m_id > kBlackScissorsId;
        }

        [[nodiscard]] constexpr Piece upper() const {
            assert(isStack());
            return type().upper().withColor(color());
        }

        [[nodiscard]] constexpr Piece lower() const {
            assert(isStack());
            return type().lower().withColor(color());
        }

        [[nodiscard]] constexpr Piece stackedOn(PieceType other) const {
            assert(m_id != kNoneId);
            assert(m_id != kWhiteWiseId);
            assert(m_id != kBlackWiseId);
            assert(m_id <= kBlackScissorsId);

            assert(other != PieceTypes::kNone);
            assert(other.raw() <= PieceTypes::kScissors.raw());

            return fromRaw((other.raw() << 3) | m_id);
        }

        [[nodiscard]] constexpr Piece stackedOn(Piece other) const {
            return stackedOn(other.type());
        }

        [[nodiscard]] static constexpr Piece fromRaw(u8 id) {
            assert(id <= kNoneId);
            return Piece{id};
        }

        [[nodiscard]] constexpr explicit operator bool() const {
            return m_id != kNoneId;
        }

        [[nodiscard]] constexpr bool operator==(const Piece&) const = default;

        constexpr Piece& operator=(const Piece&) = default;
        constexpr Piece& operator=(Piece&&) = default;

    private:
        explicit constexpr Piece(u8 id) :
                m_id{id} {}

        u8 m_id{};

        enum : u8 {
            kWhiteWiseId = 0,
            kBlackWiseId,
            kWhiteRockId,
            kBlackRockId,
            kWhitePaperId,
            kBlackPaperId,
            kWhiteScissorsId,
            kBlackScissorsId,
            _kWhiteWiseOnRockId [[maybe_unused]],
            _kBlackWiseOnRockId [[maybe_unused]],
            kWhiteRockOnRockId,
            kBlackRockOnRockId,
            kWhitePaperOnRockId,
            kBlackPaperOnRockId,
            kWhiteScissorsOnRockId,
            kBlackScissorsOnRockId,
            _kWhiteWiseOnPaperId [[maybe_unused]],
            _kBlackWiseOnPaperId [[maybe_unused]],
            kWhiteRockOnPaperId,
            kBlackRockOnPaperId,
            kWhitePaperOnPaperId,
            kBlackPaperOnPaperId,
            kWhiteScissorsOnPaperId,
            kBlackScissorsOnPaperId,
            _kWhiteWiseOnScissorsId [[maybe_unused]],
            _kBlackWiseOnScissorsId [[maybe_unused]],
            kWhiteRockOnScissorsId,
            kBlackRockOnScissorsId,
            kWhitePaperOnScissorsId,
            kBlackPaperOnScissorsId,
            kWhiteScissorsOnScissorsId,
            kBlackScissorsOnScissorsId,
            kWhiteWiseOnWiseId,
            kBlackWiseOnWiseId,
            kWhiteRockOnWiseId,
            kBlackRockOnWiseId,
            kWhitePaperOnWiseId,
            kBlackPaperOnWiseId,
            kWhiteScissorsOnWiseId,
            kBlackScissorsOnWiseId,
            kNoneId,
        };

        friend struct Pieces;
    };

    constexpr Piece PieceType::withColor(Color c) const {
        assert(*this != PieceTypes::kNone);
        assert(c != Colors::kNone);

        return Piece::fromRaw((m_id << 1) | c.raw());
    }

    struct Pieces {
        Pieces() = delete;

        static constexpr Piece kWhiteWise{Piece::kWhiteWiseId};
        static constexpr Piece kBlackRock{Piece::kBlackRockId};
        static constexpr Piece kWhiteRock{Piece::kWhiteRockId};
        static constexpr Piece kBlackWise{Piece::kBlackWiseId};
        static constexpr Piece kWhitePaper{Piece::kWhitePaperId};
        static constexpr Piece kBlackPaper{Piece::kBlackPaperId};
        static constexpr Piece kWhiteScissors{Piece::kWhiteScissorsId};
        static constexpr Piece kBlackScissors{Piece::kBlackScissorsId};
        static constexpr Piece kWhiteRockOnRock{Piece::kWhiteRockOnRockId};
        static constexpr Piece kBlackRockOnRock{Piece::kBlackRockOnRockId};
        static constexpr Piece kWhitePaperOnRock{Piece::kWhitePaperOnRockId};
        static constexpr Piece kBlackPaperOnRock{Piece::kBlackPaperOnRockId};
        static constexpr Piece kWhiteScissorsOnRock{Piece::kWhiteScissorsOnRockId};
        static constexpr Piece kBlackScissorsOnRock{Piece::kBlackScissorsOnRockId};
        static constexpr Piece kWhiteRockOnPaper{Piece::kWhiteRockOnPaperId};
        static constexpr Piece kBlackRockOnPaper{Piece::kBlackRockOnPaperId};
        static constexpr Piece kWhitePaperOnPaper{Piece::kWhitePaperOnPaperId};
        static constexpr Piece kBlackPaperOnPaper{Piece::kBlackPaperOnPaperId};
        static constexpr Piece kWhiteScissorsOnPaper{Piece::kWhiteScissorsOnPaperId};
        static constexpr Piece kBlackScissorsOnPaper{Piece::kBlackScissorsOnPaperId};
        static constexpr Piece kWhiteRockOnScissors{Piece::kWhiteRockOnScissorsId};
        static constexpr Piece kBlackRockOnScissors{Piece::kBlackRockOnScissorsId};
        static constexpr Piece kWhitePaperOnScissors{Piece::kWhitePaperOnScissorsId};
        static constexpr Piece kBlackPaperOnScissors{Piece::kBlackPaperOnScissorsId};
        static constexpr Piece kWhiteScissorsOnScissors{Piece::kWhiteScissorsOnScissorsId};
        static constexpr Piece kBlackScissorsOnScissors{Piece::kBlackScissorsOnScissorsId};
        static constexpr Piece kWhiteWiseOnWise{Piece::kWhiteWiseOnWiseId};
        static constexpr Piece kBlackWiseOnWise{Piece::kBlackWiseOnWiseId};
        static constexpr Piece kWhiteRockOnWise{Piece::kWhiteRockOnWiseId};
        static constexpr Piece kBlackRockOnWise{Piece::kBlackRockOnWiseId};
        static constexpr Piece kWhitePaperOnWise{Piece::kWhitePaperOnWiseId};
        static constexpr Piece kBlackPaperOnWise{Piece::kBlackPaperOnWiseId};
        static constexpr Piece kWhiteScissorsOnWise{Piece::kWhiteScissorsOnWiseId};
        static constexpr Piece kBlackScissorsOnWise{Piece::kBlackScissorsOnWiseId};
        static constexpr Piece kNone{Piece::kNoneId};

        static constexpr usize kCount = kNone.idx();
    };

    class Cell {
    public:
        constexpr Cell() = default;

        constexpr Cell(const Cell&) = default;
        constexpr Cell(Cell&&) = default;

        [[nodiscard]] constexpr u8 raw() const {
            return m_id;
        }

        [[nodiscard]] constexpr usize idx() const {
            return static_cast<usize>(m_id);
        }

        [[nodiscard]] constexpr u64 bit() const {
            assert(m_id != kNoneId);
            return u64{1} << m_id;
        }

        [[nodiscard]] static constexpr Cell fromRaw(u8 id) {
            assert(id <= kNoneId);
            return Cell{id};
        }

        [[nodiscard]] constexpr explicit operator bool() const {
            return m_id != kNoneId;
        }

        [[nodiscard]] constexpr bool operator==(const Cell&) const = default;

        constexpr Cell& operator=(const Cell&) = default;
        constexpr Cell& operator=(Cell&&) = default;

    private:
        explicit constexpr Cell(u8 id) :
                m_id{id} {}

        u8 m_id{};

        enum : u8 {
            kA1Id,
            kA2Id,
            kA3Id,
            kA4Id,
            kA5Id,
            kA6Id,
            kB1Id,
            kB2Id,
            kB3Id,
            kB4Id,
            kB5Id,
            kB6Id,
            kB7Id,
            kC1Id,
            kC2Id,
            kC3Id,
            kC4Id,
            kC5Id,
            kC6Id,
            kD1Id,
            kD2Id,
            kD3Id,
            kD4Id,
            kD5Id,
            kD6Id,
            kD7Id,
            kE1Id,
            kE2Id,
            kE3Id,
            kE4Id,
            kE5Id,
            kE6Id,
            kF1Id,
            kF2Id,
            kF3Id,
            kF4Id,
            kF5Id,
            kF6Id,
            kF7Id,
            kG1Id,
            kG2Id,
            kG3Id,
            kG4Id,
            kG5Id,
            kG6Id,
            kNoneId,
        };

        friend struct Cells;
    };

    struct Cells {
        Cells() = delete;

        static constexpr Cell kA1{Cell::kA1Id};
        static constexpr Cell kA2{Cell::kA2Id};
        static constexpr Cell kA3{Cell::kA3Id};
        static constexpr Cell kA4{Cell::kA4Id};
        static constexpr Cell kA5{Cell::kA5Id};
        static constexpr Cell kA6{Cell::kA6Id};
        static constexpr Cell kB1{Cell::kB1Id};
        static constexpr Cell kB2{Cell::kB2Id};
        static constexpr Cell kB3{Cell::kB3Id};
        static constexpr Cell kB4{Cell::kB4Id};
        static constexpr Cell kB5{Cell::kB5Id};
        static constexpr Cell kB6{Cell::kB6Id};
        static constexpr Cell kB7{Cell::kB7Id};
        static constexpr Cell kC1{Cell::kC1Id};
        static constexpr Cell kC2{Cell::kC2Id};
        static constexpr Cell kC3{Cell::kC3Id};
        static constexpr Cell kC4{Cell::kC4Id};
        static constexpr Cell kC5{Cell::kC5Id};
        static constexpr Cell kC6{Cell::kC6Id};
        static constexpr Cell kD1{Cell::kD1Id};
        static constexpr Cell kD2{Cell::kD2Id};
        static constexpr Cell kD3{Cell::kD3Id};
        static constexpr Cell kD4{Cell::kD4Id};
        static constexpr Cell kD5{Cell::kD5Id};
        static constexpr Cell kD6{Cell::kD6Id};
        static constexpr Cell kD7{Cell::kD7Id};
        static constexpr Cell kE1{Cell::kE1Id};
        static constexpr Cell kE2{Cell::kE2Id};
        static constexpr Cell kE3{Cell::kE3Id};
        static constexpr Cell kE4{Cell::kE4Id};
        static constexpr Cell kE5{Cell::kE5Id};
        static constexpr Cell kE6{Cell::kE6Id};
        static constexpr Cell kF1{Cell::kF1Id};
        static constexpr Cell kF2{Cell::kF2Id};
        static constexpr Cell kF3{Cell::kF3Id};
        static constexpr Cell kF4{Cell::kF4Id};
        static constexpr Cell kF5{Cell::kF5Id};
        static constexpr Cell kF6{Cell::kF6Id};
        static constexpr Cell kF7{Cell::kF7Id};
        static constexpr Cell kG1{Cell::kG1Id};
        static constexpr Cell kG2{Cell::kG2Id};
        static constexpr Cell kG3{Cell::kG3Id};
        static constexpr Cell kG4{Cell::kG4Id};
        static constexpr Cell kG5{Cell::kG5Id};
        static constexpr Cell kG6{Cell::kG6Id};
        static constexpr Cell kNone{Cell::kNoneId};

        static constexpr usize kCount = kNone.idx();
    };
} // namespace octachoron

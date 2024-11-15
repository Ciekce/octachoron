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

#include <array>
#include <iostream>
#include <span>
#include <string_view>
#include <utility>

#include "bitboard.h"
#include "move.h"

namespace octachoron {
    class Position {
    public:
        constexpr Position() {
            m_mailbox.fill(Pieces::kNone);
        }

        constexpr Position(const Position&) = default;
        constexpr Position(Position&&) = default;

        [[nodiscard]] Position applyMove(Move move) const;

        [[nodiscard]] Bitboard colorBb(Color color) const {
            assert(color != Colors::kNone);
            return m_colors[color.idx()];
        }

        [[nodiscard]] Bitboard pieceTypeBb(PieceType pieceType) const {
            assert(pieceType != PieceTypes::kNone);
            return m_pieces[pieceType.idx()];
        }

        [[nodiscard]] Bitboard pieceBb(Piece piece) const {
            assert(piece != Pieces::kNone);
            return colorBb(piece.color()) & pieceTypeBb(piece.type());
        }

        [[nodiscard]] Bitboard roleBb(Role role) const {
            assert(role != Roles::kNone);
            return m_roles[role.idx()];
        }

        [[nodiscard]] Piece pieceOn(Cell cell) const {
            assert(cell != Cells::kNone);
            return m_mailbox[cell.idx()];
        }

        [[nodiscard]] Color stm() const {
            return m_whiteToMove ? Colors::kWhite : Colors::kBlack;
        }

        [[nodiscard]] u32 halfmoves() const {
            return m_halfmoves;
        }

        [[nodiscard]] u32 fullmoves() const {
            return m_fullmoves;
        }

        void resetToStartpos();
        bool resetFromFenParts(std::span<std::string_view> fen);
        bool resetFromFen(std::string_view fen);

        [[nodiscard]] constexpr bool operator==(const Position&) const = default;

        constexpr Position& operator=(const Position&) = default;
        constexpr Position& operator=(Position&&) = default;

        [[nodiscard]] static constexpr Position startpos() {
            Position pos{};
            pos.resetToStartpos();
            return pos;
        }

        [[nodiscard]] static constexpr std::optional<Position> fromFenParts(std::span<std::string_view> fen) {
            Position pos{};

            if (pos.resetFromFenParts(fen)) {
                return pos;
            } else {
                return {};
            }
        }

        [[nodiscard]] static constexpr std::optional<Position> fromFen(std::string_view fen) {
            Position pos{};

            if (pos.resetFromFen(fen)) {
                return pos;
            } else {
                return {};
            }
        }

    private:
        std::array<Bitboard, Colors::kCount> m_colors{};
        std::array<Bitboard, PieceTypes::kCount> m_pieces{};
        std::array<Bitboard, Roles::kCount> m_roles{};
        Bitboard m_stacks{};

        std::array<Piece, Cells::kCount> m_mailbox{};

        bool m_whiteToMove{true};

        u8 m_halfmoves{};
        u16 m_fullmoves{1};

        // -> captured
        Piece addPiece(Piece piece, Cell cell);
        void removePiece(Piece piece, Cell cell);

        void replacePiece(Piece piece, Cell cell);

        // -> stacked
        Piece addStack(Piece upper, Piece lower, Cell cell);

        // -> (result, captured)
        std::pair<Piece, Piece> movePiece(Piece piece, Cell from, Cell to);

        void flipCells(Piece piece, Bitboard mask);

        friend inline std::ostream& operator<<(std::ostream& stream, const Position& pos) {
            const auto printSixLine = [&](Cell firstCell) {
                for (u8 offset = 0; offset < 6; ++offset) {
                    stream << "  " << pos.pieceOn(Cell::fromRaw(firstCell.raw() + offset));
                }
            };

            const auto printSevenLine = [&](Cell firstCell) {
                for (u8 offset = 0; offset < 7; ++offset) {
                    if (offset > 0) {
                        stream << "  ";
                    }

                    stream << pos.pieceOn(firstCell.offset(offset));
                }
            };

            printSixLine(Cells::kG1);
            stream << '\n';
            printSevenLine(Cells::kF1);
            stream << '\n';
            printSixLine(Cells::kE1);
            stream << '\n';
            printSevenLine(Cells::kD1);
            stream << '\n';
            printSixLine(Cells::kC1);
            stream << '\n';
            printSevenLine(Cells::kB1);
            stream << '\n';
            printSixLine(Cells::kA1);

            stream << "\n\n";
            stream << (pos.m_whiteToMove ? "White" : "Black") << " to move";

            return stream;
        }
    };
} // namespace octachoron

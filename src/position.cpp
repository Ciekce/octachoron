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

#include "position.h"

#include "util/parse.h"
#include "util/split.h"

namespace octachoron {
    Position Position::applyMove(Move move) const {
        auto newPos = *this;

        const auto moving = newPos.pieceOn(move.from());
        assert(moving != Pieces::kNone);
        assert(moving.color() == newPos.stm());

        Piece captured, captured2;

        if (move.isSingleUnstack()) {
            captured = newPos.addPiece(moving.upper(), move.to());
            newPos.replacePiece(moving.lower(), move.from());
        } else {
            Piece moving2;
            std::tie(moving2, captured) = newPos.movePiece(moving, move.from(), move.to());

            if (move.isDouble()) {
                if (moving != moving2) {
                    assert(!moving.isStack());
                    assert(moving2.isStack());

                    captured2 = newPos.movePiece(moving2, move.to(), move.to2()).second;
                } else {
                    captured2 = newPos.addPiece(moving2.upper(), move.to2());
                    newPos.replacePiece(moving2.lower(), move.to());
                }
            }
        }

        if (captured != Pieces::kNone || captured2 != Pieces::kNone) {
            newPos.m_halfmoves = 0;
        } else {
            ++newPos.m_halfmoves;
        }

        if (!newPos.m_whiteToMove) {
            ++newPos.m_fullmoves;
        }

        newPos.m_whiteToMove = !newPos.m_whiteToMove;

        return newPos;
    }

    void Position::resetToStartpos() {
        resetFromFen("s-p-r-s-p-r-/p-r-s-wwr-s-p-/6/7/6/P-S-R-WWS-R-P-/R-P-S-R-P-S- w 0 1");
    }

    bool Position::resetFromFenParts(std::span<std::string_view> fen) {
        const auto rows = util::split(fen[0], '/');

        if (rows.size() != 7) {
            return false;
        }

        m_mailbox.fill(Pieces::kNone);

        u8 cellIdx = 0;

        for (i32 rowIdx = 6; rowIdx >= 0; --rowIdx) {
            const auto columnCount = 6 + (rowIdx & 1);

            auto& row = rows[rowIdx];
            usize columnIdx = 0;

            for (usize i = 0; i < row.size(); ++i) {
                if (columnIdx >= columnCount) {
                    return false;
                }

                const auto c = row[i];

                if (const auto emptySquares = util::tryParseDigit(c)) {
                    columnIdx += *emptySquares;
                    cellIdx += *emptySquares;
                } else if (i + 1 < row.size()) {
                    const auto pieceStr = std::string_view{row}.substr(i++, 2);
                    if (const auto piece = Piece::fromStr(pieceStr); piece != Pieces::kNone) {
                        m_mailbox[cellIdx++] = piece;
                        ++columnIdx;
                    } else {
                        return false;
                    }
                } else {
                    return false;
                }
            }

            if (columnIdx != columnCount) {
                return false;
            }
        }

        m_colors = {};
        m_pieces = {};
        m_roles = {};
        m_stacks = Bitboards::kEmpty;

        for (cellIdx = 0; cellIdx < Cells::kCount; ++cellIdx) {
            const auto cell = Cell::fromRaw(cellIdx);
            if (const auto piece = m_mailbox[cellIdx]; piece != Pieces::kNone) {
                flipCells(piece, Bitboard::fromCell(cell));
            }
        }

        if (fen[1] == "w") {
            m_whiteToMove = true;
        } else if (fen[1] == "b") {
            m_whiteToMove = false;
        } else {
            return false;
        }

        if (!util::tryParse(m_halfmoves, fen[2]) || !util::tryParse(m_fullmoves, fen[3])) {
            return false;
        }

        return true;
    }

    bool Position::resetFromFen(std::string_view fen) {
        const auto split = util::split(fen, ' ');

        if (split.size() != 4) {
            return false;
        }

        std::vector<std::string_view> views{};
        views.reserve(split.size());

        for (const auto& str : split) {
            views.push_back(str);
        }

        return resetFromFenParts(views);
    }

    Piece Position::addPiece(Piece piece, Cell cell) {
        assert(piece != Pieces::kNone);
        assert(cell != Cells::kNone);

        const auto captured = m_mailbox[cell.idx()];

        if (captured != Pieces::kNone && !piece.isStack() && !captured.isStack() && piece.color() == captured.color()) {
            addStack(piece, captured, cell);
            return Pieces::kNone;
        }

        if (captured != Pieces::kNone) {
            assert(piece.color() != captured.color());
            flipCells(captured, Bitboard::fromCell(cell));
        }

        flipCells(piece, Bitboard::fromCell(cell));
        m_mailbox[cell.idx()] = piece;

        return captured;
    }

    void Position::removePiece(Piece piece, Cell cell) {
        assert(piece != Pieces::kNone);
        assert(cell != Cells::kNone);

        assert(m_mailbox[cell.idx()] == piece);

        flipCells(piece, Bitboard::fromCell(cell));
        m_mailbox[cell.idx()] = Pieces::kNone;
    }

    void Position::replacePiece(Piece piece, Cell cell) {
        assert(piece != Pieces::kNone);
        assert(cell != Cells::kNone);

        const auto captured = m_mailbox[cell.idx()];

        if (captured != Pieces::kNone) {
            flipCells(captured, Bitboard::fromCell(cell));
        }

        flipCells(piece, Bitboard::fromCell(cell));
        m_mailbox[cell.idx()] = piece;
    }

    Piece Position::addStack(Piece upper, Piece lower, Cell cell) {
        assert(upper != Pieces::kNone);
        assert(lower != Pieces::kNone);
        assert(cell != Cells::kNone);

        assert(!upper.isStack());
        assert(!lower.isStack());

        assert(upper.color() == lower.color());

        assert(upper.role() != Roles::kWise || lower.role() == Roles::kWise);

        assert(m_mailbox[cell.idx()] == lower);

        const auto stacked = upper.stackedOn(lower);

        const auto mask = Bitboard::fromCell(cell);

        m_pieces[lower.type().idx()] ^= mask;
        m_pieces[stacked.type().idx()] ^= mask;

        m_roles[lower.role().idx()] ^= mask;
        m_roles[stacked.role().idx()] ^= mask;

        m_stacks ^= mask;

        m_mailbox[cell.idx()] = stacked;

        return stacked;
    }

    std::pair<Piece, Piece> Position::movePiece(Piece piece, Cell from, Cell to) {
        assert(piece != Pieces::kNone);
        assert(from != Cells::kNone);
        assert(to != Cells::kNone);

        assert(m_mailbox[from.idx()] == piece);
        m_mailbox[from.idx()] = Pieces::kNone;

        const auto captured = m_mailbox[to.idx()];

        if (captured != Pieces::kNone && !piece.isStack() && !captured.isStack() && piece.color() == captured.color()) {
            flipCells(piece, Bitboard::fromCell(from));

            const auto stacked = addStack(piece, captured, to);
            return {stacked, Pieces::kNone};
        }

        const auto mask = Bitboard::fromCell(from) ^ Bitboard::fromCell(to);
        flipCells(piece, mask);

        if (captured != Pieces::kNone) {
            assert(piece.color() != captured.color());
            flipCells(captured, Bitboard::fromCell(to));
        }

        m_mailbox[to.idx()] = piece;

        return {piece, captured};
    }

    void Position::flipCells(Piece piece, Bitboard mask) {
        assert(piece != Pieces::kNone);

        m_colors[piece.color().idx()] ^= mask;
        m_pieces[piece.type().idx()] ^= mask;
        m_roles[piece.role().idx()] ^= mask;

        if (piece.isStack()) {
            m_stacks ^= mask;
        }
    }
} // namespace octachoron

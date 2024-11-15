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
                m_colors[piece.color().idx()].setCell(cell);
                m_pieces[piece.type().idx()].setCell(cell);
                m_roles[piece.role().idx()].setCell(cell);

                if (piece.isStack()) {
                    m_stacks.setCell(cell);
                }
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
} // namespace octachoron

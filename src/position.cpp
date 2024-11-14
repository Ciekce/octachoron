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

namespace octachoron {
    void Position::resetToStartpos() {
        m_colors[Colors::kWhite.idx()] = Bitboard{UINT64_C(0x1fff)};
        m_colors[Colors::kBlack.idx()] = Bitboard{UINT64_C(0x1fff00000000)};

        m_pieces = {};

        m_pieces[PieceTypes::kRock.idx()]
            .setCell(Cells::kA1)
            .setCell(Cells::kA4)
            .setCell(Cells::kB3)
            .setCell(Cells::kB6)
            .setCell(Cells::kF2)
            .setCell(Cells::kF5)
            .setCell(Cells::kG3)
            .setCell(Cells::kG6);

        m_pieces[PieceTypes::kPaper.idx()]
            .setCell(Cells::kA2)
            .setCell(Cells::kA5)
            .setCell(Cells::kB1)
            .setCell(Cells::kB7)
            .setCell(Cells::kF1)
            .setCell(Cells::kF7)
            .setCell(Cells::kG2)
            .setCell(Cells::kG5);

        m_pieces[PieceTypes::kScissors.idx()]
            .setCell(Cells::kA3)
            .setCell(Cells::kA6)
            .setCell(Cells::kB2)
            .setCell(Cells::kB5)
            .setCell(Cells::kF3)
            .setCell(Cells::kF6)
            .setCell(Cells::kG1)
            .setCell(Cells::kG4);

        m_pieces[PieceTypes::kWiseOnWise.idx()].setCell(Cells::kB4).setCell(Cells::kF4);

        m_roles[Roles::kWise.idx()] = m_pieces[PieceTypes::kWiseOnWise.idx()];
        m_roles[Roles::kRock.idx()] = m_pieces[PieceTypes::kRock.idx()];
        m_roles[Roles::kPaper.idx()] = m_pieces[PieceTypes::kPaper.idx()];
        m_roles[Roles::kScissors.idx()] = m_pieces[PieceTypes::kScissors.idx()];

        m_stacks = m_pieces[PieceTypes::kWiseOnWise.idx()];

        m_halfmoves = 0;

        regenMailbox();
    }

    void Position::regenMailbox() {
        const auto pieceOnBbs = [this](Cell cell) {
            Color color;

            if (colorBb(Colors::kWhite).getCell(cell)) {
                color = Colors::kWhite;
            } else if (colorBb(Colors::kBlack).getCell(cell)) {
                color = Colors::kBlack;
            } else {
                return Pieces::kNone;
            }

            for (usize idx = 0; idx < PieceTypes::kNone.idx(); ++idx) {
                if (m_pieces[idx].getCell(cell)) {
                    return PieceType::fromRaw(idx).withColor(color);
                }
            }

            assert(false);

            return Pieces::kNone;
        };

        m_mailbox = {};

        for (u8 cellId = 0; cellId < Cells::kNone.raw(); ++cellId) {
            const auto cell = Cell::fromRaw(cellId);
            m_mailbox[cell.idx()] = pieceOnBbs(cell);
        }
    }
} // namespace octachoron

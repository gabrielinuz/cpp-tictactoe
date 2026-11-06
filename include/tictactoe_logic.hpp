/**
 * @file tictactoe_logic.hpp
 * @author Gabriel Ferreira
 * @brief Lógica pura del juego Ta Te Ti (Tic-Tac-Toe).
 * @version 1.0
 * 
 * @details Este archivo utiliza namespaces para evitar colisiones de nombres
 * y emplea contenedores de la biblioteca estándar (std::array) para 
 * garantizar almacenamiento contiguo en memoria y máxima eficiencia.
 */

#ifndef TICTACTOE_LOGIC_HPP
#define TICTACTOE_LOGIC_HPP

#include <array>
#include <string>

namespace tictactoe
{
    /**
     * @enum Player
     * @brief Representa el estado de una celda o el turno actual.
     * @details Se utiliza 'enum class' (scoped enum) introducido en C++11 para 
     * evitar conversiones implícitas a enteros, mejorando la seguridad de tipos.
     */
    enum class Player
    {
        Empty,
        X,
        O
    };

    /**
     * @class Board
     * @brief Entidad principal que gestiona el estado del tablero de 3x3.
     */
    class Board
    {
        private:
            // std::array es un contenedor estático de tamaño fijo. 
            // A diferencia de los arreglos crudos de C, no decae a puntero 
            // y conoce su propio tamaño, lo que permite usar for-range.
            std::array<Player, 9> cells_;
            Player current_turn_;
            Player winner_;
            int moves_count_;

            /**
             * @brief Verifica si el jugador actual ha formado una línea ganadora.
             */
            void check_win_condition()
            {
                const std::array<std::array<int, 3>, 8> win_lines = 
                {{
                    {0, 1, 2}, {3, 4, 5}, {6, 7, 8},
                    {0, 3, 6}, {1, 4, 7}, {2, 5, 8},
                    {0, 4, 8}, {2, 4, 6}
                }};

                for (const auto& line : win_lines)
                {
                    if (cells_[line[0]] != Player::Empty &&
                        cells_[line[0]] == cells_[line[1]] &&
                        cells_[line[1]] == cells_[line[2]])
                    {
                        winner_ = cells_[line[0]];
                        return;
                    }
                }
            }

        public:
            Board() { reset(); }

            void reset()
            {
                cells_.fill(Player::Empty);
                current_turn_ = Player::X;
                winner_ = Player::Empty;
                moves_count_ = 0;
            }

            bool make_move(int index)
            {
                if (index < 0 || index >= 9 || cells_[index] != Player::Empty || winner_ != Player::Empty)
                {
                    return false;
                }

                cells_[index] = current_turn_;
                moves_count_++;
                
                check_win_condition();

                if (winner_ == Player::Empty)
                {
                    current_turn_ = (current_turn_ == Player::X) ? Player::O : Player::X;
                }

                return true;
            }

            Player get_cell(int index) const { return cells_[index]; }
            Player get_winner() const { return winner_; }
            bool is_draw() const { return winner_ == Player::Empty && moves_count_ == 9; }
            Player get_current_turn() const { return current_turn_; }
    };
} // namespace tictactoe

#endif // TICTACTOE_LOGIC_HPP
/**
 * @file game.hpp
 * @author Gabriel Ferreira
 * @brief Orquestador principal de la interfaz gráfica y la lógica de negocio.
 */

#ifndef GAME_HPP
#define GAME_HPP

#include "tictactoe_logic.hpp"
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/fl_ask.H>
#include <memory>
#include <array>

/**
 * @class Game
 * @brief Aplicación principal que gestiona el ciclo de vida de FLTK.
 */
class Game
{
    private:
        // Uso de std::unique_ptr para implementar RAII.
        // Garantiza que la memoria de la ventana se libere automáticamente
        // cuando el objeto Game sea destruido, previniendo memory leaks.
        std::unique_ptr<Fl_Window> window_;
        
        // Los botones de FLTK son destruidos automáticamente por la ventana padre,
        // por lo que usamos punteros crudos observadores (estándar en FLTK).
        std::array<Fl_Button*, 9> buttons_;
        
        tictactoe::Board board_;

        /**
         * @brief Callback estático requerido por FLTK.
         * @details Como FLTK está escrito en C, sus callbacks no pueden ser métodos 
         * de instancia C++. Pasamos 'this' a través del parámetro 'user_data' 
         * y lo casteamos de vuelta para acceder a la instancia actual.
         */
        static void button_callback_static(Fl_Widget* widget, void* user_data)
        {
            Game* instance = static_cast<Game*>(user_data);
            instance->handle_click(widget);
        }

        void handle_click(Fl_Widget* widget)
        {
            Fl_Button* clicked_button = static_cast<Fl_Button*>(widget);
            
            // Buscamos qué índice de botón fue presionado
            int index = -1;
            for (int i = 0; i < 9; ++i)
            {
                if (buttons_[i] == clicked_button)
                {
                    index = i;
                    break;
                }
            }

            if (index != -1 && board_.make_move(index))
            {
                update_ui();
                check_game_over();
            }
        }

        void update_ui()
        {
            for (int i = 0; i < 9; ++i)
            {
                tictactoe::Player p = board_.get_cell(i);
                if (p == tictactoe::Player::X)
                {
                    buttons_[i]->label("X");
                    buttons_[i]->labelcolor(FL_RED);
                }
                else if (p == tictactoe::Player::O)
                {
                    buttons_[i]->label("O");
                    buttons_[i]->labelcolor(FL_BLUE);
                }
                else
                {
                    buttons_[i]->label("");
                }
            }
            window_->redraw();
        }

        void check_game_over()
        {
            if (board_.get_winner() != tictactoe::Player::Empty)
            {
                std::string msg = (board_.get_winner() == tictactoe::Player::X) ? "X Wins!" : "O Wins!";
                fl_message("%s", msg.c_str());
                board_.reset();
                update_ui();
            }
            else if (board_.is_draw())
            {
                fl_message("It's a Draw!");
                board_.reset();
                update_ui();
            }
        }

    public:
        Game()
        {
            // Inicialización RAII de la ventana principal
            window_ = std::make_unique<Fl_Window>(300, 300, "Ta Te Ti - GREENSOFT");

            // Construcción del grid 3x3
            int button_size = 100;
            for (int i = 0; i < 9; ++i)
            {
                int col = i % 3;
                int row = i / 3;
                
                buttons_[i] = new Fl_Button(col * button_size, row * button_size, button_size, button_size);
                buttons_[i]->labelsize(40);
                buttons_[i]->callback(button_callback_static, this); // Pasamos 'this' como contexto
            }

            window_->end();
        }

        ~Game()
        {
            // std::unique_ptr destruye 'window_' automáticamente aquí.
            // A su vez, FLTK garantiza la destrucción de los botones hijos.
        }

        int run()
        {
            window_->show();
            // Inicia el bucle de eventos principal de FLTK (Thread bloqueante)
            return Fl::run();
        }
};

#endif // GAME_HPP
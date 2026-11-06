/**
 * @file main.cpp
 * @author Gabriel Ferreira
 * @brief Punto de entrada minimalista del orquestador.
 */

#include "game.hpp"
#include <iostream>
#include <stdexcept>

int main()
{
    try
    {
        Game app;
        return app.run();
    }
    catch (const std::exception& e)
    {
        // Captura estructurada de excepciones para evitar abortos silenciosos
        std::cerr << "[FATAL] Error de infraestructura: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}
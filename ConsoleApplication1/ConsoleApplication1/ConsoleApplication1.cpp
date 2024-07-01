#include <SDL.h>
#include <iostream>

// Dimensiones de la ventana
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

// Velocidad del personaje
const int VELOCITY = 10;

// Tamaños del personaje
const int CHARACTER_WIDTH = 50;
const int CHARACTER_HEIGHT_STANDING = 100;
const int CHARACTER_HEIGHT_CROUCHING = 50;

// Altura y duración del salto
const int JUMP_HEIGHT = 100;
const int JUMP_DURATION = 500; // Duración en milisegundos

int main(int argc, char* argv[]) {
    // Inicialización de SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "No se pudo inicializar SDL: " << SDL_GetError() << std::endl;
        return -1;
    }

    // Crear la ventana
    SDL_Window* window = SDL_CreateWindow("Juego SDL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "No se pudo crear la ventana: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    // Crear el renderizador
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "No se pudo crear el renderizador: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // Posición inicial del personaje
    SDL_Rect characterTop = { WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 - CHARACTER_HEIGHT_STANDING / 2, CHARACTER_WIDTH, CHARACTER_HEIGHT_STANDING / 2 };
    SDL_Rect characterBottom = { WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, CHARACTER_WIDTH, CHARACTER_HEIGHT_STANDING / 2 };

    // Bucle principal
    bool running = true;
    bool isCrouching = false;
    bool isJumping = false;
    int jumpStartTime = 0;
    int jumpStartY = 0;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                case SDLK_a: // Mover a la izquierda
                    characterTop.x -= VELOCITY;
                    characterBottom.x -= VELOCITY;
                    break;
                case SDLK_d: // Mover a la derecha
                    characterTop.x += VELOCITY;
                    characterBottom.x += VELOCITY;
                    break;
                case SDLK_w: // Mover hacia arriba
                    characterTop.y -= VELOCITY;
                    characterBottom.y -= VELOCITY;
                    break;
                case SDLK_s: // Mover hacia abajo
                    characterTop.y += VELOCITY;
                    characterBottom.y += VELOCITY;
                    break;
                case SDLK_SPACE: // Saltar
                    if (!isJumping) {
                        isJumping = true;
                        jumpStartTime = SDL_GetTicks();
                        jumpStartY = characterTop.y;
                    }
                    break;
                case SDLK_LSHIFT: // Agacharse
                    if (!isCrouching) {
                        characterTop.y += CHARACTER_HEIGHT_STANDING / 2;
                        isCrouching = true;
                    }
                    break;
                }
            }
            if (event.type == SDL_KEYUP) {
                if (event.key.keysym.sym == SDLK_LSHIFT && isCrouching) {
                    characterTop.y -= CHARACTER_HEIGHT_STANDING / 2;
                    isCrouching = false;
                }
            }
        }

        // Actualizar la posición del personaje durante el salto
        if (isJumping) {
            int jumpElapsedTime = SDL_GetTicks() - jumpStartTime;
            if (jumpElapsedTime < JUMP_DURATION / 2) {
                // Ascenso
                characterTop.y = jumpStartY - (JUMP_HEIGHT * jumpElapsedTime / (JUMP_DURATION / 2));
            }
            else if (jumpElapsedTime < JUMP_DURATION) {
                // Descenso
                characterTop.y = jumpStartY - (JUMP_HEIGHT * (JUMP_DURATION - jumpElapsedTime) / (JUMP_DURATION / 2));
            }
            else {
                isJumping = false;
                characterTop.y = jumpStartY;
            }
            characterBottom.y = characterTop.y + CHARACTER_HEIGHT_STANDING / 2;
        }

        // Limpiar la pantalla
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Dibujar el personaje
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &characterTop);
        SDL_RenderFillRect(renderer, &characterBottom);

        // Actualizar la pantalla
        SDL_RenderPresent(renderer);
    }

    // Limpiar recursos
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}










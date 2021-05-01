#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>

int main()
{
    int grid_cell_size = 50;
    int grid_width = 10;
    int grid_height = 10;

    int window_width = (grid_width * grid_cell_size) + 1;
    int window_height = (grid_height * grid_cell_size) + 1;

    // Place the grid cursor in the middle of the screen.
    SDL_Rect grid_cursor = {
        .x = (grid_width - 1) / 2 * grid_cell_size,
        .y = (grid_height - 1) / 2 * grid_cell_size,
        .w = grid_cell_size,
        .h = grid_cell_size,
    };

    // Mouse ghost cursor
    SDL_Rect grid_cursor_ghost = {
        grid_cursor.x,
        grid_cursor.y,
        grid_cell_size,
        grid_cell_size
    };

    // Dark theme
    // SDL_Color grid_background = {22, 22, 22, 255}; // Barely Black
    // SDL_Color grid_line_color = {44, 44, 44, 255}; // Dark grey
    // SDL_Color grid_cursor_ghost_color = {44, 44, 44, 255};
    // SDL_Color grid_cursor_color = {255, 255, 255, 255}; // White

    // Light Theme
    SDL_Color grid_background           = { 220, 220, 220, 255 };
    SDL_Color grid_line_color           = { 200, 200, 200, 255 };
    SDL_Color grid_cursor_ghost_color   = { 200, 200, 200, 255 };
    SDL_Color grid_cursor_color         = { 160, 160, 160, 255 };

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window * window;
    SDL_Renderer * renderer;

    SDL_CreateWindowAndRenderer(window_width, window_height, 0, &window, &renderer);
    SDL_SetWindowTitle(window, "SDL Grid");

    SDL_bool quit           = SDL_FALSE;
    SDL_bool mouse_active   = SDL_FALSE;
    SDL_bool mouse_hover    = SDL_FALSE;

    while (!quit)
    {
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_w:
                        case SDLK_UP:
                            grid_cursor.y = grid_cursor.y - grid_cell_size;
                            break;

                        case SDLK_s:
                        case SDLK_DOWN:
                            grid_cursor.y = grid_cursor.y + grid_cell_size;
                            break;

                        case SDLK_a:
                        case SDLK_LEFT:
                            grid_cursor.x = grid_cursor.x - grid_cell_size;
                            break;

                        case SDLK_d:
                        case SDLK_RIGHT:
                            grid_cursor.x = grid_cursor.x + grid_cell_size;
                            break;
                    }

                    break;

                case SDL_MOUSEMOTION:
                    grid_cursor_ghost.x = (event.motion.x / grid_cell_size) * grid_cell_size;
                    grid_cursor_ghost.y = (event.motion.y / grid_cell_size) * grid_cell_size;

                    if (!mouse_active) {
                        mouse_active = SDL_TRUE;
                    }

                    break;

                case SDL_MOUSEBUTTONDOWN:
                    grid_cursor.x = (event.motion.x / grid_cell_size) * grid_cell_size;
                    grid_cursor.y = (event.motion.y / grid_cell_size) * grid_cell_size;

                    break;

                case SDL_WINDOWEVENT: // Mouse entered window
                    if (event.window.event == SDL_WINDOWEVENT_ENTER && !mouse_hover) {
                        mouse_hover = SDL_TRUE;
                    } else if (event.window.event == SDL_WINDOWEVENT_LEAVE && mouse_hover) {
                        mouse_hover = SDL_FALSE;
                    }

                    break;

                case SDL_QUIT:
                    quit = SDL_TRUE;
                    break;
            }
        }

        // Draw grid background.
        SDL_SetRenderDrawColor(renderer, grid_background.r, grid_background.g, grid_background.b, grid_background.a);
        SDL_RenderClear(renderer);


        // Draw grid ghost cursor.
        if (mouse_active && mouse_hover) {
            SDL_SetRenderDrawColor(renderer, grid_cursor_ghost_color.r, grid_cursor_ghost_color.g, grid_cursor_ghost_color.b, grid_cursor_ghost_color.a);
            SDL_RenderFillRect(renderer, &grid_cursor_ghost);
        }


        // Draw grid cursor.
        SDL_SetRenderDrawColor(renderer, grid_cursor_color.r, grid_cursor_color.g, grid_cursor_color.b, grid_cursor_color.a);
        SDL_RenderFillRect(renderer, &grid_cursor);


        // Draw grid lines on top.
        SDL_SetRenderDrawColor(renderer, grid_line_color.r, grid_line_color.g, grid_line_color.b, grid_line_color.a);

        // Horizontal
        for (int y = 0; y < 1 + grid_height * grid_cell_size; y += grid_cell_size) {
            SDL_RenderDrawLine(renderer, 0, y, window_width, y);
        }

        // Vertical
        for (int x = 0; x < 1 + grid_width * grid_cell_size; x += grid_cell_size) {
            SDL_RenderDrawLine(renderer, x, 0, x, window_height);
        }


        /* Up until now everything was drawn behind the scenes.
           This will show the new, red contents of the window. */
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
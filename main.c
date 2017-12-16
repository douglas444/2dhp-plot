#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#define SCALE 30
#define AA_SIZE 8
#define DISPLACEMENT 50



struct coord
{
    int x, y;
};
typedef struct coord Coord;



Coord subtract_coord
(
    Coord c1,
    Coord c2
)
/* =======================================================
 * Calculates the difference between two given coordinates
 * =======================================================
 */
{
    Coord c3;

    c3.x = c1.x - c2.x;
    c3.y = c1.y - c2.y;

    return c3;
}



Coord sum_coord
(
    Coord c1,
    Coord c2
)
/* =======================================================
 * Calculates the difference between two given coordinates
 * =======================================================
 */
{
    Coord c3;

    c3.x = c1.x + c2.x;
    c3.y = c1.y + c2.y;

    return c3;
}



Coord create_new_coord(int x, int y)
/* ===========================================================
 * Returns a new variables of type Coord with the given values
 * ===========================================================
 */
{
    Coord coord;
    coord.x = x;
    coord.y = y;
    return coord;
}



Coord straight(Coord prev_move)
/* =================================================================
 * Calculates straight move corresponding to the given previous move
 * =================================================================
 */
{
    Coord move;

    if (prev_move.x == 1)
    {
        move.x = 1;
        move.y = 0;
    }
    else if (prev_move.x == -1)
    {
        move.x = -1;
        move.y = 0;
    }
    else if (prev_move.y == -1)
    {
        move.x = 0;
        move.y = -1;
    }
    else if (prev_move.y == 1)
    {
        move.x = 0;
        move.y = 1;
    }
    else
    {
        exit(1);
    }

    return move;
}



Coord left(Coord prev_move)
/* =============================================================
 * Calculates left move corresponding to the given previous move
 * =============================================================
 */
{
    Coord move;

    if (prev_move.x == 1)
    {
        move.x = 0;
        move.y = 1;
    }
    else if (prev_move.x == -1)
    {
        move.x = 0;
        move.y = -1;
    }
    else if (prev_move.y == -1)
    {
        move.x = 1;
        move.y = 0;
    }
    else if (prev_move.y == 1)
    {
        move.x = -1;
        move.y = 0;
    }
    else
    {
        exit(1);
    }

    return move;
}



Coord right(Coord prev_move)
/* ==============================================================
 * Calculates right move corresponding to the given previous move
 * ==============================================================
 */
{
    Coord move;

    if (prev_move.x == 1)
    {
        move.x = 0;
        move.y = -1;
    }
    else if (prev_move.x == -1)
    {
        move.x = 0;
        move.y = 1;
    }
    else if (prev_move.y == -1)
    {
        move.x = -1;
        move.y = 0;
    }
    else if (prev_move.y == 1)
    {
        move.x = 1;
        move.y = 0;
    }
    else
    {
        exit(1);
    }

    return move;
}



void init_SDL
(
    SDL_Window **w,
    SDL_Renderer **r,
    char *win_title,
    int win_w,
    int win_h
)
/* ====================================
 * Initializes SDL and window variables
 * ====================================
 */
{
    if(SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        exit(1);
    };
    *w = SDL_CreateWindow(win_title, 900, 50, win_w, win_h, SDL_WINDOW_SHOWN);
    *r = SDL_CreateRenderer(*w, -1, SDL_RENDERER_SOFTWARE);
}



void quit_SDL(SDL_Window **w, SDL_Renderer **r)
/* ====================================
 * Destroy SDL and window variables
 * ====================================
 */
{
    SDL_DestroyRenderer(*r);
    SDL_DestroyWindow (*w);
    SDL_Quit();
}

void generate_aa_coords
(
    char *directions,
    Coord *aa_coords,
    int *conformation_w,
    int *conformation_h,
    int seq_len
)
/* ================================================================
 * Convert a array of relative directions to a array of coordinates
 * ================================================================
 */
{
    int i;
    int max_x;
    int max_y;
    int min_x;
    int min_y;
    int x_deslocation;
    int y_deslocation;

    max_x = 0;
    max_y = 0;
    min_x = 0;
    min_y = 0;
    x_deslocation = 0;
    y_deslocation = 0;

    aa_coords[0] = create_new_coord(0, 0);
    aa_coords[1] = create_new_coord(0, 1);

    for (i = 2; i < seq_len; ++i)
    {
        switch(directions[i - 1])
        {
        case 'L':
            aa_coords[i] = sum_coord(aa_coords[i - 1], left(subtract_coord(aa_coords[i - 1], aa_coords[i - 2])));
            break;
        case 'R':
            aa_coords[i] = sum_coord(aa_coords[i - 1], right(subtract_coord(aa_coords[i - 1], aa_coords[i - 2])));
            break;
        case 'S':
            aa_coords[i] = sum_coord(aa_coords[i - 1], straight(subtract_coord(aa_coords[i - 1], aa_coords[i - 2])));
            break;
        default:
            break;
        }

        if (aa_coords[i].x < min_x)
        {
            min_x = aa_coords[i].x;
        }
        if (aa_coords[i].x > max_x)
        {
            max_x = aa_coords[i].x;
        }
        if (aa_coords[i].y < min_y)
        {
            min_y = aa_coords[i].y;
        }
        if (aa_coords[i].y > max_y)
        {
            max_y = aa_coords[i].y;
        }

    }

    if (min_x < 0)
    {
        x_deslocation = -1 * min_x;
    }
    if (min_y < 0)
    {
        y_deslocation = -1 * min_y;
    }
    if (x_deslocation != 0 || y_deslocation != 0)
    {

        for (i = 0; i < seq_len; ++i)
        {
            aa_coords[i].x += x_deslocation;
            aa_coords[i].y += y_deslocation;
        }
    }

    *conformation_w = abs(max_x - min_x);
    *conformation_h = abs(max_y - min_y);
}

int main(int argc, char **argv)
{


    if (argc != 4) {
        exit(1);
    }

    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Rect rect;
    SDL_Surface *surface;
    char win_title[10] = {'2', 'd', 'h', 'p', '_', 'p', 'l', 'o', 't', '\0'};
    int i;
    int j;
    int k;
    int aux;
    int conformation_w;
    int conformation_h;
    int seq_len;
    int x1;
    int y1;
    int x2;
    int y2;
    Coord *aa_coords;

    seq_len = strlen(argv[1]);
    aa_coords = (Coord*) malloc(sizeof(Coord) * seq_len);
    generate_aa_coords(argv[2], aa_coords, &conformation_w, &conformation_h, seq_len);


    //Initial configuration for SDL and screen

    init_SDL(&window, &renderer, win_title,
             conformation_w * SCALE + 2 * DISPLACEMENT + AA_SIZE,
             conformation_h * SCALE + 2 * DISPLACEMENT + AA_SIZE);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    //Draw edges

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    for (i = 0; i < seq_len - 1; ++i)
    {
        x1 = aa_coords[i].x * SCALE + DISPLACEMENT + AA_SIZE/2;
        y1 = aa_coords[i].y * SCALE + DISPLACEMENT + AA_SIZE/2;

        x2 = aa_coords[i + 1].x * SCALE + DISPLACEMENT + AA_SIZE/2;
        y2 = aa_coords[i + 1].y * SCALE + DISPLACEMENT + AA_SIZE/2;

        SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
    }

    //Draw HH contacts

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    for (i = 0; i < seq_len; ++i)
    {
        for (j = i - 2; j >= 0; --j)
        {
            x1 = aa_coords[i].x * SCALE + DISPLACEMENT + AA_SIZE/2;
            y1 = aa_coords[i].y * SCALE + DISPLACEMENT + AA_SIZE/2;

            x2 = aa_coords[j].x * SCALE + DISPLACEMENT + AA_SIZE/2;
            y2 = aa_coords[j].y * SCALE + DISPLACEMENT + AA_SIZE/2;

            if (sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2)) == SCALE && argv[1][i] == 'H' && argv[1][j] == 'H')
            {
                for (k = (int) SCALE/10; k < SCALE; k += (int) 2*SCALE/10)
                {
                    if (x1 == x2)
                    {
                        if (y2 < y1)
                        {
                            aux = y2;
                            y2 = y1;
                            y1 = aux;

                            aux = x2;
                            x2 = x1;
                            x1 = aux;
                        }
                        SDL_RenderDrawLine(renderer, x1, y1 + k, x2, y1 + k + (int) SCALE / 10);
                    }
                    else
                    {
                        if (x2 < x1)
                        {
                            aux = x2;
                            x2 = x1;
                            x1 = aux;

                            aux = y2;
                            y2 = y1;
                            y1 = aux;
                        }
                        SDL_RenderDrawLine(renderer, x1 + k, y1, x1 + k + (int) SCALE / 10, y1);
                    }
                }
            }


        }
    }

    //Draw AA's

    for (i = 0; i < seq_len; ++i)
    {
        if (argv[1][i] == 'H')
        {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        }
        else
        {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        }

        rect.x = aa_coords[i].x * SCALE + DISPLACEMENT;
        rect.y = aa_coords[i].y * SCALE + DISPLACEMENT;
        rect.w = AA_SIZE;
        rect.h = AA_SIZE;

        SDL_RenderFillRect(renderer, &rect);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

        x1 = rect.x;
        y1 = rect.y;
        x2 = rect.x + rect.w;
        y2 = rect.y;

        SDL_RenderDrawLine(renderer, x1, y1, x2, y2);

        x1 = rect.x;
        y1 = rect.y;
        x2 = rect.x;
        y2 = rect.y + rect.h;

        SDL_RenderDrawLine(renderer, x1, y1, x2, y2);

        x1 = rect.x + rect.w;
        y1 = rect.y + rect.h;
        x2 = rect.x + rect.w;
        y2 = rect.y;

        SDL_RenderDrawLine(renderer, x1, y1, x2, y2);

        x1 = rect.x + rect.w;
        y1 = rect.y + rect.h;
        x2 = rect.x;
        y2 = rect.y + rect.h;

        SDL_RenderDrawLine(renderer, x1, y1, x2, y2);

        if (i == 0 || i == seq_len - 1)
        {
            rect.x = aa_coords[i].x * SCALE + DISPLACEMENT - 2;
            rect.y = aa_coords[i].y * SCALE + DISPLACEMENT - 2;
            rect.w = AA_SIZE + 4;
            rect.h = AA_SIZE + 4;

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

            x1 = rect.x;
            y1 = rect.y;
            x2 = rect.x + rect.w;
            y2 = rect.y;

            SDL_RenderDrawLine(renderer, x1, y1, x2, y2);

            x1 = rect.x;
            y1 = rect.y;
            x2 = rect.x;
            y2 = rect.y + rect.h;

            SDL_RenderDrawLine(renderer, x1, y1, x2, y2);

            x1 = rect.x + rect.w;
            y1 = rect.y + rect.h;
            x2 = rect.x + rect.w;
            y2 = rect.y;

            SDL_RenderDrawLine(renderer, x1, y1, x2, y2);

            x1 = rect.x + rect.w;
            y1 = rect.y + rect.h;
            x2 = rect.x;
            y2 = rect.y + rect.h;

            SDL_RenderDrawLine(renderer, x1, y1, x2, y2);

            rect.x = aa_coords[i].x * SCALE + DISPLACEMENT - 1;
            rect.y = aa_coords[i].y * SCALE + DISPLACEMENT - 1;
            rect.w = AA_SIZE + 2;
            rect.h = AA_SIZE + 2;

            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

            x1 = rect.x;
            y1 = rect.y;
            x2 = rect.x + rect.w;
            y2 = rect.y;

            SDL_RenderDrawLine(renderer, x1, y1, x2, y2);

            x1 = rect.x;
            y1 = rect.y;
            x2 = rect.x;
            y2 = rect.y + rect.h;

            SDL_RenderDrawLine(renderer, x1, y1, x2, y2);

            x1 = rect.x + rect.w;
            y1 = rect.y + rect.h;
            x2 = rect.x + rect.w;
            y2 = rect.y;

            SDL_RenderDrawLine(renderer, x1, y1, x2, y2);

            x1 = rect.x + rect.w;
            y1 = rect.y + rect.h;
            x2 = rect.x;
            y2 = rect.y + rect.h;

            SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
        }

    }

    //Quit loop

    SDL_RenderPresent(renderer);
    surface = SDL_GetWindowSurface(window);
    SDL_SaveBMP(surface, argv[3]);

    //Free memory

    quit_SDL(&window, &renderer);
    free(aa_coords);

    return 0;
}

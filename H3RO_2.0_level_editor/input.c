/**Fichier contenant la fonction g�rant les entr�es de la part de l'utilisateur. Elle attend tous les cas suivant venant de l'utilisateur*/

#include "input.h"

 void getInput(void)
 {
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {

            case SDL_QUIT:
                exit(0);
            break;

            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        exit(0);
                    break;

                    case SDLK_LEFT:
                        input.left = 1;
                    break;

                    case SDLK_RIGHT:
                        input.right = 1;
                    break;

                    case SDLK_UP:
                        input.up = 1;
                    break;

                    case SDLK_DOWN:
                        input.down = 1;
                    break;

                    case SDLK_s:
                        input.save = 1;
                    break;

                    case SDLK_l:
                        input.load = 1;
                    break;

                    case SDLK_DELETE:
                        input.reinit = 1;
                    break;

                    case SDLK_F1:
                        input.levelup = 1;
                    break;

                    case SDLK_F2:
                        input.leveldown = 1;
                    break;

                    case SDLK_z:
                        input.next = 1;
                   break;

                    case SDLK_a:
                        input.previous = 1;
                    break;

                    default:
                    break;
                }
            break;

            case SDL_KEYUP:
                switch (event.key.keysym.sym)
                {
                    case SDLK_LEFT:
                        input.left = 0;
                    break;

                    case SDLK_RIGHT:
                        input.right = 0;
                    break;

                    case SDLK_UP:
                        input.up = 0;
                    break;

                    case SDLK_DOWN:
                        input.down = 0;
                    break;

                    default:
                    break;
                }
            break;

            case SDL_MOUSEBUTTONDOWN:
                switch(event.button.button)
                {
                    case SDL_BUTTON_LEFT:
                        input.add = 1;
                   break;

                    case SDL_BUTTON_MIDDLE:
                        input.remove = 1;
                   break;

                    case SDL_BUTTON_RIGHT:
                        input.copy = 1;
                   break;

                    case SDL_BUTTON_WHEELUP:
                        input.next = 1;
                   break;

                    case SDL_BUTTON_WHEELDOWN:
                        input.previous = 1;
                    break;


                    default:
                    break;
                }
           break;

            case SDL_MOUSEBUTTONUP:
                switch(event.button.button)
                {
                    case SDL_BUTTON_LEFT:
                        input.add = 0;
                   break;

                    case SDL_BUTTON_MIDDLE:
                        input.remove = 0;
                    break;

                    default:
                    break;
                }
            break;
        }
    }

    //Enregistrement des coordonn�es de la souris
    SDL_GetMouseState(&input.mouseX, &input.mouseY);

    input.mouseX /= TILE_SIZE;
    input.mouseY /= TILE_SIZE;

    input.mouseX *= TILE_SIZE;
    input.mouseY *= TILE_SIZE;
}

 void update(void)
{
    //Nous r�cup�rons ici les coordonn�es de la souris
    cursor.x = input.mouseX;
    cursor.y = input.mouseY;

    char file[120];

    //Gestion du scrolling avec les fl�ches
    if (input.left == 1)
    {
        map.startX -= TILE_SIZE;

        if (map.startX < 0) map.startX = 0;
    }

    else if (input.right == 1)
    {
        map.startX += TILE_SIZE;

        if (map.startX + SCREEN_WIDTH >= map.maxX) map.startX = map.maxX - SCREEN_WIDTH;
    }

    if (input.up == 1)
    {
        map.startY -= TILE_SIZE;

        if (map.startY < 0) map.startY = 0;
    }

    else if (input.down == 1)
    {
        map.startY += TILE_SIZE;

        if (map.startY + SCREEN_HEIGHT >= map.maxY) map.startY = map.maxY - SCREEN_HEIGHT;
    }

    //Passage d'une map � une autre
    if (input.levelup == 1)
    {
        game.level++;
        if (game.level > LEVEL_MAX ) game.level = 1;
        load();

        input.levelup = 0;
    }

    if (input.leveldown == 1)
    {
        game.level--;
        if (game.level < 1 ) game.level = LEVEL_MAX;
        load();

        input.leveldown = 0;
    }

    //Gestion pour le remplacement de la tile
    if (input.add == 1) map.tile[(map.startY + cursor.y) / TILE_SIZE][(map.startX + cursor.x) / TILE_SIZE] = cursor.tileID;

    //M�me chose ici � part qu'on r�initialise la tile point�e en lui donnant la valeur 0 celle d'une tile vide
    else if (input.remove == 1)
    {

        map.tile[(map.startY + cursor.y) / TILE_SIZE][(map.startX + cursor.x) / TILE_SIZE] = BLANK_TILE;

        cursor.tileID = 0;
    }

    //d�filement des tiles dans un sens o� dans un autre
    if (input.previous == 1)
    {
        cursor.tileID--;

        if (cursor.tileID < 0)
        {
            cursor.tileID = MAX_TILES - 1;
        }
        else if (cursor.tileID > MAX_TILES)
        {
            cursor.tileID = 0;
        }

        input.previous = 0;
    }

    if (input.next == 1)
    {
        cursor.tileID++;

        if (cursor.tileID < 0)
        {
            cursor.tileID = MAX_TILES - 1;
        }
        else if (cursor.tileID > MAX_TILES)
        {
            cursor.tileID = 0;
        }

        input.next = 0;
    }

    /* On copie le num�ro de la tile point�e dans le curseur pour qu'il affiche et colle
    d�sormais cette tile */

    if (input.copy == 1)
    {
        cursor.tileID = map.tile[(map.startY + cursor.y) / TILE_SIZE] [(map.startX + cursor.x) / TILE_SIZE];
        input.copy = 0;
    }

    /* Pour r�initialiser la map, on appelle la fonction reinitMap puis on recharge la map */

    if (input.reinit == 1)
    {
        sprintf(file, "map/map%d.txt", game.level);
        reinitMap(file);
        loadMap(file);
        input.reinit = 0;
    }

    /* Sauvegarde la map (cf. plus loin) */

    if (input.save == 1)
    {
        sprintf(file, "map/map%d.txt", game.level);
        saveMap(file);
        input.save = 0;
    }

    /* Charge la map (notre bonne vieille fonction ;) ) */

    if (input.load == 1)
    {
        sprintf(file, "map/map%d.txt", game.level);
        loadMap(file);
        input.load = 0;
    }

    /* On rajoute un d�lai entre 2 tours de boucle pour que le scrolling soit moins rapide */

    if (input.left == 1 || input.right == 1 || input.up == 1 || input.down == 1)
    {
        SDL_Delay(30);
    }
}

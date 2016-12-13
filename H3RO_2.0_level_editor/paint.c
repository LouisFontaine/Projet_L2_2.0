#include "paint.h"

void paintImage(SDL_Surface *image, int x, int y)
{
    SDL_Rect dest;

    /* R�gle le rectangle � blitter selon la taille de l'image source */

    dest.x = x;
    dest.y = y;
    dest.w = image->w;
    dest.h = image->h;

    /* Blitte l'image enti�re sur l'�cran aux coordonn�es x et y */

    SDL_BlitSurface(image, NULL, game.screen, &dest);
}

void paintTile(SDL_Surface *image, int destx, int desty, int srcx, int srcy)
{
    /* Rectangle de destination � blitter */
    SDL_Rect dest;

    dest.x = destx;
    dest.y = desty;
    dest.w = TILE_SIZE;
    dest.h = TILE_SIZE;

    /* Rectangle source � blitter */
    SDL_Rect src;

    src.x = srcx;
    src.y = srcy;
    src.w = TILE_SIZE;
    src.h = TILE_SIZE;

    /* Blitte la tile choisie sur l'�cran aux coordonn�es x et y */

    SDL_BlitSurface(image, &src, game.screen, &dest);

}

void paint(void)
{

    /* Affiche le fond (background) aux coordonn�es (0,0) */
    paintImage(map.background, 0, 0);

    /* Affiche la map de tiles */
    paintMap();

    /* Affiche l'�cran */
    SDL_Flip(game.screen);

    /* Delai */
    SDL_Delay(1);

}

SDL_Surface *loadImage(char *name)
{
    /* Charge une image temporaire avec SDL Image */

    SDL_Surface *temp = IMG_Load(name);
    SDL_Surface *image;

    /* Si elle n'est pas charg�e on quitte avec une erreur */
    if (temp == NULL)
    {
        printf("Failed to load image %s\n", name);

        return NULL;
    }

    /* Ajoute la transparence � l'image temporaire en accord avec les defines TRANS_R, G, B */

    SDL_SetColorKey(temp, (SDL_SRCCOLORKEY | SDL_RLEACCEL), SDL_MapRGB(temp->format, TRANS_R, TRANS_G,    TRANS_B));

    /* Convertit l'image au format de l'�cran (screen) pour un blit plus rapide */

    image = SDL_DisplayFormat(temp);

    /* Lib�re l'image temporaire */
    SDL_FreeSurface(temp);

    if (image == NULL)
    {
        printf("Failed to convert image %s to native format\n", name);

        return NULL;
    }

    /* Retourne l'image au format de l'�cran pour acc�l�rer son blit */

    return image;

}

void delay(unsigned int frameLimit)
{

    /* Gestion des 60 fps (images/stories/seconde) */

    unsigned int ticks = SDL_GetTicks();

    if (frameLimit < ticks)
    {
        return;
    }

    if (frameLimit > ticks + 16)
    {
        SDL_Delay(16);
    }

    else
    {
        SDL_Delay(frameLimit - ticks);
    }
}

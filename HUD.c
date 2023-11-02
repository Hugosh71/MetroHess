#include <stdio.h>
#include "../structures.h"

//cette partie regroupe toutes les fcts concernant les états de jeu
//nous avons défini précedemment 4 etat : init , game , gameover, win
//pour chaque etat (sauf game ou on affiche le jeu) on aura 3 fcts : une qui cree une surface de texte/dessin , une qui dessine sur le renderer  et une qui ferme/free tout


void initINIT(GameState *game) //fct pour initaliser l'écran de début de l'état INIT
{
    char tabecriture[100];//creation de tableau pour contenir le texte qu'on passe en parametre dans TTF RENDERTEXT
    sprintf(tabecriture,"x%d",(int) game->player.lives);//fct qui fait tout pareil que printf , sauf qu'elle prend en parametre un tableau dans lequel on printera les 2 autres parametres.

    SDL_Color white={255,255,255,255};//on set la couleur de l'ecriture en blanc

    SDL_Surface *livecount=TTF_RenderText_Blended(game->font,tabecriture,white); //renderTextblended cree une surface d ecriture
    game->labelW = livecount->w;//la largeur du label prend la val de la largeur de la surface
    game->labelH = livecount->h;//mm chose pour hauteur
    game->label=SDL_CreateTextureFromSurface(game->renderer,livecount);
    SDL_FreeSurface(livecount);

    SDL_Color red={210,4,4,255};

    SDL_Surface *text=TTF_RenderText_Blended(game->font2,"Survivez",red);//la meme
    game->label2W=text->w;
    game->label2H=text->h;
    game->label2=SDL_CreateTextureFromSurface(game->renderer,text);
    SDL_FreeSurface(text);




}

void drawINIT(GameState *game) //
{
    SDL_Renderer *renderer=game->renderer;
    SDL_SetRenderDrawColor(renderer,0,0,0,255);
    SDL_RenderClear(renderer);

    //desine le perso a coté du texte TTF , c'est stylé pour un ecran de chargement hehe
    SDL_Rect rect={310,230-24,80,80};
    SDL_RenderCopyEx(renderer,game->playerFrames[0],NULL,&rect,0,NULL,(game->player.facingright==0));
//le rendercopyEX prend en argument a la fin une fonction pour flip un perso

    SDL_Rect textrect={255,90-game->labelH,200,100}; //on centre le texte , soit a la main , soit en prenant moitié de la taille de l'ecran - la taille du label
    SDL_RenderCopy(renderer,game->label2,NULL,&textrect);

    SDL_SetRenderDrawColor(renderer,255,255,255,255);
    SDL_Rect livesrect={400,275-game->label2H,game->label2W,game->label2H};
    SDL_RenderCopy(renderer,game->label,NULL,&livesrect);


}

void shutdownINIT(GameState *game) // ferme tout ce qui est utilisé precedemment
{
    SDL_DestroyTexture(game->label);
    game->label=NULL; //on le pose a NULL car il est initialisé en NULL et qu'il y a une condition pour le free

    SDL_DestroyTexture(game->label2);
    game->label2=NULL;
}


void initgameover(GameState *game)
{

  SDL_Color white = { 255, 255, 255, 255 };

  SDL_Surface *text2 = TTF_RenderText_Blended(game->font, "GAME OVER!", white);
  game->labelW = text2->w;
  game->labelH = text2->h;
  game->label = SDL_CreateTextureFromSurface(game->renderer, text2);
  SDL_FreeSurface(text2);
}

void drawgameover(GameState *game)
{
  SDL_Renderer *renderer = game->renderer;
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
  SDL_Rect textRect = {350-game->labelW/2, 240-game->labelH/2, game->labelW, game->labelH };
  SDL_RenderCopy(renderer, game->label, NULL, &textRect);
}

void shutdowngameover(GameState *game)
{
  SDL_DestroyTexture(game->label);
  game->label = NULL;
}

void initgamewin(GameState *game)
{

  SDL_Color white = { 255, 255, 255, 255 };

  SDL_Surface *text3 = TTF_RenderText_Blended(game->font, "Bien joué chasseur", white);
  game->labelW = text3->w;
  game->labelH = text3->h;
  game->label = SDL_CreateTextureFromSurface(game->renderer, text3);
  SDL_FreeSurface(text3);
}

void drawgamewin(GameState *game)
{
  SDL_Renderer *renderer = game->renderer;
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

  SDL_RenderClear(renderer);

  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

  SDL_Rect textRect = { 320-game->labelW/2, 240-game->labelH/2, game->labelW, game->labelH };
  SDL_RenderCopy(renderer, game->label, NULL, &textRect);
}

void shutdowngamewin(GameState *game)
{
  SDL_DestroyTexture(game->label);
  game->label = NULL;
}


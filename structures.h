#ifndef STRUCTURES_H_INCLUDED
#define STRUCTURES_H_INCLUDED



#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

// creation de la struct player


typedef struct Player
{
    float x,y,dy,dx;//x et y pour la position du joueur et dy dx pour la velocité (autrement dit des qu'il y a un saut on utilise dy , de meme pour un mouvement on utilise dx)
    int lives; //creations vies
    int onground;//var pour savoir quand le joueur est sur le sol , retourne 1 si oui 0 inon
    int dead;
    int currentframe;//gere les frames
    int facingright;//gere la rotation avec le rendercopyEX
    int slowing;//gere le ralentissement
    int shooting;//assez obvious

}Player;

typedef struct // pour les murs
{
    int x ,y,w,h; // position x et y + largeur et hauteur du mur

}Mur;

typedef struct
{
    int x,y;//pos
    int dead;//set si il est mort
    int visible;//si visible a 0 , ne crée par le sdlrect du monstre donc il ne s'affiche pas
}Monster;

typedef struct
{
    float x,y,dx;//meme que pour player

}Bullet;

typedef struct // struct qui gere tout dans le jeu , a savoir player mur et tt les textures + les vars ( c'ets plus facile a tout gerer comme ca)
{
    Player player;//creation d'un player avec les var de la struct Player
    Mur murs[100];  //on ajoute 100 murs

    Mur lavas[100];

    int num_monster;//ajout monstre
    Monster monster[100];//100 monstre

    float scroll; //pr le scrolling

    SDL_Texture *Metroid;//pr les frames
    SDL_Texture *monstertexture;//texture monstre
    SDL_Texture *playerFrames[4];//de meme
    SDL_Texture *brick;//texture pr brick
    SDL_Texture *lava;
    SDL_Texture *explosion;//texture mort
    SDL_Texture *label;
    SDL_Texture *label2;//pour l'ecriture
    int labelW;
    int labelH;
    int label2W;
    int label2H;//pour obtenir la taille du label comme ca on peut le centrer pus facilement

    TTF_Font *font;//font c la police d ecriture
    TTF_Font *font2;

    //son
    Mix_Chunk *backgroundmusic, *jumpsound, *landsound, *deathsound, *gunshot;//creation des sons

    //temps
    int time;//pour gerer le temps des anims
    int deathcount;//meme chose que le time mais la c'est juste pour la mort
    int statusState;// pour savoir si on est au debut , en jeu ou mort


    SDL_Renderer *renderer;
}GameState; // gere tout les structures du jeu



#endif // STRUCTURES_H_INCLUDED

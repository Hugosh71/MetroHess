#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <time.h>
#include <math.h>
#include "../structures.h"
#include "../define.h"
#define GRAVITY 0.35f

SDL_Texture *bulletTexture;//creation de la texture des balles ( je n'arrive pas a l'utiliser quand je la passe dans gamestate :/  )
Bullet *bullets[Max_bullet]={NULL};//on cree un tableau de balles


void addbullet(float x,float y,float dx);
void suppbullet(int i);
void loadgame(GameState *game);
int collided(float x1,float y1,float x2,float y2,float wt1,float wt2,float ht1,float ht2);
void Update(GameState *game);
void collision(GameState *game);
int GestionInputs(SDL_Window *window,GameState *game);
void DrawRender(SDL_Renderer *renderer,GameState *game);
h
int main(int argc,char *argv[])
{//creation des fenetres et du renderer
    GameState gamestate;
    SDL_Window *window;
    SDL_Renderer *renderer;


    SDL_Init(SDL_INIT_VIDEO);


    window = SDL_CreateWindow("Metrohess",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,700,500,0);
    renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    gamestate.renderer=renderer;

    TTF_Init();

    Mix_OpenAudio(MIX_DEFAULT_FREQUENCY,MIX_DEFAULT_FORMAT,MIX_DEFAULT_CHANNELS,4096);//init musique

    loadgame(&gamestate);

    int loop = 0;
    while(!loop) // le jeu marche de cette facon : a chaque frame d'event:
        //on clear et on reactualise le renderer frame par frame
    {//boucle principale :
    loop=GestionInputs(window,&gamestate);

    Update(&gamestate);

    collision(&gamestate);

    DrawRender(renderer,&gamestate);

    }
    SDL_DestroyTexture(gamestate.Metroid); // on detruit tout apres sinon le GPU brule + on quitte les fct utilisées
    SDL_DestroyTexture(gamestate.playerFrames[0]);
    SDL_DestroyTexture(gamestate.playerFrames[1]);
    SDL_DestroyTexture(gamestate.playerFrames[2]);
    SDL_DestroyTexture(gamestate.playerFrames[3]);
    SDL_DestroyTexture(gamestate.brick);
    SDL_DestroyTexture(gamestate.lava);
    SDL_DestroyTexture(gamestate.monstertexture);
    SDL_DestroyTexture(bulletTexture);

    for(int i = 0; i < Max_bullet; i++)
        {
            suppbullet(i);
        }


    if(gamestate.label !=NULL)//si il est different de NULL  on le detruit , c'est pour ca qu'on le set a NULL dans le HUD
    {
         SDL_DestroyTexture(gamestate.label);
    }
     if(gamestate.label2 !=NULL)
    {
         SDL_DestroyTexture(gamestate.label2);
    }

    TTF_CloseFont(gamestate.font);
    TTF_CloseFont(gamestate.font2);

    Mix_FreeChunk(gamestate.backgroundmusic);
    Mix_FreeChunk(gamestate.deathsound);
    Mix_FreeChunk(gamestate.jumpsound);
    Mix_FreeChunk(gamestate.landsound);
    Mix_FreeChunk(gamestate.gunshot);


    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);

    TTF_Quit();

    SDL_Quit();
    return 0;
}

void addbullet(float x,float y,float dx)
{
    int bfound=-1;//on set bfound pour bullet found a -1 pour une boucle infinie
    for(int i=0;i<Max_bullet;i++)
    {
        if (bullets[i]==NULL)//si on ne trouve pas de balle
        {
            bfound =i;//on set bullet a i
            break;
        }
    }
    if(bfound>=0) //si on trouve une balle
    {
        int i = bfound;//le tab de i prend la valeur de la balle trouvée
        bullets[i]=malloc(sizeof(Bullet));//on fait un malloc pour que la balle presente dans le tab bullets[i] prenne les paramettres de la struct Bullet , donc x , y , dx
        bullets[i]->x=x;//on assigne chaque parametre de la balle au x ,y et dx d'une structure Bullet
        bullets[i]->y=y;
        bullets[i]->dx=dx;
    }
}

void suppbullet(int i)//suppresion de la balle a l'ecran
{
    if(bullets[i])//si une balle est trouvée
    {
        free(bullets[i]);//on supprime la balle
        bullets[i]=NULL;//on assigne le tab a NULL
    }
}

void loadgame(GameState *game)//charge le jeu , donc structure + initialisation des variables
{
    SDL_Surface *Metroidsurface=NULL;//le chargement se passe de cette facon : on cree une surface , on charge l'image dessus , on l'assigne au tab playerframes et on free la surface  / si elle existe pas on quitte

    Metroidsurface=IMG_Load("monstre1.png");//chargement des images
    if(Metroidsurface==NULL)//si elle n'est pas trouvé
    {
        printf("png introuvable"); //ben on quitte la sdl
        SDL_Quit();
        exit(1);
    }
    game->monstertexture = SDL_CreateTextureFromSurface(game->renderer, Metroidsurface);
    SDL_FreeSurface(Metroidsurface);

    Metroidsurface=IMG_Load("debout.png");
    if(Metroidsurface==NULL)
    {
        printf("non non\n");
        SDL_Quit();
        exit(1);

    }
    game->playerFrames[0]=SDL_CreateTextureFromSurface(game->renderer,Metroidsurface);
    SDL_FreeSurface(Metroidsurface);

    Metroidsurface=IMG_Load("cours1.png");
    if(Metroidsurface==NULL)
    {
    printf("tjr pas bro\n");
    SDL_Quit();
    exit(1);
    }
    game->playerFrames[1]=SDL_CreateTextureFromSurface(game->renderer,Metroidsurface);
    SDL_FreeSurface(Metroidsurface);

    Metroidsurface=IMG_Load("cours2.png");
    if(Metroidsurface==NULL)
    {
    printf("tjr pas bro\n");
    SDL_Quit();
    exit(1);
    }
    game->playerFrames[2]=SDL_CreateTextureFromSurface(game->renderer,Metroidsurface);
    SDL_FreeSurface(Metroidsurface);

     Metroidsurface=IMG_Load("cours3.png");
    if(Metroidsurface==NULL)
    {
    printf("tjr pas bro\n");
    SDL_Quit();
    exit(1);
    }
    game->playerFrames[3]=SDL_CreateTextureFromSurface(game->renderer,Metroidsurface);
    SDL_FreeSurface(Metroidsurface);

    Metroidsurface=IMG_Load("explosion.png");//animation de mort du player
    game->explosion=SDL_CreateTextureFromSurface(game->renderer,Metroidsurface);
    SDL_FreeSurface(Metroidsurface);

    Metroidsurface=IMG_Load("brick.png");//sol
    game->brick=SDL_CreateTextureFromSurface(game->renderer,Metroidsurface);
    SDL_FreeSurface(Metroidsurface);

    Metroidsurface=IMG_Load("lava.png");//lave
    game->lava=SDL_CreateTextureFromSurface(game->renderer,Metroidsurface);
    SDL_FreeSurface(Metroidsurface);

    SDL_Surface *bullet = IMG_Load("bullet.png");//anim balles
      if(!bullet)
      {
        printf("Cannot find bullet\n");
        SDL_Quit();
        exit(1);
      }
      bulletTexture = SDL_CreateTextureFromSurface(game->renderer, bullet);
      SDL_FreeSurface(bullet);

    //chargement police avec ttf
    game->font=TTF_OpenFont("police.ttf",30);//chargement police qu'on set a 30
    if(!game->font)
    {
       printf("pas de font dsl \n");
       SDL_Quit();
       exit(1);
    }

    game->font2=TTF_OpenFont("police.ttf",20);//meme que plus haut mais en plus petit
    if(!game->font2)
    {
       printf("pas de font2 dsl \n");
       SDL_Quit();
       exit(1);
    }


    //chargement son
    game->backgroundmusic=Mix_LoadWAV("Mysterious.wav");//chargement des sons .wav
    if(game->backgroundmusic!=NULL )
    {
        Mix_VolumeChunk(game->backgroundmusic,10);//on set le volume de la musique en arriere plan a 10

    }
    game->deathsound=Mix_LoadWAV("deathsound.wav");

    game->jumpsound=Mix_LoadWAV("jumpsound.wav");
    game->landsound=Mix_LoadWAV("landsound.wav");
    game->gunshot=Mix_LoadWAV("NE PAS ECOUTER.wav"); // Message a Mr.Greve : vous pouvez modifiez ce son par le son intitulé "NE PAS ECOUTEZ.wav" c'est encore plus réaliste.
        if(game->gunshot!=NULL )
    {
        Mix_VolumeChunk(game->gunshot,25);

    }

//on initialise tt les variables

    game->label=NULL; // NULL sinon detruit !
    game->player.x = 100;
    game->player.y=100;
    game->player.dx=0;//dx et dy a 0 car le player ne  bouge pas
    game->player.dy=0;
    game->player.onground=0;//spawn en altitude
    game->player.currentframe=0;
    game->player.facingright=1;//regarde a droite
    game->player.slowing=0;//pas de deplacement donc pas de rafraichissement
    game->player.shooting=0;
    game->player.lives=3;//le player a 3 vies
    game->player.dead=0;//il est en vie donc normal
    game->statusState = STATUS_STATE_INIT; //initialise le jeu en mode init , donc on ne commencd pas directement en jeu mais avec un ecran et un font
    game->num_monster=100;
    game->monster[100]=game->monster[game->num_monster];
    for(int i=0;i<game->num_monster;i++)//on charge les monstres avec une position aleatoire , en prenant en compte la taille de l'ecran bien sur (700 par 500)
    {
        game->monster[i].x=350+rand()%38400;
        game->monster[i].y=rand()%400;
    }
    //initmonsters(game);
    for(int i=0;i<game->num_monster;i++)//on les set en visible et en vivant
    {
        game->monster[i].visible=1;
        game->monster[i].dead=0;
    }

    initINIT(game);//init l ecran de debut ( le menu bas de gamme en fait)

    game->time=0;//on set le temps a 0 qui nous servira a gerer les temps d'apparition des fenetres , animations etc.
    game->scroll=0;//var pour le scroll , a 0 car on ne scroll pas encore
    game->deathcount=-1; // on utilise pas encore donc pour l'instant set a -1 (quand je la met a 0 on perd 2 vie ptit bug ...)



    for(int i=0;i<100;i++) //creation des murs et leurs tailles
    {
        game->murs[i].w=256;
        game->murs[i].h=64;
        game->murs[i].x=i*384;
        if(i==0)
        {
             game->murs[i].y=400;//premier element  du tableau avec pos fixe
        }
        else
        {
            game->murs[i].y=300+100-rand()%200; //le reste ets gerer aleatoirement
        }

    }
game->murs[99].x=350;//positionnement 99eme mur //pour tester la collision
game->murs[99].y=200;

    for(int i=0;i<100;i++) //creation laves
    {
        game->lavas[i].w=400;
        game->lavas[i].h=70;
        game->lavas[i].x=i*384;
        game->lavas[i].y=450;
    }
}

int collided(float x1,float y1,float x2,float y2,float wt1,float wt2,float ht1,float ht2)
{
    return(!((x1>(x2+wt2)) || (x2>(x1+wt1)) || (y1>(y2+ht2)) || (y2>(y1+ht1)))); //fct qui nous indique si il y a une collision.
}

void Update(GameState *game) //a mettre dans la loop principale , met a jour les mouvements , frame et var du player a chaque action
{
    //incrementation temps
    game->time++;
    if(game->statusState==STATUS_STATE_INIT);//si on est dans l'état de jeu init
    {
    if(game->time>120) // la fenetre page de garde reste  1 secondes environ
    {
        shutdownINIT(game);//ferme la page de garde et le texte
        game->statusState = STATUS_STATE_GAME; //entrons dans le jeu !
        Mix_PlayChannel(-1,game->backgroundmusic,0);//init de la musique de background
    }
    }
    if(game->statusState==STATUS_STATE_GAMEOVER)// si on est en etat de jeu gameover
    {
        if(game->time>190)//affiche 2 sec environ et on quitte
        {
            SDL_Quit();
            exit(0);
        }
    }
    else if(game->statusState == STATUS_STATE_GAME)//le jeu demarre
    {
        for(int i = 0; i < Max_bullet; i++) if(bullets[i])//si on a des balles restantes dans le tab
            {
                bullets[i]->x -= bullets[i]->dx-10; //on leur ajoute la notion de velocité (dx) pour qu'elles aillent plus vite
                if(collided(bullets[i]->x,bullets[i]->y,game->monster[i].x,game->monster[i].y,20,40,20,40))//si on detecte une collision avec la fonction collided
                {
                    for(int i=0;i<game->num_monster;i++) //dans le tab de monstre , on set le monstre non visible , on le met mort puis on baisse le nbr de monstre total du tableau(le monstre n'existe plus)
                    {
                         game->monster[i].visible=0;
                         game->monster[i].dead =1;
                         game->monster[i] = game->monster[game->num_monster - 1];
                         game->num_monster--;

                    }

                }

                if(bullets[i]->x < -38000 || bullets[i]->x > 38000)
                    {
                        suppbullet(i); //on free la balle si elle sort des limites du terrain (38 000 bloc ici)
                    }
            }
    if(!game->player.dead){ //si on est en jeu et que le joueur n'est pas mort

    //on set les mouvements
    game->player.x+=game->player.dx; //on rajoute la velocite au mouvement en x du joueur
    game->player.y += game->player.dy;//la meme pour le y


    if(game->player.dx!=0 && game->player.onground && !game->player.slowing)//si le joueur se deplace  , si il est au sol et si il ne ralentit pas  == autrement dit il est en mouvement continue
    {
        if(game->time%6==0) // 6 image seconde , franchement cela me parait assez fluide pour afficher 3 frames  , si on y touche cela modifie donc la vitesse d'affichage des frames et c moche
        {
            if(game->player.currentframe==0) //incrementation de frame , possible avec une boucle aussi
            {
                game->player.currentframe=1; // le principe est : tant qu'on est en mouvement au sol  on passe d'une frame a une autre .
            }
            else if(game->player.currentframe==1)
            {
                game->player.currentframe=2;
            }
            else if(game->player.currentframe==2)
            {
                game->player.currentframe=3;
            }
            else
            {
                game->player.currentframe=0; // des qu'on bouge plus on s'arrete a la frame d'arret
            }
        }
    }
    if(game->player.x>38300)
    {
        initgamewin(game);
        game->statusState=STATUS_STATE_WIN;
    }
    game->player.dy+=GRAVITY; //ajout gravité
    }

    if(game->player.dead && game->deathcount<0)//si le joueur est mort et que son compteur deathcount est inferieur a 0
    {
        game->deathcount=120;//on le reintilise a 120 , RAPPEL : 120 c'ets le temps que l'anim ou l'anim de mort reste
    }
    if(game->deathcount>=0)                     //si le temps de mort est sup ou egale a 0
    {
        game->deathcount--; // on l'incremente
        if(game->deathcount<0)
        {
            game->player.lives--; //on retire une vie au joueur puisque le temps de mort est fini et qu'on a relancé la page init  avec le compteur de vie

            if (game->player.lives >= 0) //si le joueur a des vies
            {
             initINIT(game);
             game->statusState = STATUS_STATE_INIT;
             game->time = 0;

             //variables du reset , ici on place juste le perso et on recrée les monstres
             game->player.dead = 0;
             game->player.x = 200 - 80;
             game->player.y = 200 - 80;
             game->player.dx = 0;
             game->player.dy = 0;
             game->player.onground = 0;
              for(int i=0;i<game->num_monster;i++)//on charge les monstres avec une position aleatoire , en prenant en compte la taille de l'ecran bien sur (700 par 500)
                    {
                        game->monster[i].x=350+rand()%38400;
                        game->monster[i].y=rand()%400;
                    }
                    for(int i=0;i<game->num_monster;i++)//on les set en visible et en vivant
                    {
                        game->monster[i].visible=1;
                        game->monster[i].dead=0;
                    }
            }
            else
            {
             initgameover(game);//si il a plus de vie
             game->statusState = STATUS_STATE_GAMEOVER; //on passe en gameover et on set le temps de jeu a 0
             game->time = 0;
            }

            }
        }
    }
    game->scroll =- game->player.x+300 ;//activation du scrolling
    if(game->scroll>0)
        {//empeche le scrolling a gauche
        game->scroll=0;
        }
    if(game->scroll<-38000+300) //quand on arrive a la limite de la map c'ets a dire 38 000 pixel + la taille du rect scrolling
    {
        game->scroll=-38000+300;
    }
}

void collision(GameState *game) //fct collision
{
    for(int i=0;i<game->num_monster;i++)
    {
        if(collided(game->player.x,game->player.y,game->monster[i].x,game->monster[i].y,80,80,60,60)) //si collision il y a entre le joueur et un monstre
        {
            if(!game->player.dead) //on bute le joueur
                {
            game->player.dead=1;
            Mix_PlayChannel(2,game->deathsound,0); //on se moque de lui HIHIHIHI
                }
            break;
        }
    }

    //on check si le perso tombe
    if(game->player.y>430){
        game->player.dead=1; //on ne se moque pas d'un homme tombé au combat
    }
    //on check les collisions pour chaque mur(bricks)
    for(int i=0;i<100;i++)
    {
        float pw=80,ph=80;//hauteur,largeur sprite
        float px=game->player.x;
        float py=game->player.y;//raccourci pour position du player
        float bx=game->murs[i].x;//raccourci brick
        float by=game->murs[i].y;
        float bw=game->murs[i].w;
        float bh=game->murs[i].h;
    if(px+pw/2 > bx && px+pw/2<bx+bw)//compliqué a expliquer en entier car pas au point , mais en gros c'ets une serie de verification , ici le joueur est sous le muret
    {

        if(py<by+bh && py>by && game->player.dy<0)  //on vérifie si on tape la tete avec le joueur contre un mur
        {
            //correction du y
            game->player.y=by+bh;
            py=by+bh;

            //on tape la tete donc stop la vélocité
            game->player.dy=0;
            game->player.onground=0;
        }
    }
        if(px+pw>bx && px<bx+bw) //il est soit au dessus soit en dessous du mur
        {

        if(py+ph>by && py<by &&game->player.dy>0) //on verifie l'aterrisage
        {
            //correction du y
            game->player.y=by-ph;
            py=by-ph;

            //on a atterrit donc stop vélocité
            game->player.dy=0;
            if(!game->player.onground)
            {
               // Mix_HaltChannel(game->musictime);
                Mix_PlayChannel(3,game->landsound,0);
                game->player.onground=1;
            }
        }
    }
    if(py+ph>by && py<by+bh)// si il n'est pas en dessous ou au dessus du block
    {

        if(px<bx+bw && px+pw > bx+bw && game->player.dx<0)//on touche le bord droit
        {
            //correction du x
            game->player.x=bx+bw;
            px=bx+bw;

            game->player.dx=0;//stop velo !
        }

        else if(px+pw>bx && px<bx && game->player.dx>0)//la meme mais la c gauche
        {
            //correction x
            game->player.x=bx-pw;
            px=bx-pw;

            game->player.dx=0; //stop la velo !
        }
    }

    }
}

int GestionInputs(SDL_Window *window,GameState *game)//a mettre dans la boucle principale , gere les input du clavier
{
    SDL_Event event;
    int loop=0;
    while(SDL_PollEvent(&event))//return True si event
        {
            switch(event.type)
            {
            case SDL_WINDOWEVENT_CLOSE:
                {
                    if(window)
                {
                    SDL_DestroyWindow(window);
                    window=NULL;
                }
                }
            break;
            case SDL_KEYDOWN:
            {
                switch(event.key.keysym.sym)
                {
                case SDLK_ESCAPE ://si echap quitte le jeu
                    loop =1;
                    break;
                case SDLK_UP://touche du haut
                    if(game->player.onground)//si le joueur est au sol quand on appui en l'air
                    {
                        game->player.dy= -8;//hauteur saut initial , l'appui continue permettra de sauter plus haut
                        game->player.onground=0;
                       // Mix_Pause(1);
                        Mix_PlayChannel(4,game->jumpsound,0);
                      //  Mix_Resume(1);
                    }
                }
            }
            break;
            case SDL_QUIT: // quitte le jeu
                loop = 1;
                break;
            }
        }
    const Uint8 *state = SDL_GetKeyboardState(NULL); //permet d'obtenir l'etat du clavier , ce n'est pas du C ++ Monsieur !!!!!
         if(state[SDL_SCANCODE_SPACE])
            {
                if(game->time %6==0) //vitesse d'anim , la meme que pour les mouvements
                {
                if(!game->player.facingright) //tire une balle mais ptit bug dessus
                {
                  addbullet(game->player.x+35, game->player.y+20, 3);
                }
                else
                {
                  addbullet(game->player.x+5, game->player.y+20, -3);
                }
                }
              //  Mix_Pause(1);
                Mix_PlayChannel(4,game->gunshot,0);
              //  Mix_Resume(1);
                game->player.shooting = 1; //le joueur est en train de tirer !
            }
            else
            {
                game->player.shooting=0;
            }
        if(state[SDL_SCANCODE_UP])//pour les appuis continus
        {
            game->player.dy -=0.2f;//on le fait monter un peu plus que le saut initial !
        }
        //marche activée :

          if(state[SDL_SCANCODE_LEFT])
          {
              game->player.dx-=0.5;
              if(game->player.dx<-6)//on fait accelerer le personnage de 0.5 a chaque fois jusqua atteindre la vitesse max de 6.
              {
                  game->player.dx=-6;
              }
              game->player.facingright=0;
              game->player.slowing=0;
          }
          else if(state[SDL_SCANCODE_RIGHT])
          {
              game->player.dx+=0.5;
              if(game->player.dx>6)
              {
                  game->player.dx=6;
              }
              game->player.facingright=1;
              game->player.slowing=0;
          }
          else
          {//syteme de ralentissement
              game->player.currentframe=0;//si il ne fait rien , frame 0
              game->player.dx*=0.8f;//si aucune touche enclenché on ralenti le perso
              game->player.slowing=1;
              if(fabsf(game->player.dx)<0.1f) //fabsf c'est val absolue , donc si la plus petit valeur du dx est atteinte , c'est a dire <0.f ben il bouge pas
              {
                  game->player.dx=0;//plus de velocite , il ne bouge plus
              }
          }

        return loop;
}

void DrawRender(SDL_Renderer *renderer,GameState *game)//creation de ce qu'il y a  a l'ecran , background couleur etc.
{
    if(game->statusState == STATUS_STATE_INIT) //passage d'etat en etat
    {
        drawINIT(game);
    }
    else if(game->statusState==STATUS_STATE_GAMEOVER)
    {
        drawgameover(game);
    }
    else if(game->statusState==STATUS_STATE_WIN)
    {
        drawgamewin(game);
    }
    else if(game->statusState == STATUS_STATE_GAME)
    {
    SDL_SetRenderDrawColor(renderer,0,0,0,0); //background noir c'est dark
    SDL_RenderClear(renderer);

    for(int i=0;i<100;i++)
    {
        SDL_Rect murRect ={game->scroll+game->murs[i].x,game->murs[i].y,game->murs[i].w,game->murs[i].h}; //on oublie pas d'ajouter le scroll au mur !
        SDL_RenderCopy(renderer,game->brick,NULL,&murRect);
    }

     for(int i=0;i<100;i++)
    {
        SDL_Rect lavaRect ={game->scroll+game->lavas[i].x,game->lavas[i].y,game->lavas[i].w,game->lavas[i].h}; //de meme
        SDL_RenderCopy(renderer,game->lava,NULL,&lavaRect);
    }
//crée un rectangle sur les coord du player
    SDL_Rect rect={game->scroll+game->player.x,game->player.y,80,80};
    SDL_RenderCopyEx(renderer,game->playerFrames[game->player.currentframe],NULL,&rect,0,NULL,(game->player.facingright==0));
//le rendercopyEX prend en argument a la fin une fonction pour flip un perso avec une condition

    if(game->player.dead) // creation de la petite flamme pour l'anim de mort
    {
    SDL_Rect rect={game->scroll+game->player.x-40+35,game->player.y-40-80,100,200};
    SDL_RenderCopyEx(renderer,game->explosion,NULL,&rect,0,NULL,(game->time%20<10));
    }

    for(int i=0;i<game->num_monster ;i++)
    {
        if(game->monster[i].visible)
            {
            SDL_Rect monsterRect={game->scroll+game->monster[i].x,game->monster[i].y,50,50};
            SDL_RenderCopy(renderer,game->monstertexture,NULL,&monsterRect);
            }
    }

    for(int i = 0; i < Max_bullet; i++)
        {
        if(bullets[i])
        {
        SDL_Rect rect = { game->scroll+bullets[i]->x+55, bullets[i]->y, 20, 20 };
        SDL_RenderCopy(renderer, bulletTexture, NULL, &rect);
        }
            }
    }
    SDL_RenderPresent(renderer);
}

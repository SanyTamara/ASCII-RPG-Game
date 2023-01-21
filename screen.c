#include "main.h"



ScreenArea* createArea(int startX, int startY, int finishX, int finishY, int conn[]){
    ScreenArea* area = malloc(sizeof(ScreenArea));
    area->start.X = startX;
    area->start.Y = startY;
    area->finish.X = finishX;
    area->finish.Y = finishY;
    for(int i = 0; i < 4; i++){
        area->connection[i] = conn[i];
    }

    return area;
};

void setScreenMap(Screen* screen){
    ScreenArea** areas = screen->areas;
    int Count = 0;
    for(int i = 0; i < HEIGHT; i++){
        for (int j = 0; j < WIDTH; j++){
            screen->ScreenMap[i][j] = EMPTY;
            if(i == WIDTH){
                screen->ScreenMap[i][j] = 10;
            }
        }
    }
    while(Count < screen->totalAreas){
        COORD* start = &areas[Count]->start;
        COORD* finish = &areas[Count]->finish;
        for(int y = (start->Y-1); y < (finish->Y+2); y++){
            for (int x = (start->X-1); x < (finish->X+2); x++){
                /*  esquerdo 0 - start-1,
                    direito  1 - start y -1 finish x+1
                    esquerdo 2 - finish y+1 start x-1
                    direito  3 - finish+1 */
                if(y == areas[Count]->start.Y-1) {
                    if(x == areas[Count]->start.X-1) screen->ScreenMap[y][x] = areas[Count]->connection[0];
                    else if(x == areas[Count]->finish.X+1) screen->ScreenMap[y][x] = areas[Count]->connection[1];
                    else screen->ScreenMap[y][x] = HORIZ;
                } else if (y == areas[Count]->finish.Y+1) {
                    if(x == areas[Count]->start.X-1) screen->ScreenMap[y][x] = areas[Count]->connection[2];
                    else if (x == areas[Count]->finish.X+1)screen->ScreenMap[y][x] = areas[Count]->connection[3];
                    else screen->ScreenMap[y][x] = HORIZ;
                }
                if(x == areas[Count]->start.X-1 || x == areas[Count]->finish.X+1){
                    if(y > areas[Count]->start.Y-1 && y < areas[Count]->finish.Y+1){
                        screen->ScreenMap[y][x] = VERTC;
                    }
                }
            }
        }
        Count++;
    }

    screen->ScreenMap[29][120] = '\0';
};

Screen* prepareScreen(Scenes* scenes, SCREEN id){
    Screen* screen = malloc(sizeof(Screen));
    int aux[4];
    screen->ID = id;
    screen->next = NULL;
    switch(id){
    case MAIN_MENU:
    case DIFFICULTY:
        aux[0] = TOP_L, aux[1] = TOP_R, aux[2] = BOT_L, aux[3] = BOT_R;
        screen->totalAreas = 2;
        screen->areas = malloc(sizeof(ScreenArea)* screen->totalAreas);
        screen->areas[0] = createArea(1,1, WIDTH-2, HEIGHT-2, aux);
        screen->areas[1] = createArea(44, 17, 74, 23, aux);
        break;
    case INTRO:
    case EXIT:
        aux[0] = TOP_L, aux[1] = TOP_R, aux[2] = BOT_L, aux[3] = BOT_R;
        screen->totalAreas = 1;
        screen->areas = malloc(sizeof(ScreenArea)* screen->totalAreas);
        screen->areas[0] = createArea(1,1, WIDTH-2, HEIGHT-2, aux);
        break;
    case GAME:
        screen->totalAreas = 4;
        screen->areas = malloc(sizeof(ScreenArea)* screen->totalAreas);

        aux[0] = TOP_L, aux[1] = T_CUT, aux[2] = BOT_L, aux[3] = B_CUT;
        screen->areas[0] = createArea(1, 1, 33, HEIGHT-4, aux);

        aux[0] = T_CUT, aux[1] = TOP_R, aux[2] = B_CUT, aux[3] = BOT_R;
        screen->areas[1] = createArea(35, 1, WIDTH-2, HEIGHT-4, aux);

        aux[0] = L_SID, aux[1] = CROSS, aux[2] = BOT_L, aux[3] = BOT_R;
        screen->areas[2] = createArea(1, HEIGHT-2, 33, HEIGHT-2, aux);

        aux[0] = CROSS, aux[1] = R_SID, aux[2] = B_CUT, aux[3] = BOT_R;
        screen->areas[3] = createArea(35, HEIGHT-2, WIDTH-2, HEIGHT-2, aux);
        break;
    case BATTLE:
        aux[0] = TOP_L, aux[1] = TOP_R, aux[2] = BOT_L, aux[3] = BOT_R;
        screen->totalAreas = 3;
        screen->areas = malloc(sizeof(ScreenArea)* screen->totalAreas);
        screen->areas[0] = createArea(1,1, WIDTH-2, 5, aux);
        screen->areas[1] = createArea(1,7, WIDTH-2, HEIGHT-8, aux);
        screen->areas[2] = createArea(1, HEIGHT-6, WIDTH-2, HEIGHT-2, aux);
        break;
    }
    setScreenMap(screen);

    if(scenes->first == NULL){
        scenes->first = scenes->last = screen;
    } else{
        Screen* aux = scenes->last;
        aux->next = screen;
        scenes->last = screen;
    }
    return screen;
};

void showScene(){
    Screen* screen = findScreen(game->scenes, game->Id);
    moveCursorTo(0,0);
    char scene[HEIGHT][WIDTH];
    for(int i = 0; i < HEIGHT; i++){
        for(int j = 0; j < WIDTH; j++){
            scene[i][j] = screen->ScreenMap[i][j];
        }
    }
    scene[29][120] = '\0';
    fputs(scene, stdout);
    if(screen->ID == MAIN_MENU){ // MENU INICIAL
        char opts[3][20]= {"JOGADOR X MAQUINA", "JOGADOR X JOGADOR", "SAIR"};
        for(int i = 0; i < 3; i++){
            int aux =18+(i*2);
            moveCursorTo(51, aux);
            if (game->activeOption == i) SetColor(ACTIVE);
            else SetColor(INACTIVE);
            printf("%s", opts[i]);
            SetColor(INACTIVE);
        }
        moveCursorTo(WIDTH-1, HEIGHT-1);
    }
    else if(screen->ID == DIFFICULTY){ // SELEÇÃO DE DIFICULDADE
        char opts[3][20]= {"FACIL", "MEDIO", "DIFICIL"};
        for(int i = 0; i < 3; i++){
            int aux =18+(i*2);
            moveCursorTo(56, aux);
            if (game->activeOption == i) SetColor(ACTIVE);
            else SetColor(INACTIVE);
            printf("%s", opts[i]);
            SetColor(INACTIVE);
        }
        moveCursorTo(WIDTH-1, HEIGHT-1);
    }

    else if(screen->ID == GAME) { // TELA DE JOGO
        for(int i = 0; i < screen->totalAreas; i++){
            int x, y;

            //Lado Esquerdo: Dados do Personagem
            if(i == 0) {
                x = screen->areas[i]->start.X+2;
                y = screen->areas[i]->start.Y+1;
                moveCursorTo(x, y);

                printf("Jogador:");
                y += 3;
                moveCursorTo(x, y);
                printf("HP: %3s/%-7s MP: %3s/", "", "", "");
                y += 2;
                moveCursorTo(x, y);
                printf("STR: %10s DEX:", "");
                y += 2;
                moveCursorTo(x, y);
                printf("WIT: %10s CHA:", "");

                y += 3;
                moveCursorTo(x, y);
                printf("DMG: %10s DEF:", "");

                y += 2;
                moveCursorTo(x, y);
                printf("%9s", "Arma:");
                y += 2;
                moveCursorTo(x, y);
                printf("%9s", "Armadura:");

                y += 2;
                moveCursorTo(x, y);
                printf("%7s Dinheiro:", "");

                y+=3;
                moveCursorTo(x, y);
                printf("%13s***", "");

                y+=2;
                moveCursorTo(x, y);

                printf("<^v> - ANDAR %2s Z - ACAO", "");
                y+=1;
                moveCursorTo(x, y);
                printf("X - ABRIR/FECHAR INVENTARIO");
                y+=1;
                moveCursorTo(x, y);
                printf("ESC - SAIR / DESISTIR");
                y+=1;
            }

            //Lado Direito: Mapa;
            if(i == 1) {
                x = screen->areas[i]->start.X;
                y = screen->areas[i]->start.Y;

                for(int i = 0; i < MAP_HEIGHT; i++){
                    moveCursorTo(x,y);
                    for(int j = 0; j < MAP_WIDTH; j++){
                        SetColor(game->maps[0]->Map[i][j].color);
                        printf("%c", game->maps[0]->Map[i][j].sprite);
                    }
                    y++;
                }
            }

            //Lado Esquerdo: Auxilio do Mapa
            if(i == 2) {

            }

            //Lado Direito: Area de mensagem e texto
            if(i == 3) {

            }
            SetColor(DEFAULT);
            update();
        }
    }
    else if (screen->ID == INTRO){ // INTRODUÇÃO E NOME DO PERSONAGEM
        int x = screen->areas[0]->start.X+3;
        int y = screen->areas[0]->start.Y+2;
        moveCursorTo(x,y);
        printf(" Uma pequena aventura, venca o duelo ao final do tempo.");
        y++;
        moveCursorTo(x,y);
        printf("[ %d TURNOS ]", DAYS);
        y+=2;
        moveCursorTo(x,y);
        printf(" Cada turno equivale a 1 dia: decida o que fazer para deixar seu personagem mais forte; ao final");
        y++;
        moveCursorTo(x,y);
        printf("de 10 dias, venca o duelo e ganhe.");
        y+=5;
        moveCursorTo(x,y);
        char name[20];

        if(game->Players == 0) printf(" Jogador, por favor insira seu nome: ");
        else printf(" Jogador 1 - Por favor insira seu nome: ");
        scanf("%s", name);
        game->Player1 = createPlayer(name, game->Difficulty);
        game->Player1->color = 1;

        if(game->Players == 1){
            y+=5;
            moveCursorTo(x,y);
            printf(" Jogador 2 - Por favor insira seu nome: ");
            scanf("%s",name);
            game->Player2 = createPlayer(name, game->Difficulty);
            game->Player2->color = 5;
        }
        y+=5;
        moveCursorTo(x,y);
        printf(" Prepare-se, a sua aventura ira comecar.");
        game->Id = GAME;
        moveCursorTo(WIDTH-1, HEIGHT-1);
    }

};

Scenes* createScene(){
    Scenes* scenes = malloc(sizeof(Scenes));
    for(int i = 0; i < SCENES; i++){
        prepareScreen(scenes, i);
    }
    return scenes;
}

Screen* findScreen(Scenes* screens, SCREEN id){
    Screen* aux = screens->first;
    while(aux != NULL){
        if(aux->ID == id){
            return aux;
        }
        aux = aux->next;
    }
    return NULL;
}

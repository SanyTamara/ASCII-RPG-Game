#include "main.h"

Tile tiles[] = {
    {178, YELLOW, true},  //GROUND
    {178, GREEN, true},   //GRASS
    {178, INACTIVE, true},//ROAD
    {126, BLUE, false},    //WATER
    {155, GRAY, false},    //STONE
    {42, GREEN_LG, false}, //WEED
    {237, GREEN, false},   //TREE
    {241, GRAY, false},    //GRAVE
    {219, GRAY, false},    //WALLS
    {49, INACTIVE, true}, //DOOR
    {58, INACTIVE, true}, //FLOOR
    {63, ROSE, false},      //SPECIAL
};
Npc npcs[];
Game* game;

void init(){
    game = malloc(sizeof(Game));
    game->scenes = createScene();
    game->Id = MAIN_MENU;
    game->Difficulty = 0;
    game->activeOption = 0;
    game->Players = 0;
    game->Turn1 = true;
    game->calendar.Day = game->calendar.Minute = game->calendar.Hour = 1;

    /*for(int y = 0; y < MAP_HEIGHT; y++){
        for(int x = 0; x < MAP_WIDTH; x++){
            if(y == 0 || y == MAP_HEIGHT-1){
                game->Map[y][x] = WALLS;
            } else {
                if(x == 0 || x == MAP_WIDTH-1) game->Map[y][x] = WALLS;
                else game->Map[y][x] = FLOOR;
            }
        }
    } */

    game->maps = createMaps();


    mainMenu(game);
    startGame(game);
}

void mainMenu(Game* game){
    showScene(game);

    while(game->Id <= DIFFICULTY){
        switch(keyInput()){
        case UP:
        case LEFT:
            if(game->activeOption == 0) game->activeOption = 2;
            else game->activeOption--;
            break;
        case DOWN:
        case RIGHT:
            if(game->activeOption == 2) game->activeOption = 0;
            else game->activeOption++;
            break;
        case ACTION:
            if(game->Id != MAIN_MENU) {
                game->Difficulty = game->activeOption;
                game->Id = INTRO;
                game->activeOption = 0;
                break;
            } else {
                if(game->activeOption < 2) {
                    game->Players = game->activeOption;
                    game->Id = DIFFICULTY;
                    game->activeOption = 0;
                    break;
                }
            }
        case CANCEL:
            if(game->Id == DIFFICULTY) {
                game->Id = MAIN_MENU;
                game->activeOption = 0;
                break;
            }
        case ESC:
            system("cls");
            exit(1);
            break;
        default:
            break;
        }
        showScene(game);
        Sleep(100);
    }
}

Map** createMaps(){
    Map** maps = malloc(sizeof(Map) * MAPS_NUMBER);

    for (int i = 0; i < MAPS_NUMBER; i++){
        maps[i] = malloc(sizeof(Map));
    }


    mapEvent* event = malloc(sizeof(mapEvent));
    event->type = tiles[SPECIAL];
    strcpy(event->name, "Tem algo no chao");
    event->eventID.itemID = MANA_POTION;
    event->pos.X = 88;
    event->pos.Y = 21;
    event->taken = false;

    mapEvent* event2 = malloc(sizeof(mapEvent));
    event2->type = (Tile) {'N', RED, false};
    strcpy(event2->name, "Carlos");
    event2->eventID.npcID = CARLOS;
    event2->pos.X = 49;
    event2->pos.Y = 4;
    event2->taken = false;

    mapEvent* event3 = malloc(sizeof(mapEvent));
    event3->type = (Tile) {'m', GREEN_LG, false};
    strcpy(event3->name, "GOBLIN");
    event3->eventID.npcID = MARGO;
    event3->pos.X = 59;
    event3->pos.Y = 7;
    event3->taken = false;

    maps[0]->numberOfEvents = 3;

    maps[0]->ID = TEST;
    maps[0]->Events = malloc(sizeof(mapEvent)*32);
    maps[0]->Events[0] = malloc(sizeof(mapEvent));
    maps[0]->Events[0] = event;
    maps[0]->Events[1] = malloc(sizeof(mapEvent));
    maps[0]->Events[1] = event2;
    maps[0]->Events[2] = malloc(sizeof(mapEvent));
    maps[0]->Events[2] = event3;



    for(int i = 0; i < MAP_HEIGHT; i++){
        for(int j = 0; j < MAP_WIDTH; j++){
            if(i == 0 || i == MAP_HEIGHT-1){
                maps[0]->Map[i][j] = tiles[WALLS];
            } else {
                if(j == 0 || j == MAP_WIDTH-1) maps[0]->Map[i][j] = tiles[WALLS];
                else maps[0]->Map[i][j] = tiles[FLOOR];
            }
        }
    }
    return maps;
};

void startGame(Game* game){
    showScene();
    Character* player;

    while(game->Id != EXIT){
        if(game->Turn1 == true) player = game->Player1;
        else player = game->Player2;
        if(game->calendar.Day == 5) {
            system("cls");
            Sleep(500);
            endGame();
        };
        if(player->actionsLeft == 0 && game->Players == 1){
            if(game->Turn1 == true) {
                game->Turn1 = false;
            }
            else {
                game->calendar.Day++;
                game->Player1->actionsLeft = ACTIONS;
                game->Player2->actionsLeft = ACTIONS;
                game->Turn1 = true;
            }
            system("cls");
            Sleep(500);
            showScene();
        } else if(player->actionsLeft == 0 && game->Players == 0){
            game->calendar.Day++;
            player->actionsLeft = ACTIONS;
            system("cls");
            Sleep(500);
            showScene();
        }
        switch(keyInput()){
        case UP:
            movePlayer(game, player->pos.X, player->pos.Y-1);
            break;
        case DOWN:
            movePlayer(game, player->pos.X, player->pos.Y+1);
            break;
        case LEFT:
            movePlayer(game, player->pos.X-1, player->pos.Y);
            break;
        case RIGHT:
            movePlayer(game, player->pos.X+1, player->pos.Y);
            break;
        case ACTION:
            int result = checkPosition();
            if (result > 1 && result < 5) findEvent();
            break;
        case CANCEL:
            openInventory(player);
        default:
            break;
        };
        Sleep(100);
    }

};
void endGame() {
    int result = comparePlayers();
    system("cls");
    if(result == 0) printf("Deu empate.\n");
    if(result == 1) printf("Jogador 1 ganhou.\n");
    if(result == 2) printf("Jogador 2 ganhou.\n");
    if(result == 5) printf("O jogo ainda nao tem final, entao modo contra computador nao tem ganhador.\n");
    printf("Esse e o fim do teste de funcoes.");

}
int comparePlayers(){
    if(game->Players == 1) {
        int pointsP1 = 0, pointsP2 = 0;
        if(game->Player1->Money > game->Player2->Money) pointsP1++;
        else if (game->Player1->Money < game->Player2->Money) pointsP2++;

        if(game->Player1->inventory->Slots.current > game->Player2->inventory->Slots.current) pointsP1+=2;
        else if (game->Player1->inventory->Slots.current < game->Player2->inventory->Slots.current) pointsP2+=2;

        if (pointsP2 < pointsP1) return 1;
        else if (pointsP2 > pointsP1) return 2;
        else return 0;
    } else return 5;
};
void findEvent(){
    Character* p;
    Screen* screen = findScreen(game->scenes, game->Id);
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &consoleInfo);
    COORD pos = consoleInfo.dwCursorPosition;
    if(game->Turn1 == true) p = game->Player1;
    else p = game->Player2;
    MAPS id = p->mapID;
    Map* map = game->maps[id];
    mapEvent* event;

    for(int i = 0; i < map->numberOfEvents; i++){
        event = map->Events[i];
        if(event->pos.X == pos.X && event->pos.Y == pos.Y) {
            moveCursorTo(screen->areas[3]->start.X, screen->areas[3]->start.Y);
            switch(event->type.sprite){
            case 'N':
            case 'n':
                if(event->taken == true) {
                    printf("Nao gaste tudo de uma vez.");
                } else {
                    printf("Estou de bom humor, toma umas moedas.");
                    p->Money += 10;
                    event->taken = true;
                }
                break;
            case 63: // é um item
                insertItem(p->inventory, event->eventID.itemID);
                Item* i = findItem(p->inventory, event->eventID.itemID);
                printf(" Voce encontrou x1 [ %s ].", i->name);
                event->taken = true;
                break;
            case 'm':
            case 'M':
                Character* g = createPlayer("Goblin", 2);
                battle(p, g);
            }
            p->actionsLeft--;
        }
    }
    update();
};
void openInventory(Character* p){

};
void update(){
    Screen* screen = findScreen(game->scenes, game->Id);

    if(screen->ID == GAME) { // TELA DE JOGO
        for(int i = 0; i < screen->totalAreas; i++){
            int x, y;
            mapEvent* event;
            Character* p;
            if(game->Turn1 == false) p = game->Player2;
            else p = game->Player1;

            if(i == 0) { //Lado Esquerdo: Dados do Personagem
                x = screen->areas[i]->start.X+2;
                y = screen->areas[i]->start.Y+2;

                moveCursorTo(x+1, y);

                printf("%-20s", p->name);

                y+=2;
                moveCursorTo(x+4, y);
                printf("%3d", p->HP.current);
                SetColor(INACTIVE);
                printf("/");
                SetColor(DEFAULT);
                printf("%-3d", p->HP.max);
                moveCursorTo(x+20, y);
                if(p->MP.max == 0) {
                    SetColor(INACTIVE);
                    printf("%2s ", "-");
                    printf("/");
                    printf(" -");
                }
                else {
                    printf("%3d", p->MP.current);
                    SetColor(INACTIVE);
                    printf("/");
                    SetColor(DEFAULT);
                    printf("%-3d", p->MP.max);
                }

                y += 2;
                moveCursorTo(x+5, y);
                printf("%3d", p->Str.level);
                moveCursorTo(x+21, y);
                printf("%3d", p->Dex.level);

                y += 2;
                moveCursorTo(x+5, y);
                printf("%3d", p->Wit.level);
                moveCursorTo(x+21, y);
                printf("%3d", p->Cha.level);

                y += 3;
                moveCursorTo(x+5, y);
                printf("%3d (+%d)", p->Atk.current, p->Atk.max);
                moveCursorTo(x+21, y);
                printf("%3d (+%d)", p->Def.current, p->Def.max);

                for(int n = 0; n < 2; n++){
                    y += 2;
                    moveCursorTo(x+10, y);
                    if(p->equipment[0] == NULL) printf("< NONE >");
                    else printf("< %s >", p->equipment[n]->name);
                }

                y += 2;
                moveCursorTo(x+18, y);
                printf("%3d", p->Money);
                y ++;
                moveCursorTo(x, y);
                printf("Dia/Turno: [ %d ]", game->calendar.Day);
            }

            if(i == 1) { //Lado Direito: Mapa;
                x = screen->areas[1]->start.X;
                y = screen->areas[1]->start.Y;
                for(int n = 0; n < MAP_HEIGHT; n++){
                    for(int j = 0; j < MAP_WIDTH; j++){
                        if(y < p->pos.Y+2 && y > p->pos.Y-2){
                            if( x < p->pos.X+2 && x > p->pos.X-2){
                                moveCursorTo(x,y);
                                SetColor(game->maps[0]->Map[n][j].color);
                                printf("%c", game->maps[0]->Map[n][j].sprite);
                            }
                        }
                        x++;
                    }
                    y++;
                    x = screen->areas[1]->start.X;
                }

                for(int n = 0; n < game->maps[0]->numberOfEvents; n++){
                    event = game->maps[0]->Events[n];
                    if( event->type.sprite == 'N' || event->taken == false) {
                        moveCursorTo(event->pos.X, event->pos.Y);
                        SetColor(event->type.color);
                        printf("%c", event->type.sprite);
                    }
                }
            }

            if(i == 2) { //Lado Esquerdo: Auxilio do Mapa
                x = screen->areas[i]->start.X;
                y = screen->areas[i]->start.Y;

                if(checkPosition() == 2 || checkPosition() == 3) {
                    for(int j = 0; j < game->maps[0]->numberOfEvents; j++){
                        event = game->maps[0]->Events[i];
                        if(event->pos.X == x && event->pos.Y == y) {
                            moveCursorTo(x, y);
                            printf("%s", event->name);
                        }
                    }
                } else {
                    moveCursorTo(x, y);
                    printf("%28s", "");
                }
            }

            /*if(i == 3) { //Lado Direito: Area de mensagem e texto
                x = screen->areas[i]->start.X+1;
                y = screen->areas[i]->start.Y;
                moveCursorTo(x, y);
                printf("%80s", "");
            } */
            printPlayer(p);
            SetColor(DEFAULT);
        }
    }

};

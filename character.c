#include "main.h"

Character* createPlayer(char name[], int dif){
    Character* p = malloc(sizeof(Character));
    strcpy(p->name, name);
    if (dif < 2) p->Str.level = 5 - dif;
    else p->Str.level = 3;
    if (dif == 0) p->Dex.level = 3;
    else p->Dex.level = 2;

    p->Wit.level = p->Cha.level = 2;
    p->Str.EXP.current = p->Dex.EXP.current = p->Wit.EXP.current = p->Cha.EXP.current = 0;
    p->Str.EXP.max = p->Dex.EXP.max = p->Wit.EXP.max = p->Cha.EXP.max = 100;

    p->HP.current = p->HP.max = (p->Str.level + p->Dex.level)* 5;
    p->MP.current = p->MP.max = 0;
    p->pos.X = 60;
    p->pos.Y = 20;
    p->Atk.current = p->Str.level/2;
    p->Def.current = 0;
    p->Atk.max = p->Def.max = 0;
    p->inventory = createInventory();
    p->equipment = createEquipSlots();
    p->actionsLeft = ACTIONS;

    if(dif == 2) p->Money = 50;
    else p->Money = (200 / (dif+1));

    p->replay = NULL;
    p->replayCount = 0;

    return p;
}

/* Configurações da Movimentação do Personagem */
void printPlayer(Character* p){
    moveCursorTo(p->pos.X, p->pos.Y);
    SetColor(1);
    printf("@");
    SetColor(DEFAULT);
    moveCursorTo(p->pos.X, p->pos.Y);
};
void movePlayer(Game* g, int x, int y){
    Character* p;
    Screen* screen = findScreen(game->scenes, game->Id);

    if(g->Turn1 == true) p = g->Player1;
    else p = g->Player2;

    moveCursorTo(x, y);
    int tileType = checkPosition();
    if(tileType == 1){
        p->pos.X = x;
        p->pos.Y = y;
        moveCursorTo(screen->areas[3]->start.X, screen->areas[3]->start.Y);
        printf("%80s", "");
        p->actionsLeft--;
        update();
    }
    else {
        if(tileType == 2 || tileType == 3) {
            for(int j = 0; j < game->maps[0]->numberOfEvents; j++){
                mapEvent* event = game->maps[0]->Events[j];
                Screen* screen = findScreen(game->scenes, game->Id);
                if(event->pos.X == x && event->pos.Y == y) {
                    moveCursorTo(screen->areas[2]->start.X, screen->areas[2]->start.Y);
                    printf("%s", event->name);
                    moveCursorTo(x, y);
                }
            }
        } else {
            moveCursorTo(screen->areas[2]->start.X, screen->areas[2]->start.Y);
            printf("%30s", "");
            moveCursorTo(x, y);
        }
    };

}
int checkPosition(){
    CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
    HANDLE hConsole= GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(hConsole, &csbiInfo);
    COORD pos = csbiInfo.dwCursorPosition; //set pos to current cursor location
    TCHAR strFromConsole[1];    //need space to only one char
    DWORD dwChars;
    ReadConsoleOutputCharacter( hConsole, strFromConsole, 1, pos, &dwChars);
    char c = strFromConsole[0];

    switch(c){
    case ':':
    case 178:
    case '/':
        return 1;
        break;
    case 63:
        return 2;
        break;
    case 'N':
    case 'n':
        return 3;
        break;
    case 'm':
    case 'M':
        return 4;
        break;
    default:
        return 0;
        break;
    }
};

/*  Configurações de Inventário e Itens */
Inventory* createInventory(){ //Cria um inventário para o jogador.
    Inventory* inv = malloc(sizeof(Inventory));
    inv->First = inv->Last = NULL;
    inv->Slots.current = 0;
    inv->Slots.max = 10;
    return inv;
};
Item** createEquipSlots(){
    int n = 2;
    Item** slots = malloc(sizeof(Item)*n);

    for(int i = 0; i < n; i++) {
        slots[i] = malloc(sizeof(Item));
        slots[i] = NULL;
    }
    return slots;
};

Item* findItem(Inventory* inv, int id){ //Usa ID para encontrar o item no inventário:
/*  - para REMOÇÃO:     em caso de uso do item, ou jogar fora;
    - para ADICIONAR:   Verifica se o item já existe no inventário para aumentar a quantidade ou criar um novo slot para o item */
    Item* item = inv->First;

    while(item != NULL){
        if( item->ID == id && item->Stack.current < item->Stack.max) return item;
        item = item->next;
    }
    return NULL;
};

void insertItem(Inventory* inv, int id){ // Utiliza o ID para adicionar item no inventário passado por ponteiro:
    //Procura o item no inventário, retornando o ponteiro para ele se existir, ou NULL caso não exista.
    Item* item = findItem(inv, id);
    if(item == NULL) {
        item = createItem(id);
        if(inv->First == NULL){
            inv->First = item;
        } else {
            item->prev = inv->Last;
        }
        inv->Last = item;
        inv->Slots.current++;
    }
    else {
        item->Stack.current++;
    }
};
Item* createItem(int id){ // Cria o teim a partir do ID, e retorna o ponteiro
    Item* item = malloc(sizeof(Item));
    switch(id){
    // Itens de recuperação
    case HEALTH_POTION:
        strcpy(item->name, "Pocao de Cura Fraca");
        strcpy(item->desc, "Uma Pocao de Cura que recupera 15HP.");
        item->ID = id;
        item->Stack.current = 1;
        item->Stack.max = 5;
        item->points = 15;
        item->value = 25;
        break;
    case MANA_POTION:
        strcpy(item->name, "Pocao de Mana Fraca");
        strcpy(item->desc, "Pocao de Mana que recupera 10MP.");
        item->ID = id;
        item->Stack.current = 1;
        item->Stack.max = 5;
        item->points = 10;
        item->value = 45;
        break;
    // Armas
    case BRANCH:
        strcpy(item->name, "Galho");
        strcpy(item->desc, "Um galho resistente. Pode ser usado como arma em caso de necessidade.");
        item->ID = id;
        item->Stack.current = 1;
        item->Stack.max = 2;
        item->points = 1;
        item->value = 1;
        break;
    case STAFF:
        strcpy(item->name, "Cajado");
        strcpy(item->desc, "Um cajado de madeira, usado para focar mana. Ou bater em alguem, voce quem sabe.");
        item->ID = id;
        item->Stack.current = 1;
        item->Stack.max = 1;
        item->points = 2;
        item->value = 15;
        break;
    case RUNIC_STAFF:
        strcpy(item->name, "Cajado Runico");
        strcpy(item->desc, "Cajado com Runas que melhora o uso de magias.");
        item->ID = id;
        item->Stack.current = 1;
        item->Stack.max = 1;
        item->points = 4;
        item->value = 85;
        break;
    case OLD_SWORD:
        strcpy(item->name, "Espada Enferrujada");
        strcpy(item->desc, "Uma espada velha e enferrujada, quase sem corte.");
        item->ID = id;
        item->Stack.current = 1;
        item->Stack.max = 1;
        item->points = 3;
        item->value = 20;
        break;
    case SWORD:
        strcpy(item->name, "Espada Comum");
        strcpy(item->desc, "Uma espada comum de aco, bem afiada.");
        item->ID = id;
        item->Stack.current = 1;
        item->Stack.max = 1;
        item->points = 6;
        item->value = 75;
        break;
    // Armaduras
    case JACQUE:
        strcpy(item->name, "Jaquetao");
        strcpy(item->desc, "Jaqueta de tecido acolchoada.");
        item->ID = id;
        item->Stack.current = 1;
        item->Stack.max = 1;
        item->points = 1;
        item->value = 35;
        break;
    case CHAIN_MAIL:
        strcpy(item->name, "Cota de Malha");
        strcpy(item->desc, "Uma camisa feita de aneis de metal interconectados entre si, protege o torso.");
        item->ID = id;
        item->Stack.current = 1;
        item->Stack.max = 1;
        item->points = 3;
        item->value = 65;
        break;
    case BRIGANDINE:
        strcpy(item->name, "Brigantina");
        strcpy(item->desc, "Couraca de tecido ou couro com placas metalicas parcialmente sobrepostas rebitadas em seu interior.");
        item->ID = id;
        item->Stack.current = 1;
        item->Stack.max = 1;
        item->points = 4;
        item->value = 95;
        break;
    // NATUREZA
    case VITALEAF:
        strcpy(item->name, "Vitaleaf");
        strcpy(item->desc, "Uma planta de folhas alongadas e pontas arredondadas verde claro.");
        item->ID = id;
        item->Stack.current = 1;
        item->Stack.max = 10;
        item->points = 2;
        item->value = 5;
        break;
    case AZULA:
        strcpy(item->name, "Azula Cantala");
        strcpy(item->desc, "Uma planta de longas folhas lanceoladas em tom azulado.");
        item->ID = id;
        item->Stack.current = 1;
        item->Stack.max = 10;
        item->points = 1;
        item->value = 40;
        break;
    case DANCINSHROOM:
        strcpy(item->name, "Cogumelo Dancante Violeta");
        strcpy(item->desc, "Com uma haste longa e fina, e pequeno chapeu convexo violeta, danca com o vento.");
        item->ID = id;
        item->Stack.current = 1;
        item->Stack.max = 25;
        item->points = 1;
        item->value = 35;
        break;
    case FLOWERINSHROOM:
        strcpy(item->name, "Cogumelo Florado Faerico");
        strcpy(item->desc, "Estranho aglomerado de cogumelos vermelhos que lembram uma flor.");
        item->ID = id;
        item->Stack.current = 1;
        item->Stack.max = 10;
        item->points = 1;
        item->value = 55;
        break;
    case MANA_SHARD:
        strcpy(item->name, "Fragmento de Mana");
        strcpy(item->desc, "Um fragmento de mana cristalizada.");
        item->ID = id;
        item->Stack.current = 1;
        item->Stack.max = 10;
        item->points = 5;
        item->value = 55;
        break;
    }

    //Item novo criando, então coloco os ponteiros como NULL, eles serão modificados se necessario antes de colocar no inventario
    item->prev = NULL;
    item->next = NULL;
    return item; //Retornando o ponteiro para adicionar no inventario.
}

void useItem(Item* item, Character* target){ //Recebe o item e do ALVO(target) que receberá o efeito.
    Inventory* inv = target->inventory;
    int itemID = item->ID; // ID no enum usado para achar os itens
    int slot;
    switch(itemID){
    case VITALEAF:
    case HEALTH_POTION:
        if(target->HP.current <= (target->HP.max - item->points) ) target->HP.current += item->points;
        else target->HP.current = target->HP.max;
        break;
    case MANA_SHARD:
    case MANA_POTION:
        if(target->MP.current <= (target->MP.max - item->points) ) target->MP.current += item->points;
        else target->MP.current = target->MP.max;
        break;
    case AZULA:
        if (target->HP.current > 4) target->HP.current -= 4;
        else target->HP.current = 1;
        target->HP.max++;
        break;
    case DANCINSHROOM:
        target->MP.max++;
        break;
    case FLOWERINSHROOM:
        target->Str.level++;
        break;
    case BRANCH:
    case STAFF:
    case RUNIC_STAFF:
    case OLD_SWORD:
    case SWORD:
        slot = 0;
        break;
    case JACQUE:
    case CHAIN_MAIL:
    case BRIGANDINE:
        slot = 1;
        break;
    }

    if(itemID >= BRANCH) { //Se o item em questão for um equipamento
        if(inv->First != inv->Last) { //Tem mais de um item no inventario
            if(item == inv->First) inv->First = inv->First->next;
            if(item == inv->Last) inv->Last = inv->Last->prev;
        }
        else { //Só tem um item
            if(target->equipment[slot] == NULL) inv->First = inv->Last = NULL;
        }
        if(target->equipment[slot] != NULL) unequip(target, slot);
        equip(target->equipment[slot], item);
        if(slot == 0) target->Atk.max += target->equipment[slot]->points;
        if(slot == 1) target->Def.max += target->equipment[slot]->points;
        inv->Slots.current--;
    }
    else { // O item em questão é consumivel
        if(item->Stack.current > 1) item->Stack.current--;
        else removeItem(item);
    }
};
void equip(Item* Slot, Item* item){
    Item* Prev = item->prev;
    Item* Next = item->next;
    if(Prev != NULL) Prev->next = Next;
    if(Next != NULL) Next->prev = Prev;
    item->prev = item->next = NULL;
    Slot = item;
};
void unequip(Character* target, int slotId){

    if(target->inventory->First == NULL) target->inventory->First = target->equipment[slotId];
    else target->inventory->Last->next = target->equipment[slotId];

    target->inventory->Last = target->equipment[slotId];
    target->inventory->Slots.current++;

    if(slotId == 0) target->Atk.max -= target->equipment[slotId]->points;
    if(slotId == 1) target->Def.max -= target->equipment[slotId]->points;

};
void removeItem(Item* item){
    Item* aux = item->prev;
    aux->next = item->next;
    aux = item->next;
    aux->prev = item->prev;
    free(item);
};

/* BATTLE */
void battle(Character* F1, Character* F2){
    system("cls");
    int turn = 0;
    int atk = 0;
    int dmg = 0;
    battleRecord* battle = malloc(sizeof(battleRecord));
    battle->f1_hp = F1->HP;
    battle->f2_mp = F1->MP;
    battle->f1_hp = F1->HP;
    battle->f2_mp = F1->MP;
    battle->Count = 0;
    while(turn < 2){
        char String[100] = "";
        if(turn == 0) {
            atk = F1->Atk.current + F1->Atk.max;
            dmg = (atk * atk) / (atk + F2->Def.current);

            F2->HP.current -= dmg;
            sprintf(String, "%s ataca, causando %d de dano em %s. ( %d/%d )", F1->name, dmg, F2->name, F2->HP.current, F2->HP.max);

            turn = 1;
        } else {
            atk = F2->Atk.current + F2->Atk.max;
            dmg = (atk * atk) / (atk + F1->Def.current);

            F1->HP.current -= dmg;
            sprintf(String, "%s ataca, causando %d de dano em %s. ( %d/%d )", F2->name, dmg, F1->name, F1->HP.current, F1->HP.max);

            turn = 0;
        }

        printf("%s\n\n", String);
        Sleep(350);

        if(F1->HP.current < 1 || F2->HP.current < 1 ){
            if(F1->HP.current < 1) printf("%s foi derrotado.", F1->name);
            if(F2->HP.current < 2) printf("%s foi derrotado.", F2->name);
            turn = 2;
        }
        if(battle->Count == 9) {
            for(int i = 0; i < 10; i++){
                if(i < 9 ) {
                    battle->rounds[i] = battle->rounds[i+1];
                } else {
                    battle->Count--;
                }
            }
        }
        strcpy(battle->rounds[battle->Count].String, String);
        battle->rounds[battle->Count].dmg = dmg;
        battle->rounds[battle->Count].target = turn;
        battle->Count++;
    }

    if(game->Player1->replayCount == 10){
        battleRecord* aux = game->Player1->replay;
        game->Player1->replay = game->Player1->replay->next;
        free(aux);
    } else {
        if(game->Player1->replay == NULL) game->Player1->replay = battle;
        else game->Player1->replay->next = battle;
        game->Player1->replayCount++;
    }

    if(game->Players == 1 && F2 == game->Player2){
        if(game->Player2->replayCount == 10){
            battleRecord* aux = game->Player2->replay;
            game->Player2->replay = game->Player2->replay->next;
            free(aux);
        } else {
            if(game->Player2->replay == NULL) game->Player2->replay = battle;
            else game->Player2->replay->next = battle;
            game->Player2->replayCount++;
        }
    }
    getch();
    showScene(game->Id);
};

/* Configurações de Habilidades e Magias */
Skills* createSkillHolder(){
    Skills* skills = malloc(sizeof(Skills));
    skills->first = skills->last = NULL;
    skills->skillCount = 0;
    return skills;
}
void learnSkill(Skills* holder, SKILL id){
    Skill* skill = createSkill(id);

    if(holder->first == NULL) holder->first = skill;
    else holder->last->next = skill;
    holder->last = skill;
};
Skill* createSkill(SKILL id){
    Skill* skill = malloc(sizeof(Skill));

    skill->level.level = skill->level.EXP.current = 0;
    skill->level.EXP.max = 100;
    skill->next = NULL;
    skill->ID = id;

    switch(id){
    case MAGIC:
        strcpy(skill->name, "Magia" );
        strcpy(skill->desc, "Habilidade de usar magias." );
        skill->modifier = 0.1;
        break;
    case MANA_BOLT:
        strcpy(skill->name, "Flecha de Mana" );
        strcpy(skill->desc, "Uma flecha de mana disparada contra inimigos." );
        skill->modifier = 1.1;
        break;
    case MANA_BULLETS:
        strcpy(skill->name, "Tiros de Mana" );
        strcpy(skill->desc, "Tiros de mana lançados contra o inimigo." );
        skill->modifier = 0.45;
        break;
    case HEALING:
        strcpy(skill->name, "Cura" );
        strcpy(skill->desc, "Utiliza mana para ajudar o corpo a se recuperar de ferimentos." );
        skill->modifier = 5;
        break;
    case SWORDMANSHIP:
        strcpy(skill->name, "Habilidade com Espadas" );
        strcpy(skill->desc, "Habilidade de usar espadas em batalha." );
        skill->modifier = 0.1;
        break;
    case CROSS_SLASH:
        strcpy(skill->name, "Corte em X" );
        strcpy(skill->desc, "Ataca com dois cortes diagonais, quem formam um X." );
        skill->modifier = 0.55;
        break;
    case WIND_SLASH:
        strcpy(skill->name, "Lamina de Vento" );
        strcpy(skill->desc, "Um corte poderoso que gera uma lamina de vento." );
        skill->modifier = 1.25;
        break;
    case QUICK_THRUSTS:
        strcpy(skill->name, "Corte em X" );
        strcpy(skill->desc, "Ataca com dois cortes diagonais, quem formam um X." );
        skill->modifier = 0.35;
        break;
    }

    return skill;
}

Skill* findSkill(Skills* holder, SKILL id){
    Skill* skill = holder->first;

    while(skill != NULL) {
        if(skill->ID == id) return skill;
        skill = skill->next;
    }

    return NULL;
};
void useSkill(){
    //if(id == QUICK_THRUSTS) calculation = ( (user->Atk * user->Atk)/(user->Atk + target->Def) ) * skill->modifier;
};


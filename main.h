#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <conio.h>
#include <string.h>
#include <windows.h>
#define HEIGHT 30
#define WIDTH 120
#define MAP_WIDTH 84
#define MAP_HEIGHT 26
#define SCENES 6
#define MAPS_NUMBER 1
#define DAYS 5
#define ACTIONS 30

//EXTERN - GLOBAL VARIABLES
extern struct Game* game;
//ENUMS
    /* UTIL */
typedef enum KEY {
    NONE, UP, DOWN, LEFT, RIGHT, ACTION, CANCEL, ESC
} KEY;
typedef enum SPRITES{
    GROUND, GRASS, ROAD,
    WATER, STONE, WEED, TREE,
    GRAVE, WALLS, DOOR, FLOOR,
    SPECIAL,
    TOP_L = 201, // ╔
    TOP_R = 187, // ╗
    BOT_L = 200, // ╚
    BOT_R = 188, // ╝
    HORIZ = 205, // ═
    VERTC = 186, // ║
    L_SID = 204, // ╠
    R_SID = 185, // ╣
    T_CUT = 203, // ╦
    B_CUT = 202, // ╩
    CROSS = 206, // ╬
    EMPTY =  32 // ESPAÇO EM BRANCO
} SPRITES;
typedef enum COLOR {
    ACTIVE = 11, INACTIVE = 7, INVISIBLE = 0,
    GREEN = 2, BLUE = 3, RED = 4, GRAY = 8, BLUE_DK = 9, GREEN_LG = 10,
    ROSE = 12, PURPLE = 13, YELLOW = 14, DEFAULT = 15
} COLOR;
    /* SCREEN */
typedef enum SCREEN{
    MAIN_MENU, DIFFICULTY, GAME, BATTLE, INTRO, EXIT
} SCREEN;
typedef enum AREA {
    SURROUND = 0, // contorna a area
    TOP = 123, MID_HRZ = 456, BOTTOM = 789, // EM COLUNAS
    LEFT_SIDE = 147, MID_VRT = 258 ,RIGHT_SIDE = 369, // EM LINHAS
    LS_TOP = 1, LS_BOT = 7, LS_MID = 4, // tipos de area lado esquerdo
    MD_TOP = 2, MIDDLE = 5, MD_BOT = 8, // tipo de areas no centro
    RS_TOP = 3, RS_MID = 6, RS_BOT = 9, // tipos de area lado direito
} AREA;/*  Referencia: Numeros usados tamanho que a area cobre
            [ 1 ] [ 2 ] [ 3 ] Exempl: 123 = cobre toda a area de cima
            [ 4 ] [ 5 ] [ 6 ]
            [ 7 ] [ 8 ] [ 9 ]   0 = CONTORNAR; */
    /* CHARACTER */
typedef enum STAT {
    STR, DEX, WITS, CHA
} STAT;
typedef enum ITEM {
    VITALEAF/*HP+2*/, HEALTH_POTION/*HP+15*/, MANA_SHARD/*MP+5*/, MANA_POTION/*MP+10*/,
    AZULA/*HP++*/, DANCINSHROOM/*MP++*/, FLOWERINSHROOM/*STR++*/,
    BRANCH, STAFF, RUNIC_STAFF, OLD_SWORD, SWORD,
    JACQUE, CHAIN_MAIL, BRIGANDINE
} ITEM;
typedef enum SKILL {
    MAGIC, HEALING, MANA_BOLT, MANA_BULLETS,
    SWORDMANSHIP, CROSS_SLASH, WIND_SLASH, QUICK_THRUSTS
} SKILL;

typedef enum NPC {
    CARLOS, DANNA, MARGO
} NPC;

typedef enum MAPS {
    TEST
} MAPS;
// STRUCTS
    /* UTIL */
typedef struct Time {
    int Day;
    int Hour, Minute;
} Time;
typedef struct currentMax {
    int current;
    int max;
} currentMax;
typedef struct Level {
    int level;
    currentMax EXP;
} Level;
    /* ITEMS */
typedef struct Item {
    char name[30];
    char desc[100];
    int ID;
    int points;
    int value;
    currentMax Stack;

    struct Item* prev;
    struct Item* next;
} Item;
typedef struct Inventory{
    Item* First;
    Item* Last;
    currentMax Slots;
} Inventory;
    /* SKILLS */
typedef struct Skill {
    char name[30];
    char desc[100];
    SKILL ID;
    Level level;
    float modifier;

    struct Skill* next;
} Skill;
typedef struct Skills {
    Skill* first;
    Skill* last;
    int skillCount;
} Skills;
    /* BATTLE */
typedef struct attackRecord{
    char String[100];
    int dmg;
    int target;
} atkRecord;
typedef struct battleRecord{
    currentMax f1_hp;
    currentMax f1_mp;
    currentMax f2_hp;
    currentMax f2_mp;
    atkRecord rounds[10];
    int Count;
    struct battleRecord* next;
} battleRecord;

    /* CHARACTER */
typedef struct Npc {
    char name[10];
    COORD pos;
    MAPS mapID;
    currentMax friendship;
    char msg[80];
    int money;
    //Item inventory[5];
} Npc;
typedef struct Enemy {
    char name[30];
    int symbol;
    COLOR color;
    COORD pos;
    MAPS mapID;

    currentMax HP, MP;
    currentMax Atk, Def;

    int money;
    Item loot[3];
} Enemy;
typedef struct Character{
    char name[30];
    COLOR color;
    COORD pos;
    MAPS mapID;
    currentMax HP, MP;
    Level Str, Dex, Wit, Cha;
    currentMax Atk, Def;

    Skills* learnedSkills;
    Item** equipment;
    Inventory* inventory;

    int Money;
    int actionsLeft;
    battleRecord* replay;
    int replayCount;
} Character;
    /* SCREEN */
typedef struct area{
    COORD start, finish;
    int connection[4];
    //AREA cover;
} ScreenArea;
typedef struct Screen{
    SCREEN ID, totalAreas;
    ScreenArea ** areas;
    char ScreenMap[HEIGHT][WIDTH];
    Character* who;
    struct Screen* next;
} Screen;
typedef struct Scenes{
    Screen* first;
    Screen* last;
} Scenes;

    /* GAME */
typedef struct Tile{
    int sprite;
    int color;
    bool walkable;
} Tile;
typedef struct mapEvent{
    char name[30];
    Tile type;
    union {
        ITEM itemID;
        NPC npcID;
    } eventID;
    bool taken;
    COORD pos;
} mapEvent;
typedef struct Map {
    int ID;
    Tile Map[MAP_HEIGHT][MAP_WIDTH];
    mapEvent** Events;
    int numberOfEvents;
} Map;

typedef struct Game {
    Scenes* scenes;
    SCREEN Id;

    int Difficulty;
    int activeOption;
    int Players;
    Map** maps;
    Time calendar;

    Character* Player1;
    Character* Player2;
    bool Turn1;
} Game;

// FUNCOES
    /* UTIL */
KEY keyInput();
void moveCursorTo(int x, int y);
void SetColor(int color);
void init();
    /* SCREEN*/
ScreenArea* createArea(int startX, int startY, int finishX, int finishY, int conec[]);
void setScreenMap(Screen* screen);
Screen* prepareScreen(Scenes* screens, SCREEN id);
void showScene();
Scenes* createScene();
Screen* findScreen(Scenes* screens, SCREEN id);
    /* GAME */
Character* createPlayer(char name[], int num);
void printPlayer(Character* p);
void movePlayer(Game* g, int x, int y);
int checkPosition();
void mainMenu(Game* game);
void openInventory(Character* p);
void findEvent();
void update();
int comparePlayers();
void endGame();
void startGame(Game* game);
Map** createMaps();

    /* BATTLE */
void battle(Character* F1, Character* F2);

    /* INVENTORY */
Inventory* createInventory();
Item** createEquipSlots();
Item* findItem(Inventory* inv, int id);
void insertItem(Inventory* inv, int id);
Item* createItem(int id);
void useItem(Item* item, Character* target);
void equip(Item* Slot, Item* item);
void unequip(Character* target, int slotId);
void removeItem(Item* item);

    /* SKILLS*/
Skills* createSkillHolder();
void learnSkill(Skills* holder, SKILL id);
Skill* createSkill(SKILL id);
Skill* findSkill(Skills* holder, SKILL id);


#endif // MAIN_H_INCLUDED

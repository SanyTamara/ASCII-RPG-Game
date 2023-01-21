#include "main.h"

KEY keyInput(){
    char key = getch();
    switch(key) {
    case 'w': // UP / PRA CIMA
    case 'W':
    case 'H':
        return UP;
    case 's': // DOWN / PRA BAIXO
    case 'S':
    case 'P':
        return DOWN;
    case 'a': // LEFT / ESQUERDA
    case 'A':
    case 'K':
        return LEFT;
    case 'd': // RIGHT / ESQUERDA
    case 'D':
    case 'M':
        return RIGHT;
    case 'z': // AÇÃO
    case 'Z':
    case 13:
        return ACTION;
    case 'x':
    case 'X': // CANCEL
        return CANCEL;
    case 27:
        return ESC;
    default: // DEFAULT
        return NONE;
    };
    Sleep(100);
};

void moveCursorTo(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
void SetColor(int color) {
    WORD charColor;
    HANDLE OutputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    if(GetConsoleScreenBufferInfo(OutputHandle, &csbi)) {
        //Mask out all but the background attribute, and add in the foreground color
        charColor = (csbi.wAttributes & 0xF0) + (color & 0x0F);
        SetConsoleTextAttribute(OutputHandle, charColor);
    };
};

/*void init(Scenes* screens){
    screens->first = screens->last = NULL;
    for(int i = 0; i < 4; i++){
        createScreen(screens, i);
    }
    //PLAYING = true;
    //SELECTED = 1;
    //ScreenID = MAIN_MENU;
    //printScreen(findScreen(screens, ScreenID));
};
*/

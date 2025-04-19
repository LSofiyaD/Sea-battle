#include <glut.h> 
#include <stdio.h>
#include <string.h> 
#include <stdlib.h> 
#include<time.h>
#include <windows.h>
typedef struct {
    int color;
    int right;
    int left;
    int down;
    int up;
    char* name;
}ButtonS;

typedef struct {
    int role;  //role 0-пусто, 1-корабль, 11-точки вокруг корабля, 2-ранен(кретик), 3-мимо(точка)
    int left;
    int right;  //благодаря этой структуре мы инициальзируем каждую клеточку 
    int up;
    int down;
}tables;

typedef struct {
    int color;
    int len;  //длинна корабля
    int ystart;
    int xstart; //начало корабля -левый верхний угол
    int ynow;
    int xnow;  //нынешнее положение корабля
    int orientation; //горизонтальная/вкртикальная
    //int health; //здоров/ранен
}ship;

enum orient {
    hor,
    vert
};

tables player_table[15][15] = { 0 };
tables PC_table[15][15] = { 0 };
GLint Width = 1500, Height = 800;

#define MENU 11 
#define NEW_GAME 22
#define CONTINUE_GAME 33 
#define ABOUT_THE_GAME 44
#define ABOUT_THE_PROJECT 55
#define win 66 
int MODE = 0;
int Y = 0;
int X = 0;
int X1 = 11 + 1;
int Y1 = 0 + 1;
int CHANGE = 0;
int COUNTER = 0;
int SIDE = 0;
int SIDE1 = 0;
int MODE1 = 0;
int window = MENU;

ButtonS new_game = { 1, 940,560,480,580, "NEW GAME" };
//ButtonS continue_game = { 1,1080,800,480,580, "CONTINUE GAME" };
ButtonS about_the_game = { 1,940,560,355,455, "INFO ABOUT THE GAME" };
ButtonS about_the_project = { 1,940,560,230,330,"INFO ABOUT THE PROJECT" };
ButtonS go_out = { 1,890,610,100,170,"GO OUT" };
//ButtonS save = { 1,300,20,20,90, "SAVE THE GAME" }; 
ButtonS go_out_play = { 1,1450, 1170, 20,90,"GO OUT" };
ButtonS play = { 1,300,20,20,90,"PLAY" };

ship single1 = { 0,1,14,0,720,90,0 };
ship single2 = { 0,1,13,10,682,470,0 };
ship single3 = { 0,1,8,6,492,318,0 };
ship single4 = { 0,1,1,6,226,318,0 };
ship double1 = { 0,2,8,2,492,166,1 };    //корабли на левом поле(могу переписать координаты под правое поле)
ship double2 = { 0,2,4,5,340,280,1 };
ship double3 = { 0,2,0,0,188,90,0 };
ship triple1 = { 0,3,5,8,378,394,0 };
ship triple2 = { 0,3,1,11,226,508,0 };
ship quadruple = { 0,4,11,5,606,280,0 };
ship thefifth = { 0,5,12,13,644,584,1 };



void button(ButtonS button) {
    if (button.color == 1) {
        glColor3ub(255, 0, 127); // малиновый
    }
    glBegin(GL_QUADS);
    glVertex2f(button.left, button.up);
    glVertex2f(button.right, button.up); //рисуем кнопку
    glVertex2f(button.right, button.down);
    glVertex2f(button.left, button.down);
    glEnd();

    glColor3f(0, 51, 102); // цвет
    glLineWidth(5);
    glBegin(GL_LINE_LOOP);
    glVertex2f(button.left, button.up);
    glVertex2f(button.right, button.up);
    glVertex2f(button.right, button.down);  //рисуем обводку кнопки
    glVertex2f(button.left, button.down);
    glEnd();

    glColor3ub(255, 255, 255); // Белый цвет текста
    if (button.right - button.left == 280) {
        glRasterPos2f(button.left + 95 - strlen(button.name) * 4, button.down + 50 - 17); // Позиция текста 
    }
    else {
        glRasterPos2f(button.left + 120 - strlen(button.name) * 4, button.up - 50 - 8);
    }
    for (int i = 0; i < strlen(button.name); i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, button.name[i]); // Отрисовываем символ   
    }
}

void drawtext(int x, int y, char* txt) {
    glColor3ub(255, 0, 147);
    glRasterPos2f(x, y - 8);
    for (int i = 0; i < strlen(txt); i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, txt[i]); // Отрисовываем символ    
    }
}

void drawkross(int who, int i, int j) {

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();                             //эти 5 строку нужжны чтобы указывать размер кнопок через писксели
    int windowWidth = 1500;  // Ширина окна
    int windowHeight = 800; // Высота окна
    glOrtho(0, windowWidth, 0, windowHeight, -1, 1);

    if (who == 1) {

        glColor3ub(255, 0, 0);
        glLineWidth(5);
        glBegin(GL_LINE_LOOP);
        glVertex2f(PC_table[i][j].left, PC_table[i][j].up);
        glVertex2f(PC_table[i][j].right, PC_table[i][j].down);
        glEnd();

        glBegin(GL_LINE_LOOP);
        glVertex2f(PC_table[i][j].right, PC_table[i][j].up);
        glVertex2f(PC_table[i][j].left, PC_table[i][j].down);
        glEnd();
    }
    else {

        glColor3ub(255, 255, 255);
        glLineWidth(5);
        glBegin(GL_LINE_LOOP);
        glVertex2f(player_table[i][j].left, player_table[i][j].up);
        glVertex2f(player_table[i][j].right, player_table[i][j].down);
        glEnd();

        glBegin(GL_LINE_LOOP);
        glVertex2f(player_table[i][j].right, player_table[i][j].up);
        glVertex2f(player_table[i][j].left, player_table[i][j].down);
        glEnd();
    }
}



void drawpoint(int who, int i, int j) {

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();                             //эти 5 строку нужжны чтобы указывать размер кнопок через писксели
    int windowWidth = 1500;  // Ширина окна
    int windowHeight = 800; // Высота окна
    glOrtho(0, windowWidth, 0, windowHeight, -1, 1);
    if (who == 1) {
        glPointSize(10);
        glBegin(GL_POINTS);
        glColor3ub(255, 0, 127);
        glVertex2f((PC_table[i][j].left + 19), (PC_table[i][j].down + 19));
        glEnd();
    }
    else {
        glPointSize(10);
        glBegin(GL_POINTS);
        glColor3f(0, 51, 255);
        glVertex2f((player_table[i][j].left + 19), (player_table[i][j].down + 19));
        glEnd();
    }
}
int ill(int flag, int y, int x) {
    // Направления: вниз, влево, вверх, вправо
    const int dy[] = { 1, 0, -1, 0 };
    const int dx[] = { 0, -1, 0, 1 };

    // Правила перехода для SIDE после разных событий
    const int next_chain[] = { 2, 3, 5, 5 };
    const int next_success[] = { 1, 2, 3, 5 };
    const int next_blocked[] = { 1, 2, 3, 5 };

    while (SIDE != 5 && MODE != 1) {
        int dir = SIDE;
        if (dir < 0 || dir > 3) break;

        // Проверка границ для текущего направления
        int can_move = 1;
        switch (dir) {
        case 0: can_move = (y < 14); break;
        case 1: can_move = (x > 0);  break;
        case 2: can_move = (y > 0);  break;
        case 3: can_move = (x < 14); break;
        }

        if (!can_move) {
            SIDE = next_blocked[dir];
            continue;
        }

        int new_y = y + dy[dir];
        int new_x = x + dx[dir];
        int role = player_table[new_y][new_x].role;

        if (role == 1 || role == 2) {
            do {
                player_table[new_y][new_x].role = 2;
                drawkross(0, new_y, new_x);
                flag--;
                y = new_y;
                x = new_x;
                new_y += dy[dir];
                new_x += dx[dir];
            } while (new_y >= 0 && new_y <= 14 && new_x >= 0 && new_x <= 14 &&
                (player_table[new_y][new_x].role == 1 || player_table[new_y][new_x].role == 2));

            if (new_y >= 0 && new_y <= 14 && new_x >= 0 && new_x <= 14) {
                player_table[new_y][new_x].role = 3;
                drawpoint(0, new_y, new_x);
            }

            SIDE = next_chain[dir];
            Y = y;
            X = x;
            MODE = (dir == 2) ? MODE : 1;
        }
        else {
            player_table[new_y][new_x].role = 3;
            drawpoint(0, new_y, new_x);
            SIDE = next_success[dir];
            Y = y;
            X = x;
            MODE = 1;
        }
        break;
    }
    return flag;
}
int fightsecond(int Flag, int mode, int y, int x) {
    srand(time(NULL));
    if (MODE == 0) {    //если это не повторный удар 
        if (SIDE == 5) { SIDE = 0; Y = 0; X = 0; }
        if (SIDE > 0) {
            Flag = ill(Flag, Y, X, 0);
        }
        else {

            while (player_table[y][x].role == 3 || player_table[y][x].role == 2) {
                //srand(time(NULL)); 
                y = 0 + rand() % 15;
                x = 0 + rand() % 15;
                if (y < 0) {
                    y = y * (-1);
                }
                if (x < 0) {
                    x = x * (-1);
                }
            }
            if (player_table[y][x].role == 1) {
                player_table[y][x].role = 2;
                drawkross(0, y, x);
                Flag--;

                Flag = ill(Flag, y, x, 0);

                //Flag = fightsecond2(Flag, y, x,0);
            }
            else if (player_table[y][x].role == 0 || player_table[y][x].role == 11) {
                player_table[y][x].role = 3;
                drawpoint(0, y, x);
                MODE = 1;
            }
        }
    }
    return Flag;
}
int repeat(int flag) {
    int dy[] = { 1, 0, -1, 0 };  // Направления: вниз, влево, вверх, вправо
    int dx[] = { 0, -1, 0, 1 };
    int next_chain[] = { 2, 3, 5, 5 };
    int next_blocked[] = { 1, 2, 3, 5 };
    int next_success[] = { 1, 2, 3, 5 };

    int x = X1, y = Y1;

    while (SIDE1 != 5) {
        int dir = SIDE1;
        if (dir >= 4) {  // Некорректное направление
            SIDE1 = 5;
            break;
        }

        // Проверка возможности движения
        int can_move = 0;
        switch (dir) {
        case 0: can_move = (y < 14); break;
        case 1: can_move = (x > 0);  break;
        case 2: can_move = (y > 0);  break;
        case 3: can_move = (x < 14); break;
        }

        if (!can_move) {
            SIDE1 = next_blocked[dir];
            continue;
        }

        int new_y = y + dy[dir];
        int new_x = x + dx[dir];
        int role = PC_table[new_y][new_x].role;

        if (role == 1) {
            // Обработка цепочки единиц
            do {
                PC_table[new_y][new_x].role = 2;
                drawkross(1, new_y, new_x);
                flag--;
                y = new_y;
                x = new_x;
                new_y += dy[dir];
                new_x += dx[dir];
            } while (new_y >= 0 && new_y <= 14 && new_x >= 0 && new_x <= 14 &&
                PC_table[new_y][new_x].role == 1);

            // Установка точки после цепочки
            if (new_y >= 0 && new_y <= 14 && new_x >= 0 && new_x <= 14) {
                PC_table[new_y][new_x].role = 3;
                drawpoint(1, new_y, new_x);
            }

            SIDE1 = next_chain[dir];
        }
        else if (role == 3) {
            SIDE1 = next_blocked[dir];
        }
        else {
            PC_table[new_y][new_x].role = 3;
            drawpoint(1, new_y, new_x);
            SIDE1 = next_success[dir];
        }
        break;
    }

    return flag;
}
int fightfirst(int flag) {
    int f = 0;
    if (SIDE1 == 5) {
        COUNTER = 0;
    }
    if (COUNTER == 0) {
        while (X1 != 0 || Y1 != 15) {
            if (X1 == 14 && SIDE1 == 5) {

                if (Y1 == 3) {
                    X1 = 7 + 1;
                    Y1 = 0 + 1;
                }
                if (Y1 == 7) {
                    X1 = 3 + 1;
                    Y1 = 0 + 1;
                }
                if (Y1 == 11) {
                    X1 = 0 + 1;
                    Y1 = 1 + 1;
                }
                SIDE1 = 0;
            }
            if (Y1 == 14 && SIDE1 == 5) {

                if (X1 == 13) {
                    X1 = 0 + 1;
                    Y1 = 5 + 1;
                }
                if (X1 == 9) {
                    X1 = 0 + 1;
                    Y1 = 9 + 1;
                }
                if (X1 == 5) {
                    X1 = 0 + 1;
                    Y1 = 13 + 1;
                    f = 1;
                }
                if (X1 == 1 && f == 1) {
                    CHANGE = 1;
                }
                SIDE1 = 0;

            }
            if (Y1 == 13 && SIDE1 == 5) {
                Y1 = Y1 + 2;
                X1 = X1 + 2;
                /*Y1++;
                X1++;*/
                SIDE1 = 0;
            }

            while (X1 <= 14) {
                if (PC_table[Y1][X1].role == 1 || PC_table[Y1][X1].role == 2) {
                    PC_table[Y1][X1].role = 2;
                    drawkross(1, Y1, X1);
                    flag--;
                    COUNTER = 1;            //it means WARKING
                    flag = repeat(flag);
                    break;
                }
                else {
                    drawpoint(1, Y1, X1);
                    PC_table[Y1][X1].role = 3;
                    if (X1 != 14 && Y1 != 14) {
                        Y1 = Y1 + 2;
                        X1 = X1 + 2;
                        /*Y1++;
                        X1++;*/
                    }
                    else {
                        SIDE1 = 5;
                    }
                    break;
                }
            }
            break;
        }

    }
    else {
        flag = repeat(flag);
    }

    return flag;
}

int fightfirst1(int flag) {
    if (SIDE1 == 5) {
        COUNTER = 0;
    }
    if (COUNTER == 0) {
        while (X1 != 13 || Y1 != 7) {

            if (X1 == 0 && SIDE1 == 5) {

                if (Y1 == 0) {
                    X1 = 4;
                    Y1 = 0;
                }
                if (Y1 == 4) {
                    X1 = 8;
                    Y1 = 0;
                }
                if (Y1 == 12) {
                    Y1 = 3;
                    X1 = 13;
                }
                if (Y1 == 8) {
                    X1 = 12;
                    Y1 = 0;
                }
                SIDE1 = 0;
            }
            if (Y1 == 14 && SIDE1 == 5) {

                if (X1 == 2) {
                    X1 = 13;
                    Y1 = 7;
                }
                if (X1 == 1) {
                    X1 = 0;
                    Y1 = 0;
                }
                if (X1 == 6) {
                    X1 = 13;
                    Y1 = 11;
                }
                SIDE1 = 0;

            }
            if ((Y1 == 1 || Y1 == 3 || Y1 == 4 || Y1 == 6 || Y1 == 0 || Y1 == 8 || Y1 == 12) && SIDE1 == 5) {
                Y1++;
                X1--;
                SIDE1 = 0;
            }
            if (X1 == 11 && Y1 == 13) {

                flag = 0;
                break;
            }
            while (X1 >= 0) {
                if (PC_table[Y1][X1].role == 1 || PC_table[Y1][X1].role == 2) {
                    PC_table[Y1][X1].role = 2;
                    drawkross(1, Y1, X1);
                    flag--;
                    COUNTER = 1;            //it means WARKING
                    flag = repeat(flag);
                    break;
                }
                else {
                    drawpoint(1, Y1, X1);
                    PC_table[Y1][X1].role = 3;
                    if (X1 != 0 && Y1 != 14) {
                        Y1++;
                        X1--;

                    }
                    else {
                        SIDE1 = 5;
                    }
                    break;
                }
            }
            break;
        }

    }
    else {
        flag = repeat(flag);
    }

    return flag;

}


void fight() {

    int flag1 = 33; //количество кораблей на первом поле
    int flag2 = 33;
    int y = 0, x = 0;
    while (flag1 > 0 && flag2 > 0) {

        flag1 = fightsecond(flag1, 0, 0, 0);
        MODE = 0;
        if (CHANGE == 0) {
            flag2 = fightfirst(flag2);
            MODE1 = 0;
        }
        else {
            flag2 = fightfirst1(flag2);
        }
        glFlush();
    }

    window = win;
}

void drawship(int color, int len, int ystart, int xstart, int ynow, int xnow, int orientation, int who) {
    if (who == 0) {
        glColor3ub(255, 0, 127);
    }
    else {
        glColor3f(0, 51, 255);
    }
    if (len == 1) {

        if (who == 0) {
            glBegin(GL_QUADS);
            glVertex2f(xnow, ynow);
            glVertex2f(xnow + 38, ynow);
            glVertex2f(xnow + 38, ynow - 38);
            glVertex2f(xnow, ynow - 38);
            glEnd();
            player_table[ystart][xstart].role = 1;
            if (ystart != 14) {
                player_table[ystart + 1][xstart].role = 11;
            }
            if (ystart != 0) {
                player_table[ystart - 1][xstart].role = 11;
            }
            if (xstart != 14) {
                player_table[ystart][xstart + 1].role = 11;
            }
            if (xstart != 0) {
                player_table[ystart][xstart - 1].role = 11;
            }
            if (xstart != 0 && ystart != 0) {
                player_table[ystart - 1][xstart - 1].role = 11;
            }
            if (xstart != 14 && ystart != 0) {
                player_table[ystart - 1][xstart + 1].role = 11;
            }
            if (xstart != 0 && ystart != 14) {
                player_table[ystart + 1][xstart - 1].role = 11;
            }
            if (xstart != 14 && ystart != 14) {
                player_table[ystart + 1][xstart + 1].role = 11;
            }
        }
        else {
            PC_table[ystart][xstart].role = 1;
            if (ystart != 14) {
                PC_table[ystart + 1][xstart].role = 11;
            }
            if (ystart != 0) {
                PC_table[ystart - 1][xstart].role = 11;
            }
            if (xstart != 14) {
                PC_table[ystart][xstart + 1].role = 11;
            }
            if (xstart != 0) {
                PC_table[ystart][xstart - 1].role = 11;
            }
            if (xstart != 0 && ystart != 0) {
                PC_table[ystart - 1][xstart - 1].role = 11;
            }
            if (xstart != 14 && ystart != 0) {
                PC_table[ystart - 1][xstart + 1].role = 11;
            }
            if (xstart != 0 && ystart != 14) {
                PC_table[ystart + 1][xstart - 1].role = 11;
            }
            if (xstart != 14 && ystart != 14) {
                PC_table[ystart + 1][xstart + 1].role = 11;
            }
        }
    }
    else {
        if (orientation == 0) {  //horixontalno

            if (who == 0) {
                glBegin(GL_QUADS);
                glVertex2f(xnow, ynow);
                glVertex2f(xnow + (38 * len), ynow);
                glVertex2f(xnow + (38 * len), ynow - 38);
                glVertex2f(xnow, ynow - 38);
                glEnd();
                if (xstart != 0) {
                    player_table[ystart][xstart - 1].role = 11;
                    if (ystart != 14) {
                        player_table[ystart + 1][xstart - 1].role = 11;
                    }
                    if (ystart != 0) {
                        player_table[ystart - 1][xstart - 1].role = 11;
                    }
                }
                while (len > 0) {
                    player_table[ystart][xstart].role = 1;
                    if (ystart != 14) {
                        player_table[ystart + 1][xstart].role = 11;
                    }
                    if (ystart != 0) {
                        player_table[ystart - 1][xstart].role = 11;
                    }
                    xstart++;
                    len--;
                }
                if (xstart <= 14) {
                    player_table[ystart][xstart].role = 11;
                    if (ystart != 14) {
                        player_table[ystart + 1][xstart].role = 11;
                    }
                    if (ystart >= 0) {
                        player_table[ystart - 1][xstart].role = 11;
                    }
                }

            }
            else {
                if (xstart != 0) {
                    PC_table[ystart][xstart - 1].role = 11;
                    if (ystart != 14) {
                        PC_table[ystart + 1][xstart - 1].role = 11;
                    }
                    if (ystart != 0) {
                        PC_table[ystart - 1][xstart - 1].role = 11;
                    }
                }
                while (len > 0) {
                    PC_table[ystart][xstart].role = 1;
                    if (ystart != 14) {
                        PC_table[ystart + 1][xstart].role = 11;
                    }
                    if (ystart != 0) {
                        PC_table[ystart - 1][xstart].role = 11;
                    }
                    xstart++;
                    len--;
                }
                if (xstart <= 14) {
                    PC_table[ystart][xstart].role = 11;
                    if (ystart <= 14) {
                        PC_table[ystart + 1][xstart].role = 11;
                    }
                    if (ystart >= 0) {
                        PC_table[ystart - 1][xstart].role = 11;
                    }
                }
            }
        }
        else {                  //vertikalno

            if (who == 0) {
                glBegin(GL_QUADS);
                glVertex2f(xnow, ynow);
                glVertex2f(xnow + 38, ynow);
                glVertex2f(xnow + 38, ynow - (38 * len));
                glVertex2f(xnow, ynow - (38 * len));
                glEnd();
                if (ystart != 14) {
                    player_table[ystart + 1][xstart].role = 11;
                    if (xstart != 14) {
                        player_table[ystart + 1][xstart + 1].role = 11;
                    }
                    if (xstart != 0) {
                        player_table[ystart + 1][xstart - 1].role = 11;
                    }
                }
                while (len > 0) {
                    player_table[ystart][xstart].role = 1;
                    if (xstart != 14) {
                        player_table[ystart][xstart + 1].role = 11;
                    }
                    if (xstart != 0) {
                        player_table[ystart][xstart - 1].role = 11;
                    }
                    ystart--;
                    len--;
                }
                if (ystart >= 0) {
                    player_table[ystart][xstart].role = 11;
                    if (xstart <= 14) {
                        player_table[ystart][xstart + 1].role = 11;
                    }
                    if (xstart >= 0) {
                        player_table[ystart][xstart - 1].role = 11;
                    }
                }
            }
            else {
                if (ystart != 14) {
                    PC_table[ystart + 1][xstart].role = 11;
                    if (xstart != 14) {
                        PC_table[ystart + 1][xstart + 1].role = 11;
                    }
                    if (xstart != 0) {
                        PC_table[ystart + 1][xstart - 1].role = 11;
                    }
                }
                while (len > 0) {
                    PC_table[ystart][xstart].role = 1;
                    if (xstart != 14) {
                        PC_table[ystart][xstart + 1].role = 11;
                    }
                    if (xstart != 0) {
                        PC_table[ystart][xstart - 1].role = 11;
                    }
                    ystart--;
                    len--;
                }
                if (ystart > 0) {
                    PC_table[ystart][xstart].role = 11;
                    if (xstart != 14) {
                        PC_table[ystart][xstart + 1].role = 11;
                    }
                    if (xstart != 0) {
                        PC_table[ystart][xstart - 1].role = 11;
                    }
                }
            }
        }
    }
    glFlush();
}

void initialization_Player() {

    for (int i = 0; i < 15; i++) {
        int help_left = 90, help_down = 150 + 38 * i;
        for (int j = 0; j < 15; j++) {
            player_table[i][j].left = help_left;
            help_left = help_left + 38;
            player_table[i][j].right = help_left;   //идём от нижнего левого угла к верхнему правому углу(j-горизонатль  i-вертикаль)
            player_table[i][j].down = help_down;
            player_table[i][j].up = help_down + 38;
            player_table[i][j].role = 0;
            //printf("%d %d : %d,%d,%d,%d,   %d\n", i, j, player_table[i][j].left, player_table[i][j].right, player_table[i][j].up, player_table[i][j].down, player_table[i][j].role); 
            //printf("%d %d : %d,%d\n", i, j, player_table[i][j].up, player_table[i][j].left); 
        }
    }
    for (int i = 0; i < 15; i++) {
        int help_left = 840, help_down = 150 + 38 * i;
        for (int j = 0; j < 15; j++) {
            PC_table[i][j].left = help_left;
            help_left = help_left + 38;
            PC_table[i][j].right = help_left;   //идём от нижнего левого угла к верхнему правому углу(j-горизонатль  i-вертикаль)
            PC_table[i][j].down = help_down;
            PC_table[i][j].up = help_down + 38;
            PC_table[i][j].role = 0;
            //printf("%d %d : %d,%d,%d,%d\n", i, j, PC_table[i][j].left, PC_table[i][j].right, PC_table[i][j].up, PC_table[i][j].down); 
            //printf("%d %d : %d,%d\n", i, j, PC_table[i][j].up, PC_table[i][j].left); 
        }
    }
}


void playingboard() {

    char laters[] = "ABCDEFGHIJKLMNO";

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();                             //эти 5 строку нужжны чтобы указывать размер кнопок через писксели
    int windowWidth = 1500;  // Ширина окна
    int windowHeight = 800; // Высота окна
    glOrtho(0, windowWidth, 0, windowHeight, -1, 1);

    glLineWidth(3);

    int start = 150; //нижняя граница
    int vert = 90; //левая граница
    int numb = 15;
    glColor3ub(255, 0, 127);
    while (start <= 720) {
        glBegin(GL_LINES);
        glVertex2d(90, start);
        glVertex2d(660, start);
        glEnd();

        // printf("First %d ", start);

        glBegin(GL_LINES);
        glVertex2d(vert, 150);
        glVertex2d(vert, 720);
        glEnd();

        //printf("First %d ", vert); 
        //printf("\n");

        if (numb != 0) {                           //выводим цивры слева на своём поле
            char str[3];
            snprintf(str, sizeof(str), "%d", numb);
            glRasterPos2f(60, start + 10);
            for (int i = 0; str[i] != '\0'; i++) {
                glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str[i]);
            }
        }
        glRasterPos2f(vert + 10, 730);
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, laters[(numb - 15) * (-1)]);
        numb--;
        start = start + 38;
        vert = vert + 38;
    }

    start = 150;
    vert = 840;
    numb = 15;
    glColor3f(0, 51, 255);
    while (start <= 720) {
        glBegin(GL_LINES);
        glVertex2d(840, start);
        glVertex2d(1410, start);
        glEnd();

        // printf("%d ", start);

        glBegin(GL_LINES);
        glVertex2d(vert, 150);
        glVertex2d(vert, 720);
        glEnd();

        //printf("%d", vert); 
        //printf("\n");
        if (numb != 0) {                           //выводим цивры слева на своём поле
            char str[3];
            snprintf(str, sizeof(str), "%d", numb);
            glRasterPos2f(810, start + 10);
            for (int i = 0; str[i] != '\0'; i++) {
                glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str[i]);
            }
        }
        glRasterPos2f(vert + 10, 730);
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, laters[(numb - 15) * (-1)]);
        numb--;
        start = start + 38;
        vert = vert + 38;
    }
    initialization_Player();
    glFlush();
}
void display() {
    glClearColor(1.0, 1.0, 1.0, 1);
    glClear(GL_COLOR_BUFFER_BIT); // Очищаем буфер цвета 

    if (window == MENU) {
        glColor3f(1.0, 0.0, 0.5); // цвет
        glLineWidth(5.0);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();                             //эти 5 строку нужжны чтобы указывать размер кнопок через писксели
        int windowWidth = 1500;
        int windowHeight = 800;
        glOrtho(0, windowWidth, 0, windowHeight, -1, 1);

        button(new_game);
        //button(continue_game);
        button(about_the_game);
        button(about_the_project);
        glFlush();
    }
    else if (window == ABOUT_THE_GAME) {
        glClearColor(1.0, 1.0, 1.0, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();                             //эти 5 строку нужжны чтобы указывать размер кнопок через писксели
        int windowWidth = 1500;  // Ширина окна
        int windowHeight = 800; // Высота окна
        glOrtho(0, windowWidth, 0, windowHeight, -1, 1);

        glLineWidth(5);
        glColor3ub(255, 0, 127);
        glBegin(GL_LINE_LOOP);
        glVertex2f(300, 200);
        glVertex2f(300, 600);
        glVertex2f(1200, 600);
        glVertex2f(1200, 200);
        glEnd();
        glFlush();

        drawtext(646, 550, "ABOUT THE GAME:");
        drawtext(320, 500, "Battleship is a strategy type guessing game for two players.");
        drawtext(320, 450, "It is played on ruled grids on which each player's fleet of warships are marked.");
        drawtext(320, 400, "The locations of the fleets are concealed from the other player.");
        drawtext(320, 350, "Players alternate turns calling shots at the other player's ships, and the objective of the");
        drawtext(320, 325, "game is to destroy the opposing player's fleet.");
        drawtext(320, 275, "Ships: aircraft carrier-1, battleship-1, cruiser-2, destroyers-3, speedboat-4.");

        button(go_out);
        glFlush();

    }
    else if (window == ABOUT_THE_PROJECT) {


        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();                             //эти 5 строку нужжны чтобы указывать размер кнопок через писксели
        int windowWidth = 1500;  // Ширина окна
        int windowHeight = 800; // Высота окна
        glOrtho(0, windowWidth, 0, windowHeight, -1, 1);

        glColor3ub(255, 0, 127);    // цвет 
        glLineWidth(5);
        glBegin(GL_LINE_LOOP);
        glVertex2f(300, 200);
        glVertex2f(300, 600);
        glVertex2f(1200, 600);
        glVertex2f(1200, 200);
        glEnd();
        glFlush();

        drawtext(550, 500, "LYASOVICH SOPHIA DENISOVNA");
        drawtext(646, 450, "5151004/30001");
        drawtext(703, 400, "2024");
        drawtext(383, 350, "PETER THE GREAT ST.PETERBURG POLYTECHNIC UNIVERSITY");
        drawtext(410, 300, "INSTITUTE OF COMPUTER SCIENCE AND CYBERSECURITY");


        button(go_out);
        glFlush();
    }
    else if (window == NEW_GAME) {
        glClearColor(1.0, 1.0, 1.0, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();                             //эти 5 строку нужжны чтобы указывать размер кнопок через писксели
        int windowWidth = 1500;  // Ширина окна
        int windowHeight = 800; // Высота окна
        glOrtho(0, windowWidth, 0, windowHeight, -1, 1);

        //button(save); 
        button(go_out_play);
        button(play);

        playingboard();

        drawship(single1.color, single1.len, single1.ystart, single1.xstart, single1.ynow, single1.xnow, single1.orientation, 0);
        drawship(single2.color, single2.len, single2.ystart, single2.xstart, single2.ynow, single2.xnow, single2.orientation, 0);
        drawship(single3.color, single3.len, single3.ystart, single3.xstart, single3.ynow, single3.xnow, single3.orientation, 0);
        drawship(single4.color, single4.len, single4.ystart, single4.xstart, single4.ynow, single4.xnow, single4.orientation, 0);
        drawship(double1.color, double1.len, double1.ystart, double1.xstart, double1.ynow, double1.xnow, double1.orientation, 0);
        drawship(double2.color, double2.len, double2.ystart, double2.xstart, double2.ynow, double2.xnow, double2.orientation, 0);
        drawship(double3.color, double3.len, double3.ystart, double3.xstart, double3.ynow, double3.xnow, double3.orientation, 0);
        drawship(triple1.color, triple1.len, triple1.ystart, triple1.xstart, triple1.ynow, triple1.xnow, triple1.orientation, 0);
        drawship(triple2.color, triple2.len, triple2.ystart, triple2.xstart, triple2.ynow, triple2.xnow, triple2.orientation, 0);
        drawship(quadruple.color, quadruple.len, quadruple.ystart, quadruple.xstart, quadruple.ynow, quadruple.xnow, quadruple.orientation, 0);
        drawship(thefifth.color, thefifth.len, thefifth.ystart, thefifth.xstart, thefifth.ynow, thefifth.xnow, thefifth.orientation, 0);

        drawship(1, 1, 12, 4, 644, 992, 1, 1);
        drawship(1, 1, 6, 6, 416, 1068, 1, 1);
        drawship(1, 1, 3, 1, 302, 878, 1, 1);
        drawship(1, 1, 1, 7, 226, 1106, 1, 1);
        drawship(1, 2, 8, 12, 492, 1296, 1, 1);
        drawship(1, 2, 4, 12, 340, 1296, 1, 1);
        drawship(1, 2, 3, 14, 302, 1372, 1, 1);
        drawship(1, 3, 13, 12, 682, 1296, 1, 1);
        drawship(1, 3, 0, 12, 188, 1296, 0, 1);
        drawship(1, 4, 8, 14, 492, 1372, 1, 1);
        drawship(1, 5, 14, 14, 720, 1372, 1, 1);
        //drawship(1, 5, 9, 2, 530, 916, 0, 1);
        //proverka();
        glFlush();


    }
    else if (window == win) {


        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();                             //эти 5 строку нужжны чтобы указывать размер кнопок через писксели
        int windowWidth = 1500;  // Ширина окна
        int windowHeight = 800; // Высота окна
        glOrtho(0, windowWidth, 0, windowHeight, -1, 1);

        glColor3ub(255, 0, 127);    // цвет 
        glLineWidth(5);
        glBegin(GL_LINE_LOOP);
        glVertex2f(300, 200);
        glVertex2f(300, 600);
        glVertex2f(1200, 600);
        glVertex2f(1200, 200);
        glEnd();

        glFlush();

        glLineWidth(5);
        glBegin(GL_LINE_LOOP);
        glVertex2f(500, 500);
        glVertex2f(1000, 500);
        glVertex2f(1000, 300);
        glVertex2f(500, 300);
        glEnd();


        glColor3ub(255, 255, 255);

        drawtext(600, 400, "PLAYER 1 IS WINNER!");

        glFlush();
    }

}

void mouse(int codebut, int pos, int x, int y) {
    int Y = Height - y;
    if (window == MENU) {
        if (codebut == GLUT_LEFT_BUTTON && pos == GLUT_DOWN && x >= about_the_project.left && x <= about_the_project.right && Y <= about_the_project.up && Y >= about_the_project.down) {
            printf("About the project.\n");
            window = ABOUT_THE_PROJECT;
            display();

        }
        if (codebut == GLUT_LEFT_BUTTON && pos == GLUT_DOWN && x >= about_the_game.left && x <= about_the_game.right && Y <= about_the_game.up && Y >= about_the_game.down) {
            printf("About the game.\n");
            window = ABOUT_THE_GAME;
            display();
        }
        if (codebut == GLUT_LEFT_BUTTON && pos == GLUT_DOWN && x >= new_game.left && x <= new_game.right && Y <= new_game.up && Y >= new_game.down) {
            printf("Play from menu.");
            window = NEW_GAME;
            display();
        }
    }
    else if (window == ABOUT_THE_PROJECT) {
        if (codebut == GLUT_LEFT_BUTTON && pos == GLUT_DOWN && x >= go_out.left && x <= go_out.right && Y <= go_out.up && y >= go_out.down) {
            printf("Go out from about the project.\n");
            window = MENU;
            display();
        }
    }
    else if (window == ABOUT_THE_GAME) {
        if (codebut == GLUT_LEFT_BUTTON && pos == GLUT_DOWN && x >= go_out.left && x <= go_out.right && Y <= go_out.up && y >= go_out.down) {
            printf("Go out from about the game.\n");
            window = MENU;
            display();
        }
    }
    else if (window == NEW_GAME) {
        if (codebut == GLUT_LEFT_BUTTON && pos == GLUT_DOWN && x <= go_out_play.right && x >= go_out_play.left && Y >= go_out_play.down && Y <= go_out_play.up) {
            printf("Go out from playing zone.");
            window = MENU;
            display();
        }
        if (codebut == GLUT_LEFT_BUTTON && pos == GLUT_DOWN && x <= play.right && x >= play.left && Y <= play.up && Y >= play.down) {
            printf("Start playing.");
            fight();
        }

    }
    else if (window == win) {
        display();
    }

}


int main(int argc, char* argv) {
    clock_t begin, end;
    double time_spent = 0;
    begin = clock();
    glutInit(&argc, argv); // Инициализируем GLUT
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); // Устанавливаем режим отображения
    glutInitWindowSize(1500, 800); // Устанавливаем размер окна
    glutCreateWindow("Морской бой"); // Создаем окно  
    glutDisplayFunc(display); // Регистрируем функцию для отрисовки
    glutMouseFunc(mouse);
    glutMainLoop(); // Запускаем цикл GLUT
    end = clock();
    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Время:%lf", time_spent);
    return 0;
}


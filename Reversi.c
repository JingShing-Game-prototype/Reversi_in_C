#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define size 8
#define hint_x -2

int dx[8] = {-1, 0, 1, -1, 1, -1, 0, 1};
int dy[8] = {1, 1, 1, 0, 0, -1, -1, -1};

int hint_pos_x[30] = {0};
int hint_pos_y[30] = {0};
int max_hint_pos;

int board[size][size];

void initGame();
void printGameField();
int countBlack();
int countWhite();
bool hasFlipLine(int player, int x, int y);
void flipPieces(int player, int x, int y);
bool gameover();
bool need_hint_pos = true;

int main() {
    system("chcp 65001 & cls");
    int player = 0;
    int select_hint;
    initGame();
    while (!gameover()) {
        hint_points(player);
        printGameField();
        if (!max_hint_pos){
                printf("Now it's %s turn.\n", player?"white":"black");
                printf("no selection. so skip your turn\n");
                system("pause");
                player = player? 0: 1;
                continue;
        }
        printf("Now it's %s turn.\n", player?"white":"black");
        printf("Please input the hint number which you want: ");
        scanf("%d", &select_hint);
        select_hint -= 1;
        if (select_hint > max_hint_pos)continue;
        else if(select_hint < 0)continue;
        //printf("fliped -> x:%d, y:%d\n", hint_pos_x[select_hint], hint_pos_y[select_hint]);
        flipPieces(player, hint_pos_x[select_hint], hint_pos_y[select_hint]);
        player = player? 0: 1;
    }
    if (countBlack() > countWhite()) printf("Game over!!! The winner is Black.\n");
    else if (countBlack() < countWhite()) printf("Game over!!! The winner is White.\n");
    else printf("Game over!!! The result is draw.\n");
    return 0;
}

void initGame() {
    for (int i = 0; i < size; ++i)
        for (int j = 0; j < size; ++j) board[i][j] = -1;
    board[3][3] = 1; board[4][4] = 1;
    board[3][4] = 0; board[4][3] = 0;
}

void printGameField() {
    printf("   1  2  3  4  5  6  7  8\n");
    printf("\033[7m  ┌──┬──┬──┬──┬──┬──┬──┬──┐\033[m\n");
    for (int i = 0; i < size; ++i) {
        printf("%d ", i + 1);
        for (int j = 0; j < size; ++j) {
            if (board[i][j] == -1) printf("\033[7m│  \033[m");
            else if (board[i][j] == 1) printf("\033[7m│○\033[m");
            else if (board[i][j] == 0)printf("\033[7m│●\033[m");
            else if (board[i][j] == hint_x)printf("\033[7m│ X\033[m");
        }
        printf("\033[7m│\033[m\n");
        if (i != size - 1) printf("\033[7m  ├──┼──┼──┼──┼──┼──┼──┼──┤\033[m\n");
    }
    printf("\033[7m  └──┴──┴──┴──┴──┴──┴──┴──┘\033[m\n");
    printf("Black: %d, White: %d\n\n", countBlack(), countWhite());
}

void hint_points(int player){
    // set  as hint_x for hint
    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            if(board[i][j]== hint_x)board[i][j] = -1;
        }
    }
    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            if(board[i][j] == -1)if(hasFlipLine(player, i, j))board[i][j] = hint_x;
        }
    }
    int number_of_pos = 0;
    if (need_hint_pos)
        for(int i = 0; i < size; i++){
            for(int j = 0; j < size; j++){
                if(board[i][j]==hint_x){
                        number_of_pos++;
                        printf("no.%d -> x: %d, y: %d\n", number_of_pos, i+1, j+1);
                        hint_pos_x[number_of_pos - 1] = i;
                        hint_pos_y[number_of_pos - 1] = j;
                }
            }
        }
    max_hint_pos = number_of_pos;
}

int countBlack() {
    int ans = 0;
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (!board[i][j]) ++ans;
        }
    }
    return ans;
}

int countWhite() {
    int ans = 0;
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (board[i][j] == 1) ++ans;
        }
    }
    return ans;
}

bool hasFlipLine(int player, int x, int y) {
    for (int i = 0; i < 8; ++i) {
        int a = x, b = y, counts = 0;
        while (true) {
            a += dx[i];
            b += dy[i];
            if (a > size || a < 0 || b > size || b < 0) break;
            if (board[a][b] < 0) break;
            if (board[a][b] == player && !counts)break;
            if (board[a][b] != player && board[a][b] != -1) ++counts;
            else if (counts && board[a][b] == player) return true;
        }
    }
    return false;
}

void flipPieces(int player, int x, int y) {
    board[x][y] = player;
    bool direction[8];
    for (int i = 0; i < 8; ++i) direction[i] = false;
    for (int i = 0; i < 8; ++i) {
        int a = x, b = y, counts = 0;
        while (true) {
            a += dx[i];
            b += dy[i];
            if (a > size || a < 0 || b > size || b < 0) break;
            if (board[a][b] < 0) break;
            if (board[a][b] != player && board[a][b] != -1) ++counts;
            else if (counts && board[a][b] == player) {
                direction[i] = true;
                break;
            }
        }
    }
    for (int i = 0; i < 8; ++i) {
        int a = x, b = y;
        while (direction[i]) {
            a += dx[i];
            b += dy[i];
            if (board[a][b] == player) break;
            board[a][b] = player;
        }
    }
}

bool gameover() {
    if (!countBlack() || !countWhite() || (countBlack() + countWhite() == size * size)) return true;
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (hasFlipLine(0, i, j)) return false;
            if (hasFlipLine(1, i, j)) return false;
        }
    }
    return true;
}

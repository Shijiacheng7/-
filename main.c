#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#define ROWS 9
#define COLS 9
#define MINE_NUM 10

void InitBoard(char board[ROWS][COLS], int rows, int cols, char set);
void DisplayBoard(char board[ROWS][COLS], int row, int col);
void SetMine(char board[ROWS][COLS], int row, int col);
int GetMineCount(char board[ROWS][COLS], int row, int col);
void ExpandBoard(char board[ROWS][COLS], char show_board[ROWS][COLS], int row, int col);
int CheckWin(char show_board[ROWS][COLS], int row, int col);

int main()
{
    system("chcp 65001 > nul");
    char board[ROWS][COLS];
    char show_board[ROWS][COLS];
    int row, col;
    int x, y, m;


    printf("请输入雷区大小（9x9），用空格隔开：\n");
    scanf("%d %d", &row, &col);
    if (row != 9 || col != 9) {
        printf("输入错误！\n");
        return 0;
    }

    InitBoard(board, row, col, '0');
    InitBoard(show_board, row, col, '*');

    SetMine(board, row, col);

    while (1) {
        DisplayBoard(show_board, row, col);
        printf("请输入坐标和操作 (x y m): m = 1/2\n 1=click 2=mark\n");
        scanf("%d %d %d", &x, &y, &m);

        if (x < 1 || x > row || y < 1 || y > col) {
            printf("输入错误！\n");
            continue;
        }

        x--;
        y--;  // 将输入的坐标转换为数组下标

        if (m == 1) {  // 打开格子
            if (show_board[x][y] != '*' && show_board[x][y] != '?') {
                printf("该位置已被打开!\n");
                continue;
            }

            if (board[x][y] == 'M') {
                DisplayBoard(board, row, col);
                printf("你炸了！Game Over!\n");
                break;
            } else {
                ExpandBoard(board, show_board, x, y);
            }
        } else if (m == 2) {  // 标记雷
            if (show_board[x][y] == '*' || show_board[x][y] == '?') {
                show_board[x][y] = '?';
            } else {
                printf("该位置已被打开!\n");
                continue;
            }
        } else {
            printf("输入错误！\n");
            continue;
        }

        if (CheckWin(show_board, row, col) == 1) {
            DisplayBoard(board, row, col);
            printf("恭喜你！你赢了！\n");
            break;
        }
    }

    return 0;
}

// 初始化数组
void InitBoard(char board[ROWS][COLS], int rows, int cols, char set)
{
    int i, j;
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            board[i][j] = set;
        }
    }
}

// 显示雷区
void DisplayBoard(char board[ROWS][COLS], int row, int col)
{
    int i, j;
    printf("    ");
    for (i = 1; i <= col; i++) {
        printf("%d ", i);
    }
    printf("\n");
    printf("  +");
    for (i = 1; i <= col; i++) {
        printf("--");
    }
    printf("\n");
    for (i = 0; i < row; i++) {
        printf("%d | ", i + 1);
        for (j = 0; j < col; j++) {
            printf("%c ", board[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// 放置地雷
void SetMine(char board[ROWS][COLS], int row, int col)
{
    int i, j;
    int count = MINE_NUM;

    srand((unsigned int) time(NULL));
    while (count > 0) {
        i = rand() % row;
        j = rand() % col;
        if (board[i][j] == '0') {
            board[i][j] = 'M';
            count--;
        }
    }
}

// 计算一个位置周围的地雷数
int GetMineCount(char board[ROWS][COLS], int row, int col)
{
    int i, j;
    int count = 0;
    for (i = row - 1; i <= row + 1; i++) {
        for (j = col - 1; j <= col + 1; j++) {
            if (i < 0 || i >= ROWS || j < 0 || j >= COLS) {
                continue;
            } else if (board[i][j] == 'M') {
                count++;
            }
        }
    }
    return count;
}

// 扩展周围的格子
void ExpandBoard(char board[ROWS][COLS], char show_board[ROWS][COLS], int row, int col)
{
    int i, j;
    int count;

    if (show_board[row][col] != '*') {
        return;
    }

    count = GetMineCount(board, row, col);
    if (count > 0) {
        show_board[row][col] = count + '0';
    } else {
        show_board[row][col] = ' ';
        for (i = row - 1; i <= row + 1; i++) {
            for (j = col - 1; j <= col + 1; j++) {
                if (i < 0 || i >= ROWS || j < 0 || j >= COLS) {
                    continue;
                } else if (show_board[i][j] == '*') {
                    ExpandBoard(board, show_board, i, j);
                }
            }
        }
    }
}

// 检查是否游戏胜利
int CheckWin(char show_board[ROWS][COLS], int row, int col)
{
    int i, j;
    for (i = 0; i < row; i++) {
        for (j = 0; j < col; j++) {
            if (show_board[i][j] == '*' || show_board[i][j] == '?') {
                return 0;
            }
        }
    }
    return 1;
}

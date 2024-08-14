#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

int ROWS;
int COLUMNS;
int MINES;
char MINE;
char FLAG;

void print_board(char **brd, bool hide_mines, bool **flag_tiles);
void put_mines(char **brd);
int get_mines(char **brd, int row, int column);
int mine_tile(char **brd, int row, int column);

int main(int argc, char *argv[]) {
    srand(time(NULL));
    printf("select difficulty:\n1. beginner\n2. intermediate\n3. expert\n");
    printf("expert difficulty will be picked by default[1/2/3]: ");
    int difficulty;
    scanf("%d", &difficulty);
    if (difficulty == 1)
        ROWS = 9, COLUMNS = 9, MINES = 10;
    else if (difficulty == 2)
        ROWS = 16, COLUMNS = 16, MINES = 40;
    else
        ROWS = 30, COLUMNS = 16, MINES = 99;

    MINE = 'X', FLAG = 'F';
    char **board = (char **) calloc(ROWS, sizeof(char *));
    bool **flagged = (bool **) calloc(ROWS, sizeof(bool *));
    for (int i = 0; i < ROWS; ++i) {
        board[i] = (char *) calloc(COLUMNS, sizeof(char));
        flagged[i] = (bool *) calloc(COLUMNS, sizeof(bool));
    }

    put_mines(board);
    print_board(board, true, flagged);

    int total_mined = ROWS*COLUMNS-MINES;
    while (true) {
        int i, j;
        printf("enter row: ");
        scanf("%d", &i);
        printf("enter column: ");
        scanf("%d", &j);
        if (i < 1 || i > ROWS || j < 1 || j > COLUMNS) {
            printf("Out of bounds\nTry again\n");
            continue;
        } else if (flagged[i-1][j-1]) {
            printf("mine or remove flag[1/0]? ");
            int choice;
            scanf("%d", &choice);
            if (!choice) {
                flagged[i-1][j-1] = false;
                print_board(board, true, flagged);
                continue;
            }
        } else {
            printf("mine or put flag[1/0]? ");
            int choice;
            scanf("%d", &choice);
            if (!choice) {
                flagged[i-1][j-1] = true;
                print_board(board, true, flagged);
                continue;
            }
        }
        flagged[i-1][j-1] = false;
        
        int status = mine_tile(board, i-1, j-1);
        if (status == -1) {
            print_board(board, false, flagged);
            printf("You fell on a mine\nGame Over\n");
            break;
        }
        print_board(board, true, flagged);

        int mined = 0;
        for (int r = 0; r < ROWS; ++r)
            for (int c = 0; c < COLUMNS; ++c)
                if (board[r][c] && board[r][c] != MINE)
                    ++mined;

        if (mined == total_mined) {
            printf("You win\n");
            break;
        }
        printf("You are safe\n");
    }

    for (int i = 0; i < ROWS; ++i) {
        free(board[i]);
        free(flagged[i]);
    }
    free(board);
    free(flagged);

    return 0;
}
void print_board(char **brd, bool hide_mines, bool **flag_tiles) {
    printf("0  ");
    for (int i = 0; i < COLUMNS; ++i)
        printf(" %d%c", i+1, i+1 < 10 ? ' ' : '\0');
    putchar('\n');
    for (int i = 0; i < ROWS; ++i) {
        printf("%d%s", i+1, i+1 < 10 ? "  " : " ");
        for (int j = 0; j < COLUMNS; ++j) {
            char chr = '\0';
            bool print_space = !brd[i][j] || brd[i][j] == MINE && hide_mines;
            if (print_space && flag_tiles[i][j])
                chr = FLAG;
            else if (print_space)
                chr = ' ';
            else
                chr = brd[i][j];
            printf("[%c]", chr);
        }
        putchar('\n');
    }
}
void put_mines(char **brd) {
    for (int i = 0; i < MINES; ++i) {
        int r = rand()%ROWS, c = rand()%COLUMNS;
        if (!brd[r][c])
            brd[r][c] = MINE;
        else
            --i;
    }
}
int get_mines(char **brd, int row, int column) {
    int res = 0;
    for (int i = row-1; i <= row+1; ++i) {
        if (i < 0 || i >= ROWS)
            continue;
        for (int j = column-1; j <= column+1; ++j) {
            if (j < 0 || j >= COLUMNS || i == row && j == column)
                continue;
            res += brd[i][j] == MINE ? 1 : 0;
        }
    }
    return res;
}
int mine_tile(char **brd, int row, int column) {
    if (brd[row][column] == MINE)
        return -1;
    int mines = get_mines(brd, row, column);
    if (mines > 0) {
        brd[row][column] = '0' + mines;
        return 0;
    }
    brd[row][column] = '0';
    for (int i = row-1; i <= row+1; ++i) {
        if (i < 0 || i >= ROWS)
            continue;
        for (int j = column-1; j <= column+1; ++j) {
            if (j < 0 || j >= COLUMNS || brd[i][j] == '0')
                continue;
            int status = mine_tile(brd, i, j);
        }
    }
    return 0;
}

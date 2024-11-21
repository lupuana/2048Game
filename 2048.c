#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curses.h>
#include <time.h>

void menu (int *choice) {
    // to initialize screen
    *choice = -1;
    int nrows, ncols, i;
	WINDOW *wnd = initscr();
    // to recognize arrows
    keypad(wnd, TRUE);
	getmaxyx(wnd, nrows, ncols);
    // to activate colour
    start_color();
    int highlight = 0;  // index of the current option
    const char options[3][20] = {"New Game", "Resume", "Quit"};
    while (1) {
        clear();
        // the middle of the screen for the menu
        int middRow = (nrows - 5) / 2;
        int middCol = (ncols - 4) / 2;
        // bold and color
        wattron(wnd, A_BOLD | COLOR_PAIR(2));
        // show title "2048"
        mvwprintw(wnd, middRow - 9, middCol - 25, "    .`````-.   .-```````-.       ,---.     .-''''-.    ");
        mvwprintw(wnd, middRow - 8, middCol - 25, "   /   ,-.  \\ / ,```````. \\     /,--.|    /  _--.  \\   ");
        mvwprintw(wnd, middRow - 7, middCol - 25, "  (___/  |   ||/ .-./ )  \\|    //_  ||    |_( )_ ' |   ");
        mvwprintw(wnd, middRow - 6, middCol - 25, "        .'  / || \\ '_ .')||   /_( )_||    (_ o _). /   ");
        mvwprintw(wnd, middRow - 5, middCol - 25, "    _.-'_.-'  ||(_ (_) _)||  /(_ o _)|   .'(_,_).  `.  ");
        mvwprintw(wnd, middRow - 4, middCol - 25, "  _/_  .'     ||  / .  \\ || / /(_,_)||_ |_( )_    \\  | ");
        mvwprintw(wnd, middRow - 3, middCol - 25, " ( ' )(__..--.||  `-'`'` ||/  `-----' ||(_ o _)   /  | ");
        mvwprintw(wnd, middRow - 2, middCol - 25, "(_{;}_)      |\'._______.'/`-------|||-' (_,_)..-'  .'  ");
        mvwprintw(wnd, middRow - 1, middCol - 25, " (_,_)-------' '._______.'         '-'     `-....--'   ");
        wattroff(wnd, A_BOLD | COLOR_PAIR(2));
        // show options menu
        for (i = 0; i < 3; i++) {
            if (i == highlight) // highlight current option
                wattron(wnd, COLOR_PAIR(1));
            else wattron(wnd, COLOR_PAIR(2));
            // the middle for each option
            middCol = (ncols - strlen(options[i])) / 2;
            // to show the option
            mvwprintw(wnd, middRow + i + 2, middCol, "%s", options[i]);
            wattroff(wnd, COLOR_PAIR(1));
        }
        noecho();
        cbreak();
        // to get user input
        int ch = getch();
        switch (ch) {
            case KEY_UP: {
                highlight = (highlight + 2) % 3;
                break;
            }
            case KEY_DOWN: {
                highlight = (highlight + 1) % 3;
                break;
            }
            case 10: { // Enter key
                *choice = highlight;
                break;
            }
        }
        // break the loop if a choice is made
        if (*choice != -1) {
            break;
        }
    }
    wrefresh(wnd);
    endwin();
}

void writeMatrix (char fileName[20], int matrix[][4], int *score, int *bestScore) {
    FILE *file = fopen(fileName, "w");
    fprintf(file, "%d\n", *score);
    fprintf(file, "%d\n", *bestScore);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            fprintf(file, "%d ", matrix[i][j]);
        }
        fprintf(file, "\n");
    }
    fclose(file);
}

void readMatrix (char fileName[20], int matrix[][4], int *score, int *bestScore) {
    FILE *file = fopen(fileName, "r");
    fscanf(file, "%d", score);
    fscanf(file, "%d", bestScore);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            fscanf(file, "%d", &matrix[i][j]); 
        }
    }
    fclose(file);
}

void generateNew (char fileName[20], int matrix[][4], int *score, int *bestScore) {
    readMatrix(fileName, matrix, score, bestScore);
    int row, col;
    srand(time(NULL));
    row = rand() % 4;
    col = rand() % 4;
    while (matrix[row][col] != 0) {
        row = rand() % 4;
        col = rand() % 4;
    }
    matrix[row][col] = (rand() % 2 + 1) * 2;
    writeMatrix(fileName, matrix, score, bestScore);
}

void moveUp (char fileName[20], int matrix[][4], int *score, int *bestScore) {
    int i, j, k;
    int ok = 0; // invalid move
    int before[4][4];
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            before[i][j] = matrix[i][j];
        }
    } // copy of initial matrix
    for (j = 0; j < 4; j++) { // for each column
        // to combine equal values and move non-zero values up
        for (i = 0; i < 3; i++) {
            if (matrix[i][j] == 0) {
                continue;
            }
            for (k = i + 1; k < 4; k++) {
                if (matrix[k][j] == 0) {
                    continue;
                }
                if (matrix[i][j] == matrix[k][j]) {
                    matrix[i][j] *= 2;
                    *score += matrix[i][j]; // to update the score
                    matrix[k][j] = 0;
                }
                break;
            }
        }
        // to move non-zero values up
        for (i = 0; i < 4; i++) {
            if (matrix[i][j] == 0) {
                for (k = i + 1; k < 4; k++) {
                    if (matrix[k][j] != 0) {
                        matrix[i][j] = matrix[k][j];
                        matrix[k][j] = 0;
                        break;
                    }
                }
            }
        }
    }
    // to update the best score
    if (*score > *bestScore) {
        *bestScore = *score;
    }
    for(i = 0; i < 4; i++) {
        for(j = 0; j < 4; j++) {
            if(before[i][j] != matrix[i][j])
            {
                ok = 1; // a modification was made
                break;
            }
        }
    }
    if(ok == 1) {
        writeMatrix(fileName, matrix, score, bestScore);
        generateNew(fileName, matrix, score, bestScore);
    }
}

void moveDown (char fileName[20], int matrix[][4], int *score, int *bestScore) {
    int i, j, k;
    int ok = 0; // invalid move
    int before[4][4];
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            before[i][j] = matrix[i][j];
        }
    } // copy of initial matrix
    for (j = 0; j < 4; j++) { // for each column
        // to combine equal values and move non-zero values down
        for (i = 3; i > 0; i--) {
            if (matrix[i][j] == 0) {
                continue;
            }
            for (k = i - 1; k >= 0; k--) {
                if (matrix[k][j] == 0) {
                    continue;
                }
                if (matrix[i][j] == matrix[k][j]) {
                    matrix[i][j] *= 2;
                    *score += matrix[i][j]; // to update the score
                    matrix[k][j] = 0;
                }
                break;
            }
        }
        // to move non-zero values down
        for (i = 3; i >= 0; i--) {
            if (matrix[i][j] == 0) {
                for (k = i - 1; k >= 0; k--) {
                    if (matrix[k][j] != 0) {
                        matrix[i][j] = matrix[k][j];
                        matrix[k][j] = 0;
                        break;
                    }
                }
            }
        }
    }
    // to update the best score
    if (*score > *bestScore) {
        *bestScore = *score;
    }
    for(i = 0; i < 4; i++) {
        for(j = 0; j < 4; j++) {
            if(before[i][j] != matrix[i][j])
            {
                ok = 1; // a modification was made
                break;
            }
        }
    }
    if(ok == 1) {
        writeMatrix(fileName, matrix, score, bestScore);
        generateNew(fileName, matrix, score, bestScore);
    }
}

void moveRight (char fileName[20], int matrix[][4], int *score, int *bestScore) {
    int i, j, k;
    int ok = 0; // invalid move
    int before[4][4];
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            before[i][j] = matrix[i][j];
        }
    } // copy of initial matrix
    for (i = 0; i < 4; i++) { // for each row
        // to combine equal values and move non-zero values right
        for (j = 3; j > 0; j--) {
            if (matrix[i][j] == 0) {
                continue;
            }
            for (k = j - 1; k >= 0; k--) {
                if (matrix[i][k] == 0) {
                    continue;
                }
                if (matrix[i][j] == matrix[i][k]) {
                    matrix[i][j] *= 2;
                    *score += matrix[i][j]; // to update the score
                    matrix[i][k] = 0;
                }
                break;
            }
        }
        // to move non-zero values right
        for (j = 3; j >= 0; j--) {
            if (matrix[i][j] == 0) {
                for (k = j - 1; k >= 0; k--) {
                    if (matrix[i][k] != 0) {
                        matrix[i][j] = matrix[i][k];
                        matrix[i][k] = 0;
                        break;
                    }
                }
            }
        }
    }
    // to update the best score
    if (*score > *bestScore) {
        *bestScore = *score;
    }
    for(i = 0; i < 4; i++) {
        for(j = 0; j < 4; j++) {
            if(before[i][j] != matrix[i][j])
            {
                ok = 1; // a modification was made
                break;
            }
        }
    }
    if(ok == 1) {
        writeMatrix(fileName, matrix, score, bestScore);
        generateNew(fileName, matrix, score, bestScore);
    }
}

void moveLeft (char fileName[20], int matrix[][4], int *score, int *bestScore) {
    int i, j, k;
    int ok = 0; // invalid move
    int before[4][4];
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            before[i][j] = matrix[i][j];
        }
    } // copy of initial matrix
    for (i = 0; i < 4; i++) { // for each row
        // to combine equal values and move non-zero values left
        for (j = 0; j < 3; j++) {
            if (matrix[i][j] == 0) {
                continue;
            }
            for (k = j + 1; k < 4; k++) {
                if (matrix[i][k] == 0) {
                    continue;
                }
                if (matrix[i][j] == matrix[i][k]) {
                    matrix[i][j] *= 2;
                    *score += matrix[i][j]; // to update the score
                    matrix[i][k] = 0;
                }
                break;
            }
        }
        // to move non-zero values left
        for (j = 0; j < 4; j++) {
            if (matrix[i][j] == 0) {
                for (k = j + 1; k < 4; k++) {
                    if (matrix[i][k] != 0) {
                        matrix[i][j] = matrix[i][k];
                        matrix[i][k] = 0;
                        break;
                    }
                }
            }
        }
    }
    // to update the best score
    if (*score > *bestScore) {
        *bestScore = *score;
    }
    for(i = 0; i < 4; i++) {
        for(j = 0; j < 4; j++) {
            if(before[i][j] != matrix[i][j])
            {
                ok = 1; // a modification was made
                break;
            }
        }
    }
    if(ok == 1) {
        writeMatrix(fileName, matrix, score, bestScore);
        generateNew(fileName, matrix, score, bestScore);
    }
}

void how (char fileName[20], int matrix[][4], int *score, int *bestScore) {
    int nrows, ncols, i, j;
    // to initialize the window
	WINDOW *wnd = initscr();
	getmaxyx(wnd, nrows, ncols);
    // to activate colouring
    start_color();
    int midRow = nrows / 2;
    int midCol = ncols / 2;
    // a box
    for(i = midRow - 6; i <= midRow + 6; i++) {
        for(j = midCol - 15; j <= midCol + 15; j++) {
            if(i == midRow - 6 || i == midRow + 6
            || j == midCol - 15 || j == midCol + 15) {
                // the border
                wattron(wnd, COLOR_PAIR(3));
            }
            else wattron(wnd, COLOR_PAIR(4));
            mvwprintw(wnd, i, j, " ");
            wattroff(wnd, COLOR_PAIR(3));
            wattroff(wnd, COLOR_PAIR(4));
        }
    }
    // the text
    wattron(wnd, COLOR_PAIR(5));
    wattron(wnd, A_BOLD);
    mvwprintw(wnd, midRow - 4, midCol - 13, "HOW TO PLAY");
    wattroff(wnd, A_BOLD);
    mvwprintw(wnd, midRow - 2, midCol - 13, "Use the arrow keys to move");
    mvwprintw(wnd, midRow - 1, midCol - 13, "the tiles. Tiles with the");
    mvwprintw(wnd, midRow, midCol - 13, "same number merge into one");
    mvwprintw(wnd, midRow + 1, midCol - 13, "when they touch. Add them");
    mvwprintw(wnd, midRow + 2, midCol - 13, "up to reach 2048!");
    wattron(wnd, A_BOLD);
    mvwprintw(wnd, midRow + 4, midCol - 13, "~press Q to go back~");
    wattroff(wnd, A_BOLD);
    wattroff(wnd, COLOR_PAIR(5));
    while (getch() != 'q') {
        // waiting for the user to press q
    }
    return;  
}

void winMessage (int *choice) {
    int nrows, ncols, i, j;
    // to initialize the window
	WINDOW *wnd = initscr();
	getmaxyx(wnd, nrows, ncols);
    // to activate colouring
    start_color();
    int midRow = nrows / 2;
    int midCol = ncols / 2;
    // the box
    for(i = midRow - 5; i <= midRow + 3; i++) {
        for(j = midCol - 20; j <= midCol + 20; j++) {
            if(i == midRow - 5 || i == midRow + 3
            || j == midCol - 20 || j == midCol + 20
            || j == midCol - 19 || j == midCol + 19) {
                // the border
                wattron(wnd, COLOR_PAIR(3));
            }
            else wattron(wnd, COLOR_PAIR(4));
            mvwprintw(wnd, i, j, " ");
            wattroff(wnd, COLOR_PAIR(3));
            wattroff(wnd, COLOR_PAIR(4));
        }
    }
    // the text
    wattron(wnd, COLOR_PAIR(5));
    wattron(wnd, A_BOLD);
    mvwprintw(wnd, midRow - 2, midCol - 4, "YOU WON!");
    wattroff(wnd, A_BOLD);
    mvwprintw(wnd, midRow, midCol - 11, "~press Q to go to menu~");
    wattroff(wnd, COLOR_PAIR(5));
    while (getch() != 'q') {
        // waiting for the user to press q
    }
    *choice = 3;
    return;  
}

void loseMessage (int *choice) {
    int nrows, ncols, i, j;
    // to initialize the window
	WINDOW *wnd = initscr();
	getmaxyx(wnd, nrows, ncols);
    // to activate colouring
    start_color();
    int midRow = nrows / 2;
    int midCol = ncols / 2;
    // the box
    for(i = midRow - 5; i <= midRow + 3; i++) {
        for(j = midCol - 20; j <= midCol + 20; j++) {
            if(i == midRow - 5 || i == midRow + 3
            || j == midCol - 20 || j == midCol + 20
            || j == midCol - 19 || j == midCol + 19) {
                // the border
                wattron(wnd, COLOR_PAIR(3));
            }
            else wattron(wnd, COLOR_PAIR(4));
            mvwprintw(wnd, i, j, " ");
            wattroff(wnd, COLOR_PAIR(3));
            wattroff(wnd, COLOR_PAIR(4));
        }
    }
    // the text
    wattron(wnd, COLOR_PAIR(5));
    wattron(wnd, A_BOLD);
    mvwprintw(wnd, midRow - 2, midCol - 4, "YOU LOST:(");
    wattroff(wnd, A_BOLD);
    mvwprintw(wnd, midRow, midCol - 11, "~press Q to go to menu~");
    wattroff(wnd, COLOR_PAIR(5));
    while (getch() != 'q') {
        // waiting for the user to press q
    }
    *choice = 3;
    return;  
}

int winner (int matrix[][4]) {
    int ok = 0; // no winner
    int i, j;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            if(matrix[i][j] == 2048) {
                ok = 1; // winner
                return ok;
            }
        }
    }
    return ok; // no winner yet
}

int loser(char fileName[20], int matrix[][4], int *score, int *bestScore) {
    int before[4][4];
    int i, j;
    // looking for an empty space
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            if (matrix[i][j] == 0) {
                return 0; // an empty space was found
            }
            before[i][j] = matrix[i][j]; // to copy the matrix
        }
    }
    // to verify if there is an empty space
    int direction;
    for (direction = 0; direction < 4; direction++) {
        switch (direction) {
            case 0: 
                moveUp(fileName, matrix, score, bestScore); 
                break;
            case 1: 
                moveDown(fileName, matrix, score, bestScore); 
                break;
            case 2: 
                moveLeft(fileName, matrix, score, bestScore); 
                break;
            case 3: 
                moveRight(fileName, matrix, score, bestScore); 
                break;
        }
        // checking if the matrix has changed
        for (i = 0; i < 4; i++) {
            for (j = 0; j < 4; j++) {
                if (before[i][j] != matrix[i][j]) {
                    for (i = 0; i < 4; i++) {
                        for (j = 0; j < 4; j++) {
                            matrix[i][j] = before[i][j]; // to copy the matrix
                        }
                    }
                    // to cancel the move
                    writeMatrix(fileName, matrix, score, bestScore);
                    return 0; // there is one more possible move
                }
            }
        }
    }
    return 1;
}

void show (char fileName[20], int matrix[][4], int *score, int *bestScore) {
    int  nrows, ncols, i, j, k, p, m = 0, n = 0;
    // to initialize the window
	WINDOW *wnd = initscr();
	getmaxyx(wnd, nrows, ncols);
    nrows++;
    nrows--;
    wrefresh(wnd);
    start_color();
    // for the middle of the window
    int middCol = ncols / 2;
    readMatrix(fileName, matrix, score, bestScore);
    for (i = 8; i <= 26; i += 6) {
        for (j = (middCol - 18); j <= (middCol + 18) ; j += 12) {
            if (matrix[m][n] == 0) {
                for(k = i - 2; k <= i + 2; k++) {
                    for(p = j - 5; p <= j + 5; p++) {
                        // to color the box
                        wattron(wnd, COLOR_PAIR(6));
                        mvwprintw(wnd, k, p, " ");
                        wattroff(wnd, COLOR_PAIR(6));
                    }
                }
            }
            else if(matrix[m][n] == 2) {
                for(k = i - 2; k <= i + 2; k++) {
                    for(p = j - 5; p <= j + 5; p++) {
                        // to color the box
                        wattron(wnd, COLOR_PAIR(8));
                        mvwprintw(wnd, k, p, " ");
                        wattroff(wnd, COLOR_PAIR(8));
                    }
                }
                // to color the number
                wattron(wnd, COLOR_PAIR(7));
                mvwprintw(wnd, i, j, "%d", matrix[m][n]);
                wattroff(wnd, COLOR_PAIR(7));
            }
            else if (matrix[m][n] == 4) {
                for(k = i - 2; k <= i + 2; k++) {
                    for(p = j - 5; p <= j + 5; p++) {
                        // to color the box
                        wattron(wnd, COLOR_PAIR(10));
                        mvwprintw(wnd, k, p, " ");
                        wattroff(wnd, COLOR_PAIR(10));
                    }
                }
                // to color the number
                wattron(wnd, COLOR_PAIR(9));
                mvwprintw(wnd, i, j, "%d", matrix[m][n]);
                wattroff(wnd, COLOR_PAIR(9));
            }
            else if (matrix[m][n] == 8) {
                for(k = i - 2; k <= i + 2; k++) {
                    for(p = j - 5; p <= j + 5; p++) {
                        // to color the box
                        wattron(wnd, COLOR_PAIR(12));
                        mvwprintw(wnd, k, p, " ");
                        wattroff(wnd, COLOR_PAIR(12));
                    }
                }
                // to color the number
                wattron(wnd, COLOR_PAIR(11));
                mvwprintw(wnd, i, j, "%d", matrix[m][n]);
                wattroff(wnd, COLOR_PAIR(11));
            }
            else if (matrix[m][n] == 16) {
                for(k = i - 2; k <= i + 2; k++) {
                    for(p = j - 5; p <= j + 5; p++) {
                        // to color the box
                        wattron(wnd, COLOR_PAIR(14));
                        mvwprintw(wnd, k, p, " ");
                        wattroff(wnd, COLOR_PAIR(14));
                    }
                }
                // to color the number
                wattron(wnd, COLOR_PAIR(13));
                mvwprintw(wnd, i, j - 1, "%d", matrix[m][n]);
                wattroff(wnd, COLOR_PAIR(13));
            }
            else if (matrix[m][n] == 32) {
                for(k = i - 2; k <= i + 2; k++) {
                    for(p = j - 5; p <= j + 5; p++) {
                        // to color the box
                        wattron(wnd, COLOR_PAIR(16));
                        mvwprintw(wnd, k, p, " ");
                        wattroff(wnd, COLOR_PAIR(16));
                    }
                }
                // to color the number
                wattron(wnd, COLOR_PAIR(15));
                mvwprintw(wnd, i, j - 1, "%d", matrix[m][n]);
                wattroff(wnd, COLOR_PAIR(15));
            }
            else if (matrix[m][n] == 64) {
                for(k = i - 2; k <= i + 2; k++) {
                    for(p = j - 5; p <= j + 5; p++) {
                        // to color the box
                        wattron(wnd, COLOR_PAIR(18));
                        mvwprintw(wnd, k, p, " ");
                        wattroff(wnd, COLOR_PAIR(18));
                    }
                }
                // to color the number
                wattron(wnd, COLOR_PAIR(17));
                mvwprintw(wnd, i, j - 1, "%d", matrix[m][n]);
                wattroff(wnd, COLOR_PAIR(17));
            }
            else if (matrix[m][n] == 128) {
                for(k = i - 2; k <= i + 2; k++) {
                    for(p = j - 5; p <= j + 5; p++) {
                        // to color the box
                        wattron(wnd, COLOR_PAIR(20));
                        mvwprintw(wnd, k, p, " ");
                        wattroff(wnd, COLOR_PAIR(20));
                    }
                }
                // to color the number
                wattron(wnd, COLOR_PAIR(19));
                mvwprintw(wnd, i, j - 1, "%d", matrix[m][n]);
                wattroff(wnd, COLOR_PAIR(19));
            }
            else if (matrix[m][n] == 256) {
                for(k = i - 2; k <= i + 2; k++) {
                    for(p = j - 5; p <= j + 5; p++) {
                        // to color the box
                        wattron(wnd, COLOR_PAIR(22));
                        mvwprintw(wnd, k, p, " ");
                        wattroff(wnd, COLOR_PAIR(22));
                    }
                }
                // to color the number
                wattron(wnd, COLOR_PAIR(21));
                mvwprintw(wnd, i, j - 1, "%d", matrix[m][n]);
                wattroff(wnd, COLOR_PAIR(21));
            }
            else if (matrix[m][n] == 512) {
                for(k = i - 2; k <= i + 2; k++) {
                    for(p = j - 5; p <= j + 5; p++) {
                        // to color the box
                        wattron(wnd, COLOR_PAIR(24));
                        mvwprintw(wnd, k, p, " ");
                        wattroff(wnd, COLOR_PAIR(24));
                    }
                }
                // to color the number
                wattron(wnd, COLOR_PAIR(23));
                mvwprintw(wnd, i, j - 1, "%d", matrix[m][n]);
                wattroff(wnd, COLOR_PAIR(23));
            }
            else if (matrix[m][n] == 1024) {
                for(k = i - 2; k <= i + 2; k++) {
                    for(p = j - 5; p <= j + 5; p++) {
                        // to color the box
                        wattron(wnd, COLOR_PAIR(26));
                        mvwprintw(wnd, k, p, " ");
                        wattroff(wnd, COLOR_PAIR(26));
                    }
                }
                // to color the number
                wattron(wnd, COLOR_PAIR(25));
                mvwprintw(wnd, i, j - 2, "%d", matrix[m][n]);
                wattroff(wnd, COLOR_PAIR(25));
            }
            else if (matrix[m][n] == 2048) {
                for(k = i - 2; k <= i + 2; k++) {
                    for(p = j - 5; p <= j + 5; p++) {
                        // to color the box
                        wattron(wnd, COLOR_PAIR(28));
                        mvwprintw(wnd, k, p, " ");
                        wattroff(wnd, COLOR_PAIR(28));
                    }
                }
                // to color the number
                wattron(wnd, COLOR_PAIR(27));
                mvwprintw(wnd, i, j - 2, "%d", matrix[m][n]);
                wattroff(wnd, COLOR_PAIR(27));
            }
            n = (n + 1) % 4;
        }
        m++;
    }
    wrefresh(wnd);
    endwin();
}

void newGame (char fileName[20], int *score, int *bestScore, int *choice, int matrix[][4]) {
    int nrows, ncols, i, j;
    // to initialize the window
	WINDOW *wnd = initscr();
	getmaxyx(wnd, nrows, ncols);
    nrows++;
    nrows--;
    raw();
    // for the middle of the window
    int middCol = ncols / 2;
    // to activate colouring
    start_color(); 
    // for date and time
    int datewndWidth = 20, datewndHeight = 3;
    WINDOW *datewnd = newwin(datewndHeight, datewndWidth, 0, ncols - datewndWidth);
    box(datewnd, 0, 0);
    // to randomly generate 2 numbers
    srand(time(NULL));
    if(*choice == 0) { // new game
        int firstRow = rand() % 4;
        int firstCol = rand() % 4;
        matrix[firstRow][firstCol] = (rand() % 2 + 1) * 2;
        int secondRow = rand() % 4;
        int secondCol = rand() % 4;
        while (firstRow == secondRow && firstCol == secondCol) {
            // randomly generate 2 positions
            secondRow = rand() % 4;
            secondCol = rand() % 4;
        }
        matrix[secondRow][secondCol] = (rand() % 2 + 1) * 2;
    }
    writeMatrix(fileName, matrix, score, bestScore);
    while (1) {
        clear();
        wattron(wnd, COLOR_PAIR(31));
        mvwprintw(wnd, 1, middCol - 24, "How to play (Press X)\n");
        mvwprintw(wnd, 2, middCol - 24, "Score");
        mvwprintw(wnd, 2, middCol + 15, "Best score");
        mvwprintw(wnd, 3, middCol - 24, "%d", *score);
        mvwprintw(wnd, 3, middCol + 15, "%d", *bestScore);
        mvwprintw(wnd, 31, middCol - 24, "Press Q to go back to menu");
        wattroff(wnd, A_BOLD | COLOR_PAIR(31));
        readMatrix (fileName, matrix, score, bestScore);
        // game board
        for (i = 5; i < 30; i++) {
            for (j = (middCol - 24); j <= (middCol + 24) ; j++){
                if ((i % 6 == 5) || ((j - middCol) % 12 == 0)){
                    // border
                    wattron(wnd, COLOR_PAIR(29));
                    mvwprintw(wnd, i, j, " ");
                    wattroff(wnd, COLOR_PAIR(29));
                }
                else { // empty box
                    wattron(wnd, COLOR_PAIR(30));
                    mvwprintw(wnd, i, j, " ");
                    wattroff(wnd, COLOR_PAIR(30));
                }
            }
        }
        show(fileName, matrix, score, bestScore);
        time_t currentTime = time(NULL);
        struct tm *local = localtime(&currentTime);
        char bufferDate[80];
        strftime(bufferDate, 80, "%H:%M %d/%m/%Y", local);
        werase(datewnd);
        box(datewnd, 0, 0);
        mvwprintw(datewnd, 1, 1, "%s", bufferDate);
        wrefresh(datewnd);
        if (winner(matrix)) { // checking for a winner
            winMessage(choice);
            break;
        }
        if (loser(fileName, matrix, score, bestScore)) { // checking for a loser
            loseMessage(choice);
            break;
        }
        // to activate the arrow keys
        keypad(wnd, TRUE);
        noecho();
        cbreak();
        int ch = getch();
        switch (ch) {
            case KEY_UP: 
                moveUp(fileName, matrix, score, bestScore);
                show(fileName, matrix, score, bestScore);
                break;
            case KEY_DOWN: 
                moveDown(fileName, matrix, score, bestScore);
                show(fileName, matrix, score, bestScore);
                break;
            case KEY_LEFT: 
                moveLeft(fileName, matrix, score, bestScore);
                show(fileName, matrix, score, bestScore);
                break;
            case KEY_RIGHT: 
                moveRight(fileName, matrix, score, bestScore);
                show(fileName, matrix, score, bestScore);
                break;
            case 'x':
                how(fileName, matrix, score, bestScore);
                break;
            case 'q':
            *choice = 3;
            return; // to leave the game
        }
    }
    endwin();
}

int main() {
    int bestScore = 0, score = 0, i, j;
    int choice = -1; // the choice of the user
    int matrix[4][4];
    char fileName[20] = "array.txt";
    initscr();
    start_color();
    // menu colors
    // highlighted color
    init_pair(1, COLOR_BLUE, COLOR_WHITE);
    // normal color and title
    init_pair(2, COLOR_BLUE, COLOR_BLACK);
    // pop ups
    // the border
    init_color(21, 255, 54, 171);
    init_pair(3, 21, 21);
    // interior
    init_color(22, 255, 184, 222);
    init_pair(4, 22, 22);
    // text
    init_pair(5, COLOR_WHITE, 22);

    // the game
    // empty box color
    init_color(9, 201, 24, 74);  // light pink
    init_pair(6, 9, 9);
    // number 2
    init_color(10, 0, 204, 0); // green
    init_pair(7, COLOR_BLACK, 10); 
    init_pair(8, 10, 10); // the box for number 2
    // number 4
    init_pair(9, COLOR_BLACK, COLOR_GREEN); 
    init_pair(10, COLOR_GREEN, COLOR_GREEN); // the box for number 4
    // number 8
    init_pair(11, COLOR_BLACK, COLOR_BLUE);
    init_pair(12, COLOR_BLUE, COLOR_BLUE);
    // number 16
    init_color(13, 255, 149, 0); // orange
    init_pair(13, COLOR_WHITE, 13);
    init_pair(14, 13, 13);
    // number 32
    init_pair(15, COLOR_BLACK, COLOR_MAGENTA);
    init_pair(16, COLOR_MAGENTA, COLOR_MAGENTA);
    // number 64
    init_color(15, 0, 149, 255); // blue
    init_pair(17, COLOR_BLACK, 15);
    init_pair(18, 15, 15);
    // number 128
    init_pair(19, COLOR_WHITE, COLOR_RED);
    init_pair(20, COLOR_RED, COLOR_RED);
    // number 256
    init_color(17, 255, 0, 255); // pink
    init_pair(21, COLOR_WHITE, 17);
    init_pair(22, 17, 17);
    // number 512
    init_color(18, 106, 0, 255); // purple
    init_pair(23, COLOR_BLACK, 18);
    init_pair(24, 18, 18);
    // number 1024
    init_color(19, 250, 66, 16); // orange
    init_pair(25, COLOR_BLACK, 19);
    init_pair(26, 19, 19);
    // number 2048
    init_color(20, 20, 255, 30); // blue
    init_pair(27, COLOR_BLACK, 20);
    init_pair(28, 20, 20);

    // new game
    // border color
    init_color(8, 164, 19, 60);  // dark pink
    init_pair(29, 8, 8);
    // empty box color
    init_color(9, 201, 24, 74);  // light pink
    init_pair(30, 9, 9);
    // text color
    init_pair(31, 9, COLOR_BLACK);
    // to keep the best score
    readMatrix(fileName, matrix, &score, &bestScore);
    while (1) {
        menu(&choice); // to show the menu
        if (choice == 0 || choice == 1) { // new game or resume
            if (choice == 0) { // new game
                for (i = 0; i < 4; i++) {
                    for (j = 0; j < 4; j++) {
                        matrix[i][j] = 0;
                    }
                } // resetting the matrix
                score = 0; // resetting the score
            }
            else if (choice == 1) { // resume
                // to read the state of the game
                readMatrix(fileName, matrix, &score, &bestScore);
            }
            writeMatrix(fileName, matrix, &score, &bestScore);
            newGame(fileName, &score, &bestScore, &choice, matrix);
        }
        if (choice == 2) { // quit
            break;
        }
        if (choice == 3) { // coming from new game
            choice = -1;
            continue; // continuing the loop to recall the menu
        }
    }
    clear();
    refresh();
    endwin();
    return 0;
}
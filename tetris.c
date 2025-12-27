#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <wchar.h>
#include <locale.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>

#define ROWS 16
#define COLS 56
#define EMPTY L' '
#define COLOR_RESET   L"\x1b[0m"
#define COLOR_BLACK   L"\x1b[40m"
#define COLOR_RED     L"\x1b[31m"
#define COLOR_GREEN   L"\x1b[32m"
#define COLOR_YELLOW  L"\x1b[33m"
#define COLOR_BLUE    L"\x1b[34m"

void setupMap(wchar_t map[ROWS][COLS]);
void drawMap(wchar_t map[ROWS][COLS]);
int gameMenu();
bool isWall(wchar_t character);
void clearScreen();
void handleInput(char *direction);
void enableRawMode();
void disableRawMode();
int kbhit();



int main(){
    setlocale(LC_ALL, ""); 
    wprintf(L"\x1b[?25l");
    enableRawMode(); 
    srand(time(NULL)); 
    wchar_t map[ROWS][COLS];
    bool gameIsRunning = true;
    
    while (gameIsRunning)
    {
        int gamersChoice = gameMenu();
        if (gamersChoice == 1)
        {
            setupMap(map);
            drawMap(map);
        }
        if (gamersChoice == 2)
        {
            
        }
        if (gamersChoice == 3)
        {
            break;
        }
    }

    disableRawMode();

    return 0;
}

void setupMap(wchar_t map[ROWS][COLS]) {

    wchar_t Layout[ROWS][COLS + 1] = {
        L"┏━━━━━━━━━━━━━━━━━━━━━━━┓              ┏━━━━━━━━━━━━━━━┓",
        L"┃                       ┃              ┃ Next in queue ┃",
        L"┃                       ┃              ┃               ┃",
        L"┃                       ┃              ┃               ┃",
        L"┃                       ┃              ┃               ┃",
        L"┃                       ┃              ┗━━━━━━━━━━━━━━━┛",
        L"┃                       ┃                               ",
        L"┃                       ┃                               ",
        L"┃                       ┃                               ",
        L"┃                       ┃                               ",
        L"┃                       ┃                               ",
        L"┃                       ┃                               ",
        L"┃                       ┃                               ",
        L"┃                       ┃                               ",
        L"┃                       ┃                               ",
        L"┗━━━━━━━━━━━━━━━━━━━━━━━┛                               ",

    };

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {   
            map[i][j] = Layout[i][j];
        }
    }
}

void drawMap(wchar_t map[ROWS][COLS]) {

    wprintf(L"\x1b[H"); // To stop the flickering
    
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            wchar_t tile = map[i][j];

            if (isWall(tile))
            {
                wprintf(L"%ls%lc", COLOR_BLUE, tile);
            }
            else if (tile == EMPTY) 
            {
                wprintf(L"%ls%lc", COLOR_RESET, tile);
            }
            else 
            {
                wprintf(L"%ls%lc", COLOR_RESET, tile); 
            }
            
        }
        wprintf(L"\n");
    }
}

int gameMenu(){

    wprintf(L" %ls\n", COLOR_BLACK);
    wprintf(L"T E T R I S\n\n\n\n");
    wprintf(L"[1] - Play game.\n\n");
    wprintf(L"[2] - See Instructions.\n\n");
    wprintf(L"[3] - Quit.\n\n");
    wprintf(L"%ls", COLOR_RESET);
    
    while (true)
    {
        char playerChoice = getchar();
        if (playerChoice == '1')
        {
            return 1;
        }
        if (playerChoice == '2')
        {
            return 2;
        }
        if (playerChoice == '3')
        {
            return 3;
        }
    }
}

/*int playing() {


    return;
}*/

// Function to detect the walls
bool isWall(wchar_t character) {
    return character == L'┏' || character == L'┓' || character == L'┗' || character == L'┛' || character == L'━' || character == L'┃';
}

void clearScreen() {
    wprintf(L"\x1b[2J\x1b[3J\x1b[H"); // 2J = Clear Screen, 3J = Clear History, H = Go Home
}

// Enable "Raw Mode" (disable waiting for Enter)
void enableRawMode() {
    struct termios term;
    tcgetattr(STDIN_FILENO, &term); // Get current settings
    term.c_lflag &= ~(ICANON | ECHO); // Disable buffering (ICANON) and local echo (ECHO)
    tcsetattr(STDIN_FILENO, TCSANOW, &term); // Apply settings
}

// Disable "Raw Mode" (restore settings when game ends)
void disableRawMode() {
    struct termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag |= (ICANON | ECHO); // Re-enable them
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

// Check if a key has been pressed
int kbhit() {
    int bytesWaiting;
    ioctl(STDIN_FILENO, FIONREAD, &bytesWaiting);
    return bytesWaiting;
}

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

#define ROWS 15
#define COLS 25
#define EMPTY L' '
#define COLOR_RESET   L"\x1b[0m"
#define COLOR_RED     L"\x1b[31m"
#define COLOR_GREEN   L"\x1b[32m"
#define COLOR_YELLOW  L"\x1b[33m"
#define COLOR_BLUE    L"\x1b[34m"

void setupMap(wchar_t map[ROWS][COLS]);
void drawMap(wchar_t map[ROWS][COLS]);
int gameMenu();
bool isWall(wchar_t character);
void Instructions();
void clearScreen();
char gameOverScreen();
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
    bool gameOver = false;

    
    while (gameIsRunning)
    {
        int gamersChoice = gameMenu();
        setupMap(map);
        if (gamersChoice == 1)
        {
            while (!gameOver)
            {
                drawMap(map);
            }
            
        }
        if (gamersChoice == 2)
        {
            Instructions();
            clearScreen();
            gameOverScreen();
            continue;
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
        L"┏━━━━━━━━━━━━━━━━━━━━━━━┓",
        L"┃                       ┃",
        L"┃                       ┃",
        L"┃                       ┃",
        L"┃                       ┃",
        L"┃                       ┃",
        L"┃                       ┃",
        L"┃                       ┃",
        L"┃                       ┃",
        L"┃                       ┃",
        L"┃                       ┃",
        L"┃                       ┃",
        L"┃                       ┃",
        L"┃                       ┃",
        L"┗━━━━━━━━━━━━━━━━━━━━━━━┛",

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
                wprintf(L"%lc", tile);
            }
            
        }
        wprintf(L"\n");
    }
}

int gameMenu(){

    clearScreen();

    wprintf(L"T E T R I S\n\n\n\n");
    wprintf(L"[1] - Play game.\n\n");
    wprintf(L"[2] - See Instructions.\n\n");
    wprintf(L"[3] - Quit.\n\n");
    
    while (true)
    {
        char playerchoice = getchar();
        if (playerchoice == '1')
        {
            return 1;
        }
        if (playerchoice == '2')
        {
            return 2;
        }
        if (playerchoice == '3')
        {
            return 3;
        }
        usleep(100000); 
    }
}

// Function to detect the walls
bool isWall(wchar_t character) {
    return character == L'┏' || character == L'┓' || character == L'┗' || character == L'┛' || character == L'━' || character == L'┃';
}

void Instructions(){

    clearScreen();

    wprintf(L"┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n");
    wprintf(L"┃           T E T R I S           ┃\n");
    wprintf(L"┃                                 ┃\n");
    wprintf(L"┃  [UP ARROW] - Rotate            ┃\n");
    wprintf(L"┃                                 ┃\n");
    wprintf(L"┃  [LEFT ARROW] - Move Left       ┃\n");
    wprintf(L"┃                                 ┃\n");
    wprintf(L"┃  [RIGHT ARROW] - Move Right     ┃\n");
    wprintf(L"┃                                 ┃\n");
    wprintf(L"┃  [DOWN ARROW] - Move Down Faster┃\n");
    wprintf(L"┃                                 ┃\n");
    wprintf(L"┃  [Q] - To Pause The Game        ┃\n");
    wprintf(L"┃                                 ┃\n");
    wprintf(L"┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n");
    wprintf(L"\n\n");
    wprintf(L"PRESS ANY KEY TO LEAVE THIS SCREEN.\n\n");

    while (kbhit())
    {
        getchar();
    }
    getchar();
}

void clearScreen() {
    wprintf(L"\x1b[2J\x1b[3J\x1b[H"); // 2J = Clear Screen, 3J = Clear History, H = Go Home
}

char gameOverScreen(){

    clearScreen();

    wprintf(L"        GGGGGGGGGGGGG               AAA               MMMMMMMM               MMMMMMMMEEEEEEEEEEEEEEEEEEEEEE          OOOOOOOOO     VVVVVVVV           VVVVVVVVEEEEEEEEEEEEEEEEEEEEEERRRRRRRRRRRRRRRRR\n");
    wprintf(L"     GGG::::::::::::G              A:::A              M:::::::M             M:::::::ME::::::::::::::::::::E        OO:::::::::OO   V::::::V           V::::::VE::::::::::::::::::::ER::::::::::::::::R\n");
    wprintf(L"   GG:::::::::::::::G             A:::::A             M::::::::M           M::::::::ME::::::::::::::::::::E      OO:::::::::::::OO V::::::V           V::::::VE::::::::::::::::::::ER::::::RRRRRR:::::R\n");
    wprintf(L"  G:::::GGGGGGGG::::G            A:::::::A            M:::::::::M         M:::::::::MEE::::::EEEEEEEEE::::E     O:::::::OOO:::::::OV::::::V           V::::::VEE::::::EEEEEEEEE::::ERR:::::R     R:::::R\n");
    wprintf(L" G:::::G       GGGGGG           A:::::::::A           M::::::::::M       M::::::::::M  E:::::E       EEEEEE     O::::::O   O::::::O V:::::V           V:::::V   E:::::E       EEEEEE  R::::R     R:::::R\n");
    wprintf(L"G:::::G                        A:::::A:::::A          M:::::::::::M     M:::::::::::M  E:::::E                  O:::::O     O:::::O  V:::::V         V:::::V    E:::::E               R::::R     R:::::R\n");
    wprintf(L"G:::::G                       A:::::A A:::::A         M:::::::M::::M   M::::M:::::::M  E::::::EEEEEEEEEE        O:::::O     O:::::O   V:::::V       V:::::V     E::::::EEEEEEEEEE     R::::RRRRRR:::::R\n");
    wprintf(L"G:::::G    GGGGGGGGGG        A:::::A   A:::::A        M::::::M M::::M M::::M M::::::M  E:::::::::::::::E        O:::::O     O:::::O    V:::::V     V:::::V      E:::::::::::::::E     R:::::::::::::RR\n");
    wprintf(L"G:::::G    G::::::::G       A:::::A     A:::::A       M::::::M  M::::M::::M  M::::::M  E:::::::::::::::E        O:::::O     O:::::O     V:::::V   V:::::V       E:::::::::::::::E     R::::RRRRRR:::::R\n");
    wprintf(L"G:::::G    GGGGG::::G      A:::::AAAAAAAAA:::::A      M::::::M   M:::::::M   M::::::M  E::::::EEEEEEEEEE        O:::::O     O:::::O      V:::::V V:::::V        E::::::EEEEEEEEEE     R::::R     R:::::R\n");
    wprintf(L"G:::::G        G::::G     A:::::::::::::::::::::A     M::::::M    M:::::M    M::::::M  E:::::E                  O:::::O     O:::::O       V:::::V:::::V         E:::::E               R::::R     R:::::R\n");
    wprintf(L" G:::::G       G::::G    A:::::AAAAAAAAAAAAA:::::A    M::::::M     MMMMM     M::::::M  E:::::E       EEEEEE     O::::::O   O::::::O        V:::::::::V          E:::::E       EEEEEE  R::::R     R:::::R\n");
    wprintf(L"  G:::::GGGGGGGG::::G   A:::::A             A:::::A   M::::::M               M::::::MEE::::::EEEEEEEE:::::E     O:::::::OOO:::::::O         V:::::::V         EE::::::EEEEEEEE:::::ERR:::::R     R:::::R\n");
    wprintf(L"   GG:::::::::::::::G  A:::::A               A:::::A  M::::::M               M::::::ME::::::::::::::::::::E      OO:::::::::::::OO           V:::::V          E::::::::::::::::::::ER::::::R     R:::::R\n");
    wprintf(L"     GGG::::::GGG:::G A:::::A                 A:::::A M::::::M               M::::::ME::::::::::::::::::::E        OO:::::::::OO              V:::V           E::::::::::::::::::::ER::::::R     R:::::R\n");
    wprintf(L"        GGGGGG   GGGGAAAAAAA                   AAAAAAAMMMMMMMM               MMMMMMMMEEEEEEEEEEEEEEEEEEEEEE          OOOOOOOOO                 VVV            EEEEEEEEEEEEEEEEEEEEEERRRRRRRR     RRRRRRR\n");    
    wprintf(L"\n\n\n\n");

    wprintf(L"                                                                                    SELECT AN OPTION:\n\n");
    wprintf(L"                                                                                            [1] - Play Again.\n\n");
    wprintf(L"                                                                                            [2] - Return to the Main Menu.\n\n");
    wprintf(L"                                                                                            [3] - Quit\n\n");

    while (true)
    {
        char playerchoice = getchar();
        if (playerchoice == '1') // Play again
        {
            return 1;
        }
        if (playerchoice == '2') // Return to the menu
        {
            return 2;
        }
        if (playerchoice == '3') // Quit
        {
            return 3;
        }
    }
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
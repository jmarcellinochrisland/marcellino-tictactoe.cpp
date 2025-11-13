#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <cstdlib>

using namespace std;

class Player {
private:
    string name;
    char symbol;
    int wins;
    int losses;
    int draws;

public:
    Player(string n = "", char s = ' ') : name(n), symbol(s), wins(0), losses(0), draws(0) {}

    void setName(string n) { name = n; }
    void setSymbol(char s) { symbol = s; }

    string getName() const { return name; }
    char getSymbol() const { return symbol; }
    int getWins() const { return wins; }
    int getLosses() const { return losses; }
    int getDraws() const { return draws; }

    void addWin() { wins++; }
    void addLoss() { losses++; }
    void addDraw() { draws++; }

    void displayStats() const {
        cout << name << " (" << symbol << ") - Menang: " << wins 
             << " | Kalah: " << losses << " | Seri: " << draws << "\n";
    }
};

class TicTacToe {
private:
    vector<vector<char>> board;
    Player* currentPlayer;
    Player* player1;
    Player* player2;
    int movesCount;
    vector<string> moveHistory;

public:
    TicTacToe(Player* p1, Player* p2) {
        board = vector<vector<char>>(3, vector<char>(3, ' '));
        player1 = p1;
        player2 = p2;
        currentPlayer = p1;
        movesCount = 0;
    }

    void clearScreen() {
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
    }

    void displayHeader() {
        cout << "\n";
        cout << "       TIC TAC TOE - GAME BOARD        \n";
        cout << "\n";
        cout << "Turn: " << currentPlayer->getName() << " [" << currentPlayer->getSymbol() << "]\n";
        cout << "----------------------------------------\n";
    }

    void displayBoard() {
        displayHeader();
        cout << "\n";
        cout << "   0   1   2 \n";
        for (int i = 0; i < 3; ++i) {
            cout << "  -----------\n";
            cout << i << " | ";
            for (int j = 0; j < 3; ++j) {
                cout << board[i][j] << " | ";
            }
            cout << "\n";
        }
        cout << "  -----------\n";
        cout << "\n";
        cout << "Move History:\n";
        for (const string& move : moveHistory) {
            cout << move << "\n";
        }
        cout << "\n";
    }

    bool makeMove(int row, int col) {
        if (row < 0 || row >= 3 || col < 0 || col >= 3 || board[row][col] != ' ') {
            return false;
        }
        board[row][col] = currentPlayer->getSymbol();
        movesCount++;
        moveHistory.push_back(currentPlayer->getName() + " placed " + currentPlayer->getSymbol() + " at (" + to_string(row) + ", " + to_string(col) + ")");
        return true;
    }

    bool checkWin() {
        char sym = currentPlayer->getSymbol();
        for (int i = 0; i < 3; ++i) {
            if ((board[i][0] == sym && board[i][1] == sym && board[i][2] == sym) ||
                (board[0][i] == sym && board[1][i] == sym && board[2][i] == sym)) {
                return true;
            }
        }
        if ((board[0][0] == sym && board[1][1] == sym && board[2][2] == sym) ||
            (board[0][2] == sym && board[1][1] == sym && board[2][0] == sym)) {
            return true;
        }
        return false;
    }

    bool checkDraw() {
        return movesCount == 9;
    }

    void switchPlayer() {
        currentPlayer = (currentPlayer == player1) ? player2 : player1;
    }

    void resetGame() {
        board = vector<vector<char>>(3, vector<char>(3, ' '));
        movesCount = 0;
        moveHistory.clear();
        currentPlayer = player1;
    }

    void play() {
        int row, col;
        while (true) {
            clearScreen();
            displayBoard();
            cout << currentPlayer->getName() << ", masukkan baris dan kolom (0-2) untuk simbol " << currentPlayer->getSymbol() << ": ";
            
            // FIX: Better input validation
            if (!(cin >> row >> col)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Input tidak valid. Masukkan angka 0-2.\n";
                cin.ignore();
                continue;
            }

            if (!makeMove(row, col)) {
                cout << "Gerakan tidak valid. Coba lagi.\n";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cin.get();
                continue;
            }

            if (checkWin()) {
                clearScreen();
                displayBoard();
                cout << "Selamat " << currentPlayer->getName() << "! Anda menang!\n";
                currentPlayer->addWin();
                if (currentPlayer == player1) player2->addLoss();
                else player1->addLoss();
                break;
            }

            if (checkDraw()) {
                clearScreen();
                displayBoard();
                cout << "Permainan berakhir seri!\n";
                player1->addDraw();
                player2->addDraw();
                break;
            }

            switchPlayer();
        }
    }

    void displayFinalStats() {
        cout << "\nStatistik Akhir:\n";
        cout << "=====================================\n";
        player1->displayStats();
        player2->displayStats();
        cout << "=====================================\n";
    }

    void displayCurrentStats() const {
        cout << "\nStatistik Saat Ini:\n";
        cout << "=====================================\n";
        player1->displayStats();
        player2->displayStats();
        cout << "=====================================\n";
    }
};

// FIX: Added main() function and game loop
int main() {
    string name1, name2;
    char playAgain = 'y';

    cout << "   SELAMAT DATANG DI TIC TAC TOE       \n\n";

    cout << "Masukkan nama pemain 1: ";
    getline(cin, name1);
    cout << "Masukkan nama pemain 2: ";
    getline(cin, name2);

    Player player1(name1, 'X');
    Player player2(name2, 'O');

    while (playAgain == 'y' || playAgain == 'Y') {
        TicTacToe game(&player1, &player2);
        game.play();
        game.displayCurrentStats();

        cout << "Ingin bermain lagi? (y/n): ";
        cin >> playAgain;
        cin.ignore();
    }

    cout << "\nTerima kasih telah bermain!\n";
    cout << "Statistik Final:\n";
    player1.displayStats();
    player2.displayStats();

    return 0;
}

#include <iostream>
#include <Windows.h>
#include <fstream>
#include <ctime>
#include <cmath>
using namespace std;
int beads[5][5] = {//RED = 1 =Player1 and BLUE = 2= Player2
	{1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1},
	{1, 1, 0, 2, 2},
	{2, 2, 2, 2, 2},
	{2, 2, 2, 2, 2}
};
int coin_toss() {
	srand(time(0));
	return rand() % 2;
}
void displayboard(int arr[5][5]) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | BACKGROUND_RED | BACKGROUND_GREEN);
	std::cout << "\t\t\t\t\t    Current game board  :  \n";
	SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

	for (int i = 0; i < 5; i++) {
		cout << "\t\t\t\t\t\t";
		for (int j = 0; j < 5; j++) {
			if (arr[i][j] == 0) {
				cout << "o ";
			}
			else if (arr[i][j] == 1) {
				cout << "\033[1;31mo\033[1;0m" << " ";
			}
			else {
				cout << "\033[1;32mo\033[1;0m" << " ";
			}
		}
		cout << endl;
	}
}
bool correctbead(int arr[5][5], int row, int col, int player) {
	return (row >= 0 && row < 5 && col >= 0 && col < 5 && arr[row][col] == player);
}

bool possiblemove(int arr[5][5], int row, int col, int fromrow, int fromcol, int currentplayer) {
	if (row - fromrow > 2 || row - fromrow < -2) return false;
	if (col - fromcol > 2 || col - fromcol < -2) return false;
	if (!(row >= 0 && row < 5 && col >= 0 && col < 5 && arr[row][col] == 0)) return false;
	if (row - fromrow != 0 && col - fromcol != 0 && abs(row - fromrow) != abs(col - fromcol)) return false;
	if (row - fromrow == 2 || col - fromcol == 2 || row - fromrow == -2 || col - fromcol == -2) {
		if (arr[fromrow + (row - fromrow) / 2][fromcol + (col - fromcol) / 2] == currentplayer) return false;
		if (arr[fromrow + (row - fromrow) / 2][fromcol + (col - fromcol) / 2] == 0) return false;
	}
	return true;
}
void movebeads(int arr[5][5], int fromrow, int fromcol, int frow, int fcol) {
	arr[frow][fcol] = arr[fromrow][fromcol];
	arr[fromrow][fromcol] = 0;
}


void safeFile(int currentPlayer, int arr[5][5])
{
	ofstream fout;
	fout.open("12game.txt");
	fout << currentPlayer << endl;
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			fout << arr[i][j] << " ";
		}
		fout << endl;
	}
	fout.close();
}
bool getFile(int& currentPlayer, int arr[5][5])
{
	ifstream fin;
	fin.open("12game.txt");
	if (!fin.is_open()) return false;
	fin >> currentPlayer;
	if (currentPlayer != 1 && currentPlayer != 2) return false;
	fin.ignore();
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			fin >> arr[i][j];
			fin.ignore();
		}
		fin.ignore();
	}
	fin.close();
	return true;
}
bool checkwin(int arr[5][5], int player) {
	for (int i = 0; i < 5; ++i) {
		for (int j = 0; j < 5; ++j) {
			if (arr[i][j] == player) {
				return false; 
			}
		}
	}
	return true; 
}
int main() {
	const int red = 1;
	const int green = 2;
	int e;
	int currentplayer = 0;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | BACKGROUND_GREEN);
	cout << "\t\t\t\t\t  WELCOME TO 12 BEAD GAME  " << endl;
	SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	cout << "Enter 1 if you want to Resume Previous Game and 2 to start a New One :";
	cin >> e;
	if (true && e == 1) {
		if (!getFile(currentplayer, beads)) {
			cout << "Sorry No Previous Record " << endl;
			currentplayer = 0;
		}
	}
	else if (e == 2);
	else
	{
		cout << "Invalid Input !!" << endl;
		return 0;
	}
	if (currentplayer == 0) {
		if (coin_toss() == 0) {
			cout << "Player 1 (RED) has won the toss!!\n";
			currentplayer = red;
		}
		else {
			cout << "Player 2 (GREEN) has won the toss!!\n";
			currentplayer = green;
		}
	}
	displayboard(beads);
	while (true) {
		if (checkwin(beads, red)) {
			cout << "Player 2 (GREEN) wins!" << endl;
			break;
		}
		else if (checkwin(beads, green)) {
			cout << "Player 1 (RED) wins!" << endl;
			break;
		}
		int fromrow, fromcol;
		cout << "It's Player " << currentplayer << "'s turn\n";
		cout << "Enter Row of the bead you want to move (0-4): ";
		cin >> fromrow;
		cout << "Enter Column of the bead you want to move (0-4): ";
		cin >> fromcol;
		if (!correctbead(beads, fromrow, fromcol, currentplayer)) {
			cout << "Wrong bead selection. Try Again .\n";
			continue;
		}
		cout << "Choices for moves are:\n";
		int countChoices = 0;
		for (int i = 0; i < 5; ++i) {
			for (int j = 0; j < 5; ++j) {
				if (possiblemove(beads, i, j, fromrow, fromcol, currentplayer)) {
					cout << "(" << i << "," << j << ") ";
					countChoices++;
				}
			}
		}
		if (countChoices == 0) {
			cout << "There are no move choices for this bead." << endl;
			continue;
		}
		cout << endl;
		int frow, fcol;
		cout << "Enter Row where you want to move the bead: ";
		cin >> frow;
		cout << "Enter Column where you want to move the bead: ";
		cin >> fcol;
		if (!possiblemove(beads, frow, fcol, fromrow, fromcol, currentplayer)) {
			cout << " The selected move is not possible. Try again!!\n";
			continue;
		}
		movebeads(beads, fromrow, fromcol, frow, fcol);
		int opponent;
		if (currentplayer == red) {
			opponent = green;
		}
		else {
			opponent = red;
		}
		int rdiff[] = { -1, -1, 0, 1, 1, 1, 0, -1 };
		int cdiff[] = { 0, 1, 1, 1, 0, -1, -1, -1 };
		for (int i = 0; i < 8; ++i) {
			int newrow = frow + rdiff[i];
			int newcol = fcol + cdiff[i];
			if (newrow >= 0 && newrow < 5 && newcol >= 0 && newcol < 5 && beads[newrow][newcol] == opponent) {
				int nextrow = newrow + rdiff[i];
				int nextcol = newcol + cdiff[i];
				if (nextrow >= 0 && nextrow < 5 && nextcol >= 0 && nextcol < 5 && beads[nextrow][nextcol] == 0) {
					beads[newrow][newcol] = 0;
				}
			}
		}

		safeFile(currentplayer, beads);
		displayboard(beads);

		if (currentplayer == red) {
			currentplayer = green;
		}
		else {
			currentplayer = red;
		}
	}
	return 0;

}

#include <iostream>
#include <math.h>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <Windows.h>

using namespace std;

const int HEIGHT = 20, WIDTH = 60;

// Colors codes
enum ConsoleColor
{
	Black = 0,
	Blue = 1,
	Green = 2,
	Cyan = 3,
	Red = 4,
	Magenta = 5,
	Brown = 6,
	LightGray = 7,
	DarkGray = 8,
	LightBlue = 9,
	LightGreen = 10,
	LightCyan = 11,
	LightRed = 12,
	LightMagenta = 13,
	Yellow = 14,
	White = 15
};

void SetColor(int text, int background)
{
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdOut, (WORD)((background << 4) | text));
}

class Cell {
public:
	int x, y;
	char sym;
	bool isObstacle;
	Cell* cameFrom;
	float f; // Sum of g and h
	float g; // Cost of step
	float h; // Heuristic (Aproximate distance to goal)

	void setState(bool state) {
		isObstacle = state;
		sym = isObstacle ? '*' : '.';
	}

	void calcF() { f = g + h; }
	Cell() {
		setState(false); // false means its'nt an obstacle
	};

	Cell(int X, int Y, bool state = false) :x(X), y(Y) {
		setState(state);
	};

	void setPos(int X, int Y) {
		x = X;
		y = Y;
	};

	bool operator==(Cell obj) {
		return this->x == obj.x && this->y == obj.y;
	}

	vector <Cell> getNeighbors(Cell area[][WIDTH]) {
		vector <Cell> neighbors;

		for (int i = 0; i < HEIGHT; i++)
			for (int j = 0; j < WIDTH; j++) {
				if (abs(area[i][j].x - this->x) <= 1 && abs(area[i][j].y - this->y) <= 1 && !(area[i][j] == *this))
				{
					neighbors.push_back(area[i][j]);
				}
			}
		return neighbors;
	}
};

float distance(Cell start, Cell finish) {
	return sqrt(pow(finish.x - start.x, 2) + pow(finish.y - start.y, 2));
}

int main()
{
	srand(time(NULL));

	int i, j;

	Cell area[HEIGHT][WIDTH];

	for (i = 0; i < HEIGHT; i++)
		for (j = 0; j < WIDTH; j++) {
			area[i][j].setPos(j, i);
			area[i][j].g = 80;
			if ((j == 5) && (i != 15) || ( j == 30) && (i == HEIGHT - 1)) {
				area[i][j].setState(true);
			}
			else {
				area[i][j].setState(false);
			}
		}

	vector <Cell> openSet, closeSet;

	int startX = 0, startY = 0;
	int targetX = WIDTH - 1, targetY = HEIGHT - 1;

	openSet.push_back(Cell(startX, startY));
	openSet[0].g = 0;
	openSet[0].h = distance(openSet[0], area[targetY][targetX]);
	openSet[0].calcF();

	// cout << "G = " << openSet[0].g << endl;
	// cout << "H = " << openSet[0].h << endl;
	// cout << "F = " << openSet[0].f << endl;

	vector <Cell> path; // Path Cells
	int k = 0; // Path cell index

	while (openSet.size() != 0) {
		Cell current;
		int minF = 0;

		for (i = 0; i < openSet.size(); i++)
			if (openSet[minF].f > openSet[i].f) minF = i;

		current = openSet[minF];
		if (current.isObstacle) {
			closeSet.push_back(current);
			vector <Cell>::iterator iter = find(openSet.begin(), openSet.end(), current);
			openSet.erase(iter);
			continue;
		}
		path.push_back(current);

		// cout << "Current Pos. X: " << current.x << " Y: " << current.y << endl;

		if (current == area[targetY][targetX]) {
			cout << "Path found!" << endl;
			break;
		}

		// Removing current cell
		vector <Cell>::iterator iter = find(openSet.begin(), openSet.end(), current);
		openSet.erase(iter);

		vector <Cell> neighbors = current.getNeighbors(area);

		for (i = 0; i < neighbors.size(); i++)
		{
			float g_tmp = current.g + 1;
			if (g_tmp < neighbors[i].g) {
				neighbors[i].cameFrom = &path[k];
				neighbors[i].g = g_tmp;
				neighbors[i].f = g_tmp + distance(neighbors[i], area[targetY][targetX]);

				if (find(openSet.begin(), openSet.end(), neighbors[i]) == openSet.end())
					openSet.push_back(neighbors[i]);
			}

		}

		k++;
		//Sleep(400);
	}

	// cout << openSet.size() << endl;

	for (int i = 0; i < path.size(); i++) {
		area[path[i].y][path[i].x].sym = '+';
	}

	area[startY][startX].sym = 'S';
	area[targetY][targetX].sym = 'F';

	for (i = 0; i < HEIGHT; i++) {
		for (j = 0; j < WIDTH; j++) {
			if (area[i][j].sym == '+') SetColor(Yellow, Black);
			if (area[i][j].sym == 'S') SetColor(LightGreen, Black);
			if (area[i][j].sym == 'F') SetColor(LightRed, Black);
			if (area[i][j].sym == '.') SetColor(DarkGray, Black);
			if (area[i][j].sym == '*') SetColor(Red, Black);
			cout << area[i][j].sym;
		}
		cout << endl;
	}

	SetColor(7, 0);

	return 0;
}

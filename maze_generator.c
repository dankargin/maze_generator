#include <stdio.h>

#include <stdlib.h>

#define _CRT_SECURE_NO_WARNINGS

void display(int** maze, int m, int n) {

	for (int i = 1; i < m - (m % 2 == 1); i++) {

		for (int j = 0; j < n - (n % 2 == 0); j++)

			if (maze[i][j] == 1)

				printf("# ");

			else if (maze[i][j] == 0)

				printf("' ");

			else

				printf("  ");

		printf("\n");

	}

}

int** get_closed_maze(int m, int n) {

	int** maze = (int**)malloc(m * sizeof(int*));

	for (int i = 0; i < m; i++)

		maze[i] = (int*)malloc(n * sizeof(int));

	for (int i = 0; i < m; i++)

		for (int j = 0; j < n; ++j)

			maze[i][j] = 1;

	return maze;

}

int step[4][2] = { {2, 0}, {0, 2}, {-2, 0}, {0, -2} };

int* generate_perm() {

	int* perm = (int*)malloc(4 * sizeof(int));

	for (int i = 0; i < 4; ++i)

		perm[i] = i;

	int j, temp;

	for (int i = 3; i >= 0; i--) {

		j = rand() % (i + 1);

		temp = perm[i];

		perm[i] = perm[j];

		perm[j] = temp;

	}

	return perm;

}

int generate_path(int** maze, int x, int y, int end, int n, int m, int home) {

	if ((x >= n - 1) || (x < 1) || (y < 1))

		return -1;

	if (y >= m - 1) {

		if (x == end) {

			for (int i = y - 2; i < m; ++i)

				maze[i][x] = 0;

			return 1;

		}

		else {

			return -1;

		}

	}

	if (maze[y][x] == 0)

		return -1;

	maze[y - step[home][0] / 2][x - step[home][1] / 2] = 0;

	maze[y][x] = 0;

	int* perm = generate_perm();

	int nx, ny;

	for (int i = 0; i < 4; i++) {

		nx = x + step[perm[i]][1];

		ny = y + step[perm[i]][0];

		if (generate_path(maze, nx, ny, end, n, m, perm[i]) > 0) {

			free(perm);

			return 1;

		}

	}

	free(perm);

	maze[y - step[home][0] / 2][x - step[home][1] / 2] = 1;

	maze[y][x] = 1;

	return -1;

}

int generate_single_deadlock(int** maze, int x, int y, int n, int m, int home) {

	if ((x >= n - 1) || (x < 1) || (y < 1) || (y >= m - 1))

		return -1;

	if ((maze[y][x] == 0) || (maze[y][x] == -1))

		return -1;

	maze[y - step[home][0] / 2][x - step[home][1] / 2] = -1;

	maze[y][x] = -1;

	int* perm = generate_perm();

	int nx, ny;

	for (int i = 0; i < 4; i++) {

		nx = x + step[perm[i]][1];

		ny = y + step[perm[i]][0];

		generate_single_deadlock(maze, nx, ny, n, m, perm[i]);

	}

	free(perm);

	return 1;

}

int generate_deadlocks(int** maze, int x, int y, int end, int n, int m, int home) {

	int nx, ny;

	if ((x == end) && (y >= m - 2))

		return 1;

	for (int i = 0; i < 4; i++) {

		nx = x + step[i][1];

		ny = y + step[i][0];

		generate_single_deadlock(maze, nx, ny, n, m, i);

	}

	for (int i = 0; i < 4; i++) {

		if (i == ((home + 2) % 4)) {

			continue;

		}

		nx = x + step[i][1];

		ny = y + step[i][0];

		if ((nx > 0) && (nx < n) && (ny > 0) && (ny < m))

			if (maze[ny - step[i][0] / 2][nx - step[i][1] / 2] == 0)

				return generate_deadlocks(maze, nx, ny, end, n, m, i);

	}

}

int main() {

	int m, n;

	printf("Enter m:");

	scanf_s("%d", &m);

	if (m < 5) {

		printf("m have to be more than 4");

		return 0;

	}

	printf("Enter n:");

	scanf_s("%d", &n);

	if (n < 5) {

		printf("n have to be more than 4");

		return 0;

	}
	srand(time(NULL));

	unsigned long t = rand();

	srand(t);

	int** maze = get_closed_maze(m, n);

	int begin = 1 + 2 * (rand() % (n / 2 - 1)), end = 1 + 2 * (rand() % (n / 2 - 1));

	maze[0][begin] = 0;

	maze[m - 1][end] = 0;

	int ans = generate_path(maze, begin, 2, end, n, m, 0);

	generate_deadlocks(maze, begin, 2, end, n, m, 0);

	printf("\n");

	display(maze, m, n);

	for (int i = 0; i < m; ++i)

		free(maze[i]);

	free(maze);

	return 0;
}

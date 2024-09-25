#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_HEALTH 3

// Utility function to swap two integers
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Utility function to shuffle an array
void shuffle(int *array, int size) {
    for (int i = 0; i < size; i++) {
        int randIndex = rand() % size;
        swap(&array[i], &array[randIndex]);
    }
}

// Utility function to check if a value is valid in the current cell
int isValid(int size, int board[size][size], int row, int col, int num) {
    for (int x = 0; x < size; x++) {
        if (board[row][x] == num || board[x][col] == num)
            return 0;
    }

    int boxSize = size == 9 ? 3 : 2;
    int startRow = row - row % boxSize;
    int startCol = col - col % boxSize;

    for (int i = 0; i < boxSize; i++)
        for (int j = 0; j < boxSize; j++)
            if (board[i + startRow][j + startCol] == num)
                return 0;

    return 1;
}

// Recursive backtracking function to generate a valid Sudoku board
int fillBoard(int size, int board[size][size], int row, int col) {
    if (row == size - 1 && col == size)
        return 1;

    if (col == size) {
        row++;
        col = 0;
    }

    if (board[row][col] != 0)
        return fillBoard(size, board, row, col + 1);

    for (int num = 1; num <= size; num++) {
        if (isValid(size, board, row, col, num)) {
            board[row][col] = num;

            if (fillBoard(size, board, row, col + 1))
                return 1;
        }

        board[row][col] = 0;
    }

    return 0;
}

// Generate a valid Sudoku board
void generateValidSudoku(int size, int board[size][size]) {
    // Initialize an empty board
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            board[i][j] = 0;

    // Fill the board using the backtracking algorithm
    fillBoard(size, board, 0, 0);
}

// Remove elements to create a puzzle based on difficulty
void removeElements(int size, int board[size][size], int clues) {
    int total_cells = size * size;

    for (int i = 0; i < total_cells - clues; i++) {
        int x = rand() % size;
        int y = rand() % size;
        while (board[x][y] == 0) {
            x = rand() % size;
            y = rand() % size;
        }
        board[x][y] = 0;
    }
}

// Display the board with gridlines and labels
void displayBoard(int size, int board[size][size]) {
    printf("\n    ");
    for (int col = 0; col < size; col++) {
        printf(" %2d ", col + 1);  // Print column numbers
    }
    printf("\n   +");
    for (int col = 0; col < size; col++) {
        printf("---+");  // Print top boundary of cells
    }
    printf("\n");

    for (int row = 0; row < size; row++) {
        printf("%2d |", row + 1);  // Print row numbers
        for (int col = 0; col < size; col++) {
            if (board[row][col] == 0)
                printf(" _ |");  // Empty cell
            else
                printf(" %2d|", board[row][col]);  // Filled cell
        }
        printf("\n   +");
        for (int col = 0; col < size; col++) {
            printf("---+");  // Print bottom boundary of cells
        }
        printf("\n");
    }
}

// Main game logic
// Main game logic
void playSudoku(int size, int board[size][size], int solution[size][size]) {
    int x, y, value, health = MAX_HEALTH;
    while (health > 0) {
        // Display the board before every move
        displayBoard(size, board);

        // Ask for user input
        printf("Enter coordinates (row column) and value (e.g., 1 3 5): ");
        scanf("%d %d %d", &x, &y, &value);
        x--; y--; // Convert to 0-indexed

        // Check if the input is within valid range
        if (x >= 0 && x < size && y >= 0 && y < size) {
            // Check if the current cell is empty
            if (board[x][y] == 0) {
                // Check if the value matches the solution
                if (solution[x][y] == value) {
                    board[x][y] = value;  // Place the value on the board
                    printf("Correct move!\n");
                } else {
                    health--;  // Decrease health for wrong move
                    printf("Wrong move! Health remaining: %d\n", health);
                }
            } else {
                printf("This spot is already filled. Try again.\n");
            }
        } else {
            printf("Invalid coordinates. Try again.\n");
        }

        // After the move, display the updated board
        displayBoard(size, board);

        // Ask if the player wants a clue
        if (health > 0) {
            printf("Do you want a clue? (1 for yes, 0 for no): ");
            int clue;
            scanf("%d", &clue);
            if (clue) {
                // Provide a clue
                int clue_x = rand() % size;
                int clue_y = rand() % size;
                while (board[clue_x][clue_y] != 0) {
                    clue_x = rand() % size;
                    clue_y = rand() % size;
                }
                board[clue_x][clue_y] = solution[clue_x][clue_y];
                printf("Clue given at (%d, %d): %d\n", clue_x + 1, clue_y + 1, solution[clue_x][clue_y]);

                // Display updated board after giving clue
                displayBoard(size, board);
            }
        }
    }

    printf("Game over! You're out of health.\n");
}

void copyBoard(int size, int src[size][size], int dest[size][size]) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            dest[i][j] = src[i][j];
        }
    }
}

int main() {
    srand(time(0));
    int choice;
    printf("Choose Sudoku size: 1. 4x4 2. 9x9: ");
    scanf("%d", &choice);

    if (choice == 1) {
        printf("You chose 4x4 Sudoku.\n");
        int board_4x4[4][4];
        int solution_4x4[4][4];
        generateValidSudoku(4, solution_4x4);
        copyBoard(4, solution_4x4, board_4x4);
        removeElements(4, board_4x4, 8); // Create a puzzle by removing some numbers
        playSudoku(4, board_4x4, solution_4x4);
    } else if (choice == 2) {
        printf("You chose 9x9 Sudoku. Select difficulty: 1. Easy 2. Hard 3. Diabolical: ");
        int difficulty, clues;
        scanf("%d", &difficulty);
        
        if (difficulty == 1) clues = 35;
        else if (difficulty == 2) clues = 30;
        else clues = 25;

        int board_9x9[9][9];
        int solution_9x9[9][9];
        generateValidSudoku(9, solution_9x9);
        copyBoard(9, solution_9x9, board_9x9);
        removeElements(9, board_9x9, clues); // Create a puzzle based on difficulty
        playSudoku(9, board_9x9, solution_9x9);
    } else {
        printf("Invalid choice.\n");
    }

    return 0;
}

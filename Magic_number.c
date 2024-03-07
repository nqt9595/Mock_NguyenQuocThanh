#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAX_NAME_LENGTH 50
#define LEADERBOARD_FILE "leaderboard.txt"
#define LEADERBOARD_SIZE 5

typedef struct {
    char name[MAX_NAME_LENGTH];
    float luckyRatio;
} Player;

void generateMagicNumber(char* magicNumber) {
    srand((unsigned int)time(NULL)); // Seed the random number generator with current time
    int num = rand() % 9000 + 1000; // Ensure a 4-digit number
    sprintf(magicNumber, "%04d", num);
}

void compareNumbers(const char* magicNumber, const char* userGuess, char* result) {
    for (int i = 0; i < 4; i++) {
        result[i] = (magicNumber[i] == userGuess[i]) ? userGuess[i] : '-';
    }
    result[4] = '\0'; // Null-terminate the string
}

void saveToLeaderboard(const char* playerName, float luckyRatio) {
    FILE* file = fopen(LEADERBOARD_FILE, "a"); // Append mode
    if (file != NULL) {
        fprintf(file, "%s %.2f\n", playerName, luckyRatio);
        fclose(file);
    }
}

void loadAndDisplayLeaderboard() {
    Player leaderboard[LEADERBOARD_SIZE] = {0};
    FILE* file = fopen(LEADERBOARD_FILE, "r");
    char line[100];
    int count = 0;

    if (file != NULL) {
        while (fgets(line, sizeof(line), file) != NULL && count < LEADERBOARD_SIZE) {
            sscanf(line, "%49s %f", leaderboard[count].name, &leaderboard[count].luckyRatio);
            count++;
        }
        fclose(file);
    }

    // Sort the loaded results by luckyRatio in descending order
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (leaderboard[i].luckyRatio < leaderboard[j].luckyRatio) {
                Player temp = leaderboard[i];
                leaderboard[i] = leaderboard[j];
                leaderboard[j] = temp;
            }
        }
    }

    printf("\nTop %d Players:\n", LEADERBOARD_SIZE);
    for (int i = 0; i < count; i++) {
        printf("%d. %s - Lucky Ratio: %.2f\n", i + 1, leaderboard[i].name, leaderboard[i].luckyRatio);
    }
}

int main() {
    char playerName[MAX_NAME_LENGTH];
    char magicNumber[5], userGuess[5], result[5];
    int totalGuesses, correctGuesses;
    char continuePlaying;

    printf("Enter your name: ");
    scanf("%49s", playerName);

    do {
        generateMagicNumber(magicNumber);
        printf("A 4-digit number has been generated. Start guessing!\n");

        totalGuesses = 0;
        correctGuesses = 0;
        do {
            printf("Enter your guess: ");
            scanf("%4s", userGuess);
            totalGuesses++;

            compareNumbers(magicNumber, userGuess, result);
            printf("Result: %s\n", result);

            if (strcmp(magicNumber, userGuess) == 0) {
                printf("Congratulations, %s! You've guessed the magic number: %s\n", playerName, magicNumber);
                correctGuesses++;
                float luckyRatio = (float)correctGuesses / totalGuesses;
                printf("%s, your lucky ratio this round is: %.2f\n", playerName, luckyRatio);
                saveToLeaderboard(playerName, luckyRatio);
                break;
            }
        } while (1);

        loadAndDisplayLeaderboard();

        printf("Do you want to continue playing? (y/n): ");
        scanf(" %c", &continuePlaying); // Note the space before %c to consume any leftover newline characters
    } while (continuePlaying == 'y' || continuePlaying == 'Y');

    printf("Thank you for playing!\n");

    return 0;
}

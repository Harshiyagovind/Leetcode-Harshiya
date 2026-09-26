#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* getHint(char* secret, char* guess) {
    int bulls = 0;
    int cows = 0;
    
    // Frequency tracking array for digits '0' through '9'
    int secret_counts[10] = {0};
    int guess_counts[10] = {0};
    
    int len = strlen(secret);
    
    for (int i = 0; i < len; i++) {
        if (secret[i] == guess[i]) {
            // Perfect match in digit and position
            bulls++;
        } else {
            // Mismatch: register frequencies to calculate cows later
            secret_counts[secret[i] - '0']++;
            guess_counts[guess[i] - '0']++;
        }
    }
    
    // The number of cows for each digit is the minimum of its occurrences 
    // in the remaining unmatched sections of secret and guess
    for (int i = 0; i < 10; i++) {
        cows += (secret_counts[i] < guess_counts[i]) ? secret_counts[i] : guess_counts[i];
    }
    
    // Allocate buffer for the result string (e.g., "11A11B\0")
    // 20 characters is more than enough for standard integer ranges
    char* result = (char*)malloc(20 * sizeof(char));
    sprintf(result, "%dA%dB", bulls, cows);
    
    return result;
}
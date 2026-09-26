#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

// Helper function to perform big-integer addition using strings
char* addStrings(const char* num1, const char* num2) {
    int len1 = strlen(num1);
    int len2 = strlen(num2);
    int max_len = len1 > len2 ? len1 : len2;
    
    // Allocate space for the sum string (max_len + 1 for carry + 1 for null-terminator)
    char* res = (char*)malloc((max_len + 2) * sizeof(char));
    int res_idx = 0;
    
    int i = len1 - 1, j = len2 - 1, carry = 0;
    while (i >= 0 || j >= 0 || carry) {
        int sum = carry;
        if (i >= 0) sum += num1[i--] - '0';
        if (j >= 0) sum += num2[j--] - '0';
        carry = sum / 10;
        res[res_idx++] = (sum % 10) + '0';
    }
    res[res_idx] = '\0';
    
    // Reverse the generated result string to get the correct order
    for (int k = 0; k < res_idx / 2; k++) {
        char temp = res[k];
        res[k] = res[res_idx - 1 - k];
        res[res_idx - 1 - k] = temp;
    }
    
    return res;
}

// Helper function to recursively validate the remaining sequence
bool checkSequence(const char* num1, const char* num2, const char* remaining) {
    // If we successfully consumed the entire string, it's a valid additive sequence
    if (strlen(remaining) == 0) {
        return true;
    }
    
    // Calculate the expected next string sum
    char* sum_str = addStrings(num1, num2);
    int sum_len = strlen(sum_str);
    
    // Check if the remaining string starts with our calculated sum
    if (strncmp(remaining, sum_str, sum_len) != 0) {
        free(sum_str);
        return false; // Mismatch, backtrack
    }
    
    // Progress deeper into the string: num2 becomes the new num1, sum_str becomes the new num2
    bool is_valid = checkSequence(num2, sum_str, remaining + sum_len);
    
    free(sum_str);
    return is_valid;
}

bool isAdditiveNumber(char* num) {
    int n = strlen(num);
    if (n < 3) return false;
    
    // Loop through all possible lengths for the first number
    for (int i = 1; i <= n / 2; i++) {
        // Leading zeros check: A number cannot start with '0' unless it is exactly "0"
        if (num[0] == '0' && i > 1) break;
        
        char* num1 = (char*)malloc((i + 1) * sizeof(char));
        strncpy(num1, num, i);
        num1[i] = '\0';
        
        // Loop through all possible lengths for the second number
        // The remaining string length (n - i - j) must be at least as large as max(i, j)
        for (int j = 1; n - i - j >= (i > j ? i : j); j++) {
            // Leading zeros check for the second number
            if (num[i] == '0' && j > 1) break;
            
            char* num2 = (char*)malloc((j + 1) * sizeof(char));
            strncpy(num2, num + i, j);
            num2[j] = '\0';
            
            // Validate if this combination can cleanly exhaust the sequence
            if (checkSequence(num1, num2, num + i + j)) {
                free(num1);
                free(num2);
                return true;
            }
            
            free(num2);
        }
        free(num1);
    }
    
    return false;
}
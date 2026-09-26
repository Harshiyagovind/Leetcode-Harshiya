#include <string.h>
#include <stdbool.h>

// Helper macro to find the minimum of two values
#define MIN(a, b) ((a) < (b) ? (a) : (b))
// Helper macro to find the maximum of two values
#define MAX(a, b) ((a) > (b) ? (a) : (b))

int strongPasswordChecker(char* password) {
    int n = strlen(password);
    
    // 1. Identify missing character types (Lower, Upper, Digit)
    bool has_lower = false, has_upper = false, has_digit = false;
    for (int i = 0; i < n; i++) {
        if (password[i] >= 'a' && password[i] <= 'z') has_lower = true;
        else if (password[i] >= 'A' && password[i] <= 'Z') has_upper = true;
        else if (password[i] >= '0' && password[i] <= '9') has_digit = true;
    }
    
    int missing_types = (has_lower ? 0 : 1) + (has_upper ? 0 : 1) + (has_digit ? 0 : 1);
    
    // --- CASE 1: Password is too short (Length < 6) ---
    if (n < 6) {
        // Insertions can simultaneously fix missing lengths and missing types
        return MAX(6 - n, missing_types);
    }
    
    // Track repeating groups and total replacements needed
    int replaces = 0;
    int one_seq_deletes = 0; // Lengths like 3, 6, 9 (removes 1 character to reduce 1 replacement)
    int two_seq_deletes = 0; // Lengths like 4, 7, 10 (removes 2 characters to reduce 1 replacement)
    
    for (int i = 2; i < n; ) {
        if (password[i] == password[i - 1] && password[i - 1] == password[i - 2]) {
            int len = 2;
            while (i < n && password[i] == password[i - 1]) {
                len++;
                i++;
            }
            replaces += len / 3;
            
            if (len % 3 == 0) {
                one_seq_deletes += 1;
            } else if (len % 3 == 1) {
                two_seq_deletes += 2;
            }
        } else {
            i++;
        }
    }
    
    // --- CASE 2: Password has an acceptable length (6 to 20) ---
    if (n <= 20) {
        // Replacements can simultaneously fix repeating groups and missing types
        return MAX(replaces, missing_types);
    }
    
    // --- CASE 3: Password is too long (Length > 20) ---
    int excess_deletes = n - 20;
    int current_deletes = excess_deletes;
    
    // Priority 1: Use deletions on len % 3 == 0 groups (1 deletion saves 1 replacement)
    int use1 = MIN(current_deletes, one_seq_deletes);
    current_deletes -= use1;
    replaces -= use1;
    
    // Priority 2: Use deletions on len % 3 == 1 groups (2 deletions save 1 replacement)
    int use2 = MIN(current_deletes, two_seq_deletes);
    current_deletes -= use2;
    replaces -= use2 / 2;
    
    // Priority 3: Remaining deletions applied to any group (3 deletions save 1 replacement)
    int use3 = MIN(current_deletes, replaces * 3);
    replaces -= use3 / 3;
    
    // Total steps = structural deletes + max of remaining replacements vs missing types
    return excess_deletes + MAX(replaces, missing_types);
}
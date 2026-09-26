#include <stdlib.h>

/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
int* maxSlidingWindow(int* nums, int numsSize, int k, int* returnSize) {
    if (numsSize == 0 || k == 0) {
        *returnSize = 0;
        return NULL;
    }

    // Number of elements in the output array
    int resSize = numsSize - k + 1;
    int* result = (int*)malloc(resSize * sizeof(int));
    *returnSize = resSize;

    // Deque stores the indices of elements. 
    // Max size of the deque inside a sliding window of size k is k.
    int* deque = (int*)malloc(numsSize * sizeof(int));
    int head = 0; // Front of the deque
    int tail = 0; // Back of the deque

    for (int i = 0; i < numsSize; i++) {
        // 1. Remove elements from the front if they are out of the current window boundary
        if (head < tail && deque[head] <= i - k) {
            head++;
        }

        // 2. Maintain monotonic property: remove elements from the back 
        // that are smaller than the current element nums[i]
        while (head < tail && nums[deque[tail - 1]] <= nums[i]) {
            tail--;
        }

        // 3. Add the current element's index to the back of the deque
        deque[tail++] = i;

        // 4. If the window has reached size k, the maximum element is at the front
        if (i >= k - 1) {
            result[i - k + 1] = nums[deque[head]];
        }
    }

    // Free the auxiliary deque array
    free(deque);
    return result;
}

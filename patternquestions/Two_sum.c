#include <stdio.h>
/*GANDU QUESTIONS */

void twoSum(int* nums, int numsSize, int target, int* returnSize, int* result) {
    for (int i = 0; i < numsSize; i++) {
        for (int j = i + 1; j < numsSize; j++) {
            if (nums[i] + nums[j] == target) {
                result[0] = i;
                result[1] = j;
                *returnSize = 2;
                return;  
            }
        }
    }
    *returnSize = 0;  
}

int main() {
    int nums[100];  
    int i, numsSize, target;

    printf("Enter the size of array: ");
    scanf("%d", &numsSize);

    printf("Enter the values in array: ");
    for (i = 0; i < numsSize; i++) {
        scanf("%d", &nums[i]);
    }

    printf("Enter target sum: ");
    scanf("%d", &target);

    int result[2];
    int returnSize;

    twoSum(nums, numsSize, target, &returnSize, result);

    if (returnSize == 2) {
        printf("Indices: [%d, %d]\n", result[0], result[1]);
    } else {
        printf("No two numbers add up to the target.\n");
    }

    return 0;
}

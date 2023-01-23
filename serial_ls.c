#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <string.h>
#include <sys/time.h>
#include <math.h>

// Maximum random value
const int MAX_RANDOM_VAL = 65535;

// Structure for the linked list node
struct node {
    int data;
    struct node *next;
};


// Check if the given value is node of the linked list or not
// Linked list is sorted in ascending order
// Return 1 if the value is node of the linked list and 0 otherwise
int Member (int value, struct node* head_p) {
    struct node* curr_p = head_p;

    while (curr_p != NULL && curr_p->data < value)
        curr_p = curr_p->next;

    if (curr_p == NULL || curr_p->data > value) {
        return 0;
    } else {
        return 1;
    }
}

// Insert the given value to the linked list
// Linked list is sorted in ascending order
// Return 1 if the value is inserted and 0 otherwise
int Insert(int value, struct node** head_pp) {
    struct node* curr_p = *head_pp;
    struct node* pred_p = NULL;
    struct node* temp_p;

    while (curr_p != NULL && curr_p->data < value) {
        pred_p = curr_p;
        curr_p = curr_p->next;
    }

    if (curr_p == NULL || curr_p->data > value) {
        temp_p = malloc(sizeof(struct node)); // Allocate memory for the new node
        temp_p->data = value; // Assign the value to the new node
        temp_p->next = curr_p; // Link the new node to the linked list
        if (pred_p == NULL)
            *head_pp = temp_p; // If the predecessor is NULL, the new node is the head of the linked list
        else
            pred_p->next = temp_p; // Link the predecessor to the new node
        return 1;
    } else {
        return 0;
    }
}

// Delete the given value from the linked list
// Linked list is sorted in ascending order
// Return 1 if the value is deleted and 0 otherwise
int Delete(int value, struct node** head_pp) {
    struct node* curr_p = *head_pp;
    struct node* pred_p = NULL;

    while (curr_p != NULL && curr_p->data < value) {
        pred_p = curr_p;
        curr_p = curr_p->next;
    }

    if (curr_p != NULL && curr_p->data == value) {
        if (pred_p == NULL) {
            // If the predecessor is NULL, the current node is the head of the linked list, then set the head to the 
            // successor of the current node
            *head_pp = curr_p->next; 
            free(curr_p); // Free the memory of the current node
        } else {
            pred_p->next = curr_p->next; // Link the predecessor to the successor of the current node
            free(curr_p); // Free the memory of the current node
        }
        return 1;
    } else {
        return 0;
    }
}

// Generate random value
int RandomValue() {
    srand(time(0));
    return rand() % MAX_RANDOM_VAL;
}

// Populate the linked list
// Linked list is sorted in ascending order
void Populate(struct node** head_pp, int n) {
    for (int i = 0; i < n; i++) {
        Insert(i, head_pp);
    }
}

// Create operation array
// m = total number of operations
// prob_member = probability of member operations
// prob_insert = probability of insert operations
// prob_delete = probability of delete operations
void CreateOperationArray(int* operation_array, int m, float prob_member, float prob_insert, float prob_delete) {

    int m_member = round(m * prob_member);
    int m_insert = round(m * prob_insert);
    int m_delete = round(m * prob_delete);

    int i;
    for (i = 0; i < m_member; i++) {
        operation_array[i] = 0;
    }
    for (i = m_member; i < m_member + m_insert; i++) {
        operation_array[i] = 1;
    }
    for (i = m_member + m_insert; i < m_member + m_insert + m_delete; i++) {
        operation_array[i] = 2;
    }

    // Shuffle the operation array
    srand(time(0));
    for (i = 0; i < m_member + m_insert + m_delete; i++) {
        // int j = i + rand() / (RAND_MAX / (m_member + m_insert + m_delete - i) + 1);
        int j = rand() % (i + 1);
        int t = operation_array[j];
        operation_array[j] = operation_array[i];
        operation_array[i] = t;
    }
}

// Run serial linked list operations
double RunSerialLinkedListOperations(int n, int m, float prob_member, float prob_insert, float prob_delete) {
    struct node* head_p = NULL;
    Populate(&head_p, n);

    int* operation_array = malloc(m * sizeof(int));
    CreateOperationArray(operation_array, m, prob_member, prob_insert, prob_delete);

    clock_t start, end;
    double time_taken;

    start = clock();

    int i;
    for (i = 0; i < m; i++) {
        int value = RandomValue();
        if (operation_array[i] == 0) {
            Member(value, head_p);
        } else if (operation_array[i] == 1) {
            Insert(value, &head_p);
        } else {
            Delete(value, &head_p);
        }
    }

    end = clock();
    time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;

    return time_taken;
}

int main() {
    int n = 1000;
    int m = 10000;
    int caseNum;
    int testRuns;

    float prob_member;
    float prob_insert;
    float prob_delete;

    printf("Enter the case number: ");
    scanf("%d", &caseNum);
    printf("Enter the number of test runs required: ");
    scanf("%d", &testRuns);

    switch (caseNum) {
        case 1:
            prob_member = 0.99;
            prob_insert = 0.005;
            prob_delete = 0.005;
            break;
        case 2:
            prob_member = 0.90;
            prob_insert = 0.05;
            prob_delete = 0.05;
            break;
        case 3:
            prob_member = 0.50;
            prob_insert = 0.25;
            prob_delete = 0.25;
            break;
        default:
            printf("Invalid case number");
            return 0;
    }

    double *results = malloc(testRuns * sizeof(double));

    for (int i = 0; i < testRuns; i++) {
        results[i] = RunSerialLinkedListOperations(n, m, prob_member, prob_insert, prob_delete);
    }

    double sum = 0;
    for (int i = 0; i < testRuns; i++) {
        sum += results[i];
    }
    double mean = sum / testRuns;
    printf("Mean: %f\n", mean);

    double sd = 0;
    for (int i = 0; i < testRuns; i++) {
        sd += pow(results[i] - mean, 2);
    }
    sd = sqrt(sd / testRuns);
    printf("Standard Deviation: %f\n", sd);
    return 0;
}

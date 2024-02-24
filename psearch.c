#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#define MAX_LINE_LENGTH 1024

typedef struct {
    char *keyword;
    char *filename;
} ThreadArg;

typedef struct ListNode {
    char *filename;
    int lineNum;
    char line[MAX_LINE_LENGTH];
    struct ListNode *next;
} ListNode;

ListNode *head = NULL;
sem_t listSem;

void *searchFile(void *arg);
void addToList(char *filename, int lineNum, char *line);
void writeOutput(const char *outputFilename);

int main(int argc, char *argv[]) {
    if (argc < 5) {
        fprintf(stderr, "Usage: %s <keyword> <n> <inputfile1> ... <inputfileN> <outputfile>\n", argv[0]);
        return 1;
    }
    
    struct timespec start, end;
    long long duration;

    char *keyword = argv[1];
    int numFiles = atoi(argv[2]);
    char *outputFilename = argv[3 + numFiles];

    pthread_t threads[numFiles];
    ThreadArg args[numFiles];

    sem_init(&listSem, 0, 1);
    
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int i = 0; i < numFiles; i++) {
        args[i].keyword = keyword;
        args[i].filename = argv[3 + i];
        pthread_create(&threads[i], NULL, searchFile, &args[i]); //pthread
    }

    for (int i = 0; i < numFiles; i++) {
        pthread_join(threads[i], NULL);
    }
    
    clock_gettime(CLOCK_MONOTONIC, &end);

    writeOutput(outputFilename);
    sem_destroy(&listSem);
    
    duration = (end.tv_sec - start.tv_sec) * 1000000000LL + (end.tv_nsec - start.tv_nsec);
    printf("Execution Time: %lld nanoseconds\n", duration);

    return 0;
}

void *searchFile(void *arg) {
    ThreadArg *threadArg = (ThreadArg *)arg;
    char *keyword = threadArg->keyword;
    char *filename = threadArg->filename;

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return NULL;
    }

    char line[MAX_LINE_LENGTH];
    int lineNum = 0;

    while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
        lineNum++;
        if (strstr(line, keyword) != NULL) {
            sem_wait(&listSem);
            addToList(filename, lineNum, line);
            sem_post(&listSem);
        }
    }

    fclose(file);
    return NULL;
}

void addToList(char *filename, int lineNum, char *line) {
    ListNode *newNode = malloc(sizeof(ListNode));
    if (newNode == NULL) {
        perror("Failed to allocate memory for list node");
        exit(1);
    }

    newNode->filename = strdup(filename);
    newNode->lineNum = lineNum;
    strncpy(newNode->line, line, MAX_LINE_LENGTH);
    newNode->next = NULL;

    if (head == NULL) {
        head = newNode;
    } else {
        ListNode *current = head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }
}

void writeOutput(const char *outputFilename) {
    FILE *outputFile = fopen(outputFilename, "w");
    if (outputFile == NULL) {
        perror("Error opening output file");
        exit(1);
    }

    ListNode *current = head;
    while (current != NULL) {
        fprintf(outputFile, "%s, %d: %s", current->filename, current->lineNum, current->line);
        ListNode *temp = current;
        current = current->next;
        free(temp->filename);
        free(temp);
    }

    fclose(outputFile);
}

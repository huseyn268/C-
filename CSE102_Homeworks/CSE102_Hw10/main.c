/*
 * CSE 102 Spring 2025 - Assignment 10
 * Question Answering (QA) System
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024
#define MAX_PAIRS 1000
#define DEFAULT_MATCH_THRESHOLD 80

/* Data structures */
typedef struct {
    int* embedding;
    char* text;
} Sentence;

typedef struct {
    Sentence question;
    Sentence answer;
    int* knowledge_entry;
} QAPair;

/* Function prototypes */
int read_metadata(FILE* file, int* num_pairs, int* emb_size);
int read_sentence(FILE* file, Sentence* sentence, int emb_size);
int read_qa_pairs(FILE* file, QAPair* pairs, int num_pairs, int emb_size);
float calculate_matching_score(int* emb1, int* emb2, int emb_size);
void query_knowledge_base(QAPair* pairs, int num_pairs, int* query_embedding, int emb_size, int threshold);
int* text_to_embedding(char* text, int emb_size);
void cleanup(QAPair* pairs, int num_pairs);

/*
 * Main function
 */
int main() {
    FILE* file;
    QAPair* pairs;
    int num_pairs, emb_size;
    char input[MAX_LINE_LENGTH];
    int* query_embedding;
    int match_threshold = DEFAULT_MATCH_THRESHOLD;
    int i;
    
    /* Open embeddings.txt file */
    file = fopen("embeddings.txt", "r");
    if (file == NULL) {
        printf("Could not open file: embeddings.txt\n");
        return 1;
    }
    
    /* Read metadata */
    if (!read_metadata(file, &num_pairs, &emb_size)) {
        printf("Could not read metadata!\n");
        fclose(file);
        return 1;
    }
    
    printf("Metadata: %d question-answer pairs, %d embedding size\n", num_pairs, emb_size);
    
    /* Allocate memory for QA pairs */
    pairs = (QAPair*)malloc(num_pairs * sizeof(QAPair));
    if (pairs == NULL) {
        printf("Memory allocation error!\n");
        fclose(file);
        return 1;
    }
    
    /* Read QA pairs and create knowledge base */
    if (!read_qa_pairs(file, pairs, num_pairs, emb_size)) {
        printf("Could not read QA pairs!\n");
        fclose(file);
        free(pairs);
        return 1;
    }
    
    fclose(file);
    
    printf("Knowledge base successfully created.\n");
    
    /* User interface */
    while (1) {
        printf("\nEnter a question (type 'q' to quit): ");
        fgets(input, MAX_LINE_LENGTH, stdin);
        
        /* Remove newline character */
        input[strcspn(input, "\n")] = 0;
        
        if (strcmp(input, "q") == 0) {
            break;
        }
        
        /* Convert question to embedding */
        printf("Query: %s\n", input);
        query_embedding = text_to_embedding(input, emb_size);
        if (query_embedding == NULL) {
            printf("Could not create embedding!\n");
            continue;
        }
        
        /* Query the knowledge base */
        query_knowledge_base(pairs, num_pairs, query_embedding, emb_size, match_threshold);
        
        free(query_embedding);
    }
    
    /* Clean up */
    cleanup(pairs, num_pairs);
    free(pairs);
    
    return 0;
}

/*
 * Read metadata from file
 */
int read_metadata(FILE* file, int* num_pairs, int* emb_size) {
    char line[MAX_LINE_LENGTH];
    
    /* Read first line (number of pairs) */
    if (fgets(line, MAX_LINE_LENGTH, file) == NULL) {
        return 0;
    }
    sscanf(line, "%d", num_pairs);
    
    /* Read second line (embedding size) */
    if (fgets(line, MAX_LINE_LENGTH, file) == NULL) {
        return 0;
    }
    sscanf(line, "%d", emb_size);
    
    return 1;
}

/*
 * Read a sentence embedding from file
 */
int read_sentence(FILE* file, Sentence* sentence, int emb_size) {
    char line[MAX_LINE_LENGTH];
    char* token;
    int i = 0;
    
    /* Read text */
    if (fgets(line, MAX_LINE_LENGTH, file) == NULL) {
        return 0;
    }
    /* Remove trailing newline */
    line[strcspn(line, "\n")] = 0;
    
    /* Allocate memory for text and copy */
    sentence->text = (char*)malloc((strlen(line) + 1) * sizeof(char));
    if (sentence->text == NULL) {
        return 0;
    }
    strcpy(sentence->text, line);
    
    /* Read embedding */
    if (fgets(line, MAX_LINE_LENGTH, file) == NULL) {
        free(sentence->text);
        return 0;
    }
    
    sentence->embedding = (int*)malloc(emb_size * sizeof(int));
    if (sentence->embedding == NULL) {
        free(sentence->text);
        return 0;
    }
    
    token = strtok(line, " ");
    while (token != NULL && i < emb_size) {
        sentence->embedding[i++] = atoi(token);
        token = strtok(NULL, " ");
    }
    
    return 1;
}

/*
 * Read all QA pairs from file
 */
int read_qa_pairs(FILE* file, QAPair* pairs, int num_pairs, int emb_size) {
    int i;
    int j;
    
    for (i = 0; i < num_pairs; i++) {
        /* Read question sentence */
        if (!read_sentence(file, &pairs[i].question, emb_size)) {
            return 0;
        }
        
        /* Read answer sentence */
        if (!read_sentence(file, &pairs[i].answer, emb_size)) {
            return 0;
        }
        
        /* Create knowledge base entry (Question XOR Answer) */
        pairs[i].knowledge_entry = (int*)malloc(emb_size * sizeof(int));
        if (pairs[i].knowledge_entry == NULL) {
            return 0;
        }
        
        for (j = 0; j < emb_size; j++) {
            pairs[i].knowledge_entry[j] = pairs[i].question.embedding[j] ^ pairs[i].answer.embedding[j];
        }
    }
    
    return 1;
}

/*
 * Calculate matching score between two embeddings
 */
float calculate_matching_score(int* emb1, int* emb2, int emb_size) {
    int xor_result;
    int ones_count = 0;
    int i, j;
    
    for (i = 0; i < emb_size; i++) {
        xor_result = emb1[i] ^ emb2[i];
        
        /* Count 1s in XOR result */
        for (j = 0; j < 8 * sizeof(int); j++) {
            if ((xor_result & (1 << j)) != 0) {
                ones_count++;
            }
        }
    }
    
    return 100.0 * (1.0 - (float)ones_count / emb_size);
}

/*
 * Query the knowledge base with a question
 */
void query_knowledge_base(QAPair* pairs, int num_pairs, int* query_embedding, int emb_size, int threshold) {
    int* retrieved_answer;
    int found = 0;
    int i, j, k;
    float score;
    
    retrieved_answer = (int*)malloc(emb_size * sizeof(int));
    if (retrieved_answer == NULL) {
        printf("Memory allocation error!\n");
        return;
    }
    
    printf("\n--- Query Results ---\n");
    
    for (i = 0; i < num_pairs; i++) {
        /* Knowledge base XOR query => answer */
        for (j = 0; j < emb_size; j++) {
            retrieved_answer[j] = pairs[i].knowledge_entry[j] ^ query_embedding[j];
        }
        
        /* Find answers with high matching scores */
        for (k = 0; k < num_pairs; k++) {
            score = calculate_matching_score(retrieved_answer, pairs[k].answer.embedding, emb_size);
            
            if (score >= threshold) {
                printf("Matching Score: %.2f%% - Answer: %s\n", score, pairs[k].answer.text);
                found = 1;
            }
        }
    }
    
    if (!found) {
        printf("No answer found with sufficiently high matching score.\n");
    }
    
    free(retrieved_answer);
}

/*
 * Convert text input to embedding
 * Note: This function should be implemented according to embedding scheme from Assignment 9
 * For simplicity, we're creating a random embedding here
 */
int* text_to_embedding(char* text, int emb_size) {
    int* embedding;
    int i;
    
    embedding = (int*)malloc(emb_size * sizeof(int));
    if (embedding == NULL) {
        return NULL;
    }
    
    for (i = 0; i < emb_size; i++) {
        embedding[i] = rand() % 2;  /* 0 or 1 */
    }
    
    return embedding;
}

/*
 * Free allocated memory
 */
void cleanup(QAPair* pairs, int num_pairs) {
    int i;
    
    for (i = 0; i < num_pairs; i++) {
        free(pairs[i].question.embedding);
        free(pairs[i].question.text);
        free(pairs[i].answer.embedding);
        free(pairs[i].answer.text);
        free(pairs[i].knowledge_entry);
    }
}

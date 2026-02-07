#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHUNK 9
#define MAX_LINE 80
#define MAX_STACK 3
#define MAX_BUFFER 1024
#define MAX_INPUT_LEN 20

typedef enum { RGB, BGR, GRAY } Color;

typedef struct {
    char* items[MAX_STACK];
    int top;
} Stack;

int max_operation(int* chunk, int* corresponding_chunk_part, int chunk_size);
int avg_operation(int* chunk, int* corresponding_chunk_part, int chunk_size);
void push(Stack* stack, const char* op);
char* pop(Stack* stack);
int read_line_of_ints(FILE* file, int* array, int max_size);
void get_corresponding_chunk(int* line, int start_index, int chunk_size, int* chunk_part);
void compute_line_results(int* results, int* line, int line_length, int* chunk, int chunk_size, 
                         int (*operation)(int*, int*, int));
void write_results(FILE* output, int** results, int result_count, Color color);
void print_output_file(int* chunk, int* line2, int chunk_size, int (*operation)(int*, int*, int), int line_size, int* line3, int* line4, Color color, FILE* output_file);
void free_resources(int** result_lines, int line_count, char** operations, int op_count);

int main() {
    FILE *input_file, *output_file;
    Stack op_stack = {.top = -1};
    int chunk[MAX_CHUNK];
    int line2[MAX_LINE], line3[MAX_LINE], line4[MAX_LINE];
    int chunk_size = 0, line2_size = 0, line3_size = 0, line4_size = 0;
    char input_buffer[MAX_INPUT_LEN];
    Color color_mode = GRAY;
    int i, j;
    
    input_file = fopen("input.txt", "r");
    if (!input_file) {
        printf("Error: Cannot open input file\n");
        return 1;
    }
    
    output_file = fopen("output.txt", "w");
    if (!output_file) {
        printf("Error: Cannot open output file\n");
        fclose(input_file);
        return 1;
    }
    
    chunk_size = read_line_of_ints(input_file, chunk, MAX_CHUNK);
    
    if (chunk_size != 3 && chunk_size != 6 && chunk_size != 9) {
        fprintf(output_file, "Error: Invalid chunk size. Expected 3, 6, or 9 but got %d\n", chunk_size);
        fclose(input_file);
        fclose(output_file);
        return 1;
    }
    
    line2_size = read_line_of_ints(input_file, line2, MAX_LINE);
    line3_size = read_line_of_ints(input_file, line3, MAX_LINE);
    line4_size = read_line_of_ints(input_file, line4, MAX_LINE);
    
    if ((line2_size != 20 && line2_size != 50 && line2_size != 80) || 
        line2_size != line3_size || line3_size != line4_size) {
        fprintf(output_file, "Error: Invalid line lengths. Expected 20, 50, or 80 with all lines equal\n");
        fclose(input_file);
        fclose(output_file);
        return 1;
    }
    
    for (i = 0; i < MAX_STACK; i++) {
        printf("Enter operation %d (max/avg): ", i + 1);
        if (fgets(input_buffer, MAX_INPUT_LEN, stdin) == NULL) break;
        
        input_buffer[strcspn(input_buffer, "\n")] = 0;
        
        if (strcmp(input_buffer, "max") == 0 || strcmp(input_buffer, "avg") == 0) {
            push(&op_stack, input_buffer);
        } else {
            printf("Invalid operation. Please enter 'max' or 'avg'\n");
            i--;
        }
    }
    
    do {
        printf("Enter color mode (rgb/bgr/gray): ");
        if (fgets(input_buffer, MAX_INPUT_LEN, stdin) == NULL) break;
        
        input_buffer[strcspn(input_buffer, "\n")] = 0;
        
        if (strcmp(input_buffer, "rgb") == 0) color_mode = RGB;
        else if (strcmp(input_buffer, "bgr") == 0) color_mode = BGR;
        else if (strcmp(input_buffer, "gray") == 0) color_mode = GRAY;
        else {
            printf("Invalid color mode. Please enter 'rgb', 'bgr', or 'gray'\n");
            continue;
        }
        break;
    } while (1);
    
    {
        int op_count = op_stack.top + 1;
        char* operations[MAX_STACK];
        
        for (i = op_count - 1; i >= 0; i--) {
            operations[i] = pop(&op_stack);
        }
        
        for (i = 0; i < op_count; i++) {
            int (*operation_func)(int*, int*, int);
            
            if (strcmp(operations[i], "max") == 0) {
                operation_func = max_operation;
            } else {
                operation_func = avg_operation;
            }
            
            if (i > 0) {
                fprintf(output_file, "\n\n"); 
            }
            
            print_output_file(chunk, line2, chunk_size, operation_func, line2_size, line3, line4, color_mode, output_file);
            
            free(operations[i]);
        }
    }
    
    printf("Processing complete. Results written to output.txt\n");
    
    fclose(input_file);
    fclose(output_file);
    return 0;
}

int max_operation(int* chunk, int* corresponding_chunk_part, int chunk_size) {
    int i, product, max_value;
    
    if (chunk_size <= 0) return 0;
    
    max_value = chunk[0] * corresponding_chunk_part[0];
    
    for (i = 1; i < chunk_size; i++) {
        product = chunk[i] * corresponding_chunk_part[i];
        if (product > max_value) {
            max_value = product;
        }
    }
    
    return max_value;
}

int avg_operation(int* chunk, int* corresponding_chunk_part, int chunk_size) {
    int i, sum = 0;
    
    if (chunk_size <= 0) return 0;
    
    for (i = 0; i < chunk_size; i++) {
        sum += chunk[i] * corresponding_chunk_part[i];
    }
    
    return sum / chunk_size;
}

void push(Stack* stack, const char* op) {
    if (stack->top < MAX_STACK - 1) {
        char* op_copy = (char*)malloc(strlen(op) + 1);
        if (op_copy) {
            strcpy(op_copy, op);
            stack->items[++(stack->top)] = op_copy;
        }
    }
}

char* pop(Stack* stack) {
    if (stack->top >= 0) {
        return stack->items[(stack->top)--];
    }
    return NULL;
}

int read_line_of_ints(FILE* file, int* array, int max_size) {
    char buffer[MAX_BUFFER];
    int count = 0;
    char* token;
    
    do {
        if (fgets(buffer, MAX_BUFFER, file) == NULL) {
            return 0;
        }
    } while (buffer[0] == '\n' || buffer[0] == '\r');
    
    token = strtok(buffer, " \t\n\r");
    while (token != NULL && count < max_size) {
        array[count++] = atoi(token);
        token = strtok(NULL, " \t\n\r");
    }
    
    return count;
}

void get_corresponding_chunk(int* line, int start_index, int chunk_size, int* chunk_part) {
    int i;
    for (i = 0; i < chunk_size; i++) {
        chunk_part[i] = line[start_index + i];
    }
}

void compute_line_results(int* results, int* line, int line_length, int* chunk, int chunk_size, 
                         int (*operation)(int*, int*, int)) {
    int i;
    int* chunk_part = (int*)malloc(chunk_size * sizeof(int));
    
    if (!chunk_part) {
        printf("Error: Memory allocation failed\n");
        return;
    }
    
    for (i = 0; i <= line_length - chunk_size; i++) {
        get_corresponding_chunk(line, i, chunk_size, chunk_part);
        results[i] = operation(chunk, chunk_part, chunk_size);
    }
    
    free(chunk_part);
}

void write_results(FILE* output, int** results, int result_count, Color color) {
    int i, j;
    
    if (color == GRAY) {
        for (i = 0; i < 3; i++) {
            fprintf(output, "[");
            for (j = 0; j < result_count; j++) {
                if (j > 0) fprintf(output, ",");
                fprintf(output, "%d", results[i][j]);
            }
            fprintf(output, "]");
            
            if (i < 2) fprintf(output, "\n");
        }
    } else {
        for (i = 0; i < result_count; i++) {
            if (i > 0) fprintf(output, ",");
            
            fprintf(output, "[");
            if (color == RGB) {
                fprintf(output, "%d,%d,%d", results[0][i], results[1][i], results[2][i]);
            } else {
                fprintf(output, "%d,%d,%d", results[2][i], results[1][i], results[0][i]);
            }
            fprintf(output, "]");
        }
    }
}

void print_output_file(int* chunk, int* line2, int chunk_size, int (*operation)(int*, int*, int), int line_size, int* line3, int* line4, Color color, FILE* output_file) {
    int result_count = line_size - chunk_size + 1;
    int* result_lines[3] = {NULL, NULL, NULL};
    int i, j;
    
    for (j = 0; j < 3; j++) {
        result_lines[j] = (int*)malloc(result_count * sizeof(int));
        if (!result_lines[j]) {
            fprintf(output_file, "Error: Memory allocation failed\n");
            for (i = 0; i < j; i++) {
                free(result_lines[i]);
            }
            return;
        }
    }
    
    compute_line_results(result_lines[0], line2, line_size, chunk, chunk_size, operation);
    compute_line_results(result_lines[1], line3, line_size, chunk, chunk_size, operation);
    compute_line_results(result_lines[2], line4, line_size, chunk, chunk_size, operation);
    
    write_results(output_file, result_lines, result_count, color);
    
    for (j = 0; j < 3; j++) {
        free(result_lines[j]);
    }
}

void free_resources(int** result_lines, int line_count, char** operations, int op_count) {
    int i;
    
    for (i = 0; i < line_count; i++) {
        if (result_lines[i]) {
            free(result_lines[i]);
        }
    }
    
    for (i = 0; i < op_count; i++) {
        if (operations[i]) {
            free(operations[i]);
        }
    }
}

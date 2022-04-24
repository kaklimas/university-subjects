#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdbool.h>
#include <fcntl.h>
#define BUFFOR 256

int task_number;

struct set_object{
    char* key;           // key of object
    char* value;         // value of object
};

struct command_dictionary{
    struct set_object* objects;
    int size;
};

struct tasks{
    char* task_name; //skladnik 1
    char** commands;   // cat
    char*** arguments; // cat
    int commands_no; // char** commands lenght
};

struct command_dictionary getDict(const char* filePath){
    // open file, init things
    FILE * file = fopen(filePath, "r");
    int file_size, i=0, tasks=0, a=0, k=0;
    struct command_dictionary dict;

    // get size of file
    fseek(file, 0, SEEK_END);   
    file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // create buffer and copy data to it
    char* buffer = calloc(file_size, sizeof(char));
    fread(buffer, sizeof(char), file_size, file);

    // find number of tasks, create dict
    while (buffer[i]){
        if (buffer[i] == '\n'){
            tasks++;
            if (buffer[i+1] == '\n'){
                break;
            }
        }
        i++;
    }
    dict.objects = calloc(tasks, sizeof(struct set_object));
    dict.size = tasks;
    i = 0;
    while (buffer[i]){
        k = 0;
        dict.objects[a].key = calloc(BUFFOR, sizeof(char));
        dict.objects[a].value = calloc(BUFFOR, sizeof(char));

        while (buffer[i] != ' '){    
            dict.objects[a].key[k] = buffer[i];
            i++;
            k++;
        }
        
        while (buffer[i] == ' ' || buffer[i] == '='){
            i++;
        }

        k = 0;
        while (buffer[i] != '\n'){
            dict.objects[a].value[k] = buffer[i];
            k++;
            i++;
        }
        i++;
        a++;
        
        if (buffer[i] == '\n'){
            break;
        }
        
    }
    

    free(buffer);
    fclose(file);
    return dict;
}

void freeDict(struct command_dictionary dict){
    for (int i = 0; i < dict.size; i++){
        free(dict.objects[i].key);
        free(dict.objects[i].value);
    }
    free(dict.objects);
}

void freeTaskList(char** tasks_list){
    for (int i = 0; i < task_number; i++){
        free(tasks_list[i]);
    }
    free(tasks_list);
}

char* getDictValue(char* key, struct command_dictionary d){
    for (int i = 0; i < d.size; i++){
        if (strcmp(key, d.objects[i].key) == 0){
            return d.objects[i].value;
        }
    }
    return NULL;
}

char** getTasksList(const char* filePath){
    FILE * file = fopen(filePath, "r");
    int file_size, i=0, tasks=0, a=0, k=0, x=0;
    struct command_dictionary dict;
    struct command_dictionary d = getDict(filePath);

    // get size of file
    fseek(file, 0, SEEK_END);   
    file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // create buffer and copy data to it
    char* buffer = calloc(file_size, sizeof(char));
    fread(buffer, sizeof(char), file_size, file);

    while (buffer[i]){
        if (buffer[i] == '\n' && buffer[i+1] == '\n'){
            break;
        }
        i++;
    }
    i += 2;
    a = i;
    while (buffer[a]){
        if (buffer[a] == '\n'){
            tasks++;
        }
        a++;
    }
    tasks++;
    task_number = tasks;
    
    char** tasks_list = calloc(tasks, sizeof(char*));
    
    for (x = 0; x < tasks; x++){
        tasks_list[x] = calloc(BUFFOR, sizeof(char));
    }
    x = 0;

    while (buffer[i]){
        while (buffer[i] && buffer[i] != '\n'){
            int k = 0;
            char* task_name = calloc(BUFFOR, sizeof(char));

            while (buffer[i] && buffer[i] != ' ' && buffer[i] != '\n'){
                task_name[k] = buffer[i];
                k++;
                i++;
            }
            char* command = getDictValue(task_name, d);
            
            if (tasks_list[x][0] != '\0'){
                strcat(tasks_list[x], " | ");
            }

            strcat(tasks_list[x], command);
            while (buffer[i] && buffer[i] == ' ' || buffer[i] == '|' && buffer[i] != '\n'){
                i++;
            }
            free(task_name);
        }
        i++;
        x++;
    }
    free(file);
    freeDict(d);
    return tasks_list;    
}

char*** getCommandsList(char* command_line, int command_number){
    
    // init main table
    char*** commmands_list = calloc(command_number, sizeof(char**));
    for (int i = 0; i < command_number; i++){
        commmands_list[i] = calloc(BUFFOR,sizeof(char*));
        for (int j = 0; j < BUFFOR; j++){
            commmands_list[i][j] = calloc(BUFFOR, sizeof(char));
            commmands_list[i][j] = "."; 
        }
    }

    int i = 0, j = 0;

    // init helpful table
    char** single_commands = calloc(command_number, sizeof(char*));
    for (i = 0; i < command_number; i++){
        single_commands[i] = calloc(BUFFOR, sizeof(char));
    }
    i = 0;
    char *single_command = strtok(command_line, "|");    
    while (single_command != NULL) {
            single_commands[i] = single_command;
            single_command = strtok(NULL, "|");
            i++;
        }
    for (i = 0; i < command_number; i++){
        char* single_argument = strtok(single_commands[i], " ");
        j = 0;
        while (single_argument != NULL){
            commmands_list[i][j] = single_argument;
            single_argument = strtok(NULL, " ");
            j++;
        }
    }
    free(single_commands);

    for (i = 0; i < command_number; i++){
        j = 0;
        while (commmands_list[i][j] != "."){
            j++;
        }
    }
    return commmands_list;
}

void freeCommandsList(char*** commands_list, int command_number){
    for (int i = 0; i < command_number; i++){
        free(commands_list[i]);
    }
    free(commands_list);
}

void executeLine(char* command_line){
    int command_number = 1, i = 0, j=0, args_no=0;
    int pipes[2][2];
    char *cmds[BUFFOR];


    while (command_line[i]){
        if (command_line[i] == '|'){
            command_number++;
        }
        i++;
    }
    char*** commands_to_execute = getCommandsList(command_line, command_number);    

    for (i = 0; i < command_number; i++){
        
        if (i > 0){
            close(pipes[i%2][0]);
            close(pipes[i%2][1]);
        }
        if (pipe(pipes[i%2]) == -1){
            printf("Error!\n");
            exit(1);
        }
        pid_t child = fork();
        if (child == 0){
            if (i != command_number - 1){
                close(pipes[i%2][0]);
                dup2(pipes[i%2][1], STDOUT_FILENO);   
            }
            if (i != 0){
                close(pipes[(i + 1)%2][1]);
                dup2(pipes[(i+1)%2][0], STDIN_FILENO);
            }
    
            args_no = 0;
            j = 0;
            while (commands_to_execute[i][j] != "."){
                j++;
                args_no++;
            }
            j = 0;
            char *argv[args_no + 1];

            while (commands_to_execute[i][j] != "."){
                argv[j] = commands_to_execute[i][j];
                j++;
            }
            argv[j] = NULL;
            execvp(argv[0], argv);
           
            exit(0);
        }
    }
    close(pipes[i%2][0]);
    close(pipes[i%2][1]);
    
    while (wait(NULL) != 1 && errno != ECHILD);
    freeCommandsList(commands_to_execute, command_number);
    exit(0);
}

int main(int argc, char** argv){
    if (argc != 2){
        printf("Input file path with commands!\n");
        return 1;
    }
    char** tasks_list = getTasksList(argv[1]);


    for (int i = 0; i < task_number; i++){
        pid_t pid = fork();
        if (pid == 0){
            executeLine(tasks_list[i]);
        }
    }
    while (wait(NULL) != 1 && errno != ECHILD);

    freeTaskList(tasks_list);
}


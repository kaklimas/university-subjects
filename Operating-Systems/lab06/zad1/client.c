#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include "header.h"

#define MAXMSG 4098
int CLIENT_KEY;
bool task_up = false;
bool QUIT = true;
int id = 0;

struct user{
    int id;
    key_t user_key;
    int que_id;
    bool active;
};

struct msg{
    long type;
    struct user client;
    char content[MAXMSG];
};


void send_end_mess(){
    struct user usr = {.id = id};
    struct msg snd_msg = {.type=STOP, .client=usr};
    
    int server_id = msgget((key_t)SERVER_KEY, 0666);
    int client_id = msgget((key_t)(CLIENT_KEY), 0666);
    msgsnd(server_id, (void*)&snd_msg, MAXMSG, 0);
    msgctl(client_id, IPC_RMID, NULL);
    printf("Exiting application...\n");
    QUIT = !QUIT;
    exit(0);
}

void setup_sigaction(int signalIdx, int flags, void (*function)(int, siginfo_t*, void*)){
    static struct sigaction act;
    act.sa_sigaction = function;
    act.sa_flags = flags;
    sigemptyset(&act.sa_mask);

    sigaction(signalIdx, &act, NULL);
}
void signal_handler(int sig, siginfo_t *info, void* ucontext){
    if (QUIT){
        send_end_mess();
        printf("\nQuiting chat...\n");
        exit(0);
    } else {
        QUIT = !QUIT;
        printf("\nEnding write...\n");
    }
}
void command_handler(int sig, siginfo_t *info, void* ucontext){
    task_up = !task_up;
}


int main(int argc, char *argv[]) {
    CLIENT_KEY = CLIENT_BASIC_KEY;

    int server_id = msgget((key_t)SERVER_KEY, 0666);
    int client_id = msgget((key_t)(CLIENT_KEY), 0666 | IPC_CREAT);
    msgctl(client_id, IPC_RMID, NULL);
    client_id = msgget((key_t)CLIENT_KEY, 0666 | IPC_CREAT);

    if (server_id == -1){
        printf("Cannot get server id!\n");
        exit(1);
    }
    if (client_id == -1){
        printf("Cannot get client id!\n");
        exit(1);
    }
    struct user client = {.id = 0, .user_key = (key_t)CLIENT_KEY, .active=true, .que_id=client_id};
    struct msg snd_msg = {.type=INIT, .content="INIT", .client=client};
    struct msg rcv_msg;
    
    char choice[5];

    //setup CTRL+C
    setup_sigaction(SIGINT, SA_SIGINFO, signal_handler);
    setup_sigaction(SIGTSTP, SA_SIGINFO, command_handler);

    // send init request to server
    msgsnd(server_id, (void*)&snd_msg, MAXMSG, 0);
    

    // wait for init response
    if (msgrcv(client_id, (void*)&rcv_msg, MAXMSG, INIT, 0) > 0){
            msgctl(client_id, IPC_RMID, NULL);
            client_id = msgget((key_t)(rcv_msg.client.id + 1), 0666);
            client = rcv_msg.client;
            client.que_id = client_id;
            id = client.id;
            snd_msg.client = client;
        }

    printf("Hello new user! Server gives you unique ID: %d | You're client que KEY: %d\n", client.id, client_id);
    printf("\n\n\nThanks to the chat you can interact with other users by pressing one of following options:\n");
    printf("%s", OPTIONS);
    while (1){
        // check if user has waiting messages
        if (msgrcv(client_id, (void*)&rcv_msg, MAXMSG, 0, IPC_NOWAIT) > 0){
            if (rcv_msg.type == STOP){
                send_end_mess();
            } else if (rcv_msg.type == TO_ONE || rcv_msg.type == TO_ALL){
                printf("%s\n", rcv_msg.content);
            } 
        }
        
        
        

        // refreshing ?        


        // check if user send command
        if (task_up){
            printf("\n----------------------------------------------------\nCommand: ");
            if (scanf("%s", choice) > 0){
            QUIT = false;
            int number = atoi(choice);
            switch (atoi(choice))
            {
            case 1:
                send_end_mess();
                
                break;
            case 2:
                QUIT = true;
                snd_msg.type=LIST;
                msgsnd(server_id, (void*)&snd_msg, MAXMSG, 0);
                printf("Listing all users...\n");
                
                if (msgrcv(client_id, (void*)&rcv_msg, MAXMSG, LIST, 0) > 0){
                    printf("%s\n", rcv_msg.content);
                    break;
                }
        
                break;
            case 3:
                QUIT = false;
                printf("I need some information...\n");
                struct msg mess = {.client=client, .content = "",.type=TO_ONE};
                int id;
                char message[MAXMSG] = "";
                char tmp_mess[MAXSINGLEWORD]; 

                printf("Users ID: ");
                scanf("%d",&id);
                printf("Message: ");
                while (!QUIT){
                    if ((scanf("%s", tmp_mess)) > 0){
                        strcat(message, tmp_mess);
                        strcat(message, " ");
                    }
                }
                sprintf(mess.content, "%d: %s\n", id, message);    
                msgsnd(server_id, (void*)&mess, MAXMSG, 0);

                break;
            case 4:
                QUIT = false;
                printf("I need some information...\n");
                char m[MAXMSG] = "";
                char tmp_m[MAXSINGLEWORD]; 
            
                printf("Message: ");
                while (!QUIT){
                    if ((scanf("%s", tmp_m)) > 0){
                        strcat(m, tmp_m);
                        strcat(m, " ");
                    }
                    
                }

                break;
            default:
                //printf("Wrong option. Try again!\n");
                break;
            }
        }
        task_up = false;
        }
        
        
    }

       
}
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

bool QUIT = false;

struct db{
    struct user clients[MAX_USR_NO];
    int free_ind;
};

// functions to handle user's commmands
void save_command(struct msg message){
    FILE * f = fopen("./server_logs.txt", "a");
    if (!f){
        printf("Cannot open log file!\n");
        exit(1);
    }
    time_t now = time(NULL);
    
    switch (message.type)
    {
    case 1:
        fprintf(f, "\nMessage type: INIT | User ID: %d | Time: %s", message.client.id, ctime(&now));
        break;
    case 2:
        fprintf(f, "\nMessage type: STOP | User ID: %d | Time: %s", message.client.id, ctime(&now));
        break;
    case 3:
        fprintf(f, "\nMessage type: LIST | User ID: %d | Time: %s", message.client.id, ctime(&now));
        break;
    case 4:
        fprintf(f, "\nMessage type: TO_ALL | User ID: %d | Time: %s", message.client.id, ctime(&now));
        break;
    case 5:
        fprintf(f, "\nMessage type: TO_ONE | User ID: %d | Time: %s", message.client.id, ctime(&now));
        break;
    default:
        break;
    }
    fclose(f);
}


void server_run(bool run){
    FILE *f = fopen("./server_logs.txt", "a");
    if (!f){
        printf("Cannot open file!\n");
        exit(1);
    }
    time_t now = time(NULL);
    char *s_time = ctime(&now);

    if (run){
        fprintf(f, "\n--- Start of server working: %s\n", s_time);
    } else {
        fprintf(f, "\n--- End of server working: %s\n", s_time);
    }
    fclose(f);
}

struct db init_user(struct user new_user, struct db users_db){
    
    new_user.id = users_db.free_ind;    
    

    struct msg msg_back = {.type=1, .client=new_user, .content="INIT"};

    msgsnd(new_user.que_id, (void*)&msg_back, MAXMSG, 0);


    int new_user_id = msgget((key_t) (new_user.id+ 1), 0666 | IPC_CREAT);
    new_user.que_id = new_user_id;

    users_db.clients[users_db.free_ind] = new_user;
    users_db.free_ind++;
    save_command(msg_back);
    return users_db;
}

struct db delete_user(struct user user, struct db users_db){
    users_db.clients[user.id].active = false;
    return users_db;
}

void setup_sigaction(int signalIdx, int flags, void (*function)(int, siginfo_t*, void*)){
    static struct sigaction act;
    act.sa_sigaction = function;
    act.sa_flags = flags;
    sigemptyset(&act.sa_mask);

    sigaction(signalIdx, &act, NULL);
}

void list_users(struct user user, struct db users_db){
    char feedback[MAXMSG];
    struct msg msg_back = {.type=LIST, .client=user, .content=""};
    for (int i = 0; i < MAX_USR_NO; i++){
        if (users_db.clients[i].active){
            char buffer[20];
            sprintf(buffer, "User of ID: %d\n", users_db.clients[i].id);
            strcat(msg_back.content, buffer);
        }
    }

    msgsnd(user.que_id, (void*)&msg_back, MAXMSG, 0);    
}

void send_to_one(struct msg rcv_msg, struct db db){
    int client_id, ind=0, i=0;
    char buffer[10];
    char message[1000];

    while (rcv_msg.content[ind] && rcv_msg.content[ind]!=':'){
        buffer[ind] = rcv_msg.content[ind];
        ind++;
    }
    ind += 2;
    char* p = &rcv_msg.content[ind];

    client_id = atoi(buffer);
    struct msg msg_back = {.content="", .type=TO_ONE};
    sprintf(msg_back.content, "\n<user %d> %s", rcv_msg.client.id, p);

    msgsnd(db.clients[client_id].que_id, (void*)&msg_back, MAXMSG, 0); 

}

void end_chat(struct db db){
    struct msg end_mess = {.type=STOP};
    for (int i = 0; i < MAX_USR_NO; i++){
        if (db.clients[i].active){
            msgsnd(db.clients[i].que_id, (void*)&end_mess, MAXMSG, 0);
        }
    }
    printf("Ending work of server...\n");
}

void send_to_all(struct msg rcv_msg, struct db db){
    int client_id, i=0;
    
    rcv_msg.type = TO_ALL;
    for (i = 0; i < MAX_USR_NO; i++){
        if (db.clients[i].active && db.clients[i].id != rcv_msg.client.id){
            msgsnd(db.clients[i].que_id, (void*)&rcv_msg, MAXMSG, 0); 
        }
    }

}

void handler(int sig, siginfo_t *info, void* ucontext){
    QUIT = true;
}

int main(int argc, char *argv[]) {
    struct msg rcv_msg;
    int server_id = msgget((key_t)SERVER_KEY, 0666 | IPC_CREAT);    
    setup_sigaction(SIGINT, SA_SIGINFO, handler);
    msgctl(server_id, IPC_RMID, NULL);
    server_id = msgget((key_t)SERVER_KEY, 0666 | IPC_CREAT);

    struct db db = {.free_ind=0};
    if (server_id == -1){
        printf("Cannot get server id!\n");
        exit(1);
    }
    printf("-----------------------------------------------------------\nSERVER IS RUNNING...\n");
    server_run(true);
    while (1){
        if (QUIT){
            end_chat(db);
            msgctl(server_id, IPC_RMID, NULL);
            server_run(false);
            exit(0);
        }
        if (msgrcv(server_id, (void*)&rcv_msg, MAXMSG, 0, 0) > 0){
            if (rcv_msg.type == INIT){
                printf("Received: INIT message.\n");
                db = init_user(rcv_msg.client, db);

            } else if (rcv_msg.type == STOP){
                printf("Received: STOP message from user %d.\n", rcv_msg.client.id);
                db = delete_user(rcv_msg.client, db);
                save_command(rcv_msg);

            } else if (rcv_msg.type == LIST){
                printf("Received: LIST message from user %d.\n", rcv_msg.client.id);
                list_users(rcv_msg.client, db);
                save_command(rcv_msg);
            } else if (rcv_msg.type == TO_ONE){
                printf("Received: TO_ONE message from user %d.\n", rcv_msg.client.id);
                send_to_one(rcv_msg, db);
                save_command(rcv_msg);

            } else if (rcv_msg.type == TO_ALL){
                printf("Received: TO_ALL message from user %d.\n", rcv_msg.client.id);
                send_to_all(rcv_msg, db);
                save_command(rcv_msg);
            }
                           
        }
        
    }
    


}
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
#define SERVER_KEY 12345
#define MAXMSG 4098


// init call always 

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
struct db{
    struct user clients[MAX_USR_NO];
    int free_ind;
};

// functions to handle user's commmands
struct db init_user(struct user new_user, struct db users_db){
    
    new_user.id = users_db.free_ind;    
    

    struct msg msg_back = {.type=1, .client=new_user, .content="INIT"};

    msgsnd(new_user.que_id, (void*)&msg_back, MAXMSG, 0);


    int new_user_id = msgget((key_t) (new_user.id+ 1), 0666 | IPC_CREAT);
    new_user.que_id = new_user_id;

    users_db.clients[users_db.free_ind] = new_user;
    users_db.free_ind++;
    
    // int clients_que_id = msgget((key_t)new_user.user_key, 0666);    
    return users_db;
}

struct db delete_user(struct user user, struct db users_db){
    printf("ID TO DELETE: %d\n", user.id);
    users_db.clients[user.id].active = false;
    return users_db;
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

    printf("Currently logged users: \n%s\n", msg_back.content);
    // int clients_que_id = msgget((key_t)user.user_key, 0666);
    //printf("User KEY: %d", clients_que_id);
    printf("SENDING TO: %d\n", user.que_id);
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
    // while (rcv_msg.content[ind]){
    //     message[i] = rcv_msg.content[ind];
    //     printf("%c", message[i]);  
    //     ind++;
    // }
    client_id = atoi(buffer);
    struct msg msg_back = {.content="", .type=TO_ONE};
    sprintf(msg_back.content, "\nUser of ID: %d, sent you a message:\n%s\n", rcv_msg.client.id, p);

    msgsnd(db.clients[client_id].que_id, (void*)&msg_back, MAXMSG, 0); 

}

int main(int argc, char *argv[]) {
    struct msg rcv_msg;
    int server_id = msgget((key_t)SERVER_KEY, 0666 | IPC_CREAT);    
    msgctl(server_id, IPC_RMID, NULL);
    server_id = msgget((key_t)SERVER_KEY, 0666 | IPC_CREAT);

    struct db db = {.free_ind=0};
    if (server_id == -1){
        printf("Cannot get server id!\n");
        exit(1);
    }
    while (1){
        if (msgrcv(server_id, (void*)&rcv_msg, MAXMSG, 0, 0) > 0){
            if (rcv_msg.type == INIT){
                printf("Received: INIT message.\n");
                db = init_user(rcv_msg.client, db);

            } else if (rcv_msg.type == STOP){
                printf("Received: STOP message!\n");
                db = delete_user(rcv_msg.client, db);
                printf("--%d--\n", rcv_msg.client.id);


            } else if (rcv_msg.type == LIST){
                printf("Received: LIST message!\n");
                list_users(rcv_msg.client, db);

            } else if (rcv_msg.type == TO_ONE){
                printf("Received: TO_ONE message!\n");
                send_to_one(rcv_msg, db);

            } else if (rcv_msg.type == TO_ALL){
                printf("Received: TOALL message!\n");
            }
                           
        }
        
    }
    


}
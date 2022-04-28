#ifndef HEADER_H_
#define HEADER_H_

#define QUEUE_NAME  "./server_que"
#define MAX_SIZE    1024

// messages types
#define INIT 1
#define STOP 2
#define LIST 3
#define TO_ALL 5
#define TO_ONE 4

int CLIENT_BASIC_KEY = 11111;

const int clients_id[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};


#define SERVER_KEY 12345

// max size of message-content size
#define MSGMAX 4096
#define MAXSINGLEWORD 100
// max number of clients that can be connected with server
#define MAX_USR_NO 30

#define OPTIONS "\n---------------------------------------------------------------------------------------------------------------------------\n1. STOP - exit chat\n2. LIST - list all currently logged users\n3. 2ONE - send message to currently logged user\n4. 2ALL - send message to all currently logged users\n\n                                +++++TIPS+++++\nIf u want to stop writting (when using 3/4 opt) press ENTER to confirm, then CTRL+C. \nYou can end using chat by press double CTRL+C when typing (in 3/4 opt) or once when you are not typing.\n---------------------------------------------------------------------------------------------------------------------------\n\n"


#endif
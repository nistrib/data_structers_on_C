// functions.h
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_USERS 10000
#define NAME_LENGTH 50
#define EMAIL_DOMAIN_LENGTH 50
#define MAX_MESSAGE_LENGTH 256
#define MAX_MESSAGES 50
#define MAX_POST_LENGTH 256
#define MAX_FEED_POSTS 20


typedef struct {
    int user_id;
    char name[NAME_LENGTH];
    char email[EMAIL_DOMAIN_LENGTH];
    struct FriendNode* friends;
    struct Message* messages;
} User;

typedef struct FriendNode {
    User* user;
    struct FriendNode* next;
} FriendNode;

typedef struct UserNode {
    User* user;
    struct UserNode* next;
} UserNode;

typedef struct Message{
    int message_id;
    User* sender;
    User* receiver;
    char content[MAX_MESSAGE_LENGTH];
    struct Message* prev;
    struct Message* next;
} Message;

typedef struct Chat{
    Message* head;
    Message* tail;
    int size;
} Chat;

typedef struct ChatNode {
    Chat chat;
    struct ChatNode* next;
} ChatNode;

typedef struct LikeNode{
    User* user;
    struct LikeNode* next;
} LikeNode;

typedef struct Post {
    int post_id;
    User* user;
    char* content;
    int likes;
    LikeNode* liked_by;
    struct Post* next;
} Post;

void testingParser(int arg1, char* arg2);

int is_name_or_email_taken(const char* name, const char* email);
User* create_user(const char* name, const char* email); // int user_id is auto-generated to be unique
void add_friend(User* user1, User* user2); // users user1 and user2 are now friends
void delete_friend(User* user1, User* user2); // users user1 and user2 are no longer friends
void delete_user(User* user); // user is deleted
void print_users(); // prints all user names in ascending order
void change_user_name(User* user, char* new_name);
void change_user_email(User* user, char* new_email);
void print_friends(User* user); // prints user's friends in ascending order
User* search_user_by_name(const char* name);
User* search_user_by_email(const char* email);
User** mutual_friends(User* user1, User* user2); // returns an array of pointers to the mutual friends
void print_mutual_friends(User** friends); // prints mutual friends' user names in acsending order


int are_friends(User* user1, User* user2);
Message* create_message(User* sender, User* receiver, const char* content); // int message_id is auto-generated to be unique
void print_message(Message* message); // Function to print a single message
void display_chat(User* user1, User* user2); // print messages in FIFO
Chat* get_chat(User* user1, User* user2);

Post* new_post(User* user, const char* content); // post id is auto-generated to be unique
void add_like(Post* post, User* user); // user is the individual who liked the post
void display_feed(User* user1);

#endif

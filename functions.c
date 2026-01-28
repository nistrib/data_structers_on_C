#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"

// Utility function to test parser
void testingParser(int arg1, char *arg2) {
    printf("The parser was called with arguments: %d and %s\n", arg1, arg2);
}


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
Message* create_message(User* sender, User* receiver, const char* content); // int message_id is auto-generated to be unique
void print_message(Message* message);
void display_chat(User* user1, User* user2); // print messages in FIFO
Post* new_post(User* user, const char* content); // post id is auto-generated to be unique
void add_like(Post* post, User* user); // user is the individual who liked the post
void display_feed(User* user1);

//// WRITE FUNCTIONS BELOW

static UserNode* user_table[MAX_USERS] = {NULL}; // Hash table for users
static Post* posts = NULL;
static ChatNode* chat_list_head = NULL;

static int user_count = 0;
static int message_count = 0;
static int post_count = 0;

// Hash key for users generation
static int hash(const char* key) {
    unsigned long hash_value = 0;
    while (*key) {
        hash_value = (hash_value * 2) + *key++;
    }
    return hash_value % MAX_USERS;
}

// Function to check if a name or email is taken
int is_name_or_email_taken(const char* name, const char* email) {
    for (int i = 0; i < MAX_USERS; i++) {
        UserNode* current = user_table[i];
        while (current) {
            if ((name && strcmp(current->user->name, name) == 0) || (email && strcmp(current->user->email, email) == 0)) {
                return 1;
            }
            current = current->next;
        }
    }
    return 0;
}

// Create a new user
User* create_user(const char* name, const char* email) {
    if (user_count >= MAX_USERS) {
        return NULL;
    }

    if (is_name_or_email_taken(name, email)) {return NULL;}

    User* new_user = (User*)malloc(sizeof(User));
    if (!new_user) {return NULL;}

    new_user->user_id = user_count++;
    strncpy(new_user->name, name, NAME_LENGTH - 1);
    new_user->name[NAME_LENGTH - 1] = '\0';
    strncpy(new_user->email, email, EMAIL_DOMAIN_LENGTH - 1);
    new_user->email[EMAIL_DOMAIN_LENGTH - 1] = '\0';
    new_user->friends = NULL;

    int index = hash(name);
    UserNode* new_node = (UserNode*)malloc(sizeof(UserNode));
    if (!new_node) {
        free(new_user);
        return NULL;
    }
    new_node->user = new_user;
    new_node->next = user_table[index];
    user_table[index] = new_node;

    return new_user;
}

// Search user by name
User* search_user_by_name(const char* name) {
    int index = hash(name);
    UserNode* current = user_table[index];
    while (current) {
        if (strcmp(current->user->name, name) == 0) {
            return current->user;
        }
        current = current->next;
    }
    return NULL;
}

// Search user by email
User* search_user_by_email(const char* email) {
    for (int i = 0; i < MAX_USERS; i++) {
        UserNode* current = user_table[i];
        while (current) {
            if (strcmp(current->user->email, email) == 0) {
                return current->user;
            }
            current = current->next;
        }
    }
    return NULL;
}

// Delete a user
void delete_user(User* user) {
    if (!user) {return;}

    // Remove user from friends' lists
    for (int i = 0; i < MAX_USERS; i++) {
        UserNode* node = user_table[i];
        while (node) {
            delete_friend(node->user, user);
            node = node->next;
        }
    }

    int index = hash(user->name);
    UserNode** current = &user_table[index];
    while (*current) {
        if ((*current)->user == user) {
            UserNode* temp = *current;
            *current = (*current)->next;

            // Free user's friends
            FriendNode* friend_current = user->friends;
            while (friend_current) {
                FriendNode* temp_friend = friend_current;
                friend_current = friend_current->next;
                free(temp_friend);
            }

            free(temp->user);
            free(temp);
            return;
        }
        current = &((*current)->next);
    }

    printf("User not found in the hash table.\n");
}

// Print all users in ascending order
void print_users() {
    char* names[MAX_USERS];
    int count = 0;

    for (int i = 0; i < MAX_USERS; i++) {
        UserNode* current = user_table[i];
        while (current) {
            names[count++] = current->user->name;
            current = current->next;
        }
    }

    if (count == 0) {
        printf("\n");
        return;
    }

    // Sort names
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (strcmp(names[i], names[j]) > 0) {
                char* temp = names[i];
                names[i] = names[j];
                names[j] = temp;
            }
        }
    }

    // Print names
    for (int i = 0; i < count; i++) {
        printf("%s", names[i]);
        if (i < count - 1) printf(",");
    }
    printf("\n");
}

// Change user's name
void change_user_name(User* user, char* new_name) {
    if (!user || !new_name || is_name_or_email_taken(new_name, NULL)) {return;}

    // Remove user from hash table
    int old_index = hash(user->name);
    UserNode** current = &user_table[old_index];
    while (*current) {
        if ((*current)->user == user) {
            UserNode* temp = *current;
            *current = (*current)->next;
            free(temp);
            break;
        }
        current = &((*current)->next);
    }

    // Update name
    strncpy(user->name, new_name, NAME_LENGTH - 1);
    user->name[NAME_LENGTH - 1] = '\0';

    // Re-insert user into hash table
    int new_index = hash(new_name);
    UserNode* new_node = (UserNode*)malloc(sizeof(UserNode));
    new_node->user = user;
    new_node->next = user_table[new_index];
    user_table[new_index] = new_node;
}

// Change user's email
void change_user_email(User* user, char* new_email) {
    if (!user || !new_email || is_name_or_email_taken(NULL, new_email)) {return;}

    strncpy(user->email, new_email, EMAIL_DOMAIN_LENGTH - 1);
    user->email[EMAIL_DOMAIN_LENGTH - 1] = '\0';
}

// Add a friend
void add_friend(User* user1, User* user2) {
    if (!user1 || !user2) {return;}

    if (user1 == user2) {return;}

    // Check if they are already friends
    FriendNode* current = user1->friends;
    while (current) {
        if (current->user == user2) {return;}
        current = current->next;
    }

    // Add user2 to user1's friends list
    FriendNode* new_friend1 = (FriendNode*)malloc(sizeof(FriendNode));
    new_friend1->user = user2;
    new_friend1->next = user1->friends;
    user1->friends = new_friend1;

    // Add user1 to user2's friends list
    FriendNode* new_friend2 = (FriendNode*)malloc(sizeof(FriendNode));
    new_friend2->user = user1;
    new_friend2->next = user2->friends;
    user2->friends = new_friend2;
}

// Delete a friend
void delete_friend(User* user1, User* user2) {
    if (!user1 || !user2) {
        return;
    }

    // Remove user2 from user1's friends list
    FriendNode** current = &(user1->friends);
    while (*current) {
        if ((*current)->user == user2) {
            FriendNode* temp = *current;
            *current = (*current)->next;
            free(temp);
            break;
        }
        current = &((*current)->next);
    }

    // Remove user1 from user2's friends list
    current = &(user2->friends);
    while (*current) {
        if ((*current)->user == user1) {
            FriendNode* temp = *current;
            *current = (*current)->next;
            free(temp);
            break;
        }
        current = &((*current)->next);
    }
}

// Print a user's friends in ascending order
void print_friends(User* user) {
    if (!user) {return;}

    // Collect friends' names
    char* names[MAX_USERS];
    int count = 0;
    FriendNode* current = user->friends;
    while (current && count < MAX_USERS) {
        if (current->user) {
            names[count++] = current->user->name;
        }
        current = current->next;
    }

    // Check if has no friends
    if (count == 0) {return;}

    // Sort names
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (strcmp(names[i], names[j]) > 0) {
                char* temp = names[i];
                names[i] = names[j];
                names[j] = temp;
            }
        }
    }

    // Print names without spaces after commas
    for (int i = 0; i < count; i++) {
        printf("%s", names[i]);
        if (i < count - 1) printf(",");
    }

    printf("\n");
}

// Mutual friends
User** mutual_friends(User* user1, User* user2) {
    if (!user1 || !user2) {
        return NULL;
    }

    // Collect user1's friends
    FriendNode* current1 = user1->friends;
    User* friends1[MAX_USERS];
    int count1 = 0;
    while (current1 && count1 < MAX_USERS) {
        friends1[count1++] = current1->user;
        current1 = current1->next;
    }

    // Collect user2's friends
    FriendNode* current2 = user2->friends;
    User* friends2[MAX_USERS];
    int count2 = 0;
    while (current2 && count2 < MAX_USERS) {
        friends2[count2++] = current2->user;
        current2 = current2->next;
    }

    // Find mutual friends
    User** mutual = (User**)malloc(sizeof(User*) * (count1 > count2 ? count2 + 1 : count1 + 1)); // +1 for NULL termination
    int mutual_count = 0;

    for (int i = 0; i < count1; i++) {
        for (int j = 0; j < count2; j++) {
            if (friends1[i] == friends2[j]) {
                mutual[mutual_count++] = friends1[i];
                break;
            }
        }
    }

    // Mark the end with NULL
    mutual[mutual_count] = NULL;

    return mutual;
}

// Print mutual friends
void print_mutual_friends(User** friends) {

    // Check if no mutual friends
    if (!friends || !friends[0]) {return;}

    // Collect names
    char* names[MAX_USERS];
    int count = 0;
    for (int i = 0; friends[i]; i++) {
        names[count++] = friends[i]->name;
    }

    if (count == 0) {
        printf("No mutual friends.\n");
        return;
    }

    // Sort names
    for (int i = 0; i < count -1; i++) {
        for (int j = i +1; j < count; j++) {
            if (strcmp(names[i], names[j]) > 0) {
                char* temp = names[i];
                names[i] = names[j];
                names[j] = temp;
            }
        }
    }

    // Printing mutual friends
    printf("");
    for (int i = 0; i < count; i++) {
        printf("%s", names[i]);
        if (i < count -1) {
            printf(",");
        }
    }
    printf("\n");

    free(friends);
}

// Helper to find or create a chat
Chat* get_chat(User* user1, User* user2) {
    ChatNode* current = chat_list_head;

    // Search for an existing chat
    while (current) {
        if ((current->chat.head &&
             ((current->chat.head->sender == user1 && current->chat.head->receiver == user2) ||
              (current->chat.head->sender == user2 && current->chat.head->receiver == user1)))) {
            return &current->chat;
        }
        current = current->next;
    }

    ChatNode* new_chat_node = malloc(sizeof(ChatNode));
    if (!new_chat_node) {return NULL; // Return NULL to indicate failure
    }


    new_chat_node->chat.head = NULL;
    new_chat_node->chat.tail = NULL;
    new_chat_node->chat.size = 0;
    new_chat_node->next = chat_list_head;
    chat_list_head = new_chat_node;

    // Add the new chat node to the list
    chat_list_head = new_chat_node;
    return &new_chat_node->chat;
}

// Check if user1 & user2 are friends
int are_friends(User* user1, User* user2) {
    FriendNode* current = user1->friends;
    while (current) {
        if (current->user == user2) {
            return 1;
        }
        current = current->next;
    }
    return 0;
}

// Create message
Message* create_message(User* sender, User* receiver, const char* content){

    // Check if user1 and user2 can send messages
    if (!are_friends(sender, receiver)) {
        return NULL;
    }
    // Check message length
    if (strlen(content) >= MAX_MESSAGE_LENGTH) {
        return NULL;
    }


    Chat* chat = get_chat(sender, receiver);
    Message* new_message = malloc(sizeof(Message));
    new_message->message_id = message_count++;
    new_message->sender = sender;
    new_message->receiver = receiver;
    strncpy(new_message->content, content, MAX_MESSAGE_LENGTH);
    new_message->next = NULL;

    // Add message to chat
    if (!chat->head) {
        chat->head = chat->tail = new_message;
        new_message->prev = NULL;
    } else {
        chat->tail->next = new_message;
        new_message->prev = chat->tail;
        chat->tail = new_message;
    }
    chat->size++;

    // Enforce MAX_MESSAGES
    if (chat->size > MAX_MESSAGES) {
        Message* to_delete = chat->head;
        chat->head = chat->head->next;
        if (chat->head) {
            chat->head->prev = NULL;
        }
        free(to_delete);
        chat->size--;
    }
    return new_message;
}

// Print a message
void print_message(Message* message) {
    if (message) {
        printf("[%s:]:%s", message->sender->name, message->content);
    }
}

// Display chat messages
void display_chat(User* user1, User* user2) {
    Chat* chat = get_chat(user1, user2);
    Message* current = chat->head;
    while (current) {
        print_message(current);
        if (current->next){
            printf(",");
        }
        current = current->next;
    }
    printf("\n");
}

// Create a new post
Post* new_post(User* user, const char* content) {

    // Check for post parameters
    if (!user || !content) {return NULL;}

    // Check content length
    if (strlen(content) >= MAX_POST_LENGTH) {return NULL;}

    // Allocate memory for the new post
    Post* new_post = (Post*)malloc(sizeof(Post));
    if (!new_post) {return NULL;}

    // Initialize the new post
    new_post->post_id = post_count++;
    new_post->user = user;
    new_post->content = strdup(content);
    if (!new_post->content) {
        free(new_post);
        return NULL;
    }

    // Add the post to the front of the posts linked list
    new_post->next = posts;
    posts = new_post;
    return new_post;
}

// Add a like to a post
void add_like(Post* post, User* user){
    if (!post || !user) {return;}

    // Check if the user has already liked the post
    LikeNode* current = post->liked_by;
    while (current) {
        if (current->user == user) {return;}
        current = current->next;
    }

    // Add the user to the list of users who liked the post
    LikeNode* new_like = (LikeNode*)malloc(sizeof(LikeNode));
    if (!new_like) {return;}

    new_like->user = user;
    new_like->next = post->liked_by;
    post->liked_by = new_like;

    // Increment the like count
    post->likes++;
    // printf("User '%s' liked the post. Total likes: %d\n", user->name, post->likes);
}


void display_feed(User* user) {
    if (!user) {return;}

    // Collect posts from user and his/her friends
    Post* feed_posts[MAX_FEED_POSTS];
    int count = 0;
    Post* current_post = posts;

    while (current_post) {

        if (current_post->user == user) {
            feed_posts[count++] = current_post;
        } else {
            FriendNode* current_friend = user->friends;
            while (current_friend) {
                if (current_friend->user == current_post->user) {
                    feed_posts[count++] = current_post;
                    break;
                }
                current_friend = current_friend->next;
            }
        }
        current_post = current_post->next;

        // Stop collecting if reach the maximum feed size
        if (count >= MAX_FEED_POSTS) {break;}
    }

    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (feed_posts[i]->likes < feed_posts[j]->likes) {
                // Swap if the post has fewer likes
                Post* temp = feed_posts[i];
                feed_posts[i] = feed_posts[j];
                feed_posts[j] = temp;
            } else if (feed_posts[i]->likes == feed_posts[j]->likes) {
                // If likes are equal, sort by recency (older posts first)
                if (feed_posts[i]->post_id > feed_posts[j]->post_id) {
                    Post* temp = feed_posts[i];
                    feed_posts[i] = feed_posts[j];
                    feed_posts[j] = temp;
                }
            }
        }
    }

    // Display posts in sorted order
    for (int i = 0; i < count; i++) {
        printf("[%s:]%s", feed_posts[i]->user->name, feed_posts[i]->content, feed_posts[i]->likes);
        if (i < count - 1){
            printf(",");
        }
    }
}

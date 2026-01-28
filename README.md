# Social Media Platform - Data Structures Implementation

A comprehensive social media platform implementation in C, demonstrating advanced data structures and algorithms including hash tables, linked lists, doubly-linked lists, and graph-based friendship networks.

## Project Overview

This project implements a fully functional social media backend with user management, friend networks, messaging system, and post feed functionality. The implementation showcases efficient data structure usage for real-world applications.

**Author:** Daniil Nistribenko  
**Course:** Data Structures  
**Language:** C

## Features

### 👥 User Management
- **Create Users** - Register new users with unique names and emails
- **Search Users** - Find users by name or email using hash table lookups (O(1) average)
- **Update Profiles** - Change user names and email addresses
- **Delete Users** - Remove users and clean up all associated data
- **List All Users** - Display all registered users in alphabetical order

### 🤝 Friend Network
- **Add Friends** - Create bidirectional friendship connections
- **Remove Friends** - Unfriend users while maintaining data integrity
- **View Friends List** - Display a user's friends in alphabetical order
- **Find Mutual Friends** - Identify common friends between two users
- **Friendship Validation** - Prevent duplicate friendships and self-friending

### 💬 Messaging System
- **Send Messages** - Exchange messages between friends (friends-only)
- **View Chat History** - Display conversation history in FIFO order
- **Message Persistence** - Maintain chat history with automatic old message cleanup
- **Chat Management** - Organize messages by conversation pairs
- **Message Size Limit** - Enforce maximum message length constraints

### 📱 Social Feed
- **Create Posts** - Share content with your network
- **Like Posts** - Express appreciation for content
- **View Feed** - See posts from friends and your own posts
- **Post Ordering** - Display most recent posts first
- **Like Tracking** - Track who liked each post

## Data Structures Used

### Hash Table
- **Purpose:** User storage and retrieval
- **Size:** 10,000 buckets
- **Collision Resolution:** Chaining with linked lists
- **Hash Function:** Custom string hashing algorithm
- **Complexity:** O(1) average case for search/insert

### Linked Lists
- **Friend Lists:** Singly-linked list per user
- **User Hash Buckets:** Chaining for collision resolution
- **Post Feed:** Global linked list of all posts
- **Like Lists:** Track users who liked each post

### Doubly-Linked Lists
- **Chat Messages:** Bidirectional navigation in conversations
- **Message History:** Efficient FIFO display and old message removal

### Graph Structure
- **Friendship Network:** Undirected graph representation
- **Nodes:** Users in the network
- **Edges:** Friendship connections
- **Operations:** BFS for mutual friends discovery

## Technical Specifications

### Constants and Limits
```c
MAX_USERS           10,000  // Maximum users in system
NAME_LENGTH         50      // Maximum name length
EMAIL_DOMAIN_LENGTH 50      // Maximum email length
MAX_MESSAGE_LENGTH  256     // Maximum message content
MAX_MESSAGES        50      // Maximum messages per chat
MAX_POST_LENGTH     256     // Maximum post content
MAX_FEED_POSTS      20      // Maximum posts in feed
```

### Data Models

#### User
```c
typedef struct {
    int user_id;
    char name[NAME_LENGTH];
    char email[EMAIL_DOMAIN_LENGTH];
    struct FriendNode* friends;
    struct Message* messages;
} User;
```

#### Message
```c
typedef struct Message {
    int message_id;
    User* sender;
    User* receiver;
    char content[MAX_MESSAGE_LENGTH];
    struct Message* prev;
    struct Message* next;
} Message;
```

#### Post
```c
typedef struct Post {
    int post_id;
    User* user;
    char* content;
    int likes;
    LikeNode* liked_by;
    struct Post* next;
} Post;
```

## API Reference

### User Management Functions

```c
// Create a new user with auto-generated unique ID
User* create_user(const char* name, const char* email);

// Search for users
User* search_user_by_name(const char* name);
User* search_user_by_email(const char* email);

// Check if name or email is already taken
int is_name_or_email_taken(const char* name, const char* email);

// Update user information
void change_user_name(User* user, char* new_name);
void change_user_email(User* user, char* new_email);

// Delete user and cleanup all associations
void delete_user(User* user);

// Display all users alphabetically
void print_users();
```

### Friend Network Functions

```c
// Create bidirectional friendship
void add_friend(User* user1, User* user2);

// Remove friendship connection
void delete_friend(User* user1, User* user2);

// Check friendship status
int are_friends(User* user1, User* user2);

// Display user's friends alphabetically
void print_friends(User* user);

// Find common friends
User** mutual_friends(User* user1, User* user2);
void print_mutual_friends(User** friends);
```

### Messaging Functions

```c
// Create and send a message (auto-generated message ID)
Message* create_message(User* sender, User* receiver, const char* content);

// Display single message
void print_message(Message* message);

// Display entire conversation in FIFO order
void display_chat(User* user1, User* user2);

// Get or create chat between two users
Chat* get_chat(User* user1, User* user2);
```

### Post and Feed Functions

```c
// Create a new post (auto-generated post ID)
Post* new_post(User* user, const char* content);

// Add a like to a post
void add_like(Post* post, User* user);

// Display user's personalized feed
void display_feed(User* user);
```

## Usage Examples

### Creating Users and Building Network
```c
// Create users
User* alice = create_user("Alice", "alice@example.com");
User* bob = create_user("Bob", "bob@example.com");
User* carol = create_user("Carol", "carol@example.com");

// Build friendship network
add_friend(alice, bob);
add_friend(bob, carol);

// Find mutual friends
User** mutual = mutual_friends(alice, carol);
print_mutual_friends(mutual);  // Output: Bob
```

### Messaging Between Friends
```c
// Send messages (only works if users are friends)
create_message(alice, bob, "Hey Bob! How are you?");
create_message(bob, alice, "I'm doing great, thanks!");

// Display conversation
display_chat(alice, bob);
// Output: [Alice:]:Hey Bob! How are you?,[Bob:]:I'm doing great, thanks!
```

### Creating and Interacting with Posts
```c
// Create posts
Post* post1 = new_post(alice, "Just learned about hash tables!");
Post* post2 = new_post(bob, "C programming is fun!");

// Like posts
add_like(post1, bob);
add_like(post2, alice);

// View feed
display_feed(alice);  // Shows posts from Alice and her friends
```

## Algorithm Complexity

| Operation | Time Complexity | Space Complexity |
|-----------|----------------|------------------|
| Create User | O(1) average | O(1) |
| Search User | O(1) average | O(1) |
| Add Friend | O(1) | O(1) |
| Delete Friend | O(n) | O(1) |
| Mutual Friends | O(n×m) | O(min(n,m)) |
| Create Message | O(1) | O(1) |
| Display Chat | O(k) | O(1) |
| Create Post | O(1) | O(1) |
| Display Feed | O(p×f) | O(1) |

*Where n,m = friend counts, k = message count, p = total posts, f = friend count*

## Memory Management

The implementation includes robust memory management:
- Dynamic allocation for all data structures
- Proper deallocation on user deletion
- No memory leaks when users or friendships are removed
- Automatic cleanup of old messages when chat limit is exceeded
- Efficient memory usage with pointer-based structures

## Design Decisions

### Hash Table for Users
- **Why:** O(1) average lookup time for user searches
- **Trade-off:** Extra memory for hash table buckets vs. fast search

### Doubly-Linked List for Messages
- **Why:** Efficient bidirectional traversal and FIFO display
- **Trade-off:** Extra pointer overhead for prev pointer

### Global Post List
- **Why:** Simple feed generation without per-user duplication
- **Trade-off:** O(n) feed display but saves memory

### Friendship as Bidirectional
- **Why:** Maintains consistency and simplifies queries
- **Trade-off:** Requires updating both users when adding/removing friends

## Error Handling

The implementation includes comprehensive error checking:
- Null pointer validation
- Duplicate name/email prevention
- Message length validation
- Friends-only messaging enforcement
- Maximum message limit per chat
- Post length validation
- Memory allocation failure handling

## File Structure

```
project/
├── functions.h                                 # Header file with all structures and prototypes
├── functions.c                                 # Complete implementation
└── data_structers_report_Daniil Nistribenko.pdf  # Detailed technical report
```

## Building and Testing

### Compilation
```bash
gcc -Wall -Wextra -std=c99 -o social_media functions.c your_main.c
```

### Testing
The implementation includes a parser testing function:
```c
void testingParser(int arg1, char* arg2);
```

## Future Enhancements

Possible extensions to the project:
- [ ] Add post comments functionality
- [ ] Implement post sharing/reposting
- [ ] Add user blocking feature
- [ ] Implement message read receipts
- [ ] Add group chat support
- [ ] Implement post search functionality
- [ ] Add multimedia content support
- [ ] Implement friend suggestions algorithm
- [ ] Add privacy settings for posts
- [ ] Implement notification system

## Performance Characteristics

### Strengths
- Fast user lookup with hash table
- Efficient friend management with linked lists
- Scalable to 10,000 users
- Memory-efficient message storage
- Quick post creation and feed generation

### Limitations
- Linear search for mutual friends
- No post indexing (linear scan for feeds)
- Fixed maximum limits for various entities
- No persistent storage (in-memory only)

## Key Learning Outcomes

This project demonstrates:
1. **Hash Table Implementation** - Custom hash function and collision resolution
2. **Linked List Mastery** - Singly and doubly-linked list operations
3. **Graph Algorithms** - Friend network as graph structure
4. **Memory Management** - Dynamic allocation and proper cleanup
5. **Data Structure Selection** - Choosing appropriate structures for use cases
6. **Algorithm Analysis** - Understanding time/space complexity trade-offs

Academic Integrity
This is an educational project completed for a Data Structures course. The code demonstrates understanding of fundamental data structures and their real-world applications.
License
MIT License
Copyright (c) 2024 Daniil Nistribenko
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

Note: This implementation is designed for educational purposes and demonstrates core data structure concepts. A production social media platform would require additional features like authentication, encryption, database persistence, and scalability considerations.

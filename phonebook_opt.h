#ifndef _PHONEBOOK_H
#define _PHONEBOOK_H

#define MAX_LAST_NAME_SIZE 16

/* TODO: After modifying the original version, uncomment the following
 * line to set OPT properly */
#define OPT 1
typedef struct __PHONE_BOOK_ENTRY {
    char lastName[MAX_LAST_NAME_SIZE];
    struct __PHONE_BOOK_DETAILED *pdetail;
    struct __PHONE_BOOK_ENTRY *pNext;
} entry;

typedef struct __PHONE_BOOK_DETAILED {
    char firstName[16];
    char email[16];
    char phone[10];
    char cell[10];
    char addr1[16];
    char addr2[16];
    char city[16];
    char state[2];
    char zip[5];
} phoneBookDetailed;

typedef struct HASHTABLE {
    struct __PHONE_BOOK_ENTRY **hTable;
} hashtable;

extern unsigned int HashTableSize;

entry *findName(hashtable *pTable, const char lastName[]);
void append(hashtable *pThable, const char lastName[], entry *BigMemory, unsigned long counts);
hashtable *createHashTable(unsigned int hSize);
#endif

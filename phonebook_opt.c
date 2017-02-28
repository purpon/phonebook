#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stddef.h>
#include "phonebook_opt.h"

unsigned int HashTableSize;
unsigned int BKDRHash(const char *str);
int insertHashTable(hashtable *pHtable, entry *idata);
entry *GetHashTable(hashtable *pHtable, const char *lastName);


/* FILL YOUR OWN IMPLEMENTATION HERE! */
entry *findName (hashtable *pHtable, const char lastName[])
{
    /* TODO: implement */
    entry *KeyHead=GetHashTable(pHtable, lastName);
    while(KeyHead) {
        if(strcasecmp(lastName, KeyHead->lastName) == 0)
            return KeyHead;
        KeyHead=KeyHead->pNext;
    }
    return NULL;
}

void append (hashtable *pHtable, const char lastName[], entry *BigMemory, unsigned long pCounts)
{
    if(!pHtable) {
        printf("In append Hashtable is null! \n");
        return;
    }
    entry *e;
    /* allocate memory for the new entry and put lastName */
    e=&BigMemory[pCounts];
    strcpy(e->lastName, lastName);
    e->pNext = NULL;
    insertHashTable(pHtable,e);
}


unsigned int BKDRHash (const char *str)
{

    unsigned int seed = 131; // 31 131 1313 13131 131313 etc..
    unsigned int hash = 0;
    while (*str) {
        hash = hash * seed + (*str++);
    }
    return (hash & 0x7FFFFFFF);
}

hashtable *createHashTable (unsigned int hSize)
{
    hashtable *hTable;
    hTable=malloc(sizeof(hashtable));
    if(!hTable) {
        printf("Hash table create fail!\n");
        return NULL;
    }
    hTable->hTable=(entry**)malloc(sizeof(struct __PHONE_BOOK_ENTRY *)*hSize);
    if(!hTable->hTable) {
        printf("Hash table create fail!\n");
        free(hTable);
        return NULL;
    }
    HashTableSize=hSize;
    return hTable;
}

int insertHashTable (hashtable *pHtable ,entry *idata)
{
    unsigned int key;
    entry **head;
    char *id;
    if(!idata || !pHtable ) {
        return 0;
    }
    id=idata->lastName;
    key= BKDRHash(id);
    key%=HashTableSize;
    head=&(pHtable->hTable[key]);
    while((*head)) {
        head=&(*head)->pNext;
    }
    *head=idata;
    return 0;
}

entry *GetHashTable (hashtable *pHtable, const char *lastName)
{
    if(!pHtable) {
        return 0;
    }
    unsigned int key;
    key=BKDRHash(lastName);
    if(!key) {
        return NULL;
    }
    key%=HashTableSize;
    return pHtable->hTable[key];
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>

#include IMPL

#ifdef OPT
#define OUT_FILE "opt.txt"
#else
#define OUT_FILE "orig.txt"
#endif

#define DICT_FILE "./dictionary/words.txt"
char **GetRandName(int counts)
{
    char line[MAX_LAST_NAME_SIZE];
    long fileSize;
    long randFileOffset;
    FILE *fp;
    char **nameVector;
    nameVector=(char **)malloc(sizeof(char**)*counts);
    for(int icount = 0; icount < counts; icount++) {
        nameVector[icount]=(char *)malloc(sizeof(line));
        if(!nameVector[icount]) {
            printf("Memort error!\n");
            return NULL;
        }
    }
    if(!nameVector) {
        printf("Memory error!\n");
        return NULL;
    }
    fp=fopen(DICT_FILE,"r");
    if(!fp) {
        printf("File open error!\n");
        return NULL;
    }
    fseek(fp,0,SEEK_END);
    fileSize=ftell(fp);
    int i=0;
    srand(time(NULL));
    for(i=0; i<counts; i++) {
        randFileOffset=rand()%(fileSize+1-0);
        fseek(fp,0,SEEK_SET);
        fseek(fp,randFileOffset,SEEK_SET);
        fgets(line,sizeof(line),fp);
        fgets(line,sizeof(line),fp);
        int count=0;
        while(*(line+count++)!='\n') {
            ;
        }
        *(line+count-1)='\0';
        //printf("Words =%s\n",line);
        strncpy(nameVector[i],line,sizeof(line));
    }
    fclose(fp);
    return nameVector;
}

static double diff_in_second(struct timespec t1, struct timespec t2)
{
    struct timespec diff;
    if (t2.tv_nsec-t1.tv_nsec < 0) {
        diff.tv_sec  = t2.tv_sec - t1.tv_sec - 1;
        diff.tv_nsec = t2.tv_nsec - t1.tv_nsec + 1000000000;
    } else {
        diff.tv_sec  = t2.tv_sec - t1.tv_sec;
        diff.tv_nsec = t2.tv_nsec - t1.tv_nsec;
    }
    return (diff.tv_sec + diff.tv_nsec / 1000000000.0);
}

int main(int argc, char *argv[])
{
    FILE *fp;
    int i = 0;
    char line[MAX_LAST_NAME_SIZE];
    struct timespec start, end;
    double cpu_time1, cpu_time2;

    /* check file opening */
    fp = fopen(DICT_FILE, "r");
    if (fp == NULL) {
        printf("cannot open the file\n");
        return -1;
    }

    /* build the entry */
    entry *pHead, *e;
    pHead = (entry *) malloc(sizeof(entry));
    printf("size of entry : %lu bytes\n", sizeof(entry));
    e = pHead;
    e->pNext = NULL;
#if defined(__GNUC__)
    __builtin___clear_cache((char *) pHead, (char *) pHead + sizeof(entry));
#endif
    clock_gettime(CLOCK_REALTIME, &start);
#if OPT
    hashtable *_hTable;
    _hTable=createHashTable(82889);
    assert(_hTable);
    unsigned long counts=0;
    char ch;
    while(!feof(fp)) {
        ch = fgetc(fp);
        if(ch == '\n') {
            counts++;
        }
    }
    entry *BigEntryMemory=(entry *)malloc(sizeof(entry)*counts);
    if(!BigEntryMemory) {
        printf("Create a big size memory fail.Memory error!\n");
    }
    counts=0;
    fseek(fp,0,SEEK_SET);
#endif
    while (fgets(line, sizeof(line), fp)) {
        while (line[i] != '\0')
            i++;
        line[i - 1] = '\0';
        i = 0;
#if OPT
        append(_hTable,line,BigEntryMemory,counts++);
#else
        e = append(line, e);
#endif
    }
    clock_gettime(CLOCK_REALTIME, &end);
    cpu_time1 = diff_in_second(start, end);

    /* close file as soon as possible */
    fclose(fp);


    /* the givn last name to find */
    e = pHead;
    /**
    	char input[MAX_LAST_NAME_SIZE] = "zyxel";
    #if OPT
        assert(findName(_hTable,input) &&
               "Did you implement findName() in " IMPL "?");
        assert(0 == strcmp(findName(_hTable,input)->lastName, "zyxel"));

    #else
        assert(findName(input, e) &&
               "Did you implement findName() in " IMPL "?");
        assert(0 == strcmp(findName(input, e)->lastName, "zyxel"));
    #endif
    **/
#if defined(__GNUC__)
    __builtin___clear_cache((char *) pHead, (char *) pHead + sizeof(entry));
#endif
    /* compute the execution time */
    clock_gettime(CLOCK_REALTIME, &start);
    int findNameCount=100;
    char **FindNameVector;
    FindNameVector=GetRandName(findNameCount);
    assert(FindNameVector);
    entry *fEntry;
    for(int iFind = 0 ; iFind < findNameCount ; iFind++) {
#if OPT
        fEntry=findName(_hTable,FindNameVector[iFind]);
#else
        fEntry=findName(FindNameVector[iFind], e);
#endif
        assert(fEntry);
        assert(0==strcmp(fEntry->lastName,FindNameVector[iFind]));
    }
    clock_gettime(CLOCK_REALTIME, &end);
    cpu_time2 = diff_in_second(start, end);

    FILE *output = fopen(OUT_FILE, "a");
    fprintf(output, "append() findName() total %lf %lf %lf\n", cpu_time1, cpu_time2,cpu_time1+cpu_time2);
    fclose(output);

    printf("execution time of append() : %lf sec\n", cpu_time1);
    printf("execution time of findName() : %lf sec\n", cpu_time2);
    printf("Total exection time : %lf sec\n",cpu_time1+cpu_time2);
    if (pHead->pNext) free(pHead->pNext);
    free(pHead);
#if OPT
    free(BigEntryMemory);
    free(_hTable);
#endif
    return 0;
}

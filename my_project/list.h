#ifndef LIST_H
#define LIST_H

typedef struct tagBoudray{
    int x;
    int y;
    struct tagBoudray header;
    struct tagBoudray next;
}Boudary;

typedef struct tagBoud_List{
    Boudary *phead;
    struct tagBoud_List *lnext;
}Boud_List;

#endif // LIST_H

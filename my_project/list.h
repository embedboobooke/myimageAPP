#ifndef LIST_H
#define LIST_H

typedef struct tagBoudary
{
  int x;
  int y;
  struct tagBoudary *head;
  struct tagBoudary *next;
}Boudary;

typedef struct tagBoud_List
{
  Boudary *phead;
  struct tagBoud_List *lnext;
}Boud_List;

#endif // LIST_H

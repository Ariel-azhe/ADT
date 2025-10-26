#include "symtable.h"

size_t size;

int main(void)
{
    SymTable_T oSymtable = SymTable_new();
    int *res = NULL;
    const void *rget;
    const void *pvValue;
    const char a = 'a';
    int num = 1;
    const char *pcKey = &a;
    pvValue = &num;
    oSymtable->first = (struct Node*)calloc(1, sizeof(struct Node));
    oSymtable->first->key = pcKey;
    oSymtable->first->value = pvValue;


    /*SymTable_put(oSymtable, pcKey, pvValue);*/
    rget = SymTable_get(oSymtable, pcKey);
    if (rget == NULL)
    {
        return 3;
    }
    res = (int*)rget;
    return *res;
}
SymTable_T SymTable_new(void)
{
    SymTable_T sym = (struct List*)calloc(1, sizeof(struct List));
    if (sym == NULL)
    {
        return NULL;
    }
    sym->first = NULL;
    return sym;
}

  void SymTable_free(SymTable_T oSymTable)
  {
    while (size > 0)
    {
        struct Node *fst_next = oSymTable->first->next;
        free(oSymTable->first);
        oSymTable->first = fst_next;
        size--;
    }
  }

  size_t SymTable_getLength(SymTable_T oSymTable)
  {
    return size;
  }

  int SymTable_put(SymTable_T oSymTable,
     const char *pcKey, const void *pvValue)
     {
        struct Node *pFst = NULL;
        if (SymTable_contains(oSymTable, pcKey))
        {
            return 0;
        }
        else
        {
            struct Node *sym_fst = oSymTable->first;
            struct Node *fst = (struct Node*)calloc(1, sizeof(struct Node));
            fst->key = pcKey;
            fst->value = pvValue; 
            fst->next = sym_fst; 
            oSymTable->first = fst;
            size++;
            return 1;
        }
     }

  void *SymTable_replace(SymTable_T oSymTable,
     const char *pcKey, const void *pvValue)
     {
        struct Node *cur = oSymTable->first;
        int replaced = 0;
        while (cur->next != NULL)
        {
            if (*(cur->key) == *pcKey)
            {
                cur->value = (void*)pvValue;
                replaced = 1;
            }
            cur++;
        }
        if (replaced == 0)
        {
            free(cur);
            cur = NULL;
            return NULL;
        }
        free(cur);
        cur = NULL;
        return (void*)pvValue;
     }

  int SymTable_contains(SymTable_T oSymTable, const char *pcKey)
  {
    struct Node *cur = oSymTable->first;
    while (cur != NULL)
    {
        if (*(cur->key) == *pcKey)
        {
            return 1;
        }
        cur=cur->next;
    }
    return 0;
  }

  void *SymTable_get(SymTable_T oSymTable, const char *pcKey)
  {
    struct Node *cur = oSymTable->first;
    while (cur != NULL)
    {
        if (*(cur->key) == *pcKey)
        {
            return (void*)cur->value;
        }
        cur = cur->next;
    }
    return NULL;
  }

  void *SymTable_remove(SymTable_T oSymTable, const char *pcKey)
  {
    struct Node *prev = oSymTable->first;
    int removed = 0;
    const void *pvValue;
    while (prev->next != NULL)
    {
        if (*(prev->next->key) == *pcKey)
        {
            struct Node *r_next = prev->next->next;
            pvValue = prev->next->value;
            free(prev->next);
            prev->next = r_next;
            removed = 1;
            size--;
        }
        prev++;
    }
    if (removed == 0)
    {
        free(prev);
        prev = NULL;
        free((void*)pvValue);
        pvValue = NULL;
    }
    free(prev);
    prev = NULL;
    return (void*)pvValue;
  }

  void SymTable_map(SymTable_T oSymTable,
     void (*pfApply)(const char *pcKey, void *pvValue, void *pvExtra),
     const void *pvExtra)
     {
        struct Node *cur = oSymTable->first;
        while (cur->next != NULL)
        {
            (*pfApply)(cur->key, (void*)cur->value, (void*)pvExtra);
        }
        free(cur);
        cur = NULL;
    }

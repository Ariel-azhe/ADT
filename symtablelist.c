#include "symtable.h"

size_t size;
/*
int main(void)
{
    SymTable_T oSymtable = SymTable_new();
    int *res = NULL;
    const void *rget;
    const void *pvValue;
    const char a = 'b';
    int num = 2;
    const char *pcKey = &a;
    pvValue = &num;

    SymTable_put(oSymtable, pcKey, pvValue);
    rget = SymTable_get(oSymtable, pcKey);
    if (rget == NULL)
    {
        return 3;
    }
    res = (int*)rget;
    return *res;
}
    */

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
    assert(oSymTable != NULL);
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
    assert(oSymTable != NULL);
    return size;
  }

  int SymTable_put(SymTable_T oSymTable,
     const char *pcKey, const void *pvValue)
     {
        assert(oSymTable != NULL);
        assert(pcKey != NULL);
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
        void *rvalue = NULL;
        struct Node *cur = oSymTable->first;
        assert(oSymTable != NULL);
        assert(pcKey != NULL);
        while (cur != NULL)
        {
            if (*(cur->key) == *pcKey)
            {
                rvalue = cur->value;
                cur->value = (void*)pvValue;
            }
            cur=cur->next;
        }
        return rvalue;
     }

  int SymTable_contains(SymTable_T oSymTable, const char *pcKey)
  {
    struct Node *cur = oSymTable->first;
    assert(oSymTable != NULL);
    assert(pcKey != NULL);
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
    assert(oSymTable != NULL);
    assert(pcKey != NULL);
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
    struct Node *cur = oSymTable->first;
    struct Node *prev = oSymTable->first;
    int removed = 0;
    const void *pvValue;
    assert(oSymTable != NULL);
    assert(pcKey != NULL);
    while (cur != NULL)
    {
        if (*(cur->key) == *pcKey)
        {
            struct Node *dnext = cur->next;
            pvValue = cur->value;
            if (cur != prev)
            {
                prev->next = dnext;
            }
            if (oSymTable->first == cur)
            {
                oSymTable->first = cur->next;
            }
            free(cur);
            removed = 1;
            size--;
        }
        prev = cur;
        cur=cur->next;
    }
    if (removed == 0)
    {
        pvValue = NULL;
    }
    return (void*)pvValue;
  }

  void SymTable_map(SymTable_T oSymTable,
     void (*pfApply)(const char *pcKey, void *pvValue, void *pvExtra),
     const void *pvExtra)
     {
        struct Node *cur = oSymTable->first;
        assert(oSymTable != NULL);
        while (cur != NULL)
        {
            (*pfApply)(cur->key, (void*)cur->value, (void*)pvExtra);
            cur=cur->next;
        }
    }

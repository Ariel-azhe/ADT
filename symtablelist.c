#include "symtable.h"

size_t size;

SymTable_T SymTable_new(void)
{
    SymTable_T sym = calloc(1, sizeof(struct List));
    if (sym == NULL)
    {
        return NULL;
    }
    return sym;
}

  void SymTable_free(SymTable_T oSymTable)
  {
    while (size > 0)
    {
        struct Node *fst_next = oSymTable->first->next;
        free(oSymTable->first);
        oSymTable->first = NULL;
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
            struct Node fst;
            fst.key = pcKey;
            fst.value = pvValue;
            fst.next = sym_fst; 
            pFst = &fst;
            oSymTable->first = pFst;
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
            return NULL;
        }
        return (void*)pvValue;
     }

  int SymTable_contains(SymTable_T oSymTable, const char *pcKey)
  {
    struct Node *cur = oSymTable->first;
    while (cur->next != NULL)
    {
        if (*(cur->key) == *pcKey)
        {
            return 1;
        }
        cur++;
    }
    return 0;
  }

  void *SymTable_get(SymTable_T oSymTable, const char *pcKey)
  {
    struct Node *cur = oSymTable->first;
    while (cur->next != NULL)
    {
        if (*(cur->key) == *pcKey)
        {
            return (void*)cur->value;
        }
        cur++;
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
        }
        prev++;
    }
    if (removed == 0)
    {
        return NULL;
    }
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
    }

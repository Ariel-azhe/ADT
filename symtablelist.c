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
            struct Node *fst = calloc(1, sizeof(struct Node));
            fst->key = pcKey;
            fst->value = pvValue; 
            fst->next = sym_fst; 
            oSymTable->first = fst;
            size++;
            return 1;
        }
     }



#include "symtable.h"

size_t size;

/*
int main(void)
{
    SymTable_T oSymTable = SymTable_new();
    char acJeter[] = "Jeter";
   char acMantle[] = "Mantle";
   char acGehrig[] = "Gehrig";
   char acRuth[] = "Ruth";
   char acShortstop[] = "Shortstop";
   char acCenterField[] = "Center Field";
   char acFirstBase[] = "First Base";
   char acRightField[] = "Right Field";

   char acBrown[] = "Brown";
   
   char *pcValue;
   int iSuccessful;
   int iFound;
   size_t uLength;

   
   iSuccessful = SymTable_put(oSymTable, acJeter, acShortstop);
   

   
   iSuccessful = SymTable_put(oSymTable, acMantle, acCenterField);



   

   iFound = SymTable_contains(oSymTable, "Maris");

   return iFound;
}
*/

SymTable_T SymTable_new(void)
{
    SymTable_T sym = (struct List*)calloc(1, sizeof(struct List));
    struct Node *fst = (struct Node*)calloc(1, sizeof(struct Node));
    sym->first = fst;
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
            fst->key = (const char*)malloc(strlen(pcKey) + 1);
            strcpy((char*)fst->key, pcKey);
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
            if (!(strcmp(pcKey, cur->key)))
            {
                rvalue = (void*)(cur->value);
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
        if (!(strcmp(pcKey, cur->key)))
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
        if (!(strcmp(pcKey, cur->key)))
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
    const void *pvValue = NULL;
    assert(oSymTable != NULL);
    assert(pcKey != NULL);
    while (cur != NULL)
    {
        if (!(strcmp(pcKey, cur->key)))
        {
            struct Node *dnext = cur->next;
            pvValue = cur->value;
            if (oSymTable->first == cur)
            {
                oSymTable->first = cur->next;
            }
            else
            {
                prev->next = dnext;

            }
            free((void*)cur->key);
            cur->key = NULL;
            free(cur);
            cur = NULL;
            size--;
        }
        else
        {
            prev = cur;
            cur=cur->next;
        }
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

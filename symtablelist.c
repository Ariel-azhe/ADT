/*this file implements the symtable client functions with
an underlying linked list structure*/
#include "symtable.h"

/*the number of key-value pairs in the list currently*/
static size_t size;

/*the structure of every node in the linked list*/
struct Node {

        /*the pointer to the key string*/
        const char *key;

        /*the pointer to the value with generic type*/
        const void *value;

        /*the pointer to the next binding in the list*/
        struct Node *next;
    };

    /*the struct holding the list implementation of sym table*/
    struct Table {

        /*the starting node of the list*/
        struct Node *first;
    };

SymTable_T SymTable_new(void)
{
    SymTable_T sym = (struct Table*)calloc(1, sizeof(struct Table));
    size = 0;
    return sym;
}

  void SymTable_free(SymTable_T oSymTable)
  {
    struct Node *fst_next = NULL;
    assert(oSymTable != NULL);
    while (size > 0)
    {
        if (oSymTable->first == NULL)
        {
            fst_next = NULL;
        }
        else
        {
            fst_next = oSymTable->first->next;
            free((void*)oSymTable->first->key);
            oSymTable->first->key = NULL;
        }
        free(oSymTable->first);
        oSymTable->first = fst_next;
        size--;
    }
    free(oSymTable);
    oSymTable = NULL;
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
            if (fst == NULL)
            {
                return 0;
            }
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
        assert(pfApply != NULL);
        while (cur != NULL)
        {
            (*pfApply)(cur->key, (void*)cur->value, (void*)pvExtra);
            cur=cur->next;
        }
    }

#include "symtable.h"

size_t bindings = 0;
int hkeys[BUCKET_COUNT];
/*creates new empty symbol table or returns NULL
    if insufficient memory*/
    SymTable_T SymTable_new(void)
    {
        SymTable_T table = (SymTable_T)calloc(1, sizeof(struct Table));
        return table;
    }

    /*frees all memory occupied by oSymTable*/
  void SymTable_free(SymTable_T oSymTable)
  {
    size_t i = 0;
    while (i < sizeof(oSymTable->buckets))
    {
        struct Binding *cur = oSymTable->buckets[i];
        while (cur != NULL)
        {
            free(cur);
            cur=cur->next;
        }
        oSymTable->buckets[i] = NULL;
        i++;
    }
  }

  /*returns the number of bindings in oSymTable*/
  size_t SymTable_getLength(SymTable_T oSymTable)
  {
    return bindings;
  }

  /*Adds a new binding with key pcKey and value pvValue to oSymTable
  if oSymTable doesn't contain a binding with key pcKey already.
  Return 0 and leave oSymTable unchanged otherwise.*/
  int SymTable_put(SymTable_T oSymTable,
     const char *pcKey, const void *pvValue)
     {
        struct Binding *cur = NULL;
        struct Binding *newB;
        if (hkeys[(int)*pcKey] == 0)
        {
            hkeys[(int)*pcKey] = SymTable_hash(pcKey, BUCKET_COUNT);
        }
        if (SymTable_contains(oSymTable, pcKey))
        {
            return 0;
        }
        cur = oSymTable->buckets[hkeys[(int)*pcKey]];
        newB = (struct Binding*)calloc(1, sizeof(struct Binding));
        newB->key = pcKey;
        newB->value = pvValue;
        newB->next = cur;
        oSymTable->buckets[hkeys[(int)*pcKey]] = newB;
        bindings++;
        return 1;
     }
    
    /*Change the value of the binding within oSymTable
    with key pcKey to pvValue if such binding exists in 
    oSymTable. Return NULL otherwise.*/
  void *SymTable_replace(SymTable_T oSymTable,
     const char *pcKey, const void *pvValue)
     {
        void *rvalue = NULL;
        struct Binding *cur = NULL;
        if (hkeys[(int)*pcKey] == 0)
        {
            hkeys[(int)*pcKey] = SymTable_hash(pcKey, BUCKET_COUNT);
        }
        cur = oSymTable->buckets[hkeys[(int)*pcKey]];
        while (cur != NULL)
        {
            if (!(strcmp(pcKey, cur->key)))
            {
                rvalue = (void*)cur->value;
                cur->value = pvValue;
            }
            cur=cur->next;
        }
        return rvalue;
     }

     /*Return 1 if oSymTable contains a binding with key pcKey.
     Return 0 otherwise.*/
  int SymTable_contains(SymTable_T oSymTable, const char *pcKey)
  {
    struct Binding *cur = NULL;
    if (hkeys[(int)*pcKey] == 0)
    {
        hkeys[(int)*pcKey] = SymTable_hash(pcKey, BUCKET_COUNT);
    }
    cur = oSymTable->buckets[hkeys[(int)*pcKey]];
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

  /*Return the value of the binding with key pcKey if such key 
  exists in oSymTable. Return NULL otherwise.*/
  void *SymTable_get(SymTable_T oSymTable, const char *pcKey)
  {
    struct Binding *cur = NULL;
    if (hkeys[(int)*pcKey] == 0)
    {
        hkeys[(int)*pcKey] = SymTable_hash(pcKey, BUCKET_COUNT);
    }
    cur = oSymTable->buckets[hkeys[(int)*pcKey]];
    while (cur != NULL)
    {
        if (!(strcmp(pcKey, cur->key)))
        {
            return (void*)cur->value;
        }
        cur=cur->next;
    }
    return NULL;
  }

  /*Remove the binding with key pcKey in oSymTable and return
  the value of that binding if that binding exists.
  Return NULL otherwise.*/
  void *SymTable_remove(SymTable_T oSymTable, const char *pcKey)
  {
    struct Binding *cur = NULL;
    struct Binding *prev = NULL;
    struct Binding *bnext = NULL;
    void *pvValue = NULL;
    if (hkeys[(int)*pcKey] == 0)
    {
        hkeys[(int)*pcKey] = SymTable_hash(pcKey, BUCKET_COUNT);
    }
    cur = oSymTable->buckets[hkeys[(int)*pcKey]];
    while (cur != NULL)
    {
        if (!(strcmp(pcKey, cur->key)))
        {
            bnext = cur->next;
            pvValue = cur->value;
            if (oSymTable->buckets[hkeys[(int)*pcKey]] == cur)
            {
                oSymTable->buckets[hkeys[(int)*pcKey]] = bnext;
            }
            else
            {
                prev->next = bnext;
            }
            free((void*)cur->key);
            cur->key = NULL;
            free(cur);
            cur = NULL;
            bindings--;
        }
        else
        {
            prev = cur;
            cur=cur->next;
        }
    }
        return (void*)pvValue;
  }

  /*Applies function *pfApply to all bindings in oSymTable with
  extra parameter pvExtra*/
  void SymTable_map(SymTable_T oSymTable,
     void (*pfApply)(const char *pcKey, void *pvValue, void *pvExtra),
     const void *pvExtra)
     {
        size_t i = 0;
        while (i < sizeof(oSymTable->buckets))
        {
            struct Binding *cur = oSymTable->buckets[i];
            while (cur != NULL)
            {
                (*pfApply)(cur->key, (void*)cur->value, (void*)pvExtra);
                cur=cur->next;
            }
            i++;
        }
     }

/* Return a hash code for pcKey that is between 0 and uBucketCount-1,
   inclusive. */

static size_t SymTable_hash(const char *pcKey, size_t uBucketCount)
{
   const size_t HASH_MULTIPLIER = 65599;
   size_t u;
   size_t uHash = 0;

   assert(pcKey != NULL);

   for (u = 0; pcKey[u] != '\0'; u++)
      uHash = uHash * HASH_MULTIPLIER + (size_t)pcKey[u];

   return uHash % uBucketCount;
}
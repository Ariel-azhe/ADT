#include "symtable.h"


/*keeps track of which expansion attempt program is on*/
static size_t bindex = 0;
/*keeps track of all the possible expanded to sizes*/
static size_t bucket_cnts[] = {509, 1021, 2039, 4093, 8191, 16381, 32749, 65521};

    /*the structure of a single*/
    struct Binding {
        /*the pointer to the key string*/
    const char *key;
    /*the pointer to the value with generic type*/
    const void* value;
    /*the pointer to the next binding in the bucket*/
    struct Binding *next;
    };
    /*struct of the SymTable holding to its size, length, and array of buckets*/
    struct Table {
        /*the pointer to the array of buckets in the symtable*/
    struct Binding **buckets;
    /*the amount of key-value bindings*/
    size_t bindings;
    /*the physical length of the buckets (how many buckets)*/
    size_t length;
    };


/*creates new empty symbol table or returns NULL
    if insufficient memory*/
    SymTable_T SymTable_new(void)
    {
        SymTable_T sym = (struct Table*)calloc(1, sizeof(struct Table));
        if (sym == NULL)
        {
            return NULL;
        }
        sym->buckets = (struct Binding **)calloc(bucket_cnts[bindex], sizeof(struct Binding));
        sym->bindings = 0;
        sym->length = bucket_cnts[bindex];
        return sym;
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

/*expands the hash table bucket number when bindings exceed
a certain amount*/
void SymTable_expand(SymTable_T oSymTable)
{
    struct Binding ** new_buckets = 
    (struct Binding**)realloc(oSymTable->buckets, bucket_cnts[++bindex]*sizeof(struct Binding));
    size_t i = 0;
    if (new_buckets == NULL)
    {
        return;
    }
    oSymTable->length = bucket_cnts[bindex];
    oSymTable->buckets = new_buckets;
    i = bucket_cnts[bindex-1];
    while (i < oSymTable->length)
    {
        oSymTable->buckets[i] = NULL;
        i++;
    }
    i = 0;
    while (i < bucket_cnts[bindex - 1])
    {
        struct Binding *cur = oSymTable->buckets[i];
        struct Binding *prev = oSymTable->buckets[i];
        int prev_hval = 0;
        int hvalue = 0;
        struct Binding *hnext = NULL;
        struct Binding *pnext = NULL;
        struct Binding *nmemo = NULL;
        while (cur != NULL)
        {
            prev_hval = (int)SymTable_hash(cur->key, bucket_cnts[bindex-1]);
            hvalue = (int)SymTable_hash(cur->key, oSymTable->length);
            if (prev_hval != hvalue)
            {
                pnext = cur->next;
                if (cur == oSymTable->buckets[i])
                {
                    oSymTable->buckets[i] = pnext;
                    prev = pnext;
                }
                else
                {
                    prev->next = pnext;
                }
                hnext = oSymTable->buckets[hvalue];
                nmemo = (struct Binding*)realloc((void*)cur, sizeof(struct Binding));
                if (nmemo == NULL)
                {
                    return;
                }
                nmemo->next = hnext;
                oSymTable->buckets[hvalue] = nmemo;
                cur = pnext;
            }
            else
            {
                prev = cur;
                cur = cur->next;
            }
        }
        i++;

    }
    return;
}



    /*frees all memory occupied by oSymTable*/
  void SymTable_free(SymTable_T oSymTable)
  {
    size_t i = 0;
    size_t removed = 0;
    while ((i < oSymTable->length) && (removed < oSymTable->bindings))
    {
        struct Binding *cur = oSymTable->buckets[i];
        struct Binding *cnext = NULL;
        while (cur != NULL)
        {
            cnext=cur->next;
            free((void*)cur->key);
            cur->key = NULL;
            free(cur);
            cur = cnext;
            removed++;
        }
        oSymTable->buckets[i] = NULL;
        i++;
    }
    free(oSymTable->buckets);
    oSymTable->buckets = NULL;
    free(oSymTable);
    oSymTable = NULL;
  }

  /*returns the number of bindings in oSymTable*/
  size_t SymTable_getLength(SymTable_T oSymTable)
  {
    return oSymTable->bindings;
  }

  /*Adds a new binding with key pcKey and value pvValue to oSymTable
  if oSymTable doesn't contain a binding with key pcKey already.
  Return 0 and leave oSymTable unchanged otherwise.*/
  int SymTable_put(SymTable_T oSymTable,
     const char *pcKey, const void *pvValue)
     {
        struct Binding *cur = NULL;
        struct Binding *newB;
        int hvalue = 0;
        hvalue = (int)SymTable_hash(pcKey, bucket_cnts[bindex]);
        if (SymTable_contains(oSymTable, pcKey))
        {
            return 0;
        }
        cur = oSymTable->buckets[hvalue];
        newB = (struct Binding*)calloc(1, sizeof(struct Binding));
        if (newB == NULL)
        {
            return 0;
        }
        newB->key = (const char*)malloc(strlen(pcKey) + 1);
        strcpy((char*)newB->key, pcKey);
        newB->value = pvValue;
        newB->next = cur;
        oSymTable->buckets[hvalue] = newB;
        oSymTable->bindings++;
        if (oSymTable->bindings == oSymTable->length)
        {
            SymTable_expand(oSymTable);
        }
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
        int hvalue = 0;
        hvalue = (int)SymTable_hash(pcKey, bucket_cnts[bindex]);
        cur = oSymTable->buckets[hvalue];
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
    int hvalue = 0;
    hvalue = (int)SymTable_hash(pcKey, bucket_cnts[bindex]);
    cur = oSymTable->buckets[hvalue];
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
    int hvalue = 0;
    hvalue = (int)SymTable_hash(pcKey, bucket_cnts[bindex]);
    cur = oSymTable->buckets[hvalue];
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
    int hvalue = 0;
    hvalue = (int)SymTable_hash(pcKey, bucket_cnts[bindex]);
    cur = oSymTable->buckets[hvalue];
    prev = oSymTable->buckets[hvalue];
    while (cur != NULL)
    {
        if (!(strcmp(pcKey, cur->key)))
        {
            bnext = cur->next;
            pvValue = (void*)(cur->value);
            if (oSymTable->buckets[hvalue] == cur)
            {
                oSymTable->buckets[hvalue] = bnext;
            }
            else
            {
                prev->next = bnext;
            }
            free((void*)cur->key);
            cur->key = NULL;
            free(cur);
            cur = NULL;
            oSymTable->bindings--;
        }
        else
        {
            prev = cur;
            cur=cur->next;
        }
    }
        return pvValue;
  }

  /*Applies function *pfApply to all bindings in oSymTable with
  extra parameter pvExtra*/
  void SymTable_map(SymTable_T oSymTable,
     void (*pfApply)(const char *pcKey, void *pvValue, void *pvExtra),
     const void *pvExtra)
     {
        size_t i = 0;
        while (i < oSymTable->length)
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




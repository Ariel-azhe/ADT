#include "symtable.h"

size_t bindex = 0;
size_t bucket_cnts[] = {509, 1021, 2039, 4093, 8191, 16381, 32749, 65521};
    struct Binding {
    const char *key;
    const void* value;
    struct Binding *next;
    };
    struct Table {
    struct Binding **buckets;
    size_t bindings;
    size_t length;
    };

    static void testLargeTable(int iBindingCount)
    {
   enum {MAX_KEY_LENGTH = 10};

   SymTable_T oSymTable;
   SymTable_T oSymTableSmall;
   char acKey[MAX_KEY_LENGTH];
   char *pcValue;
   int i;
   int iSmall;
   int iLarge;
   int iSuccessful;
   size_t uLength = 0;
   size_t uLength2;

   printf("------------------------------------------------------\n");
   printf("Testing a potentially large SymTable object.\n");
   printf("No output except CPU time consumed should appear here:\n");
   fflush(stdout);

   /* Note the current time. */

   /* Create oSymTableSmall, and put a couple of bindings into it. */
   oSymTableSmall = SymTable_new();
   ASSURE(oSymTableSmall != NULL);
   iSuccessful = SymTable_put(oSymTableSmall, "xxx", "xxx");
   ASSURE(iSuccessful);
   iSuccessful = SymTable_put(oSymTableSmall, "yyy", "yyy");
   ASSURE(iSuccessful);

   /* Create oSymTable, the primary SymTable object. */
   oSymTable = SymTable_new();
   ASSURE(oSymTable != NULL);

   /* Put iBindingCount new bindings into oSymTable.  Each binding's
      key and value contain the same characters. */
   for (i = 0; i < iBindingCount; i++)
   {
      sprintf(acKey, "%d", i);
      pcValue = (char*)malloc(sizeof(char) * (strlen(acKey) + 1));
      ASSURE(pcValue != NULL);
      strcpy(pcValue, acKey);
      iSuccessful = SymTable_put(oSymTable, acKey, pcValue);
      ASSURE(iSuccessful);
      uLength = SymTable_getLength(oSymTable);
      ASSURE(uLength == (size_t)(i+1));
   }

   /* Get each binding's value, and make sure that it contains
      the same characters as its key. */
   iSmall = 0;
   iLarge = iBindingCount - 1;
   while (iSmall < iLarge)
   {
      /* Get the smallest of the remaining bindings. */
      sprintf(acKey, "%d", iSmall);
      pcValue = (char*)SymTable_get(oSymTable, acKey);
      ASSURE(pcValue != NULL);
      ASSURE((pcValue != NULL) && (strcmp(pcValue, acKey) == 0));
      iSmall++;
      /* Get the largest of the remaining bindings. */
      sprintf(acKey, "%d", iLarge);
      pcValue = (char*)SymTable_get(oSymTable, acKey);
      ASSURE(pcValue != NULL);
      ASSURE((pcValue != NULL) && (strcmp(pcValue, acKey) == 0));
      iLarge--;
   }
   /* Get the middle binding -- if there is one. */
   if (iSmall == iLarge)
   {
      sprintf(acKey, "%d", iSmall);
      pcValue = (char*)SymTable_get(oSymTable, acKey);
      ASSURE(pcValue != NULL);
      ASSURE((pcValue != NULL) && (strcmp(pcValue, acKey) == 0));
   }

   /* Remove each binding. Also free each binding's value. */
   iSmall = 0;
   iLarge = iBindingCount - 1;
   while (iSmall < iLarge)
   {
      /* Remove the smallest of the remaining bindings. */
      sprintf(acKey, "%d", iSmall);
      pcValue = (char*)SymTable_remove(oSymTable, acKey);
      ASSURE(pcValue != NULL);
      ASSURE((pcValue != NULL) && (strcmp(pcValue, acKey) == 0));
      free(pcValue);
      uLength--;
      uLength2 = SymTable_getLength(oSymTable);
      ASSURE(uLength2 == uLength);
      iSmall++;
      /* Remove the largest of the remaining bindings. */
      sprintf(acKey, "%d", iLarge);
      pcValue = (char*)SymTable_remove(oSymTable, acKey);
      ASSURE(pcValue != NULL);
      ASSURE((pcValue != NULL) && (strcmp(pcValue, acKey) == 0));
      free(pcValue);
      uLength--;
      uLength2 = SymTable_getLength(oSymTable);
      ASSURE(uLength2 == uLength);
      iLarge--;
   }
   /* Remove the middle binding -- if there is one. */
   if (iSmall == iLarge)
   {
      sprintf(acKey, "%d", iSmall);
      pcValue = (char*)SymTable_remove(oSymTable, acKey);
      ASSURE(pcValue != NULL);
      ASSURE((pcValue != NULL) && (strcmp(pcValue, acKey) == 0));
      free(pcValue);
      uLength--;
      uLength2 = SymTable_getLength(oSymTable);
      ASSURE(uLength2 == uLength);  
   }

   /* Make sure oSymTableSmall hasn't been corrupted by expansion
      of oSymTable. */
   pcValue = (char*)SymTable_get(oSymTableSmall, "xxx");
   ASSURE((pcValue != NULL) && (strcmp(pcValue, "xxx") == 0));
   pcValue = (char*)SymTable_get(oSymTableSmall, "yyy");
   ASSURE((pcValue != NULL) && (strcmp(pcValue, "yyy") == 0));

   /* Free both SymTable objects. */
   SymTable_free(oSymTable);
   SymTable_free(oSymTableSmall);
}
    
   int main(void)
    {
        int iBindingCount;

        #ifndef S_SPLINT_S
   setCpuTimeLimit();
#endif

   testBasics();
   testKeyComparison();
   testKeyOwnership();
   testRemove();
   testMap();
   testEmptyTable();
   testEmptyKey();
   testNullValue();
   testLongKey();
   testTableOfTables();
   testCollisions();
   testLargeTable(iBindingCount);

   printf("------------------------------------------------------\n");
   return 0;
    }


/*creates new empty symbol table or returns NULL
    if insufficient memory*/
    SymTable_T SymTable_new(void)
    {
        SymTable_T sym = (struct Table*)calloc(1, sizeof(struct Table));
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
    (struct Binding**)realloc(oSymTable->buckets, bucket_cnts[bindex++]*sizeof(struct Binding));
    size_t i = 0;
    if (new_buckets == NULL)
    {
        return;
    }
    oSymTable->length = bucket_cnts[bindex];
    oSymTable->buckets = new_buckets;
    while (i < bucket_cnts[bindex - 1])
    {
        struct Binding *cur = oSymTable->buckets[i];
        int prev_hval = 0;
        int hvalue = 0;
        struct Binding *hnext = NULL;
        struct Binding *newB = NULL;
        struct Binding *prev = NULL;
        struct Binding *pnext = NULL;
        while (cur != NULL)
        {
            prev_hval = SymTable_hash(cur->key, bucket_cnts[bindex-1]);
            hvalue = SymTable_hash(cur->key, oSymTable->length);
            if (prev_hval != hvalue)
            {
                hnext = oSymTable->buckets[hvalue];
                newB = (struct Binding*)calloc(1, sizeof(struct Binding));
                newB->key = (const char*)malloc(strlen(cur->key) + 1);
                strcpy((char*)newB->key, cur->key);
                newB->value = cur->value;
                newB->next = hnext;
                oSymTable->buckets[hvalue] = newB;
                pnext = cur->next;
                if (prev == cur)
                {
                    oSymTable->buckets[prev_hval] = pnext;
                }
                else if (prev != NULL)
                {
                    prev->next = pnext;
                }
                free((void*)cur->key);
                cur->key = NULL;
                prev = cur;
                free(cur);
            }
            prev = cur;
            cur=cur->next;
        }
        i++;
    }
}



    /*frees all memory occupied by oSymTable*/
  void SymTable_free(SymTable_T oSymTable)
  {
    size_t i = 0;
    while (i < oSymTable->length)
    {
        struct Binding *cur = oSymTable->buckets[i];
        while (cur != NULL)
        {
            free((void*)cur->key);
            cur->key = NULL;
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
        hvalue = SymTable_hash(pcKey, bucket_cnts[bindex]);
        if (SymTable_contains(oSymTable, pcKey))
        {
            return 0;
        }
        cur = oSymTable->buckets[hvalue];
        newB = (struct Binding*)calloc(1, sizeof(struct Binding));
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
        hvalue = SymTable_hash(pcKey, bucket_cnts[bindex]);
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
    hvalue = SymTable_hash(pcKey, bucket_cnts[bindex]);
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
    hvalue = SymTable_hash(pcKey, bucket_cnts[bindex]);
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
    hvalue = SymTable_hash(pcKey, bucket_cnts[bindex]);
    cur = oSymTable->buckets[hvalue];
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




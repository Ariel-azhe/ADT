#include "symtable.h"

enum { BUCKET_COUNT = 509 };
struct Binding {
 const char *key;
 int value;
 struct Binding *next;
};
struct Table {
 struct Binding *buckets[BUCKET_COUNT];
};
typedef struct Table *SymTable_T;
size_t bindings = 0;

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

  }

  /*returns the number of bindings in oSymTable*/
  size_t SymTable_getLength(SymTable_T oSymTable);

  /*Adds a new binding with key pcKey and value pvValue to oSymTable
  if oSymTable doesn't contain a binding with key pcKey already.
  Return 0 and leave oSymTable unchanged otherwise.*/
  int SymTable_put(SymTable_T oSymTable,
     const char *pcKey, const void *pvValue);
    
    /*Change the value of the binding within oSymTable
    with key pcKey to pvValue if such binding exists in 
    oSymTable. Return NULL otherwise.*/
  void *SymTable_replace(SymTable_T oSymTable,
     const char *pcKey, const void *pvValue);

     /*Return 1 if oSymTable contains a binding with key pcKey.
     Return 0 otherwise.*/
  int SymTable_contains(SymTable_T oSymTable, const char *pcKey);

  /*Return the value of the binding with key pcKey if such key 
  exists in oSymTable. Return NULL otherwise.*/
  void *SymTable_get(SymTable_T oSymTable, const char *pcKey);

  /*Remove the binding with key pcKey in oSymTable and return
  the value of that binding if that binding exists.
  Return NULL otherwise.*/
  void *SymTable_remove(SymTable_T oSymTable, const char *pcKey);

  /*Applies function *pfApply to all bindings in oSymTable with
  extra parameter pvExtra*/
  void SymTable_map(SymTable_T oSymTable,
     void (*pfApply)(const char *pcKey, void *pvValue, void *pvExtra),
     const void *pvExtra);
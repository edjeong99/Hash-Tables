#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
  Hash table key/value pair with linked list pointer.

  Note that an instance of `LinkedPair` is also a node in a linked list.
  More specifically, the `next` field is a pointer pointing to the the 
  next `LinkedPair` in the list of `LinkedPair` nodes. 
 */
typedef struct LinkedPair {
  char *key;
  char *value;
  struct LinkedPair *next;
} LinkedPair;

/*
  Hash table with linked pairs.
 */
typedef struct HashTable {
  int capacity;
  LinkedPair **storage;
} HashTable;

/*
  Create a key/value linked pair to be stored in the hash table.
 */
LinkedPair *create_pair(char *key, char *value)
{
  LinkedPair *pair = malloc(sizeof(LinkedPair));
  pair->key = strdup(key);
  pair->value = strdup(value);
  pair->next = NULL;

  return pair;
}

/*
  Use this function to safely destroy a hashtable pair.
 */
void destroy_pair(LinkedPair *pair)
{
  if (pair != NULL) {
    free(pair->key);
    free(pair->value);
    free(pair);
  }
}

/*
  djb2 hash function

  Do not modify this!
 */
unsigned int hash(char *str, int max)
{
  unsigned long hash = 5381;
  int c;
  unsigned char * u_str = (unsigned char *)str;

  while ((c = *u_str++)) {
    hash = ((hash << 5) + hash) + c;
  }

  return hash % max;
}

/*
  Fill this in.

  All values in storage should be initialized to NULL
 */
HashTable *create_hash_table(int capacity)
{
 
  HashTable *ht = malloc(sizeof(HashTable));

  ht->capacity = capacity;

// not sure about what should be in sizeof ...
  ht->storage = calloc(capacity, sizeof(LinkedPair*));
  return ht;
 
}

/*
  Fill this in.

  Inserting values to the same index with different keys should be
  added to the corresponding LinkedPair list.

  Inserting values to the same index with existing keys can overwrite
  the value in th existing LinkedPair list.
 */
void hash_table_insert(HashTable *ht, char *key, char *value)
{
// create a new pair
 LinkedPair *new_pair = create_pair(key, value);
 // get hash table index
 unsigned int hash_index = hash(key, ht->capacity);

 
 // if the hash_index doesn't have any element
 if(ht->storage[hash_index] == NULL){
   ht->storage[hash_index] = new_pair;
 printf("inserted 1st : %s\n", ht->storage[hash_index]->value);
 
   return;
 }

//printf("inserted before While :\n");
 
 // keep track of where you are in link by using current_pair
 LinkedPair *current_pair = ht->storage[hash_index];
 LinkedPair *prev_pair = NULL;
 // move to next pair until reaching last element or found a pair with matching key
  while(current_pair && strcmp(current_pair->key, key) != 0 ){
    prev_pair = current_pair;
    current_pair = current_pair->next;
   }

// if last element is reached, add new pair to last link.
  if(current_pair == NULL){   
    current_pair = new_pair;
  }
  else{  // if matching key is found, replace value
    current_pair->value = value;
  }
  printf("inserted 2nd %s\n", current_pair->value);
 
}

/*
  Fill this in.

  Should search the entire list of LinkedPairs for existing
  keys and remove matching LinkedPairs safely.

  Don't forget to free any malloc'ed memory!
 */
void hash_table_remove(HashTable *ht, char *key)
{
  unsigned int hash_index = hash(key, ht->capacity);
 
  LinkedPair *current_pair = ht->storage[hash_index];
  LinkedPair *prev_pair= ht->storage[hash_index];

  // if ht index doesn't have any element
  if(!current_pair){
    printf("ht remove - no element in ht index\n");
    return;
  }

  // if 1st element in link match the key
  if(strcmp(current_pair->key, key) == 0){
    destroy_pair(current_pair);
    ht->storage[hash_index] = NULL;
    return;
  }

// check 2nd element till end of link
current_pair = current_pair->next;
while(current_pair){
  if(strcmp(current_pair->key, key) == 0){
    prev_pair->next = current_pair->next;
    destroy_pair(current_pair);
  }
  // move prev/current pair to next link element
  prev_pair = prev_pair->next;
  current_pair = current_pair->next;
}
}
/*
  Fill this in.

  Should search the entire list of LinkedPairs for existing
  keys.

  Return NULL if the key is not found.
 */
char *hash_table_retrieve(HashTable *ht, char *key)
{
  unsigned int hash_index = hash(key, ht->capacity);
 
  LinkedPair *current_pair = ht->storage[hash_index];

 while(current_pair){
  if(strcmp(current_pair->key, key) == 0){
 //   printf("retrieve  value = %s\n",current_pair->value );
    return current_pair->value;
  }
  current_pair = current_pair->next;
 }
  return NULL;
}

/*
  Fill this in.

  Don't forget to free any malloc'ed memory!
 */
void destroy_hash_table(HashTable *ht)
{
    LinkedPair *current_pair;
    LinkedPair *prev_pair;
    unsigned int hash_index ;
// free all Pair
for(int i =0; i < ht->capacity; i++){
  current_pair = ht->storage[i];
  
  if(current_pair){
    prev_pair = current_pair;
    current_pair = current_pair->next;
    destroy_pair(prev_pair);
  }
}

//free storage
free(ht->storage);
// free hash table
free(ht);

}

/*
  Fill this in.

  Should create a new hash table with double the capacity
  of the original and copy all elements into the new hash table.

  Don't forget to free any malloc'ed memory!
 */
HashTable *hash_table_resize(HashTable *ht)
{
  HashTable *new_ht = malloc(sizeof(HashTable));
  new_ht->capacity = 2 * ht->capacity;
  // create 2x size for storage 
  new_ht->storage = calloc(new_ht->capacity, sizeof(LinkedPair*));
  
   LinkedPair *current_pair;

  // copy all elements
  for (int i = 0; i < ht->capacity; i++){
    current_pair = ht->storage[i];
    while(current_pair){
      hash_table_insert(new_ht, current_pair->key, current_pair->value );
      current_pair = current_pair->next;
    }
  }

  destroy_hash_table(ht);
  return new_ht;
}


#ifndef TESTING
int main(void)
{
  struct HashTable *ht = create_hash_table(2);

  hash_table_insert(ht, "line_1", "Tiny hash table\n");
  hash_table_insert(ht, "line_2", "Filled beyond capacity\n");
  hash_table_insert(ht, "line_3", "Linked list saves the day!\n");

  printf("%s", hash_table_retrieve(ht, "line_1"));
  printf("%s", hash_table_retrieve(ht, "line_2"));
  printf("%s", hash_table_retrieve(ht, "line_3"));

  int old_capacity = ht->capacity;
  ht = hash_table_resize(ht);
  int new_capacity = ht->capacity;

  printf("\nResizing hash table from %d to %d.\n", old_capacity, new_capacity);

  destroy_hash_table(ht);

  return 0;
}
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "index.h"

struct index_ {
  int *key; /*array de claves */
  int *n;  /*array de numero de elementos que contiene cada clave */
  int numberKeys;
  int **pos; /*Array donde guardas todos los registros */
  type_t *tipos;
};




/* 
   Function: int index_create(char *path, int type)

   Creates a file for saving an empty index. The index is initialized
   to be of the specific tpe (in the basic version this is always INT)
   and to contain 0 entries.

   Parameters:
   path:  the file where the index is to be created
   type:  the type of the index (always INT in this version)

   Returns:
   1:   index created
   0:   parameter error or file creation problem. Index not created.
 */
int index_create(char *path, type_t type) {
  FILE *f;
    
  f=fopen(path, "w+");
  if(f==NULL){
      fprintf(stderr, "Error abriendo el fichero");
      return 0;
  }

  fwrite(type, sizeof(type_t), 1, f);

  fclose(f);

  return 1;
}



/* 
   Opens a previously created index: reads the contents of the index
   in an index_t structure that it allocates, and returns a pointer to
   it (or NULL if the files doesn't exist or there is an error). 

   NOTE: the index is stored in memory, so you can open and close the
   file in this function. However, when you are asked to save the
   index, you will not be given the path name again, so you must store
   in the structure either the FILE * (and in this case you must keep
   the file open) or the path (and in this case you will open the file
   again).

   Parameters:
   path:  the file where the index is 

   Returns:
   pt:   index opened
   NULL: parameter error or file opening problem. Index not opened.

 */
index_t* index_open(char* path) {
  FILE *f;
  index_t *t;
  
  
  f=fopen(path, "a+");
  if(f==NULL){
      fprintf(stderr, "Error abriendo el fichero");
      return 0;
  }
  
  t = (index_t*)malloc(sizeof(index_t));
  if(t==NULL) 
    return NULL;

  fseek(f, 0,SEEK_SET);
  
  t->tipos=(type_t*)malloc(sizeof(type_t));
  fread(t->tipos, sizeof(type_t), 1, f);


  return t;
}


/* 
   int index_save(index_t* index);

   Saves the current state of index in the file it came from. Note
   that the name of the file in which the index is to be saved is not
   given.  See the NOTE to index_open.  

   Parameters:
   index:  the index the function operates upon

   Returns:
   1:  index saved
   0:  error saving the index (cound not open file)

*/
int index_save(index_t* idx) {
  return 0;
}


/* 
   Function: int index_put(index_t *index, int key, long pos);

   Puts a pair key-position in the index. Note that the key may be
   present in the index or not... you must manage both situation. Also
   remember that the index must be kept ordered at all times.

   Parameters:
   index:  the index the function operates upon
   key: the key of the record to be indexed (may or may not be already
        present in the index)
   pos: the position of the corresponding record in the table
        file. This is the datum that we will want to recover when we
        search for the key.

   Return:
   n>0:  after insertion the file now contains n unique keys
   0:    error inserting the key

*/
int index_put(index_t *idx, int key, long pos) {
  return 0;
}


/* 
   Function: long *index_get(index_t *index, int key, int* nposs);

   Retrieves all the positions associated with the key in the index. 

   Parameters:
   index:  the index the function operates upon
   key: the key of the record to be searched
   nposs: output paramters: the number of positions associated to this key
   
   Returns:

   pos: an array of *nposs long integers with the positions associated
        to this key
   NULL: the key was not found   

   NOTE: the parameter nposs is not an array of integers: it is
   actually an integer variable that is passed by reference. In it you
   must store the number of elements in the array that you return,
   that is, the number of positions associated to the key. The call
   will be something like this:

   int n
   long **poss = index_get(index, key, &n);

   for (int i=0; i<n; i++) {
       Do something with poss[i]
   }

   ANOTHER NOTE: remember that the search for the key MUST BE DONE
   using binary search.

   FURTHER NOTE: the pointer returned belongs to this module. The
   caller guarantees that the values returned will not be changed.

*/
long *index_get(index_t *idx, int key, int* nposs) {
  return NULL;
}


/* 
   Closes the index by freeing the allocated resources. No operation
   on the index will be possible after calling this function. 

   Parameters:
   index:  the index the function operates upon

   Returns:
   Nothing

   NOTE: This function does NOT save the index on the file: you will
   have to call the function index_save for this.
*/
void index_close(index_t *idx) {
  return;
}


/*
  Function: long **index_get_order(index_t *index, int n, int* nposs);

  Function useful for debugging but that should not be used otherwise:
  returns the nth record in the index. DO NOT USE EXCEPT FOR
  DEBUGGING. The test program uses it.

   Parameters:
   index:  the index the function operates upon
   n: number of the record to be returned
   nposs: output paramters: the number of positions associated to this key
   
   Returns:

   pos: an array of *nposs long integers with the positions associated
        to this key
   NULL: the key was not found   


   See index_get for explanation on nposs and pos: they are the same stuff
*/
long *index_get_order(index_t *idx, int n, int* nposs) {
  return NULL;
}


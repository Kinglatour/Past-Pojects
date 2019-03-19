// Barry Latour
// Ba333597

#include <stdio.h>
#include <stdlib.h>
#include "LonelyPartyArray.h"

//++++++++++++++++++++++++++ Create Lonely Party Array +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

LonelyPartyArray *createLonelyPartyArray(int num_fragments, int fragment_length)
{
  int i;
  
  // Checking for valid arguments
  if( (num_fragments <= 0) || (fragment_length <= 0) )
    return NULL;

  LonelyPartyArray *party =  NULL; 

  party = malloc(sizeof(LonelyPartyArray));
  party->fragments = malloc(sizeof(int*) * num_fragments);
  party->fragment_sizes = malloc(sizeof(int) * num_fragments);
 
  // Free array if malloc fails 
  if(party->fragment_sizes == NULL){
    free(party->fragment_sizes);
    return NULL;
  }
   
  if(party->fragments == NULL){
    free(party->fragments);
    return NULL;
  }
  
  if(party == NULL){
    return NULL;
  } 
 
  party->size = 0;
  party->num_fragments = num_fragments;
  party->fragment_length = fragment_length;
  party->num_active_fragments = 0;
  
  // Setting elements of fragments array to NULL
  for(i = 0; i < num_fragments; i++){
    party->fragments[i] = NULL; 
  }
  
  // Setting elements of size array to 0
  for(i = 0; i < num_fragments; i++){
    party->fragment_sizes[i] = 0;
  }
  
  printf("-> A new LonelyPartyArray has emerged from the void. (capacity: %d, fragments: %d)\n",
    (party->num_fragments * party->fragment_length), party->num_fragments);
  
  return party;	
}

//++++++++++++++++++++++++++ Destroy Lonely Party Array ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

LonelyPartyArray *destroyLonelyPartyArray(LonelyPartyArray *party)
{
  int i, j;
  
  if( party == NULL )
    return NULL;
  
  j = party->num_fragments;
  
  for(i = 0; i < j; i++){
    free(party->fragments[i]);
  }
  
  free(party->fragment_sizes);
  free(party->fragments);
  free(party);
  
  printf("-> The LonelyPartyArray has returned to the void.\n");
  
	return NULL;
}

//++++++++++++++++++++++++++ Clone Lonely Party Array ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

LonelyPartyArray *cloneLonelyPartyArray(LonelyPartyArray *party)
{
  int i, j;
  
  if( party == NULL )
    return NULL;
  
  LonelyPartyArray *party_clone =  NULL;
  party_clone = malloc(sizeof(LonelyPartyArray));
  party_clone->fragments = malloc(sizeof(int*) * party->fragment_length);
  party_clone->fragment_sizes = malloc(sizeof(int) * party->fragment_length);
  
  // Free array if malloc fails 
  if( party_clone->fragments == NULL ){
    free(party_clone->fragments);
    return NULL;
  }
  
  if( party_clone->fragment_sizes == NULL ){
    free(party_clone->fragment_sizes);
    return NULL;
  }
  
  if( party_clone == NULL )
    return NULL;
    
  party_clone->size = party->size;
  party_clone->num_fragments = party->num_fragments;
  party_clone->fragment_length = party->fragment_length;
  party_clone->num_active_fragments = party->num_active_fragments;
  
  /** 
  * For loop(1), checks if party->fragments[] is not NULL, if so then allocate memory
  * for party_clone->fragments[]. Else set party_clone->fragments[i] to NULL.
  * For loop(2), iterates through party_clone->fragments[][] and sets each cell to
  * equal party->fragments[][].
  **/
  
  // Begin  for loop(1)
  for( i = 0; i < party->num_fragments; i++){
    if( party->fragments[i] != NULL ){
      party_clone->fragments[i] = malloc(sizeof(int) * party->fragment_length);
      if( party_clone->fragments[i] == NULL ){
        free(party_clone->fragments[i]);
        return NULL;
      }
    }
    else{
      party_clone->fragments[i] = NULL;
      continue;
    }
    // Begin  for loop(2)
    for(j = 0; j < party->fragment_length; j++){
      party_clone->fragments[i][j] = party->fragments[i][j];
    }
    // End of for loop(2)
  }
  // End of for loop(1)
  
  // Sets party_clone->fragment_sizes[] to equal party->fragment_sizes[]
  for( i = 0; i < party->num_active_fragments; i++){
    party_clone->fragment_sizes[i] = party->fragment_sizes[i];
  }
  
  printf("-> Successfully cloned the LonelyPartyArray. (capacity: %d, fragments: %d)\n",
    (party_clone->num_fragments * party_clone->fragment_length), party_clone->num_fragments);
  
  return party_clone;
}

//++++++++++++++++++++++++++ Set +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

int set(LonelyPartyArray *party, int index, int key)
{
  int i;
  
  if( party == NULL ){
    printf("-> Bloop! NULL pointer detected in set().\n");
    return LPA_FAILURE;
  }
    
  if( (index < 0) || (index > (party->num_fragments * party->fragment_length -1))){
    printf("-> Bloop! Invalid access in set(). (index: %d, fragment: %d, offset: %d)\n", index, (index / party->fragment_length), (index % party->fragment_length));
    return LPA_FAILURE;
  } 
  
  // Creates new party->fragments[] arrary if party->fragments[] equals NULL
  if( party->fragments[(index / party->fragment_length)] == NULL ){
    party->fragments[(index / party->fragment_length)] = malloc(sizeof(int*) * party->fragment_length);
      if(  party->fragments[(index / party->fragment_length)] == NULL ){
        free(party->fragments[(index / party->fragment_length)]);
        return LPA_FAILURE;
      }
    /** 
    * Sets all indecies party->infragments[][] to UNUSED.
    * Then incrementes relavent elements in struct and inserts key. 
    **/  
    for(i = 0; i < party->fragment_length; i++){
      party->fragments[(index / party->fragment_length)][i] = UNUSED;
    }
    
    party->num_active_fragments++;
    party->fragments[(index / party->fragment_length)][(index % party->fragment_length)] = key;
    party->fragment_sizes[(index / party->fragment_length)]++;
    party->size++;
    
    printf("-> Spawned fragment %d. (capacity: %d, indices: %d..%d)\n",
      (index / party->fragment_length), party->fragment_length, ((index / party->fragment_length) * party->fragment_length), (((index / party->fragment_length) * party->fragment_length)) + (party->fragment_length - 1));
    return LPA_SUCCESS;
  }
  /**
  * If party->fragments[] is not NULL and party->fragments[][] is UNUSED insert key
  * and incrementes relavent elements in struct.
  * If party->fragments[] is not NULL and party->fragments[][] is not UNUSED only insert key.
  **/
  else{
    if( party->fragments[(index / party->fragment_length)][(index % party->fragment_length)] == UNUSED){
      party->fragments[(index / party->fragment_length)][(index % party->fragment_length)] = key;
      party->fragment_sizes[(index / party->fragment_length)]++;
      party->size++;
    }
    else{
      party->fragments[(index / party->fragment_length)][(index % party->fragment_length)] = key;     
    }
  }  
	return LPA_SUCCESS;
}

//++++++++++++++++++++++++++ Get +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

int get(LonelyPartyArray *party, int index)
{
  if( party == NULL ){
    printf("-> Bloop! NULL pointer detected in get().\n");
    return LPA_FAILURE;
  }
  
  if( (index < 0) || (index > (party->num_fragments * party->fragment_length -1))){
    printf("-> Bloop! Invalid access in get(). (index: %d, fragment: %d, offset: %d)\n", index, (index / party->fragment_length), (index % party->fragment_length));
    return LPA_FAILURE;
  }
  
  if( party->fragments[(index / party->fragment_length)] == NULL ){
    return UNUSED;
  }
  
	return party->fragments[(index / party->fragment_length)][(index % party->fragment_length)];
}

//++++++++++++++++++++++++++ Delete ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

int delete(LonelyPartyArray *party, int index)
{
  if( party == NULL ){
    printf("-> Bloop! NULL pointer detected in delete().\n");
    return LPA_FAILURE;
  }
    
  if( (index < 0) || (index > (party->num_fragments * party->fragment_length -1))){
    printf("-> Bloop! Invalid access in delete(). (index: %d, fragment: %d, offset: %d)\n", index, (index / party->fragment_length), (index % party->fragment_length));
    return LPA_FAILURE;
  }
    
  if( party->fragments[(index / party->fragment_length)] == NULL ){
    return LPA_FAILURE;
  }
    
  if( party->fragments[(index / party->fragment_length)][(index % party->fragment_length)] == UNUSED){
    return LPA_FAILURE; 
  }
  
  /**
  * If party->fragments[][] is not empty, set party->fragments[][] to UNUSED and
  * decrement relavent elements in struct.
  * If party->fragment_sizes[] is 0 deallocate party->fragment[] and decrement 
  * relavent elements in struct.
  **/
  if( party->fragments[(index / party->fragment_length)][(index % party->fragment_length)] != UNUSED ){
    party->fragments[(index / party->fragment_length)][(index % party->fragment_length)] = UNUSED;
    party->size--;
    party->fragment_sizes[(index / party->fragment_length)]--;
  
    if( party->fragment_sizes[(index / party->fragment_length)] == 0 ){
      free(party->fragments[(index / party->fragment_length)]);
      party->fragments[(index / party->fragment_length)] = NULL;
      party->num_active_fragments--;
      printf("-> Deallocated fragment %d. (capacity: %d, indices: %d..%d)\n",
        (index / party->fragment_length), party->fragment_length, ((index / party->fragment_length) * party->fragment_length), (((index / party->fragment_length) * party->fragment_length)) + (party->fragment_length - 1));
    }
  }
  return LPA_SUCCESS;
}

//++++++++++++++++++++++++++ Print If Valid ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

int printIfValid(LonelyPartyArray *party, int index)
{
  if( party == NULL )
    return LPA_FAILURE;
    
  if( (index < 0) || (index > (party->num_fragments * party->fragment_length -1)))
    return LPA_FAILURE;
    
  if( party->fragments[(index / party->fragment_length)] == NULL )
    return LPA_FAILURE;
    
  if( party->fragments[(index / party->fragment_length)][(index % party->fragment_length)] == UNUSED)
    return LPA_FAILURE;
    
  printf("%d\n",party->fragments[(index / party->fragment_length)][(index % party->fragment_length)]);

	return LPA_SUCCESS;
}

//++++++++++++++++++++++++++ Reset Lonely Party Array ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

LonelyPartyArray *resetLonelyPartyArray(LonelyPartyArray *party)
{
  int i;
  
  if( party == NULL ){
    printf("-> Bloop! NULL pointer detected in resetLonelyPartyArray().\n");
    return party;
  }
  
  /**
  * For loop, if party->fragments[] is not NULL, free party->fragments[] and
  * set party->fragments[] to NULL and set party->fragment_sizes[] to 0.
  **/
  for( i = 0; i < party->num_fragments; i++ ){
    if( party->fragments[i] != NULL ){
      free(party->fragments[i]);
      party->fragments[i] = NULL;
      party->fragment_sizes[i] = 0;
    }
  
  // Set relavent struct elements to 0  
  party->size = 0;
  party->num_active_fragments = 0;
  
  }
  
  printf("-> The LonelyPartyArray has returned to its nascent state. (capacity: %d, fragments: %d)\n", 
    (party->num_fragments * party->fragment_length), party->num_fragments);
  
	return party;	
}

//++++++++++++++++++++++++++ Get Size ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

int getSize(LonelyPartyArray *party)
{ 
  if( party == NULL )
    return -1;	
 
  return party->size;
}

//++++++++++++++++++++++++++ Get Capacity ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

int getCapacity(LonelyPartyArray *party)
{
  if( party == NULL )
    return -1;	
 
  return (party->num_fragments * party->fragment_length);	
}

//++++++++++++++++++++++++++ Get Allocated Cell Count ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

int getAllocatedCellCount(LonelyPartyArray *party)
{
  if( party == NULL )
    return -1;	
 
  return (party->num_active_fragments * party->fragment_length);
}

//++++++++++++++++++++++++++ Get Array Size In Bytes++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

long long unsigned int getArraySizeInBytes(LonelyPartyArray *party)
{
  if( party == NULL )
    return 0;

	return (long long unsigned int)(party->num_fragments * party->fragment_length) * (long long unsigned int)(sizeof(int));
}

//++++++++++++++++++++++++++ Get Current Size In Bytes +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

long long unsigned int getCurrentSizeInBytes(LonelyPartyArray *party)
{
  long long unsigned int size;
  
  if( party == NULL )
    return 0;
  
  size = (long long unsigned int)sizeof(LonelyPartyArray*) + (long long unsigned int)sizeof(LonelyPartyArray) + (long long unsigned int)(sizeof(int*) * party->num_fragments) + 
    (long long unsigned int)(sizeof(int) * party->num_fragments) + (long long unsigned int)(sizeof(int) * party->num_active_fragments * party->fragment_length);
  
	return size;
}

//++++++++++++++++++++++++++ Difficulty Rating +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

double difficultyRating(void)
{
	return 2.0;
}

//++++++++++++++++++++++++++ Hours Spent +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

double hoursSpent(void)
{
	return 17.0;
}

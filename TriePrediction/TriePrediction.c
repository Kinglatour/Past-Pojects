// Barry Latour
// Ba333597

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "TriePrediction.h"

//-----------------------------------------------------------------------------------------------------//
//------------------------------------- Helper Functions Begin ----------------------------------------//
//-----------------------------------------------------------------------------------------------------//

// Helper function called by printTrie(). (Credit: Dr. S.)
void printTrieHelper(TrieNode *root, char *buffer, int k)
{
	int i;

	if (root == NULL)
		return;

	if (root->count > 0)
		printf("%s (%d)\n", buffer, root->count);

	buffer[k + 1] = '\0';

	for (i = 0; i < 26; i++)
	{
		buffer[k] = 'a' + i;

		printTrieHelper(root->children[i], buffer, k + 1);
	}

	buffer[k] = '\0';
}

// If printing a subtrie, the second parameter should be 1; otherwise, if
// printing the main trie, the second parameter should be 0. (Credit: Dr. S.)
void printTrie(TrieNode *root, int useSubtrieFormatting)
{
	char buffer[1026];

	if (useSubtrieFormatting)
	{
		strcpy(buffer, "- ");
		printTrieHelper(root, buffer, 2);
	}
	else
	{
		strcpy(buffer, "");
		printTrieHelper(root, buffer, 0);
	}
}

// createNode fuction taken from class notes
TrieNode *createNode(void)
{
	int i;
	TrieNode *root = malloc(sizeof(TrieNode));

	root->count = 0;
	for (i = 0; i < 26; i++)
		root->children[i] = NULL;
    root->subtrie = NULL;

	return root;
}

// insterString function taken from class notes
TrieNode *insertString(TrieNode *root, char *str)
{
	int i, index, len = strlen(str);
	TrieNode *temp;

	if (root == NULL)
		root = createNode();

	temp = root;

	for (i = 0; i < len; i++)
	{
		if (!isalpha(str[i]))
			continue;

		index = tolower(str[i]) - 'a';

		if (temp->children[index] == NULL)
			temp->children[index] = createNode();

		temp = temp->children[index];
	}

	temp->count++;
	return root;
}

// subInstertString function taken from class notes with slight modification.
// Used to instert string into subtrie
TrieNode *subInsertString(TrieNode *root, char *str)
{
  int i, index, len = strlen(str);
	TrieNode *temp;
  
  // Changed root to root->subtrie
	if (root->subtrie == NULL)
		root->subtrie = createNode();

	temp = root->subtrie;

	for (i = 0; i < len; i++)
	{
		if (!isalpha(str[i]))
			continue;

		index = tolower(str[i]) - 'a';

		if (temp->children[index] == NULL)
			temp->children[index] = createNode();

		temp = temp->children[index];
	}

	temp->count++;
	return root;
}

// Recurseive helper fuction for most frequent word
void getMostFrequentWordHelper(TrieNode *root, char *str, char *buffer, int k, int *freq)
{
  int i;

  // If NULL reached the end of branch, return 
  if (root == NULL)
		return;
  
  // Copies buffer to str if buffer count is larger than current string count and sets freq to
  // to equal the count of string in buffer
	if(root->count > freq[0])
	{
		strcpy(str, buffer);
 	  freq[0] = root->count;  
 	}

	buffer[k + 1] = '\0';
  
  // Adds a character to buffer as the trie is traversed
	for (i = 0; i < 26; i++)
	{
		buffer[k] = 'a' + i;
		getMostFrequentWordHelper(root->children[i], str, buffer, k + 1, freq);
	}

  // Empty buffer
	buffer[k] = '\0';
}

// Recurseive helper fuction for prefix count
void prefixCountHelper(TrieNode *root, int *count)
{
  int i;
  
  // If NULL reached the end of branch, return
	if (root == NULL)
		return;
  
  // If word is found add to count 
	if(root->count > 0)
	  count[0] += root->count;
  
  // Traversing through trie 
	for (i = 0; i < 26; i++)
	{
		if(root->children[i])
			prefixCountHelper(root->children[i], count);
	}
}

// Fuction to return the terminal node if any string in trie
TrieNode *getAnyNode(TrieNode *root, char *str)
{    
  int i, index, len = strlen(str);
	TrieNode *temp;

	temp = root;
  
  // If empty string is passed return original trie
	if(len == 0)
	  return root;
  
  // Tranversing troungh trie while ignoring any non-ahpla characters in string
	for(i = 0; i < len; i++)
	{
		if (!isalpha(str[i]))
			continue;

		index = tolower(str[i]) - 'a';
    
    // If next node is NULL then string is not in trie, return NULL 
		if(temp->children[index] == NULL)
		  return NULL;
      
		temp = temp->children[index];
	}

	// return terminal node
  return temp;
}
//-----------------------------------------------------------------------------------------------------//
//------------------------------------- Helper Functions End ------------------------------------------//
//-----------------------------------------------------------------------------------------------------//

//---------------------------------------- buildTrie --------------------------------------------------//
TrieNode *buildTrie(char *filename)
{
  TrieNode *root = NULL;
 	TrieNode *subroot = NULL;
 	char subbuffer[MAX_CHARACTERS_PER_WORD + 1];
	char buffer[MAX_CHARACTERS_PER_WORD + 1];
	int len, count = 0;
 
	subbuffer[0] = '\0';

	FILE *ifp;

	if ((ifp = fopen(filename, "r")) == NULL)
	  return NULL;

	// Inserting strings one-by-one into the trie
	while (fscanf(ifp, "%s", buffer) != EOF)
  {
    // Insert buffer into trie
    root = insertString(root, buffer);
    
    len = strlen(subbuffer);
      
   	// If word in subbuffer is the end of a sentence do not instert following word into subtrie
    if((subbuffer[len - 1] == '.') || (subbuffer[len - 1] == '?') || (subbuffer[len - 1] == '!'))
		{
		  strcpy(subbuffer, buffer);
  		subroot = NULL;
   	} 
    
   	// If there is root to insert subbuffer,  insert subbuffer, otherwise
    // do nothing
   	if((subroot != NULL))
    {
      strcpy(subbuffer, buffer);
      subInsertString(subroot, subbuffer);
    }
    
    // Get terminal node of string just inserted into trie and pass it to subroot
    // which will be used to insert string into substrie   
   	subroot = getNode(root, buffer);
  
 	}

	fclose(ifp);
	return root; 
}

//------------------------------------ processInputFile -----------------------------------------------//
int processInputFile(TrieNode *root, char *filename)
{
  TrieNode *subroot;
  char buffer[MAX_CHARACTERS_PER_WORD + 1];
  char *str = calloc(MAX_CHARACTERS_PER_WORD + 1, sizeof(char));
  int i, count = 0;
  
  FILE *ifp;
  
  if ((ifp = fopen(filename, "r")) == NULL)
  return 1;
   
  // Read data in input file string by string
  while (fscanf(ifp, "%s", buffer) != EOF)
	{
    // If '!' is read print trie
	  if (buffer[0] == '!')
 		  printTrie(root, 0);
    
    // If '@' is read, read and copy the next two strings and print str
  	else if(buffer[0] == '@')
  	{
      fscanf(ifp, "%s", buffer);
 		  strcpy(str, buffer);
   		fscanf(ifp, "%s", buffer);
   		count = atoi(buffer);
   		printf("%s", str);
      
      // Print next "n" words where n is the number stored in count
   		for(i = 0; i < count; i++)
   		{
        // Get the terminal node if str containing its subtrie
        subroot = getNode(root, str);
        
        // If root or root->subtrie doe not exist stop printing
     		if((subroot == NULL) || (subroot->subtrie == NULL))
    			break;
        
        // Get most frequent word in subtrie of current root and print that word           
     		getMostFrequentWord(subroot->subtrie, str);
     		printf(" %s", str);
   		}
      
 		  printf("\n");
  	}
    
    // Print string in buffer and subtrie of the string in buffer if they exist
  	else
  	{
      printf("%s\n", buffer);
   		subroot = getNode(root, buffer);
      
   		if(subroot == NULL)
   		  printf("(INVALID STRING)\n");
   		else if(subroot->subtrie == NULL)
   		  printf("(EMPTY)\n");
   		else
   		  printTrie(subroot->subtrie, 1);
  	}   
	}	
  
	free(str); 
	return 0;
}

//------------------------------------- destroyTrie ---------------------------------------------------//
TrieNode *destroyTrie(TrieNode *root)
{
  int i, j;
  
 	if(root == NULL)
 	  return NULL;
  
	for(i = 0; i < 26; i++)
	{
		if(root->children[i])
 	    destroyTrie(root->children[i]);
 	}
      
  free(root);  
	return NULL;
}

//--------------------------------------- getNode -----------------------------------------------------//
TrieNode *getNode(TrieNode *root, char *str)
{    
	int i, index, len = strlen(str);
	TrieNode *temp;

	temp = root;
  
  // If empty string is passed return original trie
	if(len == 0)
	  return root;
     
  // Tranversing troungh trie while ignoring any non-ahpla characters in string
	for (i = 0; i < len; i++)
	{
		if (!isalpha(str[i]))
			continue;

		index = tolower(str[i]) - 'a';

		if(temp->children[index] == NULL)
		  return NULL;

    temp = temp->children[index];
	}
 
  // Return node if word is in trie NULL otherwise 
  return (temp->count >= 1)? temp:NULL;
}

//-------------------------------- getMostFrequentWord ------------------------------------------------//
void getMostFrequentWord(TrieNode *root, char *str)
{  
  char buffer[1026];
	int freq[1] = {0};
	
  // Remove any garbage data from str
	str[0] = '\0';
 
	getMostFrequentWordHelper(root, str, buffer, 0, freq);
}

//----------------------------------- containsWord ----------------------------------------------------//
int containsWord(TrieNode *root, char *str)
{
  TrieNode *temp = root;
 	int i, index, len = strlen(str);
  
  // Tranversing troungh trie while ignoring any non-ahpla characters in string
 	for (i = 0; i < len; i++)
 	{
    if (!isalpha(str[i]))
			continue;
      
 	  index = tolower(str[i]) - 'a';
    
   	if(temp->children[index] == NULL)
  		return 0;
      
   	temp = temp->children[index];
 	}
	
   // Return 1 if word is in trie 0 otherwise 
   return (temp->count >= 1)? 1:0;
}

//------------------------------------ prefixCount ----------------------------------------------------//
int prefixCount(TrieNode *root, char *str)
{
	int count[1] = {0};
	TrieNode *temp;
  
  // Get terminal node of string
 	temp = getAnyNode(root, str);
   
  // Passing the trie under the string prefix and an array to store the count  
 	prefixCountHelper(temp, count);

  return count[0];  
}

//----------------------------------- difficultyRating ------------------------------------------------//
double difficultyRating(void)
{
	return 5.0;
}

//------------------------------------- hoursSpent ----------------------------------------------------//
double hoursSpent(void)
{
	return 14.0;
}

//---------------------------------------- main -------------------------------------------------------//
int main(int argc, char **argv)
{
	char *corpus = argv[1];
	char *input = argv[2];
	TrieNode *root;
  
  root = buildTrie(corpus);
  processInputFile(root, input);
  destroyTrie(root);
  
	return 0;
}

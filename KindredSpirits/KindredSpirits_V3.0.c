// Barry Latour
// Ba333597

#include <stdio.h>
#include <stdlib.h>
#include "KindredSpirits.h"

//=========================================================================================//
//=================================== Helper Fuctions Begin ===============================//
//=========================================================================================//

// Fuction to create a new node in BST
// Taken from testcases
node *createNode(int data)
{
	node *n = malloc(sizeof(node));

	n->data = data;
	n->left = n->right = NULL;

	return n;
}

// Recursive fuction to count the number of nodes in a BST
// Taken from in class notes
int count_nodes(node *root)
{
  if (root == NULL)
  return 0;
  
  return 1 + count_nodes(root->left) + count_nodes(root->right);
}

// Recursive fuction to create an array containing the preorder traversal of a BST
int preArray(node *root, int array[], int i)
{
  array[i] = root->data;
  i++;
  
  if(root->left != NULL)
    i = preArray(root->left, array, i);
  if(root->right != NULL)
    i = preArray(root->right, array, i);

  return i;
}

// Fuction to create an array containing the postorder traversal of a BST
int postArray(node *root, int array[], int i)
{
  if(root->left != NULL)
    i = postArray(root->left, array, i);
  if(root->right != NULL)
    i = postArray(root->right, array, i);

  array[i] = root->data;
  i++;
  
  return i;
}

// Fuction to Check if BST is Kindred Spirits
int kindredSpiritsHelper(node *a, node *b)
{
  if(a == NULL && b == NULL)
    return 1;
  
  if(a == NULL || b == NULL)
    return 0;
    
  int *arrayPreA, *arrayPostB, node_count_a, node_count_b, i, compare = 1;
   
  // Count the number of nodes in each BST 
  node_count_a = count_nodes(a);
  node_count_b = count_nodes(b);
  
  // If number of nodes not equal then not kindred spirits 
  if(node_count_a != node_count_b)
    return 0;
    
  // Allocate space to store pre and postorder traversals of nodes a and b
  arrayPreA = calloc(node_count_a, sizeof(int));
  arrayPostB = calloc(node_count_a, sizeof(int));
  
  // Call function to input pre or postorder traversals into arrays
  preArray(a, arrayPreA, 0);
  postArray(b, arrayPostB, 0);

  // Compares the the pre and postorder traversals of nodes a and b
  for(i = 0; i < node_count_a; i++){
    if(arrayPreA[i] == arrayPostB[i])
      continue;
    else 
      compare = 0;
      free(arrayPreA);
      free(arrayPostB);
      return compare;
  }
    
  free(arrayPreA);
  free(arrayPostB);  

  return compare;
}
//=========================================================================================//
//================================= Helper Fuctions End ===================================//
//=========================================================================================//

//===================================== isReflection ======================================//
int isReflection(node *a, node *b)
{
  if(a == NULL && b == NULL)
    return 1;
    
  if(a == NULL || b == NULL)
    return 0;
    
  if(a->data != b->data)
    return 0;
    
  return isReflection(a->left, b->right) && isReflection(a->right, b->left);
}

//==================================== makeReflection =====================================//
node *makeReflection(node *root)
{
  if(root == NULL)
    return NULL;
    
  node *root2;
    
  root2 = createNode(root->data);
  
  // If their is a left node in root create a new node in root2 on the opposite side
  if(root->left != NULL){
    root2->right = createNode(root->left->data);
  }
  // If their is a right node in root create a new node in root2 on the opposite side
  if(root->right != NULL){
    root2->left = createNode(root->right->data);
  }
  
  // Move to the next left and right node and capture the address  
  root2->left = makeReflection(root->right);
  root2->right = makeReflection(root->left);
  
  return root2;
}
//==================================== kindredSpirits =====================================//

int kindredSpirits(node *a, node *b)
{
  //Method to check if Kindred Spirits found in Helper Functions section above
  return (kindredSpiritsHelper(a,b) || kindredSpiritsHelper(b,a) ? 1 : 0);
}

//=================================== difficultyRating ====================================//
double difficultyRating(void)
{
  return 3.0;
}

//====================================== hoursSpent =======================================//
double hoursSpent(void)
{
  return 7.0;
}
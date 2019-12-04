#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

struct tree{
  int key;
  struct tree *left;
  struct tree *right;
};

struct tree *talloc(void){
  return (struct tree *) malloc(sizeof(struct tree));
}

struct tree *construct_tree(int low, int high){
  struct tree *root = talloc();
  if(low == high){
    root->key = high;
    root->left = NULL;
    root->right = NULL;
    return root;
  }
  if(high-low == 1){
    root->key = high;
    root->right = NULL;
    struct tree *child = talloc();
    child->key = low;
    child->left = NULL;
    child->right = NULL;
    root->left = child;
    return root;
  }
  root->key = low + ((high - low)/2);
  struct tree *left_child = construct_tree(low, low + ((high - low)/2) -1);
  root->left = left_child;
  struct tree *right_child = construct_tree(low + ((high - low)/2) + 1, high);
  root->right = right_child;
  return root;
}

int tree_search(int low_high[2], int key, struct tree *p){
  if (p->key == key)
    return 1;
  else if (key < p->key && p->key < low_high[1])
    low_high[1] = p->key;
  else if (low_high[0] < p->key && p->key < key)
    low_high[0] = p->key;

  if (key < p->key)
    if (p->left == NULL)
      return 0;
    else if (tree_search(low_high, key, p->left) == 1)
      return 1;
    else
      return 0;

  else
    if (p->right == NULL)
      return 0;
    else if (tree_search(low_high, key, p->right) == 1)
      return 1;
    else
      return 0;
}

void min_max_search(int low_high[2], int key, struct tree *p){
  if (p == NULL)
    return;
  if (p->key < key){
    if (p->key > low_high[1])
      low_high[1] = p->key;
    min_max_search(low_high, key, p->right);
  }
  else if (key < p->key){
    if (p->key < low_high[0])
      low_high[0] = p->key;
    min_max_search(low_high, key, p->left);
  }
}

struct tree *remove_key(int key, struct tree *p){
  void min_max_search(int low_high[2], int key, struct tree *p);
  int min_max[2] = {INT_MAX, INT_MIN};

  if (p->key == key)
    if (p->left != NULL){
      min_max_search(min_max, p->key, p->left);
      p->key = min_max[1];
      p->left = remove_key(min_max[1], p->left);
      return p;
    }
    else if (p->right != NULL){
      min_max_search(min_max, p->key, p->right);
      p->key = min_max[0];
      p->right = remove_key(min_max[0], p->right);
      return p;
    }
    else{
      free(p);
      return NULL;
    }

  else if (key < p->key)
    p->left = remove_key(key, p->left);
  else
    p->right = remove_key(key, p->right);

  return p;
}

struct tree *insert_key(int key, struct tree *p){
  if (p == NULL){
    struct tree *root = talloc();
    root->key = key;
    root->left = NULL;
    root->right = NULL;
    return root;
  }
  else if (key <= p->key){
    struct tree *child = insert_key(key, p->left);
    p->left = child;
    return p;
  }
  else{
    struct tree *child = insert_key(key, p->right);
    p->right = child;
    return p;
  }
}

struct tree *update_set(int key, struct tree *p, struct tree **ind){
  int tree_search(int low_high[2], int key, struct tree *p);
  struct tree *remove_key(int key, struct tree *p);
  int min_max[2] = {INT_MIN, INT_MAX};
  struct tree *insert_key(int key, struct tree *p);

  if (tree_search(min_max, key, p) == 1){
    *ind = insert_key(key, *ind);
    return remove_key(key, p);
  }
  else if (key - min_max[0] < min_max[1] - key){
    *ind = insert_key(min_max[0], *ind);
    return remove_key(min_max[0], p);
  }
  else if (key - min_max[0] > min_max[1] - key){
    *ind = insert_key(min_max[1], *ind);
    return remove_key(min_max[1], p);
  }
  else{
    int coin = rand() % 2;
    if (coin == 0){
      *ind = insert_key(min_max[0], *ind);
      return remove_key(min_max[0], p);
    }
    else{
      *ind = insert_key(min_max[1], *ind);
      return remove_key(min_max[1], p);
    }
  }
}

void index_tree_to_index_array(int *place, int* ray, struct tree *p){
  if (p != NULL){
    index_tree_to_index_array(place, ray, p->left);
    ray[*place] = p->key;
    *place = (*place)+1;
    index_tree_to_index_array(place, ray, p->right);
  }
}

void generate_batch_indices(int seed, int data_size, int batch_size, int *batch_indices){
  struct tree *update_set(int key, struct tree *p, struct tree **ind);
  void index_tree_to_index_array(int *place, int* ray, struct tree *p);
  struct tree *construct_tree(int low, int high);

  struct tree *potentials = construct_tree(0, data_size-1);
  struct tree *forbidden = NULL;
  srand(seed);

  for (int k = 0; k < batch_size; k++)
    potentials = update_set(rand() % data_size, potentials, &forbidden);

  int place = 0;
  index_tree_to_index_array(&place, batch_indices, forbidden);
  free(potentials);
  free(forbidden);
}

int main(int argc, char const *argv[]) {
  void generate_batch_indices(int seed, int data_size, int batch_size, int *batch_indices);
  struct tree *construct_tree(int low, int high);
  void tree_print(struct tree *p);
  void index_tree_to_index_array(int *place, int* ray, struct tree *p);

  int seed = time(0);
  int data_size = 100;
  int batch_size = 7;
  printf("\n");
  int *batch_indices = malloc(batch_size*sizeof(int));
  for (int k = 0; k < 100; k++) {
    printf("Here is a random subset for n = %d, k = %d\n", data_size, batch_size);
    generate_batch_indices(rand(), data_size, batch_size, batch_indices);
    for (int i = 0; i < batch_size; i++) {
      printf("%d, ", batch_indices[i]);
    }
    printf("\n\n");
  }
  return 0;
}

void tree_print(struct tree *p){
  if (p != NULL) {
    tree_print(p->left);
    printf("%d, ", p->key);
    tree_print(p->right);
  }
}

// int tree_count(struct tree *p){
//   if (p != NULL) {
//     int count = 1;
//     count += tree_count(p->left);
//     count += tree_count(p->right);
//     return count;
//   }
//   else
//     return 0;
// }

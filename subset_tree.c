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

  if (p->key == key)
    if (p->left != NULL){
      int min_max[2] = {INT_MAX, INT_MIN};
      min_max_search(min_max, p->key, p->left);
      p->key = min_max[1];
      p->left = remove_key(min_max[1], p->left);
      return p;
    }
    else if (p->right != NULL){
      int min_max[2] = {INT_MAX, INT_MIN};
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

unsigned uabs(int x){
  return (unsigned) abs(x);
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
  else if (uabs(key - min_max[0]) < uabs(min_max[1] - key)){
    *ind = insert_key(min_max[0], *ind);
    return remove_key(min_max[0], p);
  }
  else if (uabs(key - min_max[0]) > uabs(min_max[1] - key)){
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

void generate_subsets(int seed, int set_size, int subset_size, int *subset){
  void tree_print(struct tree *p);
  struct tree *update_set(int key, struct tree *p, struct tree **ind);
  void index_tree_to_index_array(int *place, int* ray, struct tree *p);
  struct tree *construct_tree(int low, int high);
  void tree_free(struct tree *p);

  struct tree *potentials = construct_tree(0, set_size-1);
  struct tree *forbidden = NULL;

  for (int k = 0; k < subset_size; k++)
    potentials = update_set(rand() % set_size, potentials, &forbidden);

  int place = 0;
  index_tree_to_index_array(&place, subset, forbidden);
  tree_free(potentials);
  tree_free(forbidden);
}

void tree_free(struct tree *p){
  if (p != NULL) {
    tree_free(p->left);
    tree_free(p->right);
    free(p);
  }
}

int main(int argc, char const *argv[]) {
  clock_t start = clock();
  void generate_subsets(int seed, int set_size, int subset_size, int *subset);
  struct tree *construct_tree(int low, int high);
  void tree_print(struct tree *p);
  void index_tree_to_index_array(int *place, int* ray, struct tree *p);

  int set_size, subset_size, num_of_subsets;
  if (argc > 3){
    set_size = atoi(argv[1]);
    subset_size = atoi(argv[2]);
    num_of_subsets = atoi(argv[3]);
  }
  else{
    set_size = 10000;
    subset_size = 100;
    num_of_subsets = 5;
  }

  int seed = time(NULL);
  srand(seed);
  printf("\n");
  int *subset = malloc(subset_size*sizeof(int));
  for (int k = 0; k < num_of_subsets; k++) {
    generate_subsets(seed, set_size, subset_size, subset);    printf("{");
    for (int i = 0; i < subset_size-1; i++)
      printf("%d, ", subset[i]);
    printf("%d}\n", subset[subset_size-1]);
    printf("\n");
  }
  clock_t end = clock();
  printf("main() generated %d many %d-subsets of a set of size %d in\n", num_of_subsets, subset_size, set_size);
  printf("time = %lf\n\n", ((double) (end - start)) / CLOCKS_PER_SEC);
  return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

struct tree{
  int key;
  struct tree *left;
  struct tree *right;
};

struct tree *talloc(int key){
  struct tree * root = (struct tree *) malloc(sizeof(struct tree));
  root->key = key;
  root->left = NULL;
  root->right = NULL;
  return root;
}

struct tree *insert_key(int key, struct tree *p){
  if (p == NULL){
    struct tree *root = talloc(key);
    return root;
  }
  else if (key < p->key){
    struct tree *child = insert_key(key, p->left);
    p->left = child;
    return p;
  }
  else if (key > p->key){
    struct tree *child = insert_key(key, p->right);
    p->right = child;
    return p;
  }
  else
    return NULL;
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
  void index_tree_to_index_array(int *place, int* ray, struct tree *p);
  void tree_free(struct tree *p);

  struct tree *forbidden = NULL;
  struct tree *temp = NULL;

  for (int k = 0; k < subset_size; k++){
    if ((temp = insert_key(rand() % set_size, forbidden)) == NULL)
      k = k-1;
    else
      forbidden = temp;
  }

  int place = 0;
  index_tree_to_index_array(&place, subset, forbidden);
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
    generate_subsets(seed, set_size, subset_size, subset);
    printf("{");
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

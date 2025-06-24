#include "rbtree.h"
#include <stdlib.h>

rbtree * new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  p->nil = (node_t*)malloc(sizeof(node_t));
  p->root = NULL;
  p->nil->color = RBTREE_BLACK; 

  return p;
}

static void delete_subtree(node_t * node){
  if (node == NULL) return;
  delete_subtree(node->left);
  delete_subtree(node->right);
  free(node);
}

void delete_rbtree(rbtree *t) {
  delete_subtree(t->root);
  free(t);
}

node_t* make_new_node(const key_t key){
  node_t * new_node = (node_t*)malloc(sizeof(node_t));
  new_node->color = RBTREE_RED;
  new_node->key = key;

  return new_node;
}

node_t* right_rotate(rbtree* t, node_t* cur){
  node_t* child = cur->left;
  cur->left = child->right;
  
  if(child->right != t->nil){
    child->right->parent = cur;
  }
  //부모 갱신
  child->parent = cur->parent;

  //부모 노드의 자식 연결 바꿔주기 
  if(cur->parent == t->nil){
    t->root = cur;
  }
  else if(cur == cur->parent->left){
    cur->parent->left = child;
  }
  else{
    cur->parent->right = child;
  }

  child->right = cur;
  cur->parent = child;

  return cur;
}

node_t* left_rotate(rbtree* t, node_t* cur){
  node_t* child = cur->right;
  cur->right = child->left;
  
  if(child->left != t->nil){
    child->left->parent = cur;
  }
  //부모 갱신
  child->parent = cur->parent;

  //부모 노드의 자식 연결 바꿔주기 
  if(cur->parent == t->nil){
    t->root = cur;
  }
  else if(cur == cur->parent->left){
    cur->parent->left = child;
  }
  else{
    cur->parent->right = child;
  }

  child->left = cur;
  cur->parent = child;

  return cur;
}

node_t* rbtree_isnert_fixup(rbtree* t, node_t* new_node){
  node_t* uncle = t->nil;

  while(new_node->parent->color == RBTREE_RED){
    if(new_node->parent == new_node->parent->parent->left){
      uncle = new_node->parent->parent->right;
      if(uncle->color == RBTREE_RED){
        new_node->parent->color = RBTREE_BLACK;
        uncle->color = RBTREE_BLACK;
        uncle->parent->color = RBTREE_RED;
        new_node = uncle->parent;
      }
      else{
        if(new_node == new_node->parent->right){  //RL
          new_node = new_node->parent;
          new_node = left_rotate(t,new_node);
        }
        new_node->parent->color = RBTREE_BLACK;
        new_node->parent->parent = RBTREE_RED;
        new_node = right_rotate(t,new_node->parent->parent);
      }
    }
    else{
      uncle = new_node->parent->parent->left;
      if(uncle->color == RBTREE_RED){
        new_node->parent->color = RBTREE_BLACK;
        uncle->color = RBTREE_BLACK;
        uncle->parent->color = RBTREE_RED;
        new_node = uncle->parent;
      }      
      else{
        if(new_node == new_node->parent->left){
          new_node = new_node->parent;
          new_node = right_rotate(t,new_node);
        }
        new_node->parent->color = RBTREE_BLACK;
        new_node->parent->parent->color = RBTREE_RED;
        new_node = left_rotate(t,new_node->parent->parent);
      }
    }
  }
  t->root->color = RBTREE_BLACK;
  return t->root;
}

node_t* rbtree_insert(rbtree *t, const key_t key) {
  node_t* new_node = make_new_node(key);
  new_node->left = t->nil;
  new_node->right = t->nil;
  new_node->parent = t->nil;

  node_t* cur = t->root;
  node_t* parent = t->nil;

  while(cur != t->nil){
    parent = cur;
    if(key > cur->key){
      cur = cur->right;
    }
    else{
      cur = cur->left;
    }
  }
  new_node->parent = parent;
  if(parent == t->nil){
    t->root = new_node;
  }
  else if(parent->key < key){
    parent->right = new_node;
  }
  else{
    parent->left = new_node;
  }
  t->root = rbtree_isnert_fixup(t,new_node);
  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  if(!t->root) return NULL;
  node_t * ptr = t->root;

  while(ptr != t->nil){
    if(ptr->key < key){
      ptr = ptr->right;
    }
    else if(ptr->key == key) return ptr;
    else{
      ptr = ptr->left;
    }
  }
  if(ptr == t->nil) return NULL;
  return t->root;
}

node_t *rbtree_min(const rbtree *t) {
  if(t->root == t->nil) return NULL;
  node_t* ptr = t->root;
  while(ptr->left != t->nil){
    ptr = ptr->left;
  }
  return ptr;
}

node_t *rbtree_max(const rbtree *t) {
  if(t->root == t->nil) return t->root;
  node_t* ptr = t->root;
  while(ptr->right != t->nil){
    ptr = ptr->right;
  }
  return ptr;
}

int rbtree_erase(rbtree *t, node_t *p) {
  node_t* ptr = t->root;
  while(p->key != ptr->key){
    if(ptr->key < p->key){
      ptr = ptr->right;
    }
    else if(p->key == ptr->key){
      break;
    }
    else{
      ptr = ptr->left;
    }
  }
  if(ptr->parent == t->nil){
    t->root = NULL;
    free(ptr);
  }
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}

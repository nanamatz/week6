#include "rbtree.h"
#include <stdlib.h>

rbtree * new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  node_t* nil_node = (node_t*)malloc(sizeof(node_t));
  
  nil_node->color = RBTREE_BLACK;
  nil_node->left = nil_node;
  nil_node->right = nil_node;
  nil_node->parent = nil_node;

  p->nil = nil_node;
  p->root = nil_node;

  return p;
}

static void delete_subtree(rbtree* t, node_t* node){
  if (node == t->nil) return;
  delete_subtree(t,node->left);
  delete_subtree(t,node->right);
  free(node);
}

void delete_rbtree(rbtree *t) {
  delete_subtree(t,t->root);
  free(t->nil);
  free(t);
}

void rbtree_transplant(rbtree* t, node_t* remove_node, node_t* replace_node){
  if(remove_node->parent == t->nil){
    t->root = replace_node;
  }
  else if(remove_node == remove_node->parent->left){
    remove_node->parent->left = replace_node;
  }
  else{
    remove_node->parent->right = replace_node;
  }
  replace_node->parent = remove_node->parent;

  // free(remove_node);
}

node_t* find_successor(rbtree* t,node_t* cur){
  node_t* parent = t->nil;
  while(cur != t->nil){  
    parent = cur;
    cur = cur->left;
  }
  return parent;
}

node_t* rbtree_delete_fix_up(rbtree* t, node_t* node){
  node_t* brother = t->nil;
  while(node != t->root && node->color == RBTREE_BLACK){
    if(node == node->parent->left){
      brother = node->parent->right;
      if(brother->color == RBTREE_RED){ //형제가 red이면 left-rotate 후 2,3,4 케이스에서 해결
        brother->color = RBTREE_BLACK;
        node->parent->color = RBTREE_RED;
        left_rotate(t,node->parent);
        brother = node->parent->right;
      }
      if(brother->left->color == RBTREE_BLACK && brother->right->color == RBTREE_BLACK){
        brother->color = RBTREE_RED;
        node = node->parent;  //3,4케이스에서 해결
      }
      else{
        if(brother->right->color == RBTREE_BLACK){  //오른쪽 자식이 black 왼쪽 자식은 red
          brother->left->color = RBTREE_BLACK;
          brother->color = RBTREE_RED;
          right_rotate(t,brother);
          brother = node->parent->right;
        }
        brother->color = node->parent->color;
        node->parent->color = RBTREE_BLACK;
        brother->right->color = RBTREE_BLACK;
        left_rotate(t,node->parent);
        node = t->root;
      }
    }
    else{
      brother = node->parent->left;
      if(brother->color == RBTREE_RED){
        brother->color = RBTREE_BLACK;
        node->parent->color = RBTREE_RED;
        right_rotate(t,node->parent);
        brother = node->parent->left;
      }
      if(brother->left->color == RBTREE_BLACK && brother->right->color == RBTREE_BLACK){
        brother->color = RBTREE_RED;
        node = node->parent;  //3,4케이스에서 해결
      }
      else{
        if(brother->left->color == RBTREE_BLACK){  //오른쪽 자식이 black 왼쪽 자식은 red
          brother->right->color = RBTREE_BLACK;
          brother->color = RBTREE_RED;
          left_rotate(t,brother);
          brother = node->parent->left;
        }
        brother->color = node->parent->color;
        node->parent->color = RBTREE_BLACK;
        brother->left->color = RBTREE_BLACK;
        right_rotate(t,node->parent);
        node = t->root;
      }
    }
  }
  node->color = RBTREE_BLACK;
  
  return node;
}

node_t* rbtree_delete(rbtree* t, node_t* target){
  node_t* temp = target;
  node_t* replace = t->nil;
  color_t remove_color = target->color;
  //자식이 하나거나 없는 경우
  if(target->left == t->nil){ 
    replace = target->right;
    rbtree_transplant(t,target,target->right);
  }
  else if(target->right == t->nil){ 
    replace = target->left;
    rbtree_transplant(t,target,target->left); 
  }
  else{//자식이 둘인 경우
    temp = find_successor(t,target->right);
    remove_color = temp->color;
    replace = temp->right;
    if(temp != target->right){
      rbtree_transplant(t,temp,temp->right);
      temp->right = target->right;
      temp->right->parent = temp;
    }
    else replace->parent = temp;
    rbtree_transplant(t,target,temp);
    temp->left = target->left;
    temp->left->parent = temp;
    temp->color = target->color;
  }
  if(remove_color == RBTREE_BLACK){
    rbtree_delete_fix_up(t,replace);
  }
  free(target);
  return t->root;
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
    t->root = child;
  }
  else if(cur == cur->parent->left){
    cur->parent->left = child;
  }
  else{
    cur->parent->right = child;
  }

  child->right = cur;
  cur->parent = child;

  return child;
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
    t->root = child;
  }
  else if(cur == cur->parent->left){
    cur->parent->left = child;
  }
  else{
    cur->parent->right = child;
  }

  child->left = cur;
  cur->parent = child;

  return child;
}

void rbtree_insert_fixup(rbtree* t, node_t* cur){
  node_t* uncle = t->nil;

  while(cur->parent->color == RBTREE_RED){ //4번 속성 위반 
    if(cur->parent == cur->parent->parent->left){
      uncle = cur->parent->parent->right;
      if(uncle->color == RBTREE_RED){
        cur->parent->color = RBTREE_BLACK;
        uncle->color = RBTREE_BLACK;
        cur->parent->parent->color = RBTREE_RED;
        cur = cur->parent->parent;
      }
      else{ //uncle 이 nil이거나 black일 때
        if(cur == cur->parent->right){  //LR
          cur = cur->parent;
          left_rotate(t,cur);
        }
        cur->parent->color = RBTREE_BLACK;  //RR
        cur->parent->parent->color = RBTREE_RED;
        right_rotate(t,cur->parent->parent);
      }
    }
    else{
      uncle = cur->parent->parent->left;
      if(uncle->color == RBTREE_RED){
        cur->parent->color = RBTREE_BLACK;
        uncle->color = RBTREE_BLACK;
        cur->parent->parent->color = RBTREE_RED;
        cur = cur->parent->parent;
      }      
      else{
        if(cur == cur->parent->left){ //RL
          cur = cur->parent;
          right_rotate(t,cur);
        }
        cur->parent->color = RBTREE_BLACK;  //LL
        cur->parent->parent->color = RBTREE_RED;
        left_rotate(t,cur->parent->parent);
      }
    }
  }

  t->root->color = RBTREE_BLACK;
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

  rbtree_insert_fixup(t,new_node);

  return new_node;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  node_t * ptr = t->root;
  printf("have to find key value is : %d\n",key);
  while(ptr != t->nil){
    if(ptr->key == key){
      printf("\nwhat i found : %d\n",ptr->key);
      return ptr;
    } 
    else if(ptr->key > key) ptr = ptr->left;
    else ptr = ptr->right;
  }
  return NULL;
}

node_t* rbtree_min(const rbtree *t) {
  if(t->root == t->nil) return t->nil;
  node_t* ptr = t->root;
  while(ptr->left != t->nil){
    ptr = ptr->left;
  }
  return ptr;
}

node_t* rbtree_max(const rbtree *t) {
  if(t->root == t->nil) return t->nil;
  node_t* ptr = t->root;
  while(ptr->right != t->nil){
    ptr = ptr->right;
  }
  return ptr;
}

int rbtree_erase(rbtree *t, node_t *p) {
  if(p == t->nil) return 0;
  rbtree_delete(t,p);
  return 0;
}

// void inorder_rbtree(rbtree* t,node_t* cur, key_t* arr, int i){
//   if(cur == t->nil){
//     return;
//   }
//   inorder_rbtree(t,cur->left,arr,i);
//   arr[i++] = cur->key;
//   inorder_rbtree(t,cur->right,arr,i);
// }

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  node_t* p = t->nil;
  int i =0;
  while(t->root != t->nil){
    p = rbtree_min(t);
    arr[i++] = p->key;
    rbtree_delete(t,p);
  }
  return 0;
}

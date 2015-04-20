// avltree.h



#ifndef _EG_AVLTREE_H_
#define _EG_AVLTREE_H_



#define BASEPTR_TO_DERIVEDPTR(baseptr, derivedtype, basemember) ((derivedtype*)(((char*)baseptr)-(int)&((((derivedtype*)0)->basemember))))



struct eg_avltree_node
{
  struct eg_avltree_node *p;            // parent node pointer
  struct eg_avltree_node *l;            // left node pointer
  struct eg_avltree_node *r;            // right node pointer
  int b;                                // balance factor (-1, 0, +1)
};



// prototype for user defined comparison function
typedef int (*eg_avltree_compare_func)(struct eg_avltree_node *n1, struct eg_avltree_node *n2);



struct eg_avltree
{
  struct eg_avltree_node *root;         // root node pointer
  eg_avltree_compare_func cmpfunc;      // compare function pointer
};



void eg_avltree_init(struct eg_avltree *tree, eg_avltree_compare_func pf);



struct eg_avltree_node *eg_avltree_first(struct eg_avltree *tree);



struct eg_avltree_node *eg_avltree_last(struct eg_avltree *tree);



// returns next node (0 if no next node)
struct eg_avltree_node *eg_avltree_next(struct eg_avltree_node *n);



// returns previous node (0 if no previous node)
struct eg_avltree_node *eg_avltree_prev(struct eg_avltree_node *n);



// find node using user defined comparison function
//
// input:
//   tree - ptr to tree
//   n -    node, which is not part of the tree. it belongs to user
//          data type which contains value to search for.
//
// return:
//   0 - node not found
struct eg_avltree_node *eg_avltree_find(struct eg_avltree *tree, struct eg_avltree_node *n);



// insert node to tree
struct eg_avltree_node *eg_avltree_insert(struct eg_avltree *tree, struct eg_avltree_node *n);



// remove node from tree
//
// input:
//   tree - ptr to tree
//   n -    node, which is part of the tree.
//
// return:
//   0 - node not found
//   1 - removed successfully
//
int eg_avltree_remove(struct eg_avltree *tree, struct eg_avltree_node *n);



#endif    // _EG_AVLTREE_H_

# AVL Tree

This is a library which implements AVL Tree functionality.

#### Usage

Include "avltree.h" to your source file.

```c
#include "avltree.h"
```

Create your datatype and embed library datatype in it.

```c
struct mydata
{
  int myvalue1;
  struct eg_avltree_node avlnode;
};
```

Declare a macro to convert from library datatype to your datatype

```c
#define MYDATAPTR(pnode) BASEPTR_TO_DERIVEDPTR(pnode, struct mydata, avlnode)
```

Define a comparison function

```c
int mycmpfunc(struct eg_avltree_node *node1, struct eg_avltree_node *node2)
{
  struct mydata *p1 = MYDATAPTR(node1);
  struct mydata *p2 = MYDATAPTR(node2);

  return p1->myvalue - p2->myvalue;
}
```

Declare and initialize an AVL Tree

```c
struct eg_avltree tree;

eg_avltree_init(&tree, mycmpfunc);
```

Use a tree

```c
struct mydata data;
struct mydata tmp;
struct mydata *presult;
struct eg_avltree_node *pnode;

data.myvalue = 5;
eg_avltree_insert(&tree, &data.avlnode);

tmp.myvalue = 5;
pnode = eg_avltree_find(&tree, &tmp.avlnode);
if (pnode)
  presult = MYDATAPTR(pnode);  // 'presult' now points to 'data'
```

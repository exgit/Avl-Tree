// avltree.c  -  implementation



#include "avltree.h"



// forward declaration of helper functions
static void AdjustAfterIns(struct eg_avltree *tree, struct eg_avltree_node *son, struct eg_avltree_node *curr, int hc);
static void AdjustAfterDel(struct eg_avltree *tree, struct eg_avltree_node *curr, int hc);
static void Swap(struct eg_avltree_node *n1, struct eg_avltree_node *n2, struct eg_avltree *tree);



void eg_avltree_init(struct eg_avltree *tree, eg_avltree_compare_func pf)
{
  tree->root = 0;
  tree->cmpfunc = pf;
}



struct eg_avltree_node *eg_avltree_first(struct eg_avltree *tree)
{
  struct eg_avltree_node *n = tree->root;

  if (n)
  {
    while (n->l)
      n = n->l;
  }

  return n;
}



struct eg_avltree_node *eg_avltree_last(struct eg_avltree *tree)
{
  struct eg_avltree_node *n = tree->root;

  if (n)
  {
    while (n->r)
      n = n->r;
  }

  return n;
}



struct eg_avltree_node *eg_avltree_next(struct eg_avltree_node *n)
{
  if (n->r)
    for (n=n->r; n->l; n=n->l);
  else
  {
    for (; n; n=n->p)
    {
      if (n->p && n==n->p->l)
      {
        n = n->p;
        break;
      }
    }
  }

  return n;
}



struct eg_avltree_node *eg_avltree_prev(struct eg_avltree_node *n)
{
  if (n->l)
    for (n=n->l; n->r; n=n->r);
  else
    for (; n; n=n->p)
    {
      if (n->p && n==n->p->r)
      {
        n = n->p;
        break;
      }      
    }

  return n;
}



struct eg_avltree_node *eg_avltree_find(struct eg_avltree *tree, struct eg_avltree_node *n)
{
  struct eg_avltree_node* c = tree->root;
  int res = 0;

  while (c)
  {
    res = (tree->cmpfunc)(n, c);
    if (res < 0)
      c = c->l;
    else if (res > 0)
      c = c->r;
    else
      break;
  }

  return c;
}



struct eg_avltree_node *eg_avltree_insert(struct eg_avltree *tree, struct eg_avltree_node *n)
{
  struct eg_avltree_node *c = tree->root;

  // init new node members
  n->b = 0;
  n->l = n->r = 0;

  // tree is empty
  if (!c)
  {
    tree->root = n;
    n->p = 0;
    return 0;
  }

  // tree is not empty, find node to insert into
  int hc;  // height change factor (-1, 0, or +1)
  for (;;)
  {
    int res = (tree->cmpfunc)(n, c);
    if (res < 0)
    {
      if (c->l)
        c = c->l;
      else
      {
        c->l = n;
        hc = --(c->b);
        break;
      }
    }
    else if (res > 0)
    {
      if (c->r)
        c = c->r;
      else
      {
        c->r = n;
        hc = ++(c->b);
        break;
      }
    }
    else
      return c;
  }
  n->p = c;

  // adjust tree after insertion
  AdjustAfterIns(tree, n, c, hc);
  return 0;
}



int eg_avltree_remove(struct eg_avltree *tree, struct eg_avltree_node *n)
{
  struct eg_avltree_node *p;
  int hc = 0;

  // tree or given node is empty
  if (!tree->root || !n)
    return 0;

  // node have both childs - make it a leaf node before deletion
  if (n->l && n->r)
  {
    struct eg_avltree_node *m;	// node adjacent to n by value
    if (n->b < 0)  for (m=n->l; m->r; m=m->r) continue;
    else  for (m=n->r; m->l; m=m->l) continue;
    Swap(n, m, tree);
  }

  // node have no parent (root node)
  p = n->p;
  if (!p)
  {
    tree->root = n->l ? n->l : n->r;
    if (tree->root) tree->root->p=0;
    return 1;
  }

  if (n->l)                           // node have only left child
  {
    if (n==p->l)  {hc=-1; p->l=n->l;}
    else  {hc=1; p->r=n->l;}
    n->l->p = p;
  }
  else if (n->r)                      // node have only right child
  {
    if (n==p->l)  {hc=-1; p->l=n->r;}
    else  {hc=1; p->r=n->r;}
    n->r->p = p;
  }
  else if (n->l==0 && n->r==0)        // node have no childs
  {
    if (n==p->l)  {hc=-1; p->l=0;}
    else  {hc=1; p->r=0;}
  }

  // adjust tree after deletion
  AdjustAfterDel(tree, p, hc);
  return 1;
}



/***************************************************************************\
* helper functions                                                          *
\***************************************************************************/

static void AdjustAfterIns(struct eg_avltree *tree, struct eg_avltree_node *son, struct eg_avltree_node *curr, int hc)
{
  while (hc)
  {
    // go up one level
    son = curr;
    curr = curr->p;
    if (!curr)
      break;

    if (son==curr->l)
    {
      if (curr->b >= 0)
        hc = --(curr->b);
      else
      {
        struct eg_avltree_node *p = curr->p;
        if (hc < 0)           // rotate right
        {
          if (!p) tree->root=son;  else if (p->l==curr) p->l=son;  else p->r=son;
          son->p = p;

          if (0 != (curr->l = son->r))
            son->r->p = curr;

          son->r = curr;
          curr->p = son;

          son->b = curr->b = 0;
          break;
        }
        else                  // rotate left-right
        {
          struct eg_avltree_node* r = son->r;
          if (!p) tree->root=r;  else if (p->l==curr) p->l=r;  else p->r=r;
          r->p = p;

          if (0 != (curr->l = r->r))
            r->r->p = curr;

          if (0 != (son->r = r->l))
            r->l->p = son;

          r->l = son;
          son->p = r;

          r->r = curr;
          curr->p = r;

          son->b = r->b<=0 ? 0 : -1;
          curr->b = r->b>=0 ? 0 : 1;
          r->b = 0;
          break;
        }
      }
    }
    else
    {
      if (curr->b <= 0)
        hc = ++(curr->b);
      else
      {
        struct eg_avltree_node *p = curr->p;
        if (hc > 0)           // rotate left
        {
          if (!p) tree->root=son;  else if (p->l==curr) p->l=son;  else p->r=son;
          son->p = p;

          if (0 != (curr->r = son->l))
            son->l->p = curr;

          son->l = curr;
          curr->p = son;

          son->b = curr->b = 0;
          break;
        }
        else                  // rotate right-left
        {
          struct eg_avltree_node *l = son->l;
          if (!p) tree->root=l;  else if (p->l==curr) p->l=l;  else p->r=l;
          l->p = p;

          if (0 != (curr->r = l->l))
            l->l->p = curr;

          if (0 != (son->l = l->r))
            l->r->p = son;

          l->r = son;
          son->p = l;

          l->l = curr;
          curr->p = l;

          son->b = l->b>=0 ? 0 : 1;
          curr->b = l->b<=0 ? 0 : -1;
          l->b = 0;
          break;
        }
      }
    }
  }

  return;
}



static void AdjustAfterDel(struct eg_avltree *tree, struct eg_avltree_node *curr, int hc)
{
  struct eg_avltree_node *p;

  for (;;)
  {
    p = curr->p;
    if (hc < 0)
    {
      if (curr->b <= 0)
      {
        hc = (curr->b)++;
        if (hc && p) if (curr==p->r) hc=1;
      }
      else
      {
        struct eg_avltree_node *r = curr->r;
        if (r->b >= 0)        // rotate left
        {
          if (!p) tree->root=r;  else if (p->l==curr) {hc=-1;p->l=r;}  else {hc=1;p->r=r;}
          r->p = p;

          if (0 != (curr->r = r->l))
            r->l->p = curr;

          r->l = curr;
          curr->p = r;

          curr->b -= (r->b)--;
          if (r->b) hc=0;
        }
        else                  // rotate right-left
        {
          struct eg_avltree_node *l = r->l;
          if (!p) tree->root=l;  else if (p->l==curr) {hc=-1;p->l=l;}  else {hc=1;p->r=l;}
          l->p = p;

          if (0 != (curr->r = l->l))
            l->l->p = curr;

          if (0 != (r->l = l->r))
            l->r->p = r;

          l->r = r;
          r->p = l;

          l->l = curr;
          curr->p = l;

          curr->b = l->b>0 ? -1 : 0;
          r->b = l->b<0 ? 1 : 0;
          l->b = 0;
        }
      }
    }
    else
    {
      if (curr->b >= 0)
      {
        hc = (curr->b)--;
        if (hc && p) if (curr==p->l) hc=-1;
      }
      else
      {
        struct eg_avltree_node *l = curr->l;
        if (l->b <= 0)        // rotate right
        {
          if (!p) tree->root=l;  else if (p->l==curr) {hc=-1;p->l=l;}  else {hc=1;p->r=l;}
          l->p = p;

          if (0 != (curr->l = l->r))
            l->r->p = curr;

          l->r = curr;
          curr->p = l;

          curr->b -= (l->b)++;
          if (l->b) hc=0;
        }
        else                  // rotate left-right
        {
          struct eg_avltree_node *r = l->r;
          if (!p) tree->root=r;  else if (p->l==curr) {hc=-1;p->l=r;}  else {hc=1;p->r=r;}
          r->p = p;

          if (0 != (curr->l = r->r))
            r->r->p = curr;

          if (0 != (l->r = r->l))
            r->l->p = l;

          r->r = curr;
          curr->p = r;

          r->l = l;
          l->p = r;

          curr->b = r->b<0 ? 1 : 0;
          l->b = r->b>0 ? -1 : 0;
          r->b = 0;
        }
      }
    }

    if (!p || !hc)
      break;
    curr = p;
  }

  return;
}



static void Swap(struct eg_avltree_node *n1, struct eg_avltree_node *n2, struct eg_avltree *tree)
{
  struct eg_avltree_node tmp=(*n1);
  (*n1)=(*n2);
  (*n2)=tmp;

  // correction for adjacent nodes
  if (n1->p==n1)
  {
    n1->p=n2;
    if (n2->l==n2) n2->l=n1;
    if (n2->r==n2) n2->r=n1;

    if (n1->l) n1->l->p=n1;
    if (n1->r) n1->r->p=n1;
    if (n2->l) n2->l->p=n2;
    if (n2->r) n2->r->p=n2;

    if (!n2->p) tree->root=n2;  else if (n2->p->l==n1) n2->p->l=n2;  else n2->p->r = n2;
  }
  else
  {
    if (n1->l) n1->l->p=n1;
    if (n1->r) n1->r->p=n1;
    if (n2->l) n2->l->p=n2;
    if (n2->r) n2->r->p=n2;

    if (!n2->p) tree->root=n2;  else if (n2->p->l==n1) n2->p->l=n2;  else n2->p->r = n2;
    if (n1->p->l==n2) n1->p->l=n1;  else if (n1->p->r==n2) n1->p->r = n1;
  }
}

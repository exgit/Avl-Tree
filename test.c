// test



#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "avltree.h"



// forward references for test cases
// return value 0 means test passed, !0 - test failed
int TestCase1(void);
int TestCase2(void);
int TestCase3(void);
int TestCase4(void);
int TestCase5(void);



struct mydata
{
  int myvalue;
  struct eg_avltree_node avlnode;
};

#define MYDATAPTR(pnode) BASEPTR_TO_DERIVEDPTR(pnode, struct mydata, avlnode)



int mycmpfunc(struct eg_avltree_node *node1, struct eg_avltree_node *node2)
{
  struct mydata *p1 = MYDATAPTR(node1);
  struct mydata *p2 = MYDATAPTR(node2);

  return p1->myvalue - p2->myvalue;
}



int main(int argc, char **argv)
{
  int res;

  // init random number generator
  srand(2015);

  res = TestCase1();
  if (res) goto fail;

  res = TestCase2();
  if (res) goto fail;

  res = TestCase3();
  if (res) goto fail;

  res = TestCase4();
  if (res) goto fail;

  res = TestCase5();
  if (res) goto fail;

  printf("All tests passed! \n");
  return 0;

fail:
  printf("Error found! \n");
  return 1;
}



/***************************************************************************\
* helper functions                                                          *
\***************************************************************************/



// makes array of `mydata` contain random unique values
static void InitMydataArray(struct mydata *pArr, int nSize)
{
  int i, j, tmp;

  // zero memory
  memset(pArr, 0, nSize * sizeof(struct mydata));

  // fill with sequential numbers
  for (i=0; i<nSize; i++)
    pArr[i].myvalue = i;

  // random suffle
  for (i=0; i<nSize; i++)
  {
    j = rand() * (double)(nSize-1) / RAND_MAX;
    tmp = pArr[i].myvalue;
    pArr[i].myvalue = pArr[j].myvalue;
    pArr[j].myvalue = tmp;
  }
}



// checks if tree has a given size and its nodes are correctly sorted
//
// return:
//   0 - success (tree is correct)
//   1 - failure (tree is corrupt)
static int CheckSizeAndSort(struct eg_avltree *pTree, int nSize)
{
  int i=0, v=0;
  struct eg_avltree_node *pn;
  struct mydata *pd;

  // get first node and value
  pn = eg_avltree_first(pTree);
  if (!pn) goto checksize;
  pd = MYDATAPTR(pn);
  v = pd->myvalue;
  i = 1;

  // go through tree in ascending order
  for (;;)
  {
    pn = eg_avltree_next(pn);
    if (!pn) goto checksize;

    pd = MYDATAPTR(pn);
    if (pd->myvalue <= v) return 1;
    v = pd->myvalue;

    i++;
  }

checksize:
  // check size
  if (i != nSize)
    return 1;

  // success
  return 0;
}



/***************************************************************************\
* test cases                                                                *
\***************************************************************************/

int TestCase1(void)
{
  int ret = 1;
  int i;

  struct mydata tmp, *pmydata;
  struct mydata arr[10];
  struct eg_avltree tree;
  struct eg_avltree_node *pn1;

  // print test description
  printf("Test case 1: small tree and simple checks. \n");

  // init avl tree
  eg_avltree_init(&tree, mycmpfunc);

  // try to find 5 in the tree, should fail
  tmp.myvalue = 5;
  pn1 = eg_avltree_find(&tree, &tmp.avlnode);
  if (pn1) goto exit;

  // insert 10 values into tree
  printf("\n  ");
  InitMydataArray(arr, 10);
  for (i=0; i<10; i++)
  {
    printf("%d, ", arr[i].myvalue);
    eg_avltree_insert(&tree, &arr[i].avlnode);
  }
  printf("\n");

  // try now to find 5, should succeed
  tmp.myvalue = 5;
  pn1 = eg_avltree_find(&tree, &tmp.avlnode);
  if (!pn1) goto exit;

  // check if value is actually 5
  pmydata = MYDATAPTR(pn1);
  if (pmydata->myvalue != 5) goto exit;

  // check 10 inserted values
  printf("  ");
  for (pn1=eg_avltree_first(&tree), i=0; pn1 && i<10; pn1=eg_avltree_next(pn1), i++)
  {
    pmydata = MYDATAPTR(pn1);
    if (pmydata->myvalue != i)
      goto exit;
    printf("%d, ", pmydata->myvalue);
  }
  printf("\n\n\n");

  // must be 10 values in the tree
  if (i != 10) goto exit;

  // delete nodes one by one and perform checks
  for (i=0; i<10; i++)
  {
    eg_avltree_remove(&tree, &arr[i].avlnode);
    if (CheckSizeAndSort(&tree, 9-i)) goto exit;
  }

  // all tests passed, return success
  ret = 0;

exit:
  return ret;
}



int TestCase2(void)
{
  int ret = 1;

  int i, j, repeat, rnd;

  struct mydata *arr;
  int arr_size = 10000;
  int curr_size;

  struct eg_avltree tree;

  // print test description
  printf("Test case 2: medium tree and exhaustive checks. \n");

  // init data array
  arr = malloc(arr_size * sizeof(struct mydata));
  if (!arr) goto exit;

  // repeat test 10 times
  printf("\n");
  for (repeat=0; repeat<10; repeat++)
  {
    InitMydataArray(arr, arr_size);
    eg_avltree_init(&tree, mycmpfunc);
    curr_size = 0;

    for (i=0,j=0; i<arr_size; i++)
    {
      // insert value and test tree
      eg_avltree_insert(&tree, &arr[i].avlnode);
      curr_size++;
      if (CheckSizeAndSort(&tree, curr_size)) goto exit;

      // remove value with 10% probability and test tree again
      rnd = rand() * 99.0 / RAND_MAX;
      if (rnd < 10)
      {
        eg_avltree_remove(&tree, &arr[j++].avlnode);
        if (CheckSizeAndSort(&tree, --curr_size)) goto exit;
      }
    }

    printf("  Pass %d is done.  Tree size is %d. \n", repeat+1, curr_size);
  }
  printf("\n\n");

  // all tests passed, return success
  ret = 0;

exit:
  free(arr);
  return ret;
}



int TestCase3(void)
{
  int ret = 1;

  // all tests passed, return success
  ret = 0;

//exit:
  return ret;
}



int TestCase4(void)
{
  int ret = 1;

  // all tests passed, return success
  ret = 0;

//exit:
  return ret;
}



int TestCase5(void)
{
  int ret = 1;

  // all tests passed, return success
  ret = 0;

//exit:
  return ret;
}

#include <stdio.h>
#include <stdlib.h>
#define col(a) (a == 1 ? "BLACK" : "RED")
#define NIL &sentinel           /* all leafs are sentinels */

//CLRS 
//Insertion and Deletion in a Red Black Tree
enum type {RED,BLACK};

struct Node
{
    int data;
    struct Node* left;
    struct Node* right;
    struct Node* parent;
    enum type color;
};
struct Node sentinel = { 0, NIL, NIL, 0, BLACK};



void LeftRotate(struct Node** T,struct Node** x)
{
    struct Node* y = (*x)->right;
    (*x)->right = y->left;

    if(y->left!=NULL)
        y->left->parent = *x;

    y->parent = (*x)->parent;

    if((*x)->parent == NULL)
        *T = y;
	
    else if(*x == (*x)->parent->left)
        (*x)->parent->left = y;

    else
        (*x)->parent->right = y;

    y->left = *x;

    (*x)->parent = y;

}
void RightRotate(struct Node** T,struct Node** x)
{
    struct Node* y = (*x)->left;
    (*x)->left = y->right;

    if(y->right!=NULL)
        y->right->parent = *x;

    y->parent = (*x)->parent;

    if((*x)->parent==NULL)
        *T = y;

    else if((*x)== (*x)->parent->left)
        (*x)->parent->left = y;

    else
        (*x)->parent->right = y;

    y->right = *x;
    (*x)->parent = y;

}

void RB_insert_fixup(struct Node** T, struct Node** z)
{
    struct Node* grandparent = NULL;
    struct Node* parentpt = NULL;

    while(((*z)!=*T)&& ((*z)->color!= BLACK) && ((*z)->parent->color == RED))
    {
        parentpt = (*z)->parent;
        grandparent = (*z)->parent->parent;

        if(parentpt == grandparent->left)
        {
            struct Node* uncle = grandparent->right;

            if(uncle!=NULL && uncle->color == RED)
            {
                grandparent->color = RED;
                parentpt->color = BLACK;
                uncle->color = BLACK;
                *z = grandparent;
            }

            else
            {
                if((*z) == parentpt->right)
                {
                    LeftRotate(T,&parentpt);
                    (*z) = parentpt;
                    parentpt = (*z)->parent;
                }

                RightRotate(T,&grandparent);
                parentpt->color = BLACK;
                grandparent->color = RED;
                (*z) = parentpt;
            }
        }

        else
        {
            struct Node* uncle = grandparent->left;

            if(uncle!=NULL && uncle->color == RED)
            {
                grandparent->color = RED;
                parentpt->color = BLACK;
                uncle->color = BLACK;
                (*z) = grandparent;
            }

            else
            {
                if((*z) == parentpt->left)
                {
                    RightRotate(T,&parentpt);
                    (*z) = parentpt;
                    parentpt = (*z)->parent;
                }

                LeftRotate(T,&grandparent);
                parentpt->color = BLACK;
                grandparent->color = RED;
                (*z) = parentpt;
            }
        }
    }
    (*T)->color = BLACK;

}
	
struct Node* RB_insert(struct Node* T,int data)
{
    struct Node* z = (struct Node*)malloc(sizeof(struct Node));
    z->data = data;
    z->left = NULL;
    z->right = NULL;
    z->parent = NULL;
    z->color = RED;

    struct Node* y = NULL;
    struct Node* x = T;//root

    while(x!=NULL)
    {
        y = x;
        if(z->data < x->data)
            x = x->left;

        else
            x = x->right;
    }
    z->parent = y;

    if(y==NULL)
        T = z;

    else if(z->data < y->data)
        y->left = z;

    else
        y->right = z;

    RB_insert_fixup(&T,&z);

    return T;
}

struct Node* Tree_minimum(struct Node* node)
{
    while(node->left!=NULL)
        node = node->left;

    return node;
}
struct Node* Tree_max(struct Node* node,int *i)
{

    while(node->right!=NULL){
	if(node->color == BLACK) *i=*i+1;
        node = node->right;
    }
    if(node->color == BLACK) *i=*i+1;
    return node;
}
struct Node* Tree_min(struct Node* node,int *i)
{
    while(node->left!=NULL){
	if(node->color == BLACK) *i=*i+1;
        node = node->left;
    }
    if(node->color == BLACK) *i=*i+1;
    return node;
}
void RB_delete_fixup(struct Node** T, struct Node** x)
{
    while((*x)!=*T && (*x)->color == BLACK)
    {
        if((*x)==(*x)->parent->left)
        {
            struct Node* w = (*x)->parent->right;

            if(w->color==RED)
            {
                w->color = BLACK;
                (*x)->parent->color = BLACK;
                LeftRotate(T,&((*x)->parent));
                w = (*x)->parent->right;
            }

            if(w->left->color==BLACK && w->right->color == BLACK)
            {
                w->color = RED;
                (*x) = (*x)->parent;
            }

            else
            {
                if(w->right->color == BLACK)
                {
                    w->left->color = BLACK;
                    w->color = RED;
                    RightRotate(T,&w);
                    w = (*x)->parent->right;
                }

                w->color = (*x)->parent->color;
                (*x)->parent->color = BLACK;
                w->right->color = BLACK;
                LeftRotate(T,&((*x)->parent));
                (*x) = *T;
            }
        }

        else
        {
            struct Node* w = (*x)->parent->left;

            if(w->color==RED)
            {
                w->color = BLACK;
                (*x)->parent->color = BLACK;
                RightRotate(T,&((*x)->parent));
                w = (*x)->parent->left;
            }

            if(w->right->color==BLACK && w->left->color == BLACK)
            {
                w->color = RED;
                (*x) = (*x)->parent;
            }

            else
            {
                if(w->left->color == BLACK)
                {
                    w->right->color = BLACK;
                    w->color = RED;
                    LeftRotate(T,&w);
                    w = (*x)->parent->left;
                }

                w->color = (*x)->parent->color;
                (*x)->parent->color = BLACK;
                w->left->color = BLACK;
                RightRotate(T,&((*x)->parent));
                (*x) = *T;
            }
        }
    }
    (*x)->color = BLACK;

}

void RB_transplat(struct Node** T, struct Node** u,struct Node** v)
{
    if((*u)->parent == NULL)
        *T = *v;

    else if((*u)==(*u)->parent->left)
        (*u)->parent->left = *v;
    else
        (*u)->parent->right = *v;

    if((*v)!=NULL) 
        (*v)->parent = (*u)->parent;
}

struct Node* RB_delete(struct Node *T,struct Node* z)
{
    struct Node *y = z;
    enum type yoc;
    yoc = z->color; // y's original color

    struct Node* x;

    if(z->left==NULL )
    {
        x = z->right;
        RB_transplat(&T,&z,&(z->right));
    }

    else if(z->right==NULL )
    {
        x = z->left;
        RB_transplat(&T,&z,&(z->left));
    }

    else
    {
        y = Tree_minimum(z->right);
        yoc = y->color;
        x = y->right;
	if(x == NULL) x = NIL;

        if(y->parent==z)
            x->parent = y;

        else
        {
            RB_transplat(&T,&y,&(y->right));
            y->right = z->right;
            y->right->parent = y;
        }

        RB_transplat(&T,&z,&y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }
    if(x == NULL){
	x = NIL;	
	}
    if(yoc==BLACK)
        RB_delete_fixup(&T,&x);

    return T;
}

struct Node* BST_search(struct Node* root, int x)
{
    if(root==NULL || root->data == x)  return root;
       

    if(root->data > x)
       return  BST_search(root->left,x);
    else
        return BST_search(root->right,x);
}

int main()
{
    struct Node* RBT = NULL;
    for(int i = 0; i < 50000; i++){
    RBT = RB_insert(RBT,i);
    }

    int i = 0;
    int j = 0;
    struct Node* Rbtmin = Tree_min(RBT,&i);
    struct Node* Rbtmax = Tree_max(RBT,&j);	
    struct Node* x = BST_search(RBT,1234);
    if(i == j)
    	printf("Numbers Black nodes: %d\n",i);
    else printf("Error");
    RBT = RB_delete(RBT,x);


    return 0;
}

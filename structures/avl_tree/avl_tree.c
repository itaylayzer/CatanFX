#include "avl_tree.h"
#include <stdio.h>
Node make_tree(Node *tree)
{
    Node temp = calloc(sizeof(Node), 1);
    temp->height = 1;
    *tree = temp;
    return temp;
}

// safe get height element of Node
char safe_height(Node N)
{
    if (N == NULL)
        return 0;
    return N->height;
}
char balance_factor(Node N)
{
    if (N == NULL)
        return 0;
    return safe_height(N->left) - safe_height(N->right);
}

Node right_rotate(Node y)
{
    Node x = y->left;
    Node T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = bmax(safe_height(y->left), safe_height(y->right)) + 1;
    x->height = bmax(safe_height(x->left), safe_height(x->right)) + 1;

    return x;
}

Node left_rotate(Node x)
{
    Node y = x->right;
    Node T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = bmax(safe_height(x->left), safe_height(x->right)) + 1;
    y->height = bmax(safe_height(y->left), safe_height(y->right)) + 1;

    return y;
}

Node new_node(void *data)
{
    Node node = (Node)malloc(sizeof(NodeRec));
    node->data = data;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return node;
}

Node right_left_rotate(Node node)
{
    node->right = right_rotate(node->right);
    return left_rotate(node);
}

Node left_right_rotate(Node node)
{
    node->left = left_rotate(node->left);
    return right_rotate(node);
}

Node avl_insert(Node *root, void *element, signed char (*compare)(const void *, const void *))
{
    if (*root == NULL)
    {
        *root = new_node(element);
        return *root;
    }
    char cmp_result = compare(element, (*root)->data);
    if (cmp_result < 0)
    {
        (*root)->left = avl_insert(&((*root)->left), element, compare);
    }
    else if (cmp_result > 0)
    {
        (*root)->right = avl_insert(&((*root)->right), element, compare);
    }
    else
    {
        return *root;
    }

    (*root)->height = 1 + bmax(safe_height((*root)->left), safe_height((*root)->right));

    int balance = safe_height((*root)->left) - safe_height((*root)->right);
    if (balance > 1 && compare(element, (*root)->left->data) < 0)
    {
        return right_rotate(*root);
    }

    if (balance < -1 && compare(element, (*root)->right->data) > 0)
    {
        return left_rotate(*root);
    }

    if (balance > 1 && compare(element, (*root)->left->data) > 0)
    {
        return left_right_rotate(*root);
    }

    if (balance < -1 && compare(element, (*root)->right->data) < 0)
    {
        return right_left_rotate(*root);
    }

    return *root;
}
Node avl_search(Node node, void *element, signed char (*compare)(const void *, const void *))
{
    if (node == NULL)
    {
        return NULL;
    }
    else
    {
        char cmp_result = compare(element, node->data);
        if (!cmp_result)
        {
            return node;
        }
        if (cmp_result > 0)
        {
            return avl_search(node->right, element, compare);
        }
        if (cmp_result < 0)
        {
            return avl_search(node->right, element, compare);
        }
        return NULL;
    }
}
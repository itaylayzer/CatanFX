#include "avl_tree.h"

signed char safe_height(Node N)
{
    if (N == NULL)
        return 0;
    return N->height;
}

Node new_mode(void *key)
{
    Node node = malloc(sizeof(NodeRec));
    node->data = key;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return (node);
}

Node right_rotate(Node y)
{
    Node x = y->left;
    Node T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = bmax(safe_height(y->left),
                     safe_height(y->right)) +
                1;
    x->height = bmax(safe_height(x->left),
                     safe_height(x->right)) +
                1;

    return x;
}

Node left_rotate(Node x)
{
    Node y = x->right;
    Node T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = bmax(safe_height(x->left),
                     safe_height(x->right)) +
                1;
    y->height = bmax(safe_height(y->left),
                     safe_height(y->right)) +
                1;

    return y;
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

int calc_balance(Node N)
{
    if (N == NULL)
        return 0;
    return safe_height(N->left) - safe_height(N->right);
}

Node insert(Node node, void *element, signed char (*cmp)(const void *, const void *))
{
    if (node == NULL)
    {
        return new_mode(element);
    }
    signed char cmp_result = cmp(element, node->data);

    (cmp_result < 0) &&
        (node->left = insert(node->left, element, cmp));

    (cmp_result > 0) &&
        (node->right = insert(node->right, element, cmp));

    if (!cmp_result)
    {
        return node;
    }

    node->height = 1 + bmax(safe_height(node->left),
                            safe_height(node->right));

    int balance = calc_balance(node);

    // RR
    (balance > 1 && cmp(element, node->left->data) < 0) &&
        (node = right_rotate(node));

    // LL
    (balance < -1 && cmp(element, node->right->data) > 0) &&
        (node = left_rotate(node));

    // LR
    (balance > 1 && cmp(element, node->left->data) > 0) &&
        (node = left_right_rotate(node));

    // RL
    (balance < -1 && cmp(element, node->right->data) < 0) &&
        (node = right_left_rotate(node));

    return node;
}

void avl_insert(Node *root, void *element, signed char cmp(const void *, const void *))
{
    *root = insert(*root, element, cmp);
}

Node avl_search(Node node, void *element, signed char cmp(const void *, const void *))
{
    if (node == NULL)
    {
        return NULL;
    }
    else
    {
        signed char cmp_result = cmp(node->data, element);

        (cmp_result > 0) &&
            (node = avl_search(node->left, element, cmp));

        (cmp_result < 0) &&
            (node = avl_search(node->right, element, cmp));

        return node;
    }
}
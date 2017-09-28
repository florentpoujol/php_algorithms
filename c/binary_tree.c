#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#include "utils.c"


typedef struct BinaryTreeNode {
    int key;
    int value;
    int treeSize;
    struct BinaryTreeNode *parent;
    struct BinaryTreeNode *left;
    struct BinaryTreeNode *right;
} BinaryTreeNode;


BinaryTreeNode *findl(BinaryTreeNode *root, const int key)
{
    while (1) 
    {
        if (key == root->key) {
            return root;
        }

        if (key < root->key) {
            if (root->left) {
                root = root->left;
                continue;
            }
        } else if (root->right) {
            root = root->right;
            continue;
        }

        return NULL;
    }
}


BinaryTreeNode *findr(BinaryTreeNode *root, const int key)
{
    if (key == root->key) {
        return root;
    }

    if (key < root->key) {
        if (root->left) {
            return findr(root->left, key);
        }
    } else if (root->right) {
        return findr(root->right, key);
    }

    return NULL;
}


// sort loop
void sortl(BinaryTreeNode *root, BinaryTreeNode **nodes)
{
    // array of pointers toward BinaryTreeNode
    // BinaryTreeNode **nodes = calloc(root->treeSize, sizeof(BinaryTreeNode*));
    int *flags = calloc(root->treeSize, sizeof(int));

    const int LEFT_VISITED = 1; // 001
    const int RIGHT_VISITED = 2; // 010
    const int VALUE_ADDED = 4; // 100

    while (root != NULL) 
    {
        int key = root->key;

        if (~flags[key] & LEFT_VISITED) {
            flags[key] = flags[key] | LEFT_VISITED;

            if (root->left) {
                root = root->left;
                continue;
            }
        }

        if (~flags[key] & VALUE_ADDED) {
            flags[key] = flags[key] | VALUE_ADDED;

            nodes[key] = root;
        }

        if (~flags[key] & RIGHT_VISITED) {
            flags[key] = flags[key] | RIGHT_VISITED;

            if (root->right) {
                root = root->right;
                continue;
            }
        }

        root = root->parent;
    }

    free(flags);
}


// sort recurse
void sortr(BinaryTreeNode *root, BinaryTreeNode *nodesPtrPerKey[], int sortedKeys[], int *lastSortedKeyId_ptr)
{
    if (root->left) {
        sortr(root->left, nodesPtrPerKey, sortedKeys, lastSortedKeyId_ptr);
    }

    nodesPtrPerKey[root->key] = root;
    
    if (sortedKeys != NULL) {
        sortedKeys[ *lastSortedKeyId_ptr ] = root->key;
        *lastSortedKeyId_ptr += 1;
    }

    if (root->right) {
        sortr(root->right, nodesPtrPerKey, sortedKeys, lastSortedKeyId_ptr);
    }
}


/*
 * Called by balance()
 * Recursively balance the tree, reassigning nodes parent and children
 * @param array of pointer to nodes, provided by sort()
 * @param the sorted keys array, provided by sort()
 * @param the first id in sortedKeys to take into account 
 * @param the size of the portion fo sortedKeys
 */
BinaryTreeNode *_balance(BinaryTreeNode *nodesPtrPerKey[], int sortedKeys[], int startId, int size)
{   
    BinaryTreeNode* node;
    // printf("balance startId=%d, size=%d\n", startId, size);
    if (size == 1) {
        node = nodesPtrPerKey[ sortedKeys[ startId ] ];
        // node->parent = NULL;
        // node->left = NULL;
        // node->right = NULL;
        // printf("  k alone =%d \n", node->key);
    }
    else if (size == 2) {
        node = nodesPtrPerKey[ sortedKeys[ startId + 1 ] ];
        // node->parent = NULL;

        node->left = nodesPtrPerKey[ sortedKeys[ startId ] ];
        node->left->parent = node;
        // node->left->left = NULL;
        // node->left->right = NULL;

        // node->right = NULL;
        // printf("  k=%d paired with l=%d \n", node->key, node->left->key);
    }
    else { // size >= 3
        int relativeId = (int)(size / 2); // relative Id
        int actualMiddleId = startId + relativeId;

        node = nodesPtrPerKey[ sortedKeys[ actualMiddleId ] ];
        // node->parent = NULL;

        // printf("  middleId=%d k=%d \n", actualMiddleId, node->key);
        
        node->left = _balance(nodesPtrPerKey, sortedKeys, startId, relativeId);
        node->left->parent = node;
        // node->left->left = NULL;
        // node->left->right = NULL;

        node->right = _balance(nodesPtrPerKey, sortedKeys, actualMiddleId + 1, size - relativeId - 1);
        node->right->parent = node;
        // node->right->left = NULL;
        // node->right->right = NULL;

        // printf("  middleId=%d, k=%d paired with l=%d, r=%d \n", actualMiddleId, node->key, node->left->key, node->right->key);
    }

    return node;
}


/*
 * Balance the tree which current root is the root pointer param
 * Set the new root in the same pointer
 */
BinaryTreeNode *balance(BinaryTreeNode *root)
{
    BinaryTreeNode **nodesPtrPerKey = calloc(root->treeSize, sizeof(BinaryTreeNode*));
    int *sortedKeys = calloc(root->treeSize, sizeof(int));
    
    int id = 0;
    sortr(root, nodesPtrPerKey, sortedKeys, &id);

    for (int i = 0; i < root->treeSize; ++i)
    {
        BinaryTreeNode *node = nodesPtrPerKey[ sortedKeys[ i ] ];
        node->parent = NULL;
        node->left = NULL;
        node->right = NULL;
    }

    int size = root->treeSize;
    root->treeSize = -1;
    root->parent = NULL;
    // printf("balance k=%d p=%p s=%d\n", root->key, root, root->treeSize);
    root = _balance(nodesPtrPerKey, sortedKeys, 0, size); // return new root
    root->treeSize = size;
    // printf("balance2 k=%d p=%p s=%d\n", root->key, root, root->treeSize);

    free(nodesPtrPerKey);
    free(sortedKeys);

    return root;
}


void print(BinaryTreeNode *root)
{
    const int size = root->treeSize;
    int depth = 0;
    int *flags = calloc(size, sizeof(int));
    char *str = malloc(sizeof(char) * 100);

    printf("---------------------------\n");

    while (root != NULL) 
    {
        int key = root->key;

        str_repeat(str, "  ", depth);

        if (flags[key] < 1) {
            flags[key] = 1;
            
            printf("%sk= %d \n", str, key);
        }

        if (flags[key] < 2) {
            flags[key] = 2;

            if (root->left) {
                printf("%sleft: \n", str);
                printf("%s\\ \n", str);
                root = root->left;
                depth++;
                continue;
            } 
        }

        if (flags[key] < 3) {
            flags[key] = 3;

            if (root->right) {
                printf("%sright: \n", str);
                printf("%s\\ \n", str);

                root = root->right;
                depth++;
                continue;
            }
        }

        root = root->parent;
        depth--;
    }

    printf("---------------------------\n");

    free(flags);
    free(str);
}


void insert(BinaryTreeNode *root, BinaryTreeNode *newNode)
{
    // printf("========insert ==== %p == %p = \n", root, newNode);
    while (1) 
    {
        // printf("root %p k=%d %p %p \n", root, root->key, root->left, root->right);
        if (newNode->key < root->key) {
            if (root->left != NULL) {
                root = root->left;
                continue; 
            } else {
                root->left = newNode;
                newNode->parent = root;
                return;
            }
        } 

        if (newNode->key > root->key) {
            if (root->right != NULL) {
                root = root->right;
                continue;
            } else {
                root->right = newNode;
                newNode->parent = root;
                return;
            }
        }

        return;
    }
}


void createTree(BinaryTreeNode *tree, int size, int shuffle)
{
    int i;
    int *values = malloc(sizeof(int) * size);
    for (i = 0; i < size; i++) {
        values[i] = i;
    }

    if (shuffle == 1) {
        array_shuffle(values, size);
    }
    // array_print(values, size, "Array values: ");

    // printf("create tree %p %d\n", tree, size);

    tree->value = values[0];
    tree->key = values[0];
    tree->treeSize = size;
    tree->parent = NULL; // initialize your pointers to NULL you damn fool !
    tree->left = NULL;
    tree->right = NULL;

    i = 1;
    while (i < size) {
        BinaryTreeNode *node = malloc(sizeof(BinaryTreeNode));
        node->key = values[i];
        node->value = values[i];
        node->parent = NULL;
        node->left = NULL;
        node->right = NULL;

        // printf("before insert %p %p \n", tree, node);
        insert(tree, node);
        i++;
    }

    free(values);
}


void bt_array_print(BinaryTreeNode *array[], int size, char *text)
{
    printf("%s size=%d\n", text, size);
    for (int i = 0; i < size; i++) {
        printf("%d : %d\n", i, array[i]->key);
    }
    printf("\n");
}


void bt_free(BinaryTreeNode *root)
{
    if (root->left) {
        bt_free(root->left);
    }

    if (root->right) {
        bt_free(root->right);
    }

    free(root);
}


int main(int argc, char *argv[]) 
{
    const int treeCount = atoi(argv[1]);
    const int treeSize = atoi(argv[2]);
    // printf("treeSize = %d \n", treeSize);

    srand(time(NULL));
    rand(); rand(); rand();


    REGISTER_TIME(startTime)
    // BUILD

    // note: only two trees are created
    // but find and sort operations are performed [treeCount] times on each
    BinaryTreeNode *tree = malloc(sizeof(BinaryTreeNode));
    
    // createTree(tree, treeSize, 0);
    
    createTree(tree, treeSize, 0);
    // tree = balance(tree);


    int lastSortedKeyId = 0;

    REGISTER_TIME(buildEndTime)
    // SORT Loop

    BinaryTreeNode **array = calloc(tree->treeSize, sizeof(BinaryTreeNode*));
    for (int i = 0; i < treeCount; ++i)
    {
        sortl(tree, array);
    }
    // bt_array_print(array, treeSize, "Sort L");
    // free(array);


    REGISTER_TIME(sortlEndTime)
    // SORT Loop

    // array = calloc(tree->treeSize, sizeof(BinaryTreeNode*));
    for (int i = 0; i < treeCount; ++i)
    {
        sortr(tree, array, NULL, &lastSortedKeyId);
    }
    // bt_array_print(array, treeSize, "Sort R");
    free(array);


    REGISTER_TIME(sortrEndTime)
    // FIND    

    for (int i = 0; i < treeCount; ++i)
    {
        int target = rand() % treeSize;
        BinaryTreeNode *node;
        node = findl(tree, target);

        if (! node) {
            printf("pas trouvé l! %d ", target);
        }
    }

    REGISTER_TIME(findlEndTime)

    for (int i = 0; i < treeCount; ++i)
    {
        int target = rand() % treeSize;
        BinaryTreeNode *node;
        node = findr(tree, target);

        if (! node) {
            printf("pas trouvé r! %d ", target);
        }
    }

    REGISTER_TIME(findrEndTime)

    // bt_free(tree);
    // bt_free(tree2);

    double buildDiff = getDiff(startTime, buildEndTime);
    double sortlDiff = getDiff(buildEndTime, sortlEndTime);
    double sortrDiff = getDiff(sortlEndTime, sortrEndTime);
    double findlDiff = getDiff(sortrEndTime, findlEndTime);
    double findrDiff = getDiff(findlEndTime, findrEndTime);

    printf("Pass count: %d \n", treeCount);
    printf("Tree size: %d \n", treeSize);
    printf("Build time:            %f s\n", buildDiff);
    printf("Find Loop time:        %f s\n", findlDiff);
    printf("Find Recurse time:     %f s\n", findrDiff);
    printf("sort Loop time:        %f s\n", sortlDiff);
    printf("sort Recurse time:     %f s\n", sortrDiff);

}



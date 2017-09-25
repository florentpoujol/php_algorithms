#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#include "futils.c"


typedef struct BinaryTreeNode {
    int key;
    int value;
    int treeSize;
    struct BinaryTreeNode *array; // Nodes by keys, build by sort() used by balance()
    struct BinaryTreeNode *parent;
    struct BinaryTreeNode *left;
    struct BinaryTreeNode *right;
} BinaryTreeNode;



BinaryTreeNode *find(BinaryTreeNode *root, const int key)
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
        } else { // key > root->key
            if (root->right) {
                root = root->right;
                continue;
            }
        }

        return NULL;
    }
}


void balance(BinaryTreeNode *root)
{

}


BinaryTreeNode **sort(BinaryTreeNode *root)
{
    const int size = root->treeSize;

    // array of pointers toward BinaryTreeNode
    BinaryTreeNode **nodes = calloc(size, sizeof(BinaryTreeNode*));
    int *flags = calloc(size, sizeof(int));

    int key;
    const int LEFT_VISITED = 1; // 001
    const int RIGHT_VISITED = 2; // 010
    const int VALUE_ADDED = 4; // 100

    while (root != NULL) 
    {
        key = root->key;

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
    return nodes;
}

BinaryTreeNode **sortr(BinaryTreeNode *root)
{
    const int size = root->treeSize;

    // array of pointers toward BinaryTreeNode
    BinaryTreeNode **nodes = calloc(size, sizeof(BinaryTreeNode*));
    int *flags = calloc(size, sizeof(int));

    int key;
    const int LEFT_VISITED = 1; // 001
    const int RIGHT_VISITED = 2; // 010
    const int VALUE_ADDED = 4; // 100

    while (root != NULL) 
    {
        key = root->key;

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
    return nodes;
}


void print(BinaryTreeNode *root)
{
    const int size = root->treeSize;
    int key, depth = 0;
    int *flags = calloc(size, sizeof(int));
    char *str = malloc(sizeof(char) * 100);

    printf("---------------------------\n");

    while (root != NULL) 
    {
        key = root->key;

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
    int treeSize = root->treeSize;
    while (treeSize-- > 0) 
    {
        if (newNode->key < root->key) {
            if (root->left != NULL) {
                root = root->left;
                continue; 
            } else {
                root->left = malloc(sizeof(BinaryTreeNode*));
                root->left = newNode;
                
                newNode->parent = malloc(sizeof(BinaryTreeNode*));
                newNode->parent = root;
                
                break;
            }
        }

        if (newNode->key > root->key) {
            if (root->right != NULL) {
                root = root->right;
                continue;
            } else {
                root->right = malloc(sizeof(BinaryTreeNode*));
                root->right = newNode;
                
                newNode->parent = malloc(sizeof(BinaryTreeNode*));
                newNode->parent = root;
                
                break;
            }
        }

        if (newNode->key == root->key) {
            break;
        }
    }
}


void createTree(BinaryTreeNode *tree, int size)
{
    int i;
    int *values = malloc(sizeof(int) * size);
    for (i = 0; i < size; i++) {
        values[i] = i;
    }

    array_shuffle(values, size);
    // array_print(values, size, "Array values: ");

    tree->value = values[0];
    tree->key = values[0];
    tree->treeSize = size;

    i = 1;
    while (i < size) {

        BinaryTreeNode *node = malloc(sizeof(BinaryTreeNode));
        node->key = values[i];
        node->value = values[i];

        insert(tree, node);
        i++;
    }

    free(values);
}


void createBalancedTree(BinaryTreeNode *tree, int size)
{
    int *values = malloc(sizeof(int) * size);
    for (int i = 0; i < size; i++) {
        values[i] = i;
    }

    array_shuffle(values, size);
    array_print(values, size, "Array values: ");


    int middleValue = (int)(size / 2);


    tree->value = middleValue;
    tree->key = middleValue;
    tree->treeSize = size;

    int leftValue;

    int leftSize = middleValue - 1;
    
    int rightValue;
    int rightSize = size - middleValue;

    // int leftSize = size, rightSize = size;
    
    int i = 1;
    while (i < size) {
        int value;
        if (i % 2 == 0) {
            value = (int)(leftSize / 2);
            leftSize = value - 1;
        } else {
            value = (int)(rightSize / 2);
            rightSize = value;
        }

        BinaryTreeNode *node = malloc(sizeof(BinaryTreeNode));
        node->key = value;
        node->value = value;

        insert(tree, node);
        i++;
    }

    free(values);
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
    createTree(tree, treeSize);

    // BinaryTreeNode *tree2 = malloc(sizeof(BinaryTreeNode));
    // createTree(tree2, treeSize);
    // balance(tree2);


    REGISTER_TIME(buildEndTime)
    // SORT

    for (int i = 0; i < treeCount; ++i)
    {
        BinaryTreeNode **array = sort(tree);
        // free(array);
    }


    REGISTER_TIME(toArrayEndTime)
    // FIND    

    int target;
    for (int i = 0; i < treeCount; ++i)
    {
        target = rand() % treeSize;
        BinaryTreeNode *node;
        node = find(tree, target);

        if (! node) {
            printf("pas trouvé! %d ", target);
        }
    }


    REGISTER_TIME(findEndTime)

    double buildDiff = getDiff(startTime, buildEndTime);
    double toArrayDiff = getDiff(buildEndTime, toArrayEndTime);
    double findDiff = getDiff(toArrayEndTime, findEndTime);

    printf("Pass count: %d \n", treeCount);
    printf("Tree size: %d \n", treeSize);
    printf("Build time:            %f s\n", buildDiff);
    printf("to array time:         %f s\n", toArrayDiff);
    printf("Find time:             %f s\n", findDiff);

}



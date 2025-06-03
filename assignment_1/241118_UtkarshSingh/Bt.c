#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 3
#define MIN 2

struct BTreeNode {
    int val[MAX + 1], count;
    char* name[MAX + 1];
    struct BTreeNode* link[MAX + 1];
};

struct BTreeNode* root = NULL;

// Function prototypes (forward declarations)
void insert(int val, const char* name);
void deleteNode(int val, struct BTreeNode* myNode);
void search(int val, struct BTreeNode* node);
void inorder(struct BTreeNode* node);
void levelOrder(void);

// Queue for level-order traversal
struct QueueNode {
    struct BTreeNode* btnode;
    int level;
    struct QueueNode* next;
};

struct QueueNode* front = NULL;
struct QueueNode* rear = NULL;

void enqueue(struct BTreeNode* node, int level) {
    struct QueueNode* temp = (struct QueueNode*)malloc(sizeof(struct QueueNode));
    temp->btnode = node;
    temp->level = level;
    temp->next = NULL;
    if (!rear) {
        front = rear = temp;
    } else {
        rear->next = temp;
        rear = temp;
    }
}

struct QueueNode* dequeue() {
    if (!front) return NULL;
    struct QueueNode* temp = front;
    front = front->next;
    if (!front) rear = NULL;
    return temp;
}

// Create a new node
struct BTreeNode* createNode(int val, const char* str, struct BTreeNode* child) {
    struct BTreeNode* newNode = (struct BTreeNode*)malloc(sizeof(struct BTreeNode));
    newNode->val[1] = val;
    newNode->name[1] = strdup(str);
    newNode->count = 1;
    newNode->link[0] = root;
    newNode->link[1] = child;
    return newNode;
}

// Insert node values and children at a position
void insertNode(int val, const char* name, int pos, struct BTreeNode* node, struct BTreeNode* child) {
    int j = node->count;
    while (j > pos) {
        node->val[j + 1] = node->val[j];
        node->name[j + 1] = node->name[j];
        node->link[j + 1] = node->link[j];
        j--;
    }
    node->val[j + 1] = val;
    node->name[j + 1] = strdup(name);
    node->link[j + 1] = child;
    node->count++;
}

// Split a full node during insertion
void splitNode(int val, const char* name, int* pval, char** pname, int pos, struct BTreeNode* node, struct BTreeNode* child, struct BTreeNode** newNode) {
    int median = (pos > MIN) ? MIN + 1 : MIN;

    *newNode = (struct BTreeNode*)malloc(sizeof(struct BTreeNode));
    (*newNode)->count = 0;

    int j = median + 1;
    for (int i = 1; j <= MAX; i++, j++) {
        (*newNode)->val[i] = node->val[j];
        (*newNode)->name[i] = node->name[j];
        (*newNode)->link[i] = node->link[j];
        (*newNode)->count++;
    }
    (*newNode)->link[0] = node->link[median];
    node->count = median;

    if (pos <= MIN)
        insertNode(val, name, pos, node, child);
    else
        insertNode(val, name, pos - median, *newNode, child);

    *pval = node->val[node->count];
    *pname = node->name[node->count];
    (*newNode)->link[0] = node->link[node->count];
    node->count--;
}

// Recursive function to insert value into tree
int setValue(int val, const char* name, int* pval, char** pname, struct BTreeNode* node, struct BTreeNode** child) {
    int pos;

    if (!node) {
        *pval = val;
        *pname = strdup(name);
        *child = NULL;
        return 1;
    }

    if (val < node->val[1]) pos = 0;
    else {
        for (pos = node->count; val < node->val[pos] && pos > 1; pos--);
        if (val == node->val[pos]) {
            printf("Duplicates not allowed: %d\n", val);
            return 0;
        }
    }

    if (setValue(val, name, pval, pname, node->link[pos], child)) {
        if (node->count < MAX) {
            insertNode(*pval, *pname, pos, node, *child);
        } else {
            splitNode(*pval, *pname, pval, pname, pos, node, *child, child);
            return 1;
        }
    }
    return 0;
}

// Insert a value into the B-tree
void insert(int val, const char* name) {
    int i, flag;
    char* newName;
    struct BTreeNode* child;
    flag = setValue(val, name, &i, &newName, root, &child);
    if (flag) root = createNode(i, newName, child);
}

// In-order traversal of B-tree
void inorder(struct BTreeNode* node) {
    if (!node) return;
    for (int i = 0; i < node->count; i++) {
        inorder(node->link[i]);
        printf("%d:%s ", node->val[i + 1], node->name[i + 1]);
    }
    inorder(node->link[node->count]);
}

// Level-order traversal using queue
void levelOrder() {
    if (!root) return;

    enqueue(root, 0);
    int currentLevel = -1;

    while (front) {
        struct QueueNode* qnode = dequeue();
        if (qnode->level != currentLevel) {
            currentLevel = qnode->level;
            printf("\nLevel %d: ", currentLevel);
        }

        for (int i = 1; i <= qnode->btnode->count; i++) {
            printf("[%d,%s] ", qnode->btnode->val[i], qnode->btnode->name[i]);
        }

        for (int i = 0; i <= qnode->btnode->count; i++) {
            if (qnode->btnode->link[i]) enqueue(qnode->btnode->link[i], currentLevel + 1);
        }

        free(qnode);
    }
    printf("\n");
}

// Search for a key in the B-tree
void search(int val, struct BTreeNode* node) {
    int pos;
    if (!node) {
        printf("Key %d not found.\n", val);
        return;
    }

    if (val < node->val[1]) pos = 0;
    else {
        for (pos = node->count; val < node->val[pos] && pos > 1; pos--);
        if (val == node->val[pos]) {
            printf("Key %d found with name: %s\n", val, node->name[pos]);
            return;
        }
    }

    search(val, node->link[pos]);
}

// Helper to get predecessor during deletion
void copyPredecessor(struct BTreeNode* myNode, int pos) {
    struct BTreeNode* temp = myNode->link[pos];
    while (temp->link[temp->count]) temp = temp->link[temp->count];

    myNode->val[pos] = temp->val[temp->count];
    free(myNode->name[pos]);
    myNode->name[pos] = strdup(temp->name[temp->count]);

    deleteNode(myNode->val[pos], myNode->link[pos]);
}

// Helper to get successor during deletion
void copySuccessor(struct BTreeNode* myNode, int pos) {
    struct BTreeNode* temp = myNode->link[pos + 1];
    while (temp->link[0]) temp = temp->link[0];

    myNode->val[pos] = temp->val[1];
    free(myNode->name[pos]);
    myNode->name[pos] = strdup(temp->name[1]);

    deleteNode(myNode->val[pos], myNode->link[pos + 1]);
}

// Merge two child nodes
void mergeNodes(struct BTreeNode* myNode, int pos) {
    struct BTreeNode* child1 = myNode->link[pos];
    struct BTreeNode* child2 = myNode->link[pos + 1];

    child1->count++;
    child1->val[child1->count] = myNode->val[pos];
    free(child1->name[child1->count]);
    child1->name[child1->count] = strdup(myNode->name[pos]);

    for (int i = 1, j = child1->count + 1; i <= child2->count; i++, j++) {
        child1->val[j] = child2->val[i];
        child1->name[j] = strdup(child2->name[i]);
        child1->link[j - 1] = child2->link[i - 1];
        child1->count++;
    }
    child1->link[child1->count] = child2->link[child2->count];

    for (int i = pos; i < myNode->count; i++) {
        myNode->val[i] = myNode->val[i + 1];
        free(myNode->name[i]);
        myNode->name[i] = strdup(myNode->name[i + 1]);
        myNode->link[i] = myNode->link[i + 1];
    }
    myNode->count--;
    free(child2);

    if (myNode == root && myNode->count == 0) {
        root = child1;
        free(myNode);
    }
}

// Borrow from left sibling
void borrowFromLeft(struct BTreeNode* myNode, int pos) {
    struct BTreeNode* child = myNode->link[pos];
    struct BTreeNode* leftSibling = myNode->link[pos - 1];

    for (int i = child->count; i > 0; i--) {
        child->val[i + 1] = child->val[i];
        free(child->name[i + 1]);
        child->name[i + 1] = strdup(child->name[i]);
        child->link[i + 1] = child->link[i];
    }
    child->link[1] = child->link[0];

    child->count++;
    child->val[1] = myNode->val[pos];
    free(child->name[1]);
    child->name[1] = strdup(myNode->name[pos]);
    child->link[0] = leftSibling->link[leftSibling->count];

    myNode->val[pos] = leftSibling->val[leftSibling->count];
    free(myNode->name[pos]);
    myNode->name[pos] = strdup(leftSibling->name[leftSibling->count]);
    leftSibling->count--;
}

// Borrow from right sibling
void borrowFromRight(struct BTreeNode* myNode, int pos) {
    struct BTreeNode* child = myNode->link[pos];
    struct BTreeNode* rightSibling = myNode->link[pos + 1];

    child->count++;
    child->val[child->count] = myNode->val[pos];
    free(child->name[child->count]);
    child->name[child->count] = strdup(myNode->name[pos]);
    child->link[child->count] = rightSibling->link[0];

    myNode->val[pos] = rightSibling->val[1];
    free(myNode->name[pos]);
    myNode->name[pos] = strdup(rightSibling->name[1]);

    for (int i = 1; i < rightSibling->count; i++) {
        rightSibling->val[i] = rightSibling->val[i + 1];
        free(rightSibling->name[i]);
        rightSibling->name[i] = strdup(rightSibling->name[i + 1]);
        rightSibling->link[i - 1] = rightSibling->link[i];
    }
    rightSibling->link[rightSibling->count - 1] = rightSibling->link[rightSibling->count];
    rightSibling->count--;
}

// Delete a key from the tree starting at node myNode
void deleteNode(int val, struct BTreeNode* myNode) {
    int pos;
    if (!myNode) {
        printf("Key %d not found for deletion.\n", val);
        return;
    }

    if (val < myNode->val[1]) pos = 0;
    else {
        for (pos = myNode->count; val < myNode->val[pos] && pos > 1; pos--);
        if (val == myNode->val[pos]) {
            if (myNode->link[pos - 1]) {
                copyPredecessor(myNode, pos);
            } else if (myNode->link[pos]) {
                copySuccessor(myNode, pos);
            } else {
                // Leaf node
                for (int i = pos; i < myNode->count; i++) {
                    myNode->val[i] = myNode->val[i + 1];
                    free(myNode->name[i]);
                    myNode->name[i] = strdup(myNode->name[i + 1]);
                }
                myNode->count--;
                return;
            }
            return;
        }
    }

    deleteNode(val, myNode->link[pos]);

    // Fix underflow
    if (myNode->link[pos] && myNode->link[pos]->count < MIN) {
        if (pos != 0 && myNode->link[pos - 1]->count > MIN) {
            borrowFromLeft(myNode, pos);
        } else if (pos != myNode->count && myNode->link[pos + 1]->count > MIN) {
            borrowFromRight(myNode, pos);
        } else {
            if (pos == myNode->count) pos--;
            mergeNodes(myNode, pos);
        }
    }
}

#include <iostream>
#include <cstdlib>
#include <cstdbool>
#include <cmath>
#include <ctime>

using namespace std;

const int t = 5;

class BTreeNode {
public:
    BTreeNode();
    ~BTreeNode();

    bool leaf;
    int key[2 * t - 1];
    int count;
    BTreeNode *child[2 * t];
};

BTreeNode::BTreeNode() {
    leaf = true;
    count = 0;
    for (int i = 0; i < 2 * t; i++) {
        child[i] = nullptr;
    }
}

BTreeNode::~BTreeNode() {
}

class BTree {
public:
    BTree();
    ~BTree();

    BTreeNode *root;

    void insert(int key);

    void print();
    void print(BTreeNode *node, int level);

    void splitChild(BTreeNode *parent, int index);
    void insertNonFull(BTreeNode *node, int key);

    void remove(int key);
    void removeChild(BTreeNode *node, int key);

    BTreeNode *search(int key);
};

BTree::BTree() {
    root = new BTreeNode();
    root->leaf = true;
    root->count = 0;
}

BTree::~BTree() {
    delete root;
}

void BTree::insert(int value) {
    if (root->count == 2 * t - 1) {
        BTreeNode *newRoot = new BTreeNode();
        newRoot->leaf = false;
        newRoot->child[0] = root;
        splitChild(newRoot, 0);
        int i = 0;
        if (newRoot->key[0] < value) {
            i++;
        }
        insertNonFull(newRoot, value);
        root = newRoot;
    } else {
        insertNonFull(root, value);
    }
}

void BTree::insertNonFull(BTreeNode *node, int value) {
    int i = node->count;
    if (node->leaf) {
        i--;
        while (i >= 0 && node->key[i] > value) {
            node->key[i + 1] = node->key[i];
            i--;
        }
        node->key[i + 1] = value;
        node->count++;
    } else {
        while (i >= 1 && node->key[i - 1] > value) {
            i--;
        }
        if (node->child[i]->count == 2 * t - 1) {
            splitChild(node, i);
            if (node->key[i] < value) {
                i++;
            }
        }
        insertNonFull(node->child[i], value);
    }
}

void BTree::splitChild(BTreeNode *parent, int index) {
    BTreeNode *child = parent->child[index];
    BTreeNode *newChild = new BTreeNode();

    newChild->leaf = child->leaf;
    newChild->count = t - 1;
    for (int i = 0; i < t - 1; i++) {
        newChild->key[i] = child->key[i + t];
    }
    if (!child->leaf) {
        for (int i = 0; i < t; i++) {
            newChild->child[i] = child->child[i + t];
        }
    }
    child->count = t - 1;
    for (int i = parent->count; i > index; i--) {
        parent->child[i + 1] = parent->child[i];
    }
    parent->child[index + 1] = newChild;
    for (int i = parent->count - 1; i >= index; i--) {
        parent->key[i + 1] = parent->key[i];
    }
    parent->key[index] = child->key[t - 1];
    parent->count++;
}

void BTree::print() {
    print(root, 1);
}

void BTree::print(BTreeNode *node, int level) {
    if (node == nullptr) {
        return;
    }
    printf("Level %d : ", level);

    for (int i = 0; i < node->count; i++) {
        printf("%d ", node->key[i]);
    }
    printf("\n");
    if (!node->leaf) {
        for (int i = 0; i <= node->count; i++) {
            print(node->child[i], level + 1);
        }
    }
}

void BTree::remove(int key) {
    int i = root->count - 1;
    if (root->leaf) {
        while (i >= 0 && root->key[i] > key) {
            i--;
        }
        if (i >= 0 && root->key[i] == key) {
            for (int j = i + 1; j < root->count; j++) {
                root->key[j - 1] = root->key[j];
            }
            root->count--;
        }
    }
    else {
        if (root->count > 1) {
            removeChild(root, key);
        }
        else {
            BTreeNode *left = root->child[0];
            BTreeNode *right = root->child[1];
            if (left->count == t - 1 && right->count == t - 1) {
                left->key[t - 1] = root->key[0];
                for (int j = 0; j < t - 1; j++) {
                    left->key[j + t] = right->key[j];
                }
                if (left->leaf == false) {
                    for (int j = 0; j < t; j++) {
                        left->child[j + t] = right->child[j];
                    }
                }
                left->count = 2 * t - 1;
                delete right;
                right = nullptr;
                root = left;
                removeChild(left, key);
            }
            else {
                removeChild(root, key);
            }
        }
    }
}

int findPredecessor(BTreeNode* node) {
    while (node->leaf == false) {
        node = node->child[node->count];
    }
    return node->key[node->count - 1];
}

int findSuccessor(BTreeNode* node) {
	while (node->leaf == false) {
		node = node->child[0];
	}
	return node->key[0];
}

void BTree::removeChild(BTreeNode *node, int key) {
    int i = node->count;
    printf("key: %d\n", key);
    for (int j = 0; j < node->count; j++) {
        printf("%d ", node->key[j]);
    }
    printf("\n");

    while (i > 0 && node->key[i - 1] >= key) {
        i--;
    }
    if (i >= 0 && node->key[i] == key) {
        if (node->leaf) {
            for (int j = i; j < node->count - 1; j++) {
                node->key[j] = node->key[j + 1];
            }
            node->count--;
        }
        else {
            BTreeNode * left = node->child[i];
            BTreeNode * right = node->child[i + 1];
            if (left->count > t -1) {
                int predecessorKey = findPredecessor(left);
                node->key[i] = predecessorKey;
                removeChild(left, predecessorKey);
            }
            else if (right->count > t - 1) {
                int successorKey = findSuccessor(right);
                node->key[i] = successorKey;
                removeChild(right, successorKey);
            }
            else {
                left->key[t - 1] = key;
                for (int j = 0; j < t - 1; j++) {
                    left->key[j + t] = right->key[j];
                }
                for (int j = 0; j < t; j++) {
                    left->child[j + t] = right->child[j];
                }
                left->count = 2 * t - 1;
                for (int j = i; j < node->count - 1; j++) {
                    node->key[j] = node->key[j + 1];
                }
                for (int j = i + 1; j < node->count; j++) {
                    node->child[j] = node->child[j + 1];
                }
                node->count--;
                delete right;
                right = nullptr;
                removeChild(left, key);
            }
        }
    }
    else {
        BTreeNode *child = node->child[i];
        if (child->count > t - 1) {
            removeChild(child, key);
        }
        else {
            if (i != 0 && (node->child[i - 1])->count > t - 1) {
                BTreeNode *left = node->child[i - 1];
                for (int j = t - 2; j >= 0; j--) {
                    child->key[j + 1] = child->key[j];
                }
                if (child->leaf == false) {
                    for (int j = t - 1; j >= 0; j--) {
                        child->child[j + 1] = child->child[j];
                    }
                }
                child->key[0] = node->key[i - 1];
                child->child[0] = left->child[left->count];
                child->count++;
                node->key[i - 1] = left->key[left->count - 1];
                left->count--;
            }
            else if (i != node->count && (node->child[i + 1])->count > t - 1) {
                BTreeNode *right = node->child[i + 1];
                child->key[t - 1] = node->key[i];
                child->child[t] = right->child[0];
                child->count++;
                node->key[i] = right->key[0];

                for (int j = 0; j < right->count - 1; j++) {
                    right->key[j] = right->key[j + 1];
                }

                for (int j = 0; j < right->count; j++) {
                    node->child[j] = node->child[j + 1];
                }
                right->count--;
            }
            else {
                if (i == 0) {
                    BTreeNode *right = node->child[i + 1];
                    for (int j = 0; j < t - 1; j++) {
                        right->key[j + t] = right->key[j];
                        right->key[j] = child->key[j];
                    }
                    if (right->leaf == false) {
                        for (int j = 0; j < t; j++) {
                            right->child[j + t] = right->child[j];
                            right->child[j] = child->child[j];
                        }
                    }
                    right->key[t - 1] = node->key[i];
                    right->count = 2 * t - 1;
                    for (int j = 0; j < node->count - 1; j++) {
                        node->key[j] = node->key[j + 1];
                    }
                    for (int j = 0; j < node->count; j++) {
                        node->child[j] = node->child[j + 1];
                    }
                    delete child;
                    child = right;
                    node->count--;
                }
                else {
                    BTreeNode *left = node->child[i - 1];
                    left->key[t - 1] = node->key[i - 1];
                    for (int j = 0; j < t - 1; j++) {
                        left->key[j + t] = child->key[j];
                    }
                    if (left->leaf == false) {
                        for (int j = 0; j < t; j++) {
                            left->child[j + t] = child->child[j];
                        }
                    }
                    left->count = 2 * t - 1;
                    for (int j = i - 1; j < node->count - 1; j++) {
                        node->key[j] = node->key[j + 1];
                    }
                    for (int j = i; j < node->count; j++) {
                        node->child[j] = node->child[j + 1];
                    }
                    delete child;
                    child = left;
                    node->count--;
                }
            }
            removeChild(child, key);
        }
    }

}

BTreeNode *BTree::search(int key) {
    BTreeNode *node = root;
    while (!node->leaf) {
        int i = 0;
        while (i < node->count && key > node->key[i]) {
            i++;
        }
        node = node->child[i];
    }
    int i = 0;
    while (i < node->count && key > node->key[i]) {
        i++;
    }
    if (i < node->count && key == node->key[i]) {
        return node;
    } else {
        return nullptr;
    }
}

int main() {
    BTree btree;

    int data[10000] = {0,};
    for (int i = 0; i < 1000; i++) {
        while (true) {
            int num = rand() % 10000;
            if (data[num] == 0) {
                data[num] = 1;
                break;
            }
        }
    }
    int datas[1000] = {0,};
    bool isVisited[1000] = {false,};
    int index = 0;
    for (int i = 0; i < 10000; i++) {
        if (data[i] == 1) {
            datas[index] = i;
            index++;
        }
    }

    for (int i = 0; i < 1000; i++) {
        while (true) {
            int num = rand() % 1000;
            if (isVisited[num] == false) {
                isVisited[num] = true;
                btree.insert(datas[num]);
                break;
            }
        }
    }
    btree.print();

    for (int i = 0; i < 500; i++) {
        while (true) {
            int num = rand() % 1000;
            if (isVisited[num] == true) {
                isVisited[num] = false;
                btree.remove(datas[num]);
                break;
            }
        }
        printf("1\n");
    }
    btree.print();

    return 0;
}

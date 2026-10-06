#ifndef BST_H
#define BST_H

#include <string>

using namespace std;

struct BSTNode
{
    string key;
    int id;

    BSTNode* left;
    BSTNode* right;

    BSTNode(string key, int id);
};

// Binary search tree used as the location-name lookup index.
class LocationBST
{
private:
    BSTNode* root;

    string normalize(string text);

    BSTNode* insertNode(BSTNode* node, string key, int id);

    BSTNode* searchNode(BSTNode* node, string key);

    BSTNode* removeNode(BSTNode* node, string key);

    BSTNode* minimumNode(BSTNode* node);

    void destroy(BSTNode* node);

public:
    LocationBST();
    ~LocationBST();

    void insert(string name, int id);

    bool find(string name, int& id);

    void remove(string name);

    void clear();
};

#endif
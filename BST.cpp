#include "BST.h"

#include <cctype>

BSTNode::BSTNode(string key, int id)
{
    this->key = key;
    this->id = id;

    left = nullptr;
    right = nullptr;
}

LocationBST::LocationBST()
{
    root = nullptr;
}

LocationBST::~LocationBST()
{
    clear();
}

string LocationBST::normalize(string text)
{
    for (char& c : text)
    {
        c = static_cast<char>(
            tolower(static_cast<unsigned char>(c))
        );
    }

    return text;
}

BSTNode* LocationBST::insertNode(
    BSTNode* node,
    string key,
    int id
)
{
    if (node == nullptr)
    {
        return new BSTNode(key, id);
    }

    if (key < node->key)
    {
        node->left = insertNode(node->left, key, id);
    }
    else if (key > node->key)
    {
        node->right = insertNode(node->right, key, id);
    }
    else
    {
        node->id = id;
    }

    return node;
}

void LocationBST::insert(string name, int id)
{
    string key = normalize(name);

    root = insertNode(root, key, id);
}

BSTNode* LocationBST::searchNode(
    BSTNode* node,
    string key
)
{
    if (node == nullptr)
    {
        return nullptr;
    }

    if (key == node->key)
    {
        return node;
    }

    if (key < node->key)
    {
        return searchNode(node->left, key);
    }

    return searchNode(node->right, key);
}

bool LocationBST::find(string name, int& id)
{
    string key = normalize(name);

    BSTNode* result = searchNode(root, key);

    if (result == nullptr)
    {
        return false;
    }

    id = result->id;

    return true;
}

BSTNode* LocationBST::minimumNode(BSTNode* node)
{
    BSTNode* current = node;

    while (current != nullptr && current->left != nullptr)
    {
        current = current->left;
    }

    return current;
}

BSTNode* LocationBST::removeNode(
    BSTNode* node,
    string key
)
{
    if (node == nullptr)
    {
        return nullptr;
    }

    if (key < node->key)
    {
        node->left = removeNode(node->left, key);
    }
    else if (key > node->key)
    {
        node->right = removeNode(node->right, key);
    }
    else
    {
        // No left child
        if (node->left == nullptr)
        {
            BSTNode* temp = node->right;

            delete node;

            return temp;
        }

        // No right child
        if (node->right == nullptr)
        {
            BSTNode* temp = node->left;

            delete node;

            return temp;
        }

        // Two children
        BSTNode* temp = minimumNode(node->right);

        node->key = temp->key;
        node->id = temp->id;

        node->right = removeNode(node->right, temp->key);
    }

    return node;
}

void LocationBST::remove(string name)
{
    string key = normalize(name);

    root = removeNode(root, key);
}

void LocationBST::destroy(BSTNode* node)
{
    if (node == nullptr)
    {
        return;
    }

    destroy(node->left);
    destroy(node->right);

    delete node;
}

void LocationBST::clear()
{
    destroy(root);

    root = nullptr;
}
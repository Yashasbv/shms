#include <iostream>
#include <queue>

#include <screens/MedicalHistoryScreen.h>

using namespace std;

template <typename T>
class AVL
{
    class node
    {
    public:
        T data;      // store data in AVL
        node *left;  // point to left node
        node *right; // point to right node
        int height;  // the maximum path from specified node to leaves;
    };
    node *root = NULL; // in the first time before adding in AVL Tree
    int count = 0;

    // function to calculate the maximum value from specified node
    int maxUtility(node *current)
    {
        if (current->right == NULL)
            return current->data.id;
        else
            return maxUtility(current->right);
    }

    // to return height of node
    int height(node *current)
    {
        if (current == NULL)
            return 0;
        return current->height;
    }

    // right rotation if not balanced in the left
    node *rightRotation(node *current)
    {
        node *new_node = current->left;
        current->left = new_node->right;
        new_node->right = current;
        current->height = 1 + max(height(current->left), height(current->right));
        new_node->height = 1 + max(height(new_node->left), height(new_node->right));
        return new_node;
    }

    // left rotation if not balanced in the right
    node *leftRotation(node *current)
    {
        node *new_node = current->right;
        current->right = new_node->left;
        new_node->left = current;
        current->height = 1 + max(height(current->left), height(current->right));
        new_node->height = 1 + max(height(new_node->left), height(new_node->right));
        return new_node;
    }

    // to Display the values by In Order Method .. left - node - right
    void inOrderUtility(node *current)
    {
        if (current == NULL)
        {
            return;
        }
        inOrderUtility(current->left);
        cout << current->data.id << " ";
        inOrderUtility(current->right);
    }

    // to Display the values by Pre Order Method .. node - left - right
    void preOrderUtility(node *current)
    {
        if (current == NULL)
        {
            return;
        }
        cout << current->data.id << " ";
        preOrderUtility(current->left);
        preOrderUtility(current->right);
    }

    // to Display the values by Post Order Method .. left - right - node
    void postOrderUtility(node *current)
    {
        if (current == NULL)
        {
            return;
        }
        postOrderUtility(current->left);
        postOrderUtility(current->right);
        cout << current->data.id << " ";
    }

    // to add node in the AVL Tree with balanced result
    node *insertUtility(node *current, T value)
    {
        // if AVL Tree is empty
        if (current == NULL)
        {
            node *new_node = new node;
            new_node->data = value;
            new_node->right = NULL;
            new_node->left = NULL;
            new_node->height = 1;
            current = new_node;
            return new_node;
        }
        // if AVL Tree is not empty
        if (value.id < current->data.id)
            current->left = insertUtility(current->left, value); // go to the last left
        else if (value.id > current->data.id)
            current->right = insertUtility(current->right, value);                // go to the last right
        current->height = 1 + max(height(current->left), height(current->right)); // recalculate height of new node
        int balance = height(current->left) - height(current->right);             // calculate balance with defference between left and right hand sides
        // if not balanced in the left side
        if (balance > 1)
        {
            if (value.id > current->left->data.id)
            {
                current->left = leftRotation(current->left);
                return rightRotation(current);
            }
            else
            {
                return rightRotation(current);
            }
        }
        // if not balanced in the right side
        else if (balance < -1)
        {
            if (value.id < current->right->data.id)
            {
                current->right = rightRotation(current->right);
                return leftRotation(current);
            }
            else
            {
                return leftRotation(current);
            }
        }
        return current;
    }

    // to remove node from the AVL Tree with balanced result
    node *removeUtility(node *current, T value)
    {
        // if AVL Tree is empty
        if (current == NULL)
        {
            cout << "Tree is Empty" << endl;
            return NULL;
        }
        // if AVL Tree is not empty
        if (value.id < current->data.id)
            current->left = removeUtility(current->left, value); // go to the last left
        else if (value.id > current->data.id)
            current->right = removeUtility(current->right, value); // go to the last right
        // After finding the value
        else
        {
            // if the is leaf
            if (current->left == NULL && current->right == NULL)
            {
                delete current;
                current = NULL;
            }
            // if the node has 2 childs
            else if (current->left != NULL && current->right != NULL)
            {
                int max = maxUtility(current->left);
                current->data.id = max;
                removeUtility(current->left, max);
            }
            // if the node has only child
            else
            {
                node *child = current;
                if (current->right != NULL)
                {
                    current = current->right;
                }
                else
                {
                    current = current->left;
                }
                child->left = NULL;
                child->right = NULL;
                delete child;
                child = NULL;
            }
        }
        current->height = 1 + max(height(current->left), height(current->right)); // recalculate height of new node
        int balance = height(current->left) - height(current->right);             // calculate balance with defference between left and right hand sides
        // if not balanced in the left side
        if (balance > 1)
        {
            // Left Left Case
            if (height(current->left) >= height(current->right))
            {
                return rightRotation(current);
            }
            // Left Right Case
            else
            {
                current->left = leftRotation(current->left);
                return rightRotation(current);
            }
        }
        // if not balanced in the right side
        else if (balance < -1)
        {
            // Right Right Case
            if (height(current->right) >= height(current->left))
            {
                return leftRotation(current);
            }
            // Right Left Case
            else
            {
                current->right = rightRotation(current->right);
                return leftRotation(current);
            }
        }
        return current;
    }

    // display bt level order .. level by level
    void display_BFS(T *nodes)
    {
        if (root == NULL)
            cout << "Tree is empty" << endl;
        else
        {
            queue<node *> q;
            q.push(root);
            node *current;
            int index = 0;
            while (!q.empty())
            {
                current = q.front();
                q.pop();
                cout << current->data.id << " ";
                nodes[index] = current->data;
                index++;
                if (current->left != NULL)
                    q.push(current->left);
                if (current->right != NULL)
                    q.push(current->right);
            }
            cout << endl;
        }
    }

    void inorder_fill(node *current, T *nodes, int *index)
    {
        if (current == NULL)
            return;
        inorder_fill(current->left, nodes, index);
        nodes[*index] = current->data;
        (*index)++;
        inorder_fill(current->right, nodes, index);
    }

public:
    // these Function will Appear in main when use object
    void insert(T value)
    {
        root = insertUtility(root, value);
        count++;
    }
    void remove(T value)
    {
        root = removeUtility(root, value);
        count--;
    }
    void inorder()
    {
        inOrderUtility(root);
        cout << endl;
    }
    void preOrder()
    {
        preOrderUtility(root);
        cout << endl;
    }
    void postOrder()
    {
        postOrderUtility(root);
        cout << endl;
    }

    T *levelOrder(int *len)
    {
        int index = 0;
        *len = count;
        T *nodes = new T[count];
        inorder_fill(root, nodes, &index);
        return nodes;
    }
};

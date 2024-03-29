//
// Created by deetav on 31/1/22.
//

#include <bits/stdc++.h>

using namespace std;
using namespace std::chrono;

template<typename key_type, typename mapped_type>

class [[maybe_unused]] AVLmap {
private:
    // Structure of node of AVL Tree
    
    struct Node {
        key_type key_value;
        mapped_type mapped_value;
        int height{};
        Node *left;
        Node *right;
        Node *parent;
        
        Node(key_type key_value, mapped_type mapped_value) {
                this->key_value = key_value;
                this->mapped_value = mapped_value;
                this->height = 1;
                left = nullptr;
                right = nullptr;
                parent = nullptr;
        }
    };
    
    Node *root; // Root of AVL Tree
    int sz;  // Size of AVL Tree
    
    // Height of subtree rooted at a node
    
    int height(Node *node) {
            if (node == nullptr) return 0;
            return node->height;
    }
    
    // Right rotation of subtree rooted at a node
    
    Node *rightRotate(Node *node) {
            Node *child = node->left;
            Node *grandchild = child->right;
            
            // Rotation
            node->parent = child;
            node->left = grandchild;
            if (grandchild) grandchild->parent = node;
            
            child->right = node;
            child->parent = nullptr;
            
            // Update of heights after rotation
            node->height = max(height(node->left), height(node->right)) + 1;
            child->height = max(height(child->left), height(child->right)) + 1;
            
            return child;
    }
    
    // Left rotation of subtree rooted at a node
    
    Node *leftRotate(Node *node) {
            Node *child = node->right;
            Node *grandchild = child->left;
            
            //Rotation
            node->parent = child;
            node->right = grandchild;
            if (grandchild) grandchild->parent = node;
            
            child->left = node;
            child->parent = nullptr;
            
            // Update of heights after rotation
            node->height = max(height(node->left), height(node->right)) + 1;
            child->height = max(height(child->left), height(child->right)) + 1;
            
            return child;
    }
    
    // Difference between height of left subtree and right subtree
    int getBalance(Node *node) {
            if (node == nullptr) return 0;
            return height(node->left) - height(node->right);
    }
    
    // Insert a node in the AVL tree
    Node *insertNode(Node *node, key_type k, mapped_type m) {
            
            // Standard BST insertion
            if (node == nullptr) {
                    sz++;
                    return (new Node(k, m));
            }
            if (k < node->key_value) {
                    Node *t = insertNode(node->left, k, m);
                    node->left = t;
                    t->parent = node;
            } else if (k > node->key_value) {
                    Node *t = insertNode(node->right, k, m);
                    node->right = t;
                    t->parent = node;
            } else {
                    node->mapped_value = m;                 // Given key value already exists so update the mapped value
                    return node;
            }
            
            // Height update
            node->height = 1 + max(height(node->left), height(node->right));
            
            // Re-balance if the node is unbalanced
            int balance = getBalance(node);
            if (balance > 1 && k < node->left->key_value)    // Left Left Case
                    return rightRotate(node);
            if (balance < -1 && k > node->right->key_value)  // Right Right Case
                    return leftRotate(node);
            if (balance > 1 && k > node->left->key_value) {   // Left Right Case
                    Node *t = leftRotate(node->left);
                    node->left = t;
                    t->parent = node;
                    return rightRotate(node);
            }
            if (balance < -1 && k < node->right->key_value) {  // Right Left Case
                    Node *t = rightRotate(node->right);
                    node->right = t;
                    t->parent = node;
                    return leftRotate(node);
            }
            
            return node;
    }
    
    // Find the node with The smallest key value
    Node *minValueNode(Node *node) {
            Node *current = node;
            while (current->left != nullptr) current = current->left;
            return current;
    }
    
    // Delete a node in the AVL tree
    Node *deleteNode(Node *node, key_type k) {
            
            // Standard BST delete
            if (node == nullptr) return node;
            if (k < node->key_value) {
                    Node *t = deleteNode(node->left, k);
                    node->left = t;
                    if (t) t->parent = node;
            } else if (k > node->key_value) {
                    Node *t = deleteNode(node->right, k);
                    node->right = t;
                    if (t) t->parent = node;
            } else {
                    if ((node->left == nullptr) || (node->right == nullptr)) {
                            Node *temp = node->left ? node->left : node->right;
                            if (temp == nullptr) {
                                    temp = node;
                                    node = nullptr;
                            } else {
                                    node->left = temp->left;
                                    node->right = temp->right;
                                    node->key_value = temp->key_value;
                                    node->mapped_value = temp->mapped_value;
                                    node->height = temp->height;
                            }
                            free(temp);
                            sz--;
                    } else {
                            Node *temp = minValueNode(node->right);
                            node->key_value = temp->key_value;
                            node->mapped_value = temp->mapped_value;
                            node->right = deleteNode(node->right, temp->key_value);
                    }
            }
            
            // If AVL tree is empty after deletion
            if (node == nullptr) return node;
            
            // Height Update
            node->height = 1 + max(height(node->left), height(node->right));
            
            // Re-balance if the node is unbalanced
            int balance = getBalance(node);
            if (balance > 1 && getBalance(node->left) >= 0)   // Left Left Case
                    return rightRotate(node);
            if (balance > 1 && getBalance(node->left) < 0) {  // Left Right Case
                    Node *t = leftRotate(node->left);
                    node->left = t;
                    t->parent = node;
                    return rightRotate(node);
            }
            if (balance < -1 && getBalance(node->right) <= 0) // Right Right Case
                    return leftRotate(node);
            if (balance < -1 && getBalance(node->right) > 0) {  // Right Left Case
                    Node *t = rightRotate(node->right);
                    node->right = t;
                    t->parent = node;
                    return leftRotate(node);
            }
            
            return node;
    }
    
    // Delete all nodes through Post-oder Traversal
    void destroy(Node *node) {
            if (!node) return;
            destroy(node->left);
            destroy(node->right);
            free(node);
    }

public:
    AVLmap() {
            root = nullptr;
            sz = 0;
    }
    
    class Iterator {
    private:
        Node *node_;      // Node* stored in the iterator
    
    public:
        // Constructors
        Iterator() = default;
        
        explicit Iterator(Node *node) {
                node_ = node;
        }
        
        // Overloaded Operators
        Node *operator->() {
                return node_;
        }
        
        bool operator!=(const Iterator &obj) {
                return node_ != obj.node_;
        }
        
        bool operator==(const Iterator &obj) {
                return node_ == obj.node_;
        }
        
        void operator++(int) {
                if (node_->right != 0) {
                        node_ = node_->right;
                        while (node_->left != 0) node_ = node_->left;
                } else {
                        Node *temp;
                        while (node_) {
                                temp = node_;
                                node_ = node_->parent;
                                if (node_ && node_->left == temp) break;
                        }
                }
        }
        
        void operator--(int) {
                if (node_->left != 0) {
                        node_ = node_->left;
                        while (node_->right != 0) node_ = node_->right;
                } else {
                        Node *temp;
                        while (node_) {
                                temp = node_;
                                node_ = node_->parent;
                                if (node_ && node_->right == temp) break;
                        }
                }
        }
    };
    
    mapped_type operator[](key_type key) {
            Iterator v = find(key);
            if (v == end()) return key_type();
            else return v->mapped_value;
    }
    
    // Functions to call the private functions
    void insert(key_type key, mapped_type mapped) {
            root = insertNode(root, key, mapped);
    }
    
    void erase(key_type key) {
            root = deleteNode(root, key);
    }
    
    void erase(Iterator it) {
            if (it != end()) root = deleteNode(root, it->key_value);
    }
    
    void clear() {
            destroy(root);
            root = NULL;
            sz = 0;
    }
    
    // Find node with given key
    Iterator find(key_type k) {
            Node *v = root;
            while (v) {
                    if (v->key_value == k) break;
                    else if (v->key_value < k) v = v->right;
                    else v = v->left;
            }
            if (v) return Iterator(v);
            else return end();
    }
    
    // Iterator pointing to past the largest key
    Iterator end() {
            return Iterator(nullptr);
    }
    
    // Iterator pointing to the smallest key
    Iterator begin() {
            if (!root) return Iterator(nullptr);
            else return Iterator(minValueNode(root));
    }
    
    // Size of map
    int size() {
            return sz;
    }
    
    // Check if map is empty
    bool empty() {
            return sz == 0;
    }
    
    // Iterator to smallest key value greater than or equal to given key
    Iterator lower_bound(key_type k) {
            Node *v = root, *f = NULL;
            while (v) {
                    if (v->key_value >= k) {
                            if (!f) f = v;
                            else if (f->key_value > v->key_value) f = v;
                            v = v->left;
                    } else v = v->right;
            }
            if (f) return Iterator(f);
            else return end();
    }
    
    // Iterator to smallest key value greater than given key
    Iterator upper_bound(key_type k) {
            Node *v = root, *f = nullptr;
            while (v) {
                    if (v->key_value > k) {
                            if (!f) f = v;
                            else if (f->key_value > v->key_value) f = v;
                            v = v->left;
                    } else v = v->right;
            }
            if (f) return Iterator(f);
            else return end();
    }
    
    // 1 if key is present else 0
    bool count(key_type k) {
            if (find(k) != end()) return 1;
            else return 0;
    }
    
};

int main() {
        
        auto start = high_resolution_clock::now();
        // Working -
        AVLmap<char, int> M;
        AVLmap<char, int>::Iterator it{};
        
        M.insert('x', 5);
        M.insert('%', 1);
        M.insert('0', 0);
        M.insert('E', 10);
        M.insert('q', 5);
        M.insert('*', 1);
        M.insert('7', 0);
        M.insert('B', 10);
        
        cout << M['0'] << endl;
        
        it = M.begin();
        M.erase(it);
        M.erase('x');
        it = M.find('E');
        cout << it->key_value << " " << it->mapped_value << endl;
        
        cout << M.size() << endl;
        
        for (it = M.begin(); it != M.end(); it++) cout << it->key_value << endl;
        
        cout << M.lower_bound('2')->key_value << endl;
        cout << M.upper_bound('2')->key_value << endl;
        
        cout << M.count('$') << endl;
        
        M.clear();
        if (M.empty()) cout << "AVL Map is empty\n";
        
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        
        cout << "Time taken by function: "
             << duration.count() << " microseconds" << endl;
        
        return 0;
}
        
        

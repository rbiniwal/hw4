#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    void rebalance(AVLNode<Key,Value>* node);
    void rotateRight(AVLNode<Key, Value>* node);
    void rotateLeft(AVLNode<Key, Value>* node);
    void balanceHelper(AVLNode<Key, Value>* node);
    int getHeight(AVLNode<Key, Value>* node);


};

template<class Key, class Value>
void AVLTree<Key, Value>::insert(const std::pair<const Key, Value>& new_item) {
    if (BinarySearchTree<Key, Value>::root_ == nullptr) {
        BinarySearchTree<Key, Value>::root_ = new AVLNode<Key, Value>(new_item.first, new_item.second, nullptr);
        return;
    }
    AVLNode<Key, Value>* current = static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::root_);
    AVLNode<Key, Value>* parent = nullptr;
    while (current != nullptr) {
        parent = current;
        if (new_item.first < current->getKey()) {
            current = current->getLeft();
        } else if (new_item.first > current->getKey()) {
            current = current->getRight();
        } else {
            current->setValue(new_item.second);
            return;
        }
    }
    AVLNode<Key, Value>* newNode = new AVLNode<Key, Value>(new_item.first, new_item.second, parent);
    if (new_item.first < parent->getKey()) {
        parent->setLeft(newNode);
    } else {
        parent->setRight(newNode);
    }
    balanceHelper(parent);
    rebalance(parent);
}



/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::rebalance(AVLNode<Key, Value>* node) {
    if (node == nullptr) {
        return;
    }
    int balance = node->getBalance();
    if (balance < -1) {
        if (node->getLeft()->getBalance() < 0) {
            rotateRight(node);
        }
        else {
            rotateLeft(node->getLeft());
            rotateRight(node);
        }
    }
    else if (balance > 1) {
        if (node->getRight()->getBalance() > 0) {
            rotateLeft(node);
        }
        else {
            rotateRight(node->getRight());
            rotateLeft(node);
        }
    }
    balanceHelper(node);
    AVLNode<Key, Value>* parent = node->getParent();
    if (parent != nullptr) {
        rebalance(parent);
    }
}






/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::remove(const Key& key) {
    AVLNode<Key, Value>* nodeToRemove = static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::internalFind(key));
    if (nodeToRemove == nullptr) {
        return;
    }
    AVLNode<Key, Value>* parent = nodeToRemove->getParent();
    AVLNode<Key, Value>* pred = static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::predecessor(nodeToRemove));
    BinarySearchTree<Key, Value>::remove(key);
    if (parent == nullptr){
      balanceHelper(pred);
      rebalance(pred);
    }else{
      balanceHelper(parent);
      rebalance(parent);
    }
    if ((nodeToRemove == static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::root_)) && (getHeight(nodeToRemove) == 0)) {
        BinarySearchTree<Key, Value>::root_ = nullptr;
    }
}



template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}




template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* node) {
    if (node == nullptr || node->getRight() == nullptr) return;

    AVLNode<Key, Value>* rightChild = node->getRight();
    AVLNode<Key, Value>* leftOfRight = rightChild->getLeft();

    rightChild->setLeft(node);
    node->setRight(leftOfRight);

    if (leftOfRight != nullptr) {
        leftOfRight->setParent(node);
    }

    rightChild->setParent(node->getParent());
    if (node->getParent() == nullptr) {
        BinarySearchTree<Key, Value>::root_ = rightChild;
    } else if (node == node->getParent()->getLeft()) {
        node->getParent()->setLeft(rightChild);
    } else {
        node->getParent()->setRight(rightChild);
    }
    node->setParent(rightChild);


    balanceHelper(node);
}




template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* node) {
    if (node == nullptr || node->getLeft() == nullptr) return;

    AVLNode<Key, Value>* leftChild = node->getLeft();
    AVLNode<Key, Value>* rightOfLeft = leftChild->getRight();

    leftChild->setRight(node);
    node->setLeft(rightOfLeft);

    if (rightOfLeft != nullptr) {
        rightOfLeft->setParent(node);
    }

    leftChild->setParent(node->getParent());
    if (node->getParent() == nullptr) {
        BinarySearchTree<Key, Value>::root_ = leftChild;
    } else if (node == node->getParent()->getLeft()) {
        node->getParent()->setLeft(leftChild);
    } else {
        node->getParent()->setRight(leftChild);
    }
    node->setParent(leftChild);

    balanceHelper(node);
}



template<class Key, class Value>
void AVLTree<Key, Value>::balanceHelper(AVLNode<Key, Value>* node) {
    while (node != nullptr) {
        int leftHeight = getHeight(node->getLeft());
        int rightHeight = getHeight(node->getRight());
        int balance = rightHeight - leftHeight;
        node->setBalance(balance);
        node = node->getParent();
    }
}



template<class Key, class Value>
int AVLTree<Key, Value>::getHeight(AVLNode<Key, Value>* node) {
    if (node == nullptr) {
        return 0;
    }
    int leftHeight = getHeight(node->getLeft());
    int rightHeight = getHeight(node->getRight());

    return std::max(leftHeight, rightHeight) + 1;
}




#endif

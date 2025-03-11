#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here


bool equalPaths(Node * root)
{
    // Add your code below
    if(root == NULL){
        return true;
    }
    int len = -1;
    return mapNode(root, 0, len);

}

bool mapNode(Node * root, int temp, int& len)
{
    if(root == NULL){
        return true;
    }
    if((root->left == NULL) && (root->right == NULL)){
        if(len == -1){
            len = temp + 1;
        }
        return (temp + 1 == len);
    }
    bool l = mapNode(root->left, temp + 1, len);
    bool r = mapNode(root->right, temp + 1, len);
    return (l && r);
}


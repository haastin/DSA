#include <iostream>

class Node {
    public:
    int val;
    Node * left;
    Node * right;
    Node();
};

/**
 * If there are multiple nodes with the same value, the node first discovered will be returned/deleted
 */
class BST {
    public:
    Node * root;
    BST();
    Node * get_parent_node(int val);
    Node * insert_node(int val);
    Node * delete_node(int val);
    Node * search_for_node(int val);

    Node * get_in_order_successor(Node * node);
    Node * get_in_order_predecessor(Node * node);

    void in_order_traversal(Node* node);
    void pre_order_traversal(Node* node);
    void post_order_traversal(Node* node);

};

Node::Node(){
    val = 0;
    left = nullptr;
    right = nullptr;
}

BST::BST(){
    root = nullptr;
}

Node * BST::get_in_order_predecessor(Node * node){
    Node * left_tree_node = node->left;
    while(left_tree_node->right){
        left_tree_node = left_tree_node->right;
    }
    return left_tree_node;
}

Node * BST::get_in_order_successor(Node * node){
    Node * right_tree_node = node->right;
    while(right_tree_node->left){
        right_tree_node = right_tree_node->left;
    }
    return right_tree_node;
}

void BST::in_order_traversal(Node* node){
    if(node == this->root){
        std::cout << "in order traversal: ";
    }
    if(!node){
        return;
    }
    in_order_traversal(node->left);
    std::cout << node->val << " ";
    in_order_traversal(node->right);
    if(node == this->root){
        std::cout << std::endl;
    }
    return;
}

void BST::post_order_traversal(Node* node){
    if(node == this->root){
        std::cout << "post order traversal: ";
    }
    if(!node){
        return;
    }
    //std::cout << "curr node: " << node->val <<std::endl;
    post_order_traversal(node->left);
    post_order_traversal(node->right);
    std::cout << node->val << " ";
    if(node == this->root){
        std::cout << std::endl;
    }
    return;
}

void BST::pre_order_traversal(Node* node){
    if(node == this->root){
        std::cout << "pre order traversal: ";
    }
    if(!node){
        return;
    }
    std::cout << node->val << " ";
    pre_order_traversal(node->left);
    pre_order_traversal(node->right);
    if(node == this->root){
        std::cout << std::endl;
    }
    return;
}


Node * BST::get_parent_node(int val){
    
    if(root->val == val){
        return root;
    }

    Node * comparee = root;
    while(comparee){
        if((comparee->left && val == comparee->left->val) || (comparee->right && val == comparee->right->val)){
            return comparee;
        }
        else if(val < comparee->val){ 
            comparee = comparee->left;
        }
        else if(val > comparee->val){
            comparee = comparee->right;
        }
    }
    return nullptr;
}

/**
 * A BST is not guaranteed to be balanced, so a node will always be placed at a leaf position. Simply navigate to the proper leaf position in the tree to insert at based on the current state of the tree and place the node there.
 */
Node * BST::insert_node(int val){

    Node * new_node = new Node();
    new_node->val = val;
    new_node->left = nullptr;
    new_node->right = nullptr;

    if(root == nullptr){
        root= new_node;
        return root;
    }

    Node * comparee = root;
    while(comparee){
        if(new_node->val <= comparee->val){
            if(comparee->left){
                comparee = comparee->left;
            }
            else{
                std::cout << "inserted " << new_node->val << " as left child of " << comparee->val << std::endl;
                comparee->left = new_node;
                break;
            }
        }
        else{
            if(comparee->right){
                comparee = comparee->right;
            }
            else{
                std::cout << "inserted " << new_node->val << " as right child of " << comparee->val << std::endl;
                comparee->right = new_node;
                break;
            }
        }
    }
    return new_node;
}


/**
 * Navigate through the BST, taking advantage of the fact that it is sorted
 */
Node * BST::search_for_node(int val){

    Node * comparee = root;
    while(comparee){
        if(val == comparee->val){
            return comparee;
        }
        else if(val < comparee->val){ 
            comparee = comparee->left;
        }
        else if(val > comparee->val){
            comparee = comparee->right;
        }
    }
    return nullptr;
}

/**
 * 
 */
Node * BST::delete_node(int val){

    Node * node_to_delete = search_for_node(val);
    if(!node_to_delete){
        return nullptr;
    }
    Node * parent_node = get_parent_node(node_to_delete->val);

    //if the node to be deleted is a leaf
    if(node_to_delete->left == nullptr && node_to_delete->right == nullptr){
        if(parent_node->right && parent_node->right->val == node_to_delete->val){
            parent_node->right = nullptr;
        }
        else if(parent_node->left && parent_node->left->val == node_to_delete->val){
            parent_node->left = nullptr;
        }
        else{
            //in this case root is the only node and is deleted
            parent_node = nullptr;
        }
    }
    else if(node_to_delete->right == nullptr){
        //need to get the rightmost node in the left subtree of the node being deleted, as this node guarantees it is larger than any node in the right subtree
        Node * in_order_predecessor = get_in_order_predecessor(node_to_delete);
        Node * parent = get_parent_node(in_order_predecessor->val);
        if(in_order_predecessor->left){
            //if the in order predecessor has a kid, need to reassign it to the parent of the in order predecessor
            parent->right = in_order_predecessor->left;
        }
        else{
            parent->right = nullptr;
        }
        node_to_delete->val = in_order_predecessor->val;
        free(in_order_predecessor);
    }
    else{
        //in this case the right side exists, so we can get an in order successor. doesn't matter if the right side is the only side that exists or not, because even though you can choose either the delete node's in order successor or predecessor to replace it, by default choose successor
        
        //to get the in order successor, need to get the leftmost node in the right subtree of the node being deleted, as this node guarantees it is smaller than any node in the right subtree

        Node * in_order_successor = get_in_order_successor(node_to_delete);
        Node * parent = get_parent_node(in_order_successor->val);
        if(in_order_successor->right){
            parent->left = in_order_successor->right;
        }
        else{
            parent->left = nullptr;
        }
        node_to_delete->val = in_order_successor->val;
        free(in_order_successor);
    }

    //get the first in order traversal
    return node_to_delete;
}

int main(){
    BST * bst = new BST();
    //insert root
    bst->insert_node(25);
    //insert rhs
    bst->insert_node(50);
    bst->insert_node(70);
    bst->insert_node(66);
    bst->insert_node(90);
    bst->insert_node(35);
    bst->insert_node(31);
    bst->insert_node(44);
    //insert lhs
    bst->insert_node(15);
    bst->insert_node(10);
    bst->insert_node(4);
    bst->insert_node(12);
    bst->insert_node(22);
    bst->insert_node(18);
    bst->insert_node(24);

    bst->in_order_traversal(bst->root);
    bst->pre_order_traversal(bst->root);
    bst->post_order_traversal(bst->root);

    bst->delete_node(50);
    bst->in_order_traversal(bst->root);
    bst->pre_order_traversal(bst->root);
    bst->post_order_traversal(bst->root);
}
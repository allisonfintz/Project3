
#pragma once
#include <iostream>
#include <vector>
#include <string>
using namespace std;

// this stores the parts in each recipe
struct Information{
    Information() {

    }
    string recipeName;
    vector<string> ingredients;
    string instructions;
    Information(string name, vector<string> ingr, string instr){
        this->recipeName = name;
        this->ingredients = ingr;
        this->instructions = instr;
    }
};
// struct Node is cited from https://www.geeksforgeeks.org/insertion-in-red-black-tree/?ref=lbp, similar structure implemented
struct Node{
    Information recipe;
    // stores whether node is black or red
    char color;
    Node* left;
    Node* right;
    Node* parent;
    //Node() : recipe(recipe), left(nullptr), right(nullptr), color('r'), parent(nullptr) {}
    Node(Information recipe) : recipe(recipe), left(nullptr), right(nullptr), color('r'), parent(nullptr) {}

};

class RedBlackTree{
private:
    Node* root;
    bool rightRight;
    bool leftLeft;
    bool rightLeft;
    bool leftRight;
    // code for rotations are cited from https://github.com/Bibeknam/algorithmtutorprograms/blob/master/data-structures/red-black-trees/RedBlackTree.cpp
    Node* rotateRight(Node* node){
        Node* newParent = node->left;
        node->left = newParent->right;
        // if the newParent has a right child it will reassign its parent node
        if (newParent->right != nullptr) {
            newParent->right->parent = node;
        }
        // the newParents parent is the parent of the node we are rotating
        newParent->parent = node->parent;
        // if the nodes parent is nullptr, then the root of the red black tree must be updated
        if (node->parent == nullptr) {
            this->root = newParent;
        } else if (node == node->parent->left) {
            node->parent->left = newParent;
        } else {
            node->parent->right = newParent;
        }

        newParent->right = node;
        node->parent = newParent;

        return newParent;
    }
    // same logic as rightRotation
    Node* leftRotation(Node* node){
        Node* newParent = node->right;
        node->right = newParent->left;


        if (newParent->left != nullptr) {
            newParent->left->parent = node;
        }
        newParent->parent = node->parent;
        if (node->parent == nullptr) {
            this->root = newParent;
        } else if (node == node->parent->left) {
            node->parent->left = newParent;
        } else {
            node->parent->right = newParent;
        }

        newParent->left = node;
        node->parent = newParent;


        return newParent;
    }
// insertHelper is cited from cited from https://www.geeksforgeeks.org/insertion-in-red-black-tree/?ref=lbp, similar structure implemented
    Node* insertHelper(Node* root, Information recipe){
        bool conflict = false;
        if(root == nullptr){
            // new node created because there is nullptr found to insert recipe
            return new Node(recipe);
            // if the ascii of the given recipe name is less than the ascii of the node it is being compared to then the tree will be traversed to the left
        }else if(root->recipe.recipeName > recipe.recipeName){
            root->left = insertHelper(root->left, recipe);
            // new parent is the current node we were checking
            root->left->parent = root;
            // if our current node is not the root of the tree then we can check for conflicts
            if(root != this->root ){
                // conflict when there are two red nodes attached to each other
                if(root->color== 'r' && root->left->color == 'r'){
                    conflict = true;
                }
            }
        }else{
            root->right = insertHelper(root->right, recipe);
            root->right->parent = root;

            if(root != this->root ){
                if(root->color== 'r' && root->right->color == 'r'){
                    conflict = true;
                }
            }
        }
        // these must be in if statements so that the rotations are done at the gp of the node that was inserted
        if (rightRight){
            root = rotateRight(root);
            root->color = 'b';
            root->right->color = 'r';
            rightRight = false;
        }
        if(leftRight){
            root->left = leftRotation(root->left);
            root->left->parent = root;
            root = rotateRight(root);
            root->color = 'b';
            root->right->color = 'r';
            leftRight = false;
        }
        if(leftLeft){
            root = leftRotation(root);
            root->color = 'b';
            root->left->color = 'r';
            leftLeft = false;
        }
        if(rightLeft){
            root->right = rotateRight(root->right);
            root->right->parent = root;
            root = leftRotation(root);
            root->color = 'b';
            root->left->color = 'r';
            rightLeft = false;
        }
        if(conflict){
            // checks to see if the uncle is on the left or right
            if(root->parent->right != root){
                // if the uncle is black or null we perform rotations
                if(root->parent->right == nullptr || root->parent->right->color == 'b'){
                    if (root->left != nullptr && root->left->color == 'r'){
                        rightRight = true;
                    }
                    else if(root->right != nullptr && root->right->color == 'r'){
                        leftRight = true;
                    }
                }else{
                    // otherwise if uncle is red flip colors
                    if (root->parent != this->root){
                        root->parent->color = 'r';
                    }
                    root->parent->right->color = 'b';
                    root->color = 'b';
                }
            }else{
                if(root->parent->left == nullptr || root->parent->left->color == 'b'){
                    if (root->right != nullptr && root->right->color == 'r'){
                        leftLeft = true;
                    }
                    else if(root->left != nullptr && root->left->color == 'r'){
                        rightLeft = true;
                    }
                }else{
                    // if uncle is red flip colors
                    if (root->parent != this->root){
                        root->parent->color = 'r';
                    }
                    root->parent->left->color = 'b';
                    root->color = 'b';
                }
            }
        }
        return root;
    }
    // function returns a vector of nodes that matched with the ingredients the user inputted
    vector<Node*> matchRecipeHelper(Node* node, vector<string>& userIngredients, vector<Node*>& matchedIngredients, int& count){
        if(node == nullptr){
            return matchedIngredients;
        }
        // goes through each ingredient, once the count is == to 5 we can leave loop and add it to matchIngredients
        for(string ingredient: userIngredients){
            for(int i = 0; i < node->recipe.ingredients.size()+1; i++){
                if(ingredient == node->recipe.ingredients[i]){
                    count ++;
                }
            }
        }
        // 5 ingredients have been found in recipe so we can push back that recipe
        if (count>=5){
            matchedIngredients.push_back(node);
        }
        count = 0;
        // recursively check the rest of the tree
        matchRecipeHelper(node->left, userIngredients, matchedIngredients, count);
        matchRecipeHelper(node->right, userIngredients, matchedIngredients, count);
        return matchedIngredients;
    }

public:

    RedBlackTree() : root(nullptr), rightRight(false), leftLeft(false), rightLeft(false), leftRight(false){};
    // also cited from https://www.geeksforgeeks.org/insertion-in-red-black-tree/?ref=lbp to work with our dataset
    void insertRecipe(string name, vector<string> ingredient, string instructions){
        Information recipe;
        recipe.recipeName = name;
        recipe.ingredients = ingredient;
        recipe.instructions = instructions;
        // if the tree is empty the roots color will be black
        if (root == nullptr){
            root = insertHelper(root, recipe);
            root->color = 'b';
        }else{
            root = insertHelper(root, recipe);
        }
    }
    // prints the options for recipes for the user to choose through
    vector<Node*> printRecipe(vector<string> userIngredients) {
        vector<Node *> matchedIngredients;

        int count = 0;
        matchRecipeHelper(this->root, userIngredients, matchedIngredients, count);
        if (matchedIngredients.empty()) {
            cout << "Sorry we could not find any recipes with your list of ingredients." << endl;
        } else {
            for (int i = 0; i < matchedIngredients.size(); i++) {
                cout << matchedIngredients[i]->recipe.recipeName << endl;
            }
            return matchedIngredients;
        }
    }
    vector<Node*> matchRecipe(vector<string> userIngredients) {
        // returns a vector of all recipes possible
        vector<Node *> matchedIngredients;

        int count = 0;
        matchRecipeHelper(this->root, userIngredients, matchedIngredients, count);
        if (matchedIngredients.empty()) {
            cout << "Sorry we could not find any recipes with your list of ingredients." << endl;
        } else {
            return matchedIngredients;
        }
    }
    // prints the information for the given recipe that the user inputted
    void printInfo(string recipeName, vector<string> userIngredients){
        vector<Node *> matchedRecipes = matchRecipe(userIngredients);
        int count = 0;
        for (Node *entry: matchedRecipes) {
            if (entry->recipe.recipeName == recipeName) {
                string ingr = "";
                cout << "Recipe: " << entry->recipe.recipeName << endl;
                for (string j: entry->recipe.ingredients) {
                    ingr += j + " ,";
                }
                ingr = ingr.substr(0, ingr.size() - 2);
                cout << "Ingredients: " << ingr << endl;
                cout << "Instructions: " << entry->recipe.instructions << endl;
                count++;
            }
            if (count != 0){
                break;
            }
        }
    }


};
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <istream>
#include <fstream>
#include <iostream>
#include "readcsv.h"
#include "redblacktree.h"
#include "hash.h"


using namespace std;

int main() {
    BuildDataStructures dataStruct;
    RedBlackTree tree;
    tree = dataStruct.readFile("recipes.csv");
    HashTable h = dataStruct.readHashFile("recipes.csv");
    ifstream file("recipes.csv");
    string line;
    Recipe recipe;
    //h.printHash();
    string input;
    vector<string> ingredients;
    cout << "--------------------------------------" << endl;
    cout << "Welcome to Build Your Own Plate (BYOP)" << endl;
    cout << "--------------------------------------" << endl;
    cout << "With any ingredients you can find in your home, we can offer you a wide range of recipes to use" << endl;
    cout << "Please enter 5 ingredients you currently have:" << endl;
    cout << "1.";
    getline(cin, input);
    ingredients.push_back(" " + input);
    cout << "2.";
    getline(cin, input);
    ingredients.push_back(" " + input);
    cout << "3.";
    getline(cin, input);
    ingredients.push_back(" " + input);
    cout << "4.";
    getline(cin, input);
    ingredients.push_back(" " + input);
    cout << "5.";
    getline(cin, input);
    ingredients.push_back(" " + input);
    cout << "Here are a list of recipes using the current ingredients you have:" << endl;
    string k = "Yes";
    while (k == "Yes") {
        //tree.printRecipe(ingredients);
        h.matchIngredients(ingredients);
        cout << "Enter a recipe you would like to make from the list: " << endl;
        getline(cin, input);
        istringstream iss(input);
        //tree.printInfo(input, ingredients);
        h.printInfo(ingredients, input);
        cout << "Would you like to go back to the recipe list? (Yes or No)" << endl;
        getline(cin, k);
        if (k == "No" || k == "no")
            break;
    }
    cout << "-----------------------" << endl;
    cout << "Thank for playing BYOP!" << endl;
    cout << "I hope you enjoyed and feasted!" << endl;
    cout << "-------------------------------" << endl;
}
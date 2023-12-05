#pragma once
#include <iostream>
#include <list>
#include <utility>
#include <vector>
#include <string>

using namespace std;

//Create an information struct containing all the properties of the recipe
struct Info {
    vector<string> ingredients;
    string instructions;
    Info(vector<string> ingr, string instr){
        this->ingredients = ingr;
        this->instructions = instr;
    }
};

//create a map to store the key as the recipe name and the recipe properties
struct Map{
    string key;
    Info* value;
    Map(string k, Info* val){
        key = k;
        value = val;
    }
};

//create Hash Table
class HashTable{

private:
    //make a vector as the table and a vecter if Maps as the buckets
    vector<vector<Map*>> recipeTable;
    int size;
    //create a load factor
    const double load = 0.75;
    //resize based on size*2 if table is full
    void resize(int newCapacity) {
        vector<vector<Map*>> newMap(newCapacity);
        // Rehash existing elements into the new map
        double c = 0;
        for (const auto& bucket : recipeTable) {
            if(!bucket.empty())
                c++;
            for (Map* entry : bucket) {
                int newIndex = hash(entry->key, size);
                newMap[newIndex].push_back(entry);
            }
        }
        //find load factor of the number of full buckets divided by size
        if(c/(double)size >= load){
            size = newCapacity;
            recipeTable = std::move(newMap);
        }

    }

public:

    //initialize table to size
    explicit HashTable(int size) : recipeTable(size) {
        this->size = size;
    };

    //hash based off of ascii values of recipe name
    static int hash(const std::string& key, size_t tableSize) {
        int hashValue = 0;

        for (char ch : key) {
            hashValue += static_cast<int>(ch);
        }
        return hashValue % tableSize;
    }

    // print size
    void printSize(){
        cout << size << endl;
    }

    //print hash table
    void printHash(){
        int c = 0;
        for(vector<Map*> bucket: recipeTable){
            c++;
            cout << c << endl;
            for(Map* m: bucket){
                cout <<m->key;
            }
        }
    }

    //insert new recipe
    void insert( string key, vector<string> ingredients, string instructions) {
        bool insert = true;
        for (const auto &bucket: recipeTable) {
            if (bucket.size() > 0) {
                //find unique recipes
                for (Map *entry: bucket) {
                    if (entry->key == key)
                        insert = false;
                }
            }
        }
        //insert and resize
        if (insert) {
            int index = hash(key, size);
            Info *newInfo;
            newInfo = new Info(ingredients, instructions);
            Map *newMap = new Map(key, newInfo);
            recipeTable[index].push_back(newMap);
            resize(size * 2);
        }
    }

    //find matching ingredients
    vector<Map*> matchIngredients(vector<string> ingr){
        vector<Map*> matchedRecipes;
        for (const auto& bucket : recipeTable) {
            if(bucket.size() > 0) {
                for (Map* entry: bucket) {
                    int c = 0;
                    for (int i = 0; i < entry->value->ingredients.size(); i++) {
                        string temp = entry->value->ingredients[i];
                        for (int j = 0; j < ingr.size(); j++) {
                            if (temp == ingr[j]) {
                                c++;
                            }
                        }
                    }
                    if (c >= 5) {
                        string ingr = "";
                        cout << "Recipe: " << entry->key << endl;
                        matchedRecipes.push_back(entry);
                    }
                }
            }
        }
        return matchedRecipes;
    }

    //print based on users chosen recipe
    void printInfo(vector<string> ingredients, string recipe) {
        int c = 0;
        for (const auto &bucket: recipeTable) {
            if (bucket.size() > 0) {
                for (Map *entry: bucket) {
                    if (entry->key == recipe && c ==0) {
                        c++;
                        string ingr = "";
                        ingr = ingr.substr(0, ingr.size() - 2);
                        cout << "Ingredients: " << ingr << endl;
                        int k = 1;
                        for (string i: entry->value->ingredients) {
                            if(k == 1)
                                cout << k << ".  " << i << endl;
                            else
                                cout << k << ". " << i << endl;
                            k++;
                        }
                        cout << "Instructions: " << endl;
                        cout << entry->value->instructions << endl;
                    }
                }
            }
        }
    }
};








#include <iostream>
#include <list>
#include <utility>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include "hash.h"
#include "redblacktree.h"
#pragma once


using namespace std;

struct Recipe
{
    string title;
    string portions;
    string directions;
    vector<string> ingredients;
};

class BuildDataStructures
{
private:
    vector<string> parseCSVLine(const string &line)
    {
        stringstream ss(line);
        vector<string> result;
        bool inQuotes = false;
        string cell;
        char ch;

        while (ss.get(ch))
        {
            if (ch == '\"')
            {
                inQuotes = !inQuotes;
            }
            else if (ch == ',' && !inQuotes)
            {
                result.push_back(cell);
                cell.clear();
            }
            else
            {
                cell += ch;
            }
        }

        result.push_back(cell);
        return result;
    }

    string processString(const string& str)
    {
        string result;
        result.reserve(str.size());

        for (size_t i = 0; i < str.size(); ++i)
        {
            if (str[i] == '[' || str[i] == ']' || str[i] == '\"')
            {
                continue;
            }
            else if (str[i] == ';' || (str[i] == ',' && i > 0 && str[i-1] == '.'))
            {
                if (!result.empty() && result.back() != '.')
                {
                    result += '.';
                }
            }
            else
            {
                result += str[i];
            }
        }
        return result;
    }
    vector<string> splitIngredients(const string& str)
    {
        stringstream ss(str);
        vector<string> ingredients;
        string ingredient;

        while (getline(ss, ingredient, ','))
        {
            ingredients.push_back(processString(ingredient));
        }

        return ingredients;
    }
public:
    RedBlackTree readFile(string filename)
    {
        ifstream file;
        file.open(filename);
        string line;
        Recipe recipe;
        RedBlackTree tree;
        if (file.is_open())
        {
            getline(file, line);

            while (getline(file, line) && c!=0)
            {
                vector<string> parsedLine = parseCSVLine(line);
                if (parsedLine.size() >= 4)
                {
                    recipe.title = parsedLine[0];
                    recipe.portions = processString(parsedLine[1]);
                    recipe.directions = processString(parsedLine[2]);
                    recipe.ingredients = splitIngredients(parsedLine[3]);
                    tree.insertRecipe(recipe.title, recipe.ingredients, recipe.directions);
                }

            }
            file.close();
        }
        else
        {
            cout << "Unable to open file";
        }
        return tree;
    }
    HashTable readHashFile(string filename)
    {
        ifstream file;
        file.open(filename);
        string line;
        Recipe recipe;
        HashTable h(1000);
        if (file.is_open())
        {
            getline(file, line);

            int c = 5000;
            while (getline(file, line) && c != 0)
            {
                string oldWord = "\u00b0";
                string newWord = " degrees ";
                size_t found = line.find("\\u00b0");
                while (found != string::npos)
                {
                    line.replace(found, 7, newWord);
                    found = line.find(oldWord, newWord.length());
                }
                c--;
                vector<string> parsedLine = parseCSVLine(line);
                if (parsedLine.size() >= 4)
                {
                    recipe.title = parsedLine[0];
                    recipe.portions = processString(parsedLine[1]);
                    recipe.directions = processString(parsedLine[2]);
                    recipe.ingredients = splitIngredients(parsedLine[3]);
                    h.insert(recipe.title, recipe.ingredients, recipe.directions);
                }
            }
            file.close();
        }
        else
        {
            cout << "Unable to open file";
        }
        return h;
    }

};


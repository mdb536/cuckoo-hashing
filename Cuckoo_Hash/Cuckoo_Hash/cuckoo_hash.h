//
//  cuckoo_hash.hpp
//  Cuckoo_Hash
//
//  Created by malini balan on 1/9/18.
//  Copyright © 2018 nyu.edu. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <map>

using namespace std;

extern const int HEIGHT;
extern const int WIDTH;

//initializing, running, and closing the actual table
bool initScreen(int h, int w);
int run(int arr[][2], int n, map<string, int>cycles);

//render the table
void renderTable(int arr[][2], int n);
void renderHeader(int n);
void renderArr(int arr[][2], int n, int check, int oldVal);
void renderArrText(int num, int n, SDL_Rect textBox);

//rendering search and delete animations
void renderSearch(tuple<int, int>search, int val, int n, int fail);
void renderDelete(tuple<int, int>search, int arr[][2], int val, int n);

//render different error messages and alert messages
void renderEvictionMessage(int num);
void renderCycleMessage();
void renderMessage(const char* message, SDL_Rect textBox, SDL_Color color, SDL_Color bColor);
void renderMessage(const char* message, SDL_Rect textBox, SDL_Color color);


//deal with the calculator and buttons
bool renderButtons();
int pressKeys(int x, int y, int xMin, int yMin);
void renderCalcBox(string x);



//mains methods for dealing with the functional array
tuple<int, int> cuSearch(int arr[][2], int n, int val);
int cuDelete(int arr[][2], int n, int val);
int cuInsert(int arr[][2], int n, int val, int key);
void resetArr(int arr[][2], int n);

//deals with cycles --> failstate loops
bool cycleDetection(map< string, int> cycles);

//Node class used to render slots for insert and delete animations
class Node{
public:
    int val = -1;
    int key;
    int alpha;
    int destX;
    int destY;
    SDL_Rect rect;
    SDL_Rect textBox;
    
    void init(int v, int k, int i, int n, int x, int y);
    void renderNode();
    bool moveXY();
    bool reduceAlpha();
};


//functions to set up getting the initial N value for the table
int runN();
void renderNButtons();
void renderNCalcBox(string x);



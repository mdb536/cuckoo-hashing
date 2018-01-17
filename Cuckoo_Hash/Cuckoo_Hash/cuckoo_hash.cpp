//
//  cuckoo_hash.cpp
//  Cuckoo_Hash
//
//  Created by malini balan on 1/9/18.
//  Copyright © 2018 nyu.edu. All rights reserved.
//

#include "cuckoo_hash.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <tuple>
#include <map>

const int WIDTH = 800, HEIGHT = 700;
SDL_Window *window;
SDL_Renderer *renderer;

SDL_Texture *sButton;
SDL_Texture *iButton;
SDL_Texture *dButton;
SDL_Texture *rButton;
SDL_Texture *fButton;
SDL_Texture *nButton;
SDL_Texture *keypad;

TTF_Font *font;

SDL_Texture *function1;
SDL_Texture *function2;


using namespace std;

/*
 *initializes screen based on h, w input
 */
bool initScreen(int h, int w){
    
    
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0){
        cout << "SDL couldn't init " <<  SDL_GetError( ) << endl;
        return false;
    }
    
    window = SDL_CreateWindow("cuckoo hash", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h , SDL_WINDOW_ALLOW_HIGHDPI);
    
    if(window == nullptr){
        cout << "couldn't create window " << SDL_GetError( ) << endl;
        return false;
    }
    
    renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED );
    if(renderer == nullptr){
        cout << "couldn't create renderer" << SDL_GetError( ) << endl;
        return false;
    }
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    
    if(IMG_Init(IMG_INIT_PNG) < 0){
        cout << "couldn't init images" << endl;
        return false;
    }
    if(TTF_Init() < 0){
        cout << "couldn't init font" << endl;
        return false;
    }
    font = TTF_OpenFont("resources/arial.ttf", 50);
    
    SDL_RenderSetLogicalSize( renderer, w, h );
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255 );
    
    return true;
}


/*
 *runs the animation
 */
int run(int arr[][2], int n, map<string, int>cycles){
    bool contGame = true;
    
    //flags for rendering the animations
    bool searchFlag = false;
    bool insertFlag = false;
    bool deleteFlag = false;
    int insertFailCase = 0;

    //variables for insert, search and value informaton
    Node *insert = new Node;
    tuple<int, int>search;
    int val, oldVal = -1;
    int key = 0;
    string value = "0";
    
    while(contGame){
        SDL_Event windowEvent;
        while(SDL_PollEvent( &windowEvent ) ){
            if(windowEvent.type == SDL_QUIT ){
                contGame = false;
            }
            
            //deals with mouse events
            if(windowEvent.type == SDL_MOUSEBUTTONDOWN){
                int mx, my;
                SDL_GetMouseState( &mx, &my );
                
                if(my >= 660  && my <= 690){
                    
                    //sets flags and values for search if search button is pressed
                    if(mx >= 200  && mx <= 300){
                        insertFlag = false;
                        deleteFlag = false;
                        try {
                            val = stoi(value);
                            searchFlag = true;
                            search = cuSearch(arr, n, val);
                            
                        } catch (out_of_range& oor) {
                            SDL_Rect textBox;
                            textBox.h = 25;
                            textBox.w = 180;
                            textBox.x = (WIDTH - textBox.w)/2;
                            textBox.y = (HEIGHT - textBox.h)/2 + 50;
                            
                            renderMessage("ERR: out of range", textBox, {255, 0, 0}, {0,0,0});
                            SDL_RenderPresent(renderer);
                            SDL_Delay(1000);
                            searchFlag = false;
                            
                        }
                        value = "0";
                        
                    }
                    
                    //sets flags and values for insert if insert button is pressed
                    if(mx >= 350  && mx <= 450){
                        searchFlag = false;
                        deleteFlag = false;
                        try {
                            val = stoi(value);
                            key = 0;
                            cycles.clear();
                            insertFlag = true;
                            oldVal = cuInsert(arr, n, val, key);
                            if(oldVal == val){
                                insertFailCase = 1;
                            }
                            else
                                insertFailCase = 0;
                            search = cuSearch(arr, n, val);
                            string cKey = to_string(val) + "" + to_string(get<0>(search)) + " " + to_string(get<1>(search));
                            if(cycles.count(cKey))
                                cycles[cKey] += 1;
                            else
                                cycles[cKey] = 0;
                            
                            insert->init(val, get<0>(search), get<1>(search), n, -1, -1);
                            
                        } catch (out_of_range& oor) {
                            SDL_Rect textBox;
                            textBox.h = 25;
                            textBox.w = 180;
                            textBox.x = (WIDTH - textBox.w)/2;
                            textBox.y = (HEIGHT - textBox.h)/2 + 50;
                            
                            renderMessage("ERR: out of range", textBox, {255, 0, 0}, {0,0,0});
                            SDL_RenderPresent(renderer);
                            SDL_Delay(1000);
                            
                        }
                        value = "0";
                    }
                    
                    //sets flags and values for delete if delete button is pressed
                    if(mx >= 500  && mx <= 600){
                        searchFlag = false;
                        insertFlag = false;
                        try {
                            val = stoi(value);
                            deleteFlag = true;
                            search = cuSearch(arr, n, val);
                            cuDelete(arr, n, val);
                            if(get<0>(search) != -1){
                                insert->init(val, get<0>(search), get<1>(search), n, -1, -1);
                                insert->rect.x = insert->destX;
                                insert->rect.y = insert->destY;
                                insert->textBox.x = insert->destX + (insert->rect.w - insert->textBox.w)/2;
                                insert->textBox.y = insert->destY + (insert->rect.h - insert->textBox.h)/2;
                                
                                insert->alpha = 100;
                            }
                            
                        } catch (out_of_range& oor) {
                            SDL_Rect textBox;
                            textBox.h = 25;
                            textBox.w = 180;
                            textBox.x = (WIDTH - textBox.w)/2;
                            textBox.y = (HEIGHT - textBox.h)/2 + 50;
                            
                            renderMessage("ERR: out of range", textBox, {255, 0, 0}, {0,0,0});
                            SDL_RenderPresent(renderer);
                            SDL_Delay(1000);
                            deleteFlag = false;
                            
                        }
                        value = "0";
                        
                    }
                    //resets the array if the reset is pressed
                    if(mx >= 650 && mx <= 750){
                        cout << "RESET PRESSED" << endl;
                        resetArr(arr, n);
                        cycles.clear();
                        val = -1;
                        oldVal = -1;
                        value = "0";
                        insertFlag = false;
                        deleteFlag = false;
                        searchFlag = false;
                        
                    }
                    
                }
                //deals with calculator being pressed
                if(mx >=  325 && mx <= 475 && my >= 450 && my <= 650){
                    if(pressKeys( mx,  my, 325, 450) > 9){
                        if(value.length() > 1)
                            value.pop_back();
                    }
                    else
                        value += to_string(pressKeys( mx,  my, 325, 450));
                }
                
            }
        }
        
        SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
        SDL_RenderClear(renderer);
        
        //renders the basic table, current array and any animations necessary
        renderButtons();
        renderCalcBox(value);
        renderHeader(n);
        renderArr(arr, n, insert->val, oldVal);
        renderTable(arr, n);
        
        if(searchFlag){
            renderSearch(search, val, n, 0);
        }
        if(deleteFlag){
            renderDelete(search, arr, val, n);
            //if the delete was successful it renders a node in the bucket and reduces its opacity until blank
            if(get<0>(search) != -1){
                insert->renderNode();
                if(insert->reduceAlpha()){
                    deleteFlag = false;
                    insert->val = -1;
                }
            }
        }
        
        if(insertFlag && val != -1){
            //checks for cycles
            if(cycleDetection(cycles))
                renderCycleMessage();
            //checks if the insert was not needed
            if(insertFailCase == 1)
                renderSearch(search, val, n, 1);
            else{
                insert->renderNode();
                //Successful insert animation: renders node and moves it into bucket, then reduces opacity once in place
                if(insert->moveXY()){
                    if(insert->reduceAlpha()){
                        //if eviction is necessary it renders a new node and begins adding slots to the cycles
                        if(oldVal > -1){
                            val = oldVal;
                            if(key == 0)
                                key = 1;
                            else
                                key = 0;
                            oldVal = cuInsert(arr, n, val, key);
                            search = cuSearch(arr, n, val);
                            int x = insert->destX;
                            int y = insert->destY;
                            insert->init(val, get<0>(search), get<1>(search), n, x, y);
                            string cKey = to_string(val) + "" + to_string(get<0>(search)) + " " + to_string(get<1>(search));
                            if(cycles.count(cKey))
                                cycles[cKey] = cycles[cKey] + 1;
                            else
                                cycles[cKey] = 1;
                            
                        }
                        //reset once eviction no longer needed
                        else{
                            key = 0;
                            oldVal = -1;
                            insertFlag = false;
                            insert->val = -1;
                            cycles.clear();
                        }
                    }
                    //renders eviction message when needed
                    else if(oldVal > -1){
                        renderEvictionMessage(oldVal);
                    }
                }
            }
            
        }
        SDL_RenderPresent( renderer);
        
    }
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyTexture(sButton);
    SDL_DestroyTexture(iButton);
    SDL_DestroyTexture(dButton);
    SDL_DestroyTexture(rButton);
    SDL_DestroyTexture(fButton);
    SDL_DestroyTexture(nButton);
    SDL_DestroyTexture(keypad);
    SDL_DestroyTexture(function1);
    SDL_DestroyTexture(function2);

    SDL_Quit();
    return 0;

}

/*
 *Renders the inital tables based on the n value
 *adjusts bucket height based on n value to make the table fit in the screen
 */
void renderTable(int arr[][2], int n){
    
    SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255 );
    SDL_Rect bucket;
    bucket.x = 100;
    bucket.y = 50;
    bucket.w = 150;
    bucket.h = (HEIGHT-100)/n;
    SDL_RenderDrawRect( renderer, &bucket );
    
    int i;
    for(i = 0; i < n; i++){
        SDL_RenderDrawRect( renderer, &bucket );
        bucket.x = 550;
        
        SDL_RenderDrawRect( renderer, &bucket );
        bucket.x = 100;
        bucket.y += bucket.h;
        
    }
    
}

/*
 * Renders the function headers on the top of each table
 */
void renderHeader(int n){
    SDL_Color green = {0, 255, 0};
    SDL_Color bck = {0, 0, 0};
    
    string x = "f(x) = x%" + to_string(n);
    const char *y = x.c_str();
    SDL_Surface *sMessage = TTF_RenderText_Shaded(font, y, green, bck);
    
    function1 = SDL_CreateTextureFromSurface(renderer, sMessage);
    SDL_Rect space = {120, 15, 100, 25};
    SDL_RenderCopy(renderer, function1, NULL, &space);
    
    x = "g(x) = (x/" + to_string(n) + ")%" +  to_string(n);
    y = x.c_str();
    
    sMessage = TTF_RenderText_Shaded(font, y, green, bck);
    function2 = SDL_CreateTextureFromSurface(renderer, sMessage);
    space.x = 560;
    space.w = 120;
    SDL_RenderCopy(renderer, function2, NULL, &space);
    
    SDL_DestroyTexture(function1);
    SDL_DestroyTexture(function2);
    
    SDL_FreeSurface(sMessage);
    
}

/*
 * renders the current array in the table
 *(not including a val that is currently being inserted)
 */

void renderArr(int arr[][2], int n, int check, int oldVal){
    SDL_Rect textBox;
    textBox.x = 100;
    textBox.w = 0;
    textBox.h = ((HEIGHT-100)/n)/3;
    textBox.y = 50 + ((HEIGHT-100)/n - textBox.h)/2;
    
    int i;
    for(i = 0; i < n; i++){
        if(arr[i][0] != -1){
            if(arr[i][0] != check)
                renderArrText(arr[i][0], n, textBox);
            else if(oldVal != -1)
                renderArrText(oldVal, n, textBox);
            
            
            
        }
        if(arr[i][1] != -1){
            textBox.x = 550;
            if(arr[i][1] != check)
                renderArrText(arr[i][1], n, textBox);
            else if(oldVal != -1)
                renderArrText(oldVal, n, textBox);
            
        }
        
        textBox.x = 100;
        textBox.y += (HEIGHT-100)/n;
    }
    
}

/*
 *Helper function for renderArr, specifically renders an input value in the given spot
 */
void renderArrText(int num, int n, SDL_Rect textBox){
    string str = to_string(num);
    const char *sNum = str.c_str();
    int length = strlen(sNum);
    textBox.w = (100/n) * length;
    textBox.x += (150 - textBox.w)/2;
    
    SDL_Color green = {0, 255, 0};
    SDL_Color black = {0, 0, 0};
    
    renderMessage(sNum, textBox, green, black);
}

/*
 *renders the search animation based on whether it succeeded or failed
 *Failed: failure message + possible positions for val are rendered in red
 *Success: success message + position for val rendered in green and other position in red
 */
void renderSearch(tuple<int, int>search, int val, int n, int fail){
    SDL_Color textColor;
    SDL_Surface *sText;
    SDL_Texture *tText;
    const char *message;
    if(get<0>(search) == -1){
        string sMessage = "Search failed: "  + to_string(val) + " not found";
        message = sMessage.c_str();
        textColor = {255, 0, 0};
    }
    
    else{
        string sMessage = "Search successful: " + to_string(val) + " found";
        message = sMessage.c_str();
        textColor = {0, 255, 0};
        
    }
    if(fail == 1){
        string sMessage = "insert failed: " + to_string(val) + " already in table";
        message = sMessage.c_str();
        textColor = {255, 0, 0};
        
    }
    int length = strlen(message);
    
    SDL_Rect textBox;
    textBox.h = 20;
    textBox.w = 10*length;
    textBox.x = (WIDTH - textBox.w)/2;
    textBox.y = (HEIGHT - textBox.h)/2;
    
    sText = TTF_RenderText_Shaded(font, message, textColor, {0, 0, 0});
    tText = SDL_CreateTextureFromSurface(renderer, sText);
    SDL_RenderCopy(renderer, tText, NULL, &textBox);
    
    SDL_DestroyTexture(tText);
    SDL_FreeSurface(sText);
    
    int hi = val%n;
    int gi = (val/n)%n;
    
    SDL_Rect box;
    box.x = 100;
    box.h = (HEIGHT-100)/n;
    box.y = 50 + (box.h * hi);
    box.w = 150;
    SDL_SetRenderDrawColor( renderer, 255, 0, 0, 128 );
    
    
    if(get<0>(search) == 0){
        textColor = {255, 0, 0};
        SDL_SetRenderDrawColor( renderer, 0, 255, 0, 128 );
    }
    SDL_RenderFillRect(renderer, &box);
    SDL_SetRenderDrawColor( renderer, 255, 0, 0, 128 );
    box.x = 550;
    box.y = 50 + (box.h * gi);
    if(get<0>(search) == 1){
        textColor = {255, 0, 0};
        SDL_SetRenderDrawColor( renderer, 0, 255, 0, 128);
    }
    SDL_RenderFillRect(renderer, &box);
    
}

/*
 *Renders the delete based on whether the initial search was sucessful
 * Failed: failure message + possible positions rendered in red
 * Success: success message + other possible position rendered in red and the other in green
 */
void renderDelete(tuple<int, int>search, int arr[][2], int val, int n){
    SDL_Color textColor;
    
    int hi = val%n;
    int gi = (val/n)%n;
    
    SDL_Rect box;
    box.x = 100;
    box.h = (HEIGHT-100)/n;
    box.y = 50 + (box.h * hi);
    box.w = 150;
    SDL_SetRenderDrawColor( renderer, 255, 0, 0, 128 );
    
    
    if(get<0>(search) == 0){
        textColor = {255, 0, 0};
        SDL_SetRenderDrawColor( renderer, 0, 255, 0, 128 );
    }
    SDL_RenderFillRect(renderer, &box);
    SDL_SetRenderDrawColor( renderer, 255, 0, 0, 128 );
    box.x = 550;
    box.y = 50 + (box.h * gi);
    if(get<0>(search) == 1){
        textColor = {255, 0, 0};
        SDL_SetRenderDrawColor( renderer, 0, 255, 0, 128);
    }
    SDL_RenderFillRect(renderer, &box);
    const char *message;
    if(get<0>(search) == -1){
        string sMessage = "delete failed: "  + to_string(val) + " not found";
        message = sMessage.c_str();
        textColor = {255, 0, 0};
    }
    
    else{
        string sMessage = "delete successful: " + to_string(val) + " removed from table";
        message = sMessage.c_str();
        textColor = {0, 255, 0};
        
    }
    SDL_Rect textBox;
    textBox.h = 20;
    textBox.w = 10*strlen(message);
    textBox.x = (WIDTH - textBox.w)/2;
    textBox.y = (HEIGHT - textBox.h)/2;
    
    renderMessage(message, textBox, textColor, {0,0,0});
    
    
    
}

/*
 *renders the eviction needed message
 */
void renderEvictionMessage(int num){
    string sMessage = "eviction needed: evicting " + to_string(num);
    const char *message = sMessage.c_str();
    int length = strlen(message);
    SDL_Rect textBox;
    textBox.h = 20;
    textBox.w = 10*length;
    textBox.x = (WIDTH - textBox.w)/2;
    textBox.y = (HEIGHT - textBox.h)/2;
    SDL_Color white = {255, 255, 255};
    renderMessage(message, textBox, white, {0,0,0});
}

/*
 *renders the cycle detected needed message
 */
void renderCycleMessage(){
    string sMessage = "Insert Failed: Cycle detected, need to rehash";
    const char *message = sMessage.c_str();
    int length = strlen(message);
    SDL_Rect textBox;
    textBox.h = 25;
    textBox.w = 15*length;
    textBox.x = (WIDTH - textBox.w)/2;
    textBox.y = (HEIGHT - textBox.h)/2 + 50;
    SDL_Color red = {255, 0, 0};
    renderMessage(message, textBox, red,{0,0,0});
    
}

/*
 * two overloaded functions for rendering messages,
 * implementation depends on whether background color is known
 */
void renderMessage(const char* message, SDL_Rect textBox, SDL_Color color, SDL_Color bColor){
    SDL_Surface *sText = TTF_RenderText_Shaded(font, message, color, bColor);
    SDL_Texture *tText = SDL_CreateTextureFromSurface(renderer, sText);
    SDL_RenderCopy(renderer, tText, NULL, &textBox);
    
    SDL_DestroyTexture(tText);
    SDL_FreeSurface(sText);
    
}
void renderMessage(const char* message, SDL_Rect textBox, SDL_Color color){
    SDL_Surface *sText = TTF_RenderText_Solid(font, message, color);
    SDL_Texture *tText = SDL_CreateTextureFromSurface(renderer, sText);
    SDL_RenderCopy(renderer, tText, NULL, &textBox);
    
    SDL_DestroyTexture(tText);
    SDL_FreeSurface(sText);
    
}

/*
 *loads and renders the buttons and the keypad
 */
bool renderButtons(){
    SDL_Rect space;
    space.x = 50;
    space.y = 660;
    space.w = 100;
    space.h = 30;
    
    //    fButton = IMG_LoadTexture(renderer, "img/failstate.png");
    //    if(fButton == nullptr){
    //        cout << "error loading search button" << endl;
    //    }
    //    SDL_RenderCopy(renderer, fButton, NULL, &space);
    
    sButton = IMG_LoadTexture(renderer, "img/search.png");
    if(sButton == nullptr){
        cout << "error loading search button" << endl;
    }
    space.x = 200;
    SDL_RenderCopy(renderer, sButton, NULL, &space);
    
    iButton = IMG_LoadTexture(renderer, "img/insert.png");
    if(iButton == nullptr){
        cout << "error loading insert button" << endl;
    }
    space.x = 350;
    SDL_RenderCopy(renderer, iButton, NULL, &space);
    
    
    dButton = IMG_LoadTexture(renderer, "img/delete.png");
    if(dButton == nullptr){
        cout << "error loading delete button" << endl;
    }
    space.x = 500;
    SDL_RenderCopy(renderer, dButton, NULL, &space);
    
    rButton = IMG_LoadTexture(renderer, "img/reset.png");
    if(rButton == nullptr){
        cout << "error loading delete button" << endl;
    }
    space.x = 650;
    SDL_RenderCopy(renderer, rButton, NULL, &space);
    
    keypad = IMG_LoadTexture(renderer, "img/keypad.png");
    if(keypad == nullptr){
        cout << "error loading delete button" << endl;
    }
    space.x = 325;
    space.y = 450;
    space.h = 200;
    space.w = 150;
    SDL_RenderCopy(renderer, keypad, NULL, &space);
    
    SDL_DestroyTexture(sButton);
    SDL_DestroyTexture(dButton);
    SDL_DestroyTexture(iButton);
    SDL_DestroyTexture(rButton);
    
    SDL_DestroyTexture(keypad);
    
    return true;
}

/*
 *calcuates what digit to return based on where the keypad was pressed
 */
int pressKeys(int x, int y, int xMin, int yMin){
    x = x - xMin;
    y = y - yMin;
    
    x = x/50;
    y = y/50;
    int value = x+(y*3);
    
    return value;
}

/*
 *renders the cuurent calculator value
 */
void renderCalcBox(string x){
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect space;
    space.x = 325;
    space.y = 400;
    space.w = 150;
    space.h = 100;
    
    const char* message = x.c_str();
    SDL_RenderDrawRect(renderer, &space);
    SDL_Rect textBox;
    textBox.x = 330;
    textBox.y = 425;
    textBox.h = 20;
    textBox.w = 12*(strlen(message));
    
    SDL_Color white = {255, 255, 255};
    renderMessage(message, textBox, white);
    
}

/*
 *search function for functional array
 *failed: returns -1,-1
 *Success: returns array position tuple
 */
tuple<int, int> cuSearch(int arr[][2], int n, int val){
    int hKey = val%n;
    int gKey = (val/n)%n;
    if(arr[hKey][0] == val){
        return tuple<int, int>(0, hKey);
    }
    else if(arr[gKey][1] == val){
        return tuple<int, int>(1, gKey);
    }
    else{
        return tuple<int, int>(-1, -1);
    }
}

/*
 * deals with deletions from array
 *success: returns the key
 *failed: returns 0
 */
int cuDelete(int arr[][2], int n, int val){
    int hKey = val%n;
    int gKey = (val/n)%n;
    if(arr[hKey][0] == val){
        arr[hKey][0] = -1;
        return hKey;
    }
    else if(arr[gKey][1] == val){
        
        return gKey;
    }
    return 0;
}

/*
 *simple insertions into a slot
 *returns the evicted value (-1 if there was no val to be evicted)
 */
int cuInsert(int arr[][2], int n, int val, int key){
    int place;
    int oldVal;
    if(key == 0){
        place = val%n;
    }
    else{
        place = (val/n)%n;
    }
    oldVal = arr[place][key];
    arr[place][key] = val;
    return oldVal;
}

/*
 * resets all the values in the array back into -1
 */
void resetArr(int arr[][2], int n){
    int i;
    for(i = 0; i < n;  i++ ){
        arr[i][0] = -1;
        arr[i][1] = -1;
    }
    
}

/*
 *checks the cycle map if a node has entered a specific slot more than twice
 *there has to be a cycle so it retuns true
 */
bool cycleDetection(map< string, int> cycles){
    for(map<string,int>::iterator it=cycles.begin(); it!=cycles.end(); ++it){
        cout << it->first << " :: " << it->second <<endl;
        if(it->second > 2){
            return true;
        }
    }
    return false;
}

/*
 * initializes the new node
 */
void Node::init(int v, int k, int i, int n, int x, int y){
    val = v;
    key = k;
    alpha = 255;
    rect.w = 150;
    rect.h = (600/n);
    if(x == -1)
        rect.x = (WIDTH-rect.w)/2;
    else
        rect.x = x;
    
    if(y == -1)
        rect.y = (400-rect.h)/2;
    else
        rect.y = y;
    
    if(key == 0){
        destX = 100;
    }
    else{
        destX = 550;
    }
    destY = 50 + (rect.h) * i;
    
    const char *text = to_string(val).c_str();
    textBox.h = ((HEIGHT-100)/n)/3;
    textBox.w = (100/n) * strlen(text);
    textBox.y = (rect.y + (rect.h - textBox.h)/2 );
    textBox.x = (rect.x + (rect.w - textBox.w)/2  );
    
}

/*
 *renders node
 */
void Node::renderNode(){
    SDL_SetRenderDrawColor( renderer, 0, 255, 0, alpha );
    SDL_RenderFillRect(renderer, &rect);
    const char *text = to_string(val).c_str();
    SDL_Color black = {0, 0, 0};
    SDL_Color green = {0, 255, 0};
    renderMessage(text,  textBox, black, green);
}

/*
 *moves the XY values in the Node
 */
bool Node::moveXY(){
    if(rect.x != destX){
        if(rect.x < destX){
            if(destX-rect.x < 5){
                rect.x += destX-rect.x;
                textBox.x += destX-rect.x;
            }
            else{
                rect.x += 5;
                textBox.x += 5;
                
            }
        }
        else{
            if(rect.x-destX < 5){
                rect.x -= rect.x-destX;
                textBox.x -= rect.x-destX;
            }
            else{
                rect.x -=  5;
                textBox.x -=  5;
                
            }
        }
    }
    else if(rect.y != destY){
        if(rect.y < destY){
            if(destY-rect.y < 5){
                rect.y += destY-rect.y;
                textBox.y += destY-rect.y;
                
            }
            else{
                rect.y += 5;
                textBox.y += 5;
                
            }
        }
        else{
            if(rect.y-destY < 5){
                rect.y -= rect.y-destY;
                textBox.y -= rect.y-destY;
                
            }
            else{
                rect.y -=  5;
                textBox.y -=  5;
                
            }
        }
    }
    if(rect.y == destY && rect.x == destX)
        return true;
    return false;
}

/*
 *reduces the alpha value
 */
bool Node::reduceAlpha(){
    if(alpha> 0)
        alpha -= 5;
    if(alpha == 0)
        return true;
    return false;
}


/*
 *runs the screen for setting the N value
 */
int runN(){
    bool contRun = true;
    int n = 0;
    string value = "0";
    while(contRun){
        SDL_Event windowEvent;
        while(SDL_PollEvent( &windowEvent ) ){
            if(windowEvent.type == SDL_MOUSEBUTTONDOWN){
                if(windowEvent.type == SDL_QUIT )
                    contRun = false;
                int mx, my;
                SDL_GetMouseState( &mx, &my );
                if(my >= 50 && my <= 250){
                    if(pressKeys( mx,  my, 0, 50) > 9){
                        if(value.length() > 1)
                            value.pop_back();
                    }
                    else
                        value += to_string(pressKeys( mx,  my, 0, 50));
                }
                else if(my > 250){
                    try {
                        int val = stoi(value);
                        if(val > 0){
                            n = val;
                            contRun = false;
                        }
                        
                    } catch (out_of_range& oor) {
                        SDL_Rect textBox;
                        textBox.x = 50;
                        textBox.y = 50;
                        textBox.h = 20;
                        textBox.w = 180;
                        
                        renderMessage("out of range", textBox, {255, 0, 0}, {0, 0, 0 });
                        SDL_RenderPresent(renderer);
                        SDL_Delay(100);
                    }
                }
            }
        }
        SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
        SDL_RenderClear(renderer);
        renderNButtons();
        renderNCalcBox(value);
        SDL_RenderPresent(renderer);
    }
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);

    return n;
}



/*
 *renders the keypad and the setNvalue button
 */
void renderNButtons(){
    SDL_Rect space;
    keypad = IMG_LoadTexture(renderer, "img/keypad.png");
    if(keypad == nullptr){
        cout << "error loading keypad" << endl;
    }
    space.x = 0;
    space.y = 50;
    space.h = 200;
    space.w = 150;
    SDL_RenderCopy(renderer, keypad, NULL, &space);
    
    space.x = 0;
    space.y = 250;
    space.h = 50;
    nButton = IMG_LoadTexture(renderer, "img/n.png");
    if(nButton == nullptr){
        cout << "error loading n button" << endl;
    }
    SDL_RenderCopy(renderer, nButton, NULL, &space);

    
}


/*
 *renders the calc box for displaying N value while it's being set
 */
void renderNCalcBox(string x){
    SDL_Rect space;
    space.x = 0;
    space.y = 0;
    space.w = 150;
    space.h = 50;
    const char* message = x.c_str();
    SDL_RenderDrawRect(renderer, &space);
    SDL_Rect textBox;
    textBox.x = 10;
    textBox.y = 10;
    textBox.h = 20;
    textBox.w = 12*(strlen(message));
    if(textBox.w > 140)
        textBox.w = 140;
    
    SDL_Color white = {255, 255, 255};
    renderMessage(message, textBox, white);


}


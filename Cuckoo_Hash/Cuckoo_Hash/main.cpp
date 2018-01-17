#include "cuckoo_hash.h"
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include <cmath>
using namespace std;



int main(int argc, const char * argv[]) {
    
    
    
    initScreen(300, 150);
    int n =  runN();

    if(n > 0){
        initScreen(HEIGHT, WIDTH);
        int hash[n][2];
        int i;
        for(i = 0; i < n; i++){
            hash[i][0] = -1;
            hash[i][1] = -1;
        }
        
        map<string, int>cycles;
        run(hash, n, cycles);
    }
    
}

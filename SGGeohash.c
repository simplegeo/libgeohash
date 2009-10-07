/*
 *  SGGeohash.c
 *  libgeohash
 *
 *  Created by Derek Smith on 10/6/09.
 *  Copyright 2009 SimpleGeo. All rights reserved.
 *
 */

#include "SGGeohash.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAX_LAT             90.0
#define MIN_LAT             -90.0

#define MAX_LONG            180.0
#define MIN_LONG            -180.0

#define NORTH               0
#define EAST                1
#define SOUTH               2
#define WEST                3

typedef struct SGIntervalStruct {
    
    double high;
    double low;
    
} SGInterval;

static char charMap[32] =  "0123456789bcdefghjkmnpqrstuvwxyz";

/*
 *  The follow character maps were created by Dave Troy and used in his Javascript Geohashing
 *  library. http://github.com/davetroy/geohash-js
 */

static char* evenNeighbors[] = {"p0r21436x8zb9dcf5h7kjnmqesgutwvy",
                                "bc01fg45238967deuvhjyznpkmstqrwx", 
                                "14365h7k9dcfesgujnmqp0r2twvyx8zb",
                                "238967debc01fg45kmstqrwxuvhjyznp"
                                };

static char* oddNeighbors[] = {"bc01fg45238967deuvhjyznpkmstqrwx", 
                               "p0r21436x8zb9dcf5h7kjnmqesgutwvy",
                                "238967debc01fg45kmstqrwxuvhjyznp",
                               "14365h7k9dcfesgujnmqp0r2twvyx8zb"    
                                };

static char* evenBorders[] = {"prxz", "bcfguvyz", "028b", "0145hjnp"};
static char* oddBorders[] = {"bcfguvyz", "prxz", "0145hjnp", "028b"};

unsigned int IndexForChar(char c, char* string) {
    
    int index = -1;
    int stringAmount = strlen(string);
    for(int i = 0; i < stringAmount; i++) {
        
        if(c == string[i]) {
            
            index = i; 
            break;
        }
        
    }
    
    return index;
}

//srcHash = srcHash.toLowerCase();
//var lastChr = srcHash.charAt(srcHash.length-1);
//var type = (srcHash.length % 2) ? 'odd' : 'even';
//var base = srcHash.substring(0,srcHash.length-1);
//if (BORDERS[dir][type].indexOf(lastChr)!=-1)
//base = calculateAdjacent(base, dir);
//return base + BASE32[NEIGHBORS[dir][type].indexOf(lastChr)];

char* GetNeighbor(char* hash, int direction) {
    
    int hashLength = strlen(hash);
    
	char lastChar = hash[hashLength - 1];
    
    int isOdd = hashLength % 2;
    char** border = isOdd ? oddBorders : evenBorders;
    char** neighbor = isOdd ? oddNeighbors : evenNeighbors; 
    
    char* base = malloc(sizeof(char) * 1);
    base[0] = '\0';
    strncat(base, hash, hashLength - 1);
    
	if(IndexForChar(lastChar, border[direction]) != -1)
		base = GetNeighbor(base, direction);
    
    int indexOfNeighbor = IndexForChar(lastChar, neighbor[direction]);
    lastChar = charMap[indexOfNeighbor];
        
    char* lastHash = malloc(sizeof(char) * 2);
    lastHash[0] = lastChar;
    lastHash[1] = '\0';
    strcat(base, lastHash);
    free(lastHash);
    
	return base;
}


char* SGGeohashEncode(double lat, double lng, int precision) {
    
    if(precision < 1 || precision > 20)
        precision = 12;     // Default at 12
    
    char* hash = NULL;
    
    if(lat <= 90.0 && lat >= -90.0 && lng <= 180.0 && lng >= -180.0) {
        
        hash = (char*)malloc(sizeof(char) * (precision + 1));
        hash[precision] = '\0';
        
        precision *= 5.0;
        
        SGInterval latInterval = {MAX_LAT, MIN_LAT};
        SGInterval lngInterval = {MAX_LONG, MIN_LONG};
        SGInterval* interval;
        double coord, mid;
        int isEven = 1;
        unsigned int hashChar = 0;
        for(int i = 1; i <= precision; i++) {
         
            if(isEven) {
            
                interval = &lngInterval;
                coord = lng;                
                
            } else {
                
                interval = &latInterval;
                coord = lat;   
            }
            
            mid = (interval->low + interval->high) / 2.0;
            hashChar = hashChar << 1;
            
            if(coord > mid) {
                
                interval->low = mid;
                hashChar |= 0x01;
                
            } else
                interval->high = mid;
            
            if(!(i % 5)) {
                
                hash[(i - 1) / 5] = charMap[hashChar];
                hashChar = 0;

            }
            
            isEven = !isEven;
        }
     
        
    }
    
    return hash;
}

SGGeoCoord SGGeohashDecode(char* hash) {
    
    SGGeoCoord coordinate = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    
    if(hash) {
        
        int charAmount = strlen(hash);
        
        if(charAmount) {
            
            unsigned int charMapIndex;
            SGInterval latInterval = {MAX_LAT, MIN_LAT};
            SGInterval lngInterval = {MAX_LONG, MIN_LONG};
            SGInterval *interval;
        
            int isEven = 1;
            double delta;
            for(int i = 0; i < charAmount; i++) {
            
                charMapIndex = IndexForChar(hash[i], (char*)charMap);
                
                if(charMapIndex < 0)
                    break;
            
                // Interpret the last 5 bits of the integer
                for(int j = 0; j < 5; j++) {
                
                    interval = isEven ? &lngInterval : &latInterval;
                
                    delta = (interval->high - interval->low) / 2.0;
                
                    if((charMapIndex << j) & 0x0010)
                        interval->low += delta;
                    else
                        interval->high -= delta;
                
                    isEven = !isEven;
                }
            
            }
            
            coordinate.latitude = latInterval.high - ((latInterval.high - latInterval.low) / 2.0);
            coordinate.longitude = lngInterval.high - ((lngInterval.high - lngInterval.low) / 2.0);
            
            coordinate.north = latInterval.high;
            coordinate.east = lngInterval.high;
            coordinate.south = latInterval.low;
            coordinate.west = lngInterval.low;
        }
    }
    
    return coordinate;
}


char** SGGeohashNeighbors(char* hash) {

    char** neighbors = NULL;
    
    if(hash) {
        
        // N, NE, E, SE, S, SW, W, NW
        neighbors = (char**)malloc(sizeof(char*) * 8);
        
        neighbors[0] = GetNeighbor(hash, NORTH);
        neighbors[1] = GetNeighbor(neighbors[0], EAST);
        neighbors[2] = GetNeighbor(hash, EAST);
        neighbors[3] = GetNeighbor(neighbors[2], SOUTH);
        neighbors[4] = GetNeighbor(hash, SOUTH);
        neighbors[5] = GetNeighbor(neighbors[4], WEST);                
        neighbors[6] = GetNeighbor(hash, WEST);
        neighbors[7] = GetNeighbor(neighbors[6], NORTH);        


    
    }
    
    return neighbors;
}


/*
 *  SGGeohash.c
 *  libgeohash
 *
 *  Created by Derek Smith on 10/6/09.
 *  Copyright 2009 CrashCorp. All rights reserved.
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

typedef struct SGIntervalStruct {
    
    double high;
    double low;
    
} SGInterval;

static unsigned char charMap[32] = 
"0123456789bcdefghjkmnpqrstuvwxyz";

unsigned int IndexForChar(char c) {
    
    int index = -1;
    for(int i = 0; i < 32; i++) {
        
        if(c == charMap[i]) {
            index = i; 
            break;
        }
        
    }
    
    return index;
}


char* SGGeohashEncode(double lat, double lng, int precision) {
    
    
    return NULL;
}

SGGeoCoord SGGeohashDecode(char* hash) {
    
    SGGeoCoord coordinate = {0.0, 0.0};
    
    if(hash) {
        
        int charAmount = strlen(hash);
        
        if(charAmount) {
            
            unsigned int charMapIndex;
            SGInterval latInterval = {MAX_LAT, MIN_LAT};
            SGInterval lngInterval = {MAX_LONG, MIN_LONG};
            SGInterval* interval, *firstInterval, *secondInterval;
        
            double delta;
            for(int i = 0; i < charAmount; i++) {
            
                charMapIndex = IndexForChar(hash[i]);
            
                if(i % 2) {
                
                    firstInterval = &latInterval;
                    secondInterval = &lngInterval;
                
                } else {
                
                    firstInterval = &lngInterval;
                    secondInterval = &latInterval;
                }
            
            
                // Interpret the last 5 bits of the integer
                for(int j = 0; j < 5; j++) {
                
                    interval = j % 2 ? secondInterval : firstInterval;
                
                    delta = (interval->high - interval->low) / 2.0;
                
                    if((charMapIndex << j) & 0x00000010)
                        interval->low += delta;
                    else
                        interval->high -= delta;
                
                    printf("%s %f %f %i %i\n", interval == &latInterval ? "Lat" : "Lng", interval->high, interval->low, charMapIndex, charMapIndex << j);
                
                }
            
            }
        
            coordinate.latitude = latInterval.high - (latInterval.high - latInterval.low);
            coordinate.longitude = lngInterval.high - (lngInterval.high - lngInterval.low);
        }
    }
    
    return coordinate;
}


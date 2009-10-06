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
    
    if(precision < 1 || precision > 20)
        precision = 10;
    
    char* hash = NULL;
    
    if(lat <= 90.0 && lat >= -90.0 && lng <= 180.0 && lng >= -180.0) {
        
        hash = (char*)malloc(sizeof(char) * precision + 1);
        hash[precision] = '0';
        
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
             
                hash[i / 5] = charMap[hashChar];
                hashChar = 0;

            }
            
            isEven = !isEven;
        }
     
        
    }
    
    return hash;
}

SGGeoCoord SGGeohashDecode(char* hash) {
    
    SGGeoCoord coordinate = {0.0, 0.0};
    
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
            
                charMapIndex = IndexForChar(hash[i]);
                
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
        }
    }
    
    return coordinate;
}


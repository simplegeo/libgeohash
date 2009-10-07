//
//  SGGeohashTest.c
//  libgeohash
//
//  Created by Derek Smith on 10/6/09.
//  Copyright 2009 SimpleGeo. All rights reserved.
//

#include "SGGeohash.h"

#include <string.h>
#include <stdio.h>

void checkCoords(SGGeoCoord coord, SGGeoCoord expected) {
 
    char* valueTitle = NULL;
    double expectedValue;
    double value;
    
    if(coord.latitude != expected.latitude) {
        
        valueTitle = "latitude";
        expectedValue = expected.latitude;
        value = coord.latitude;
        
    } else if(coord.longitude != expected.longitude) {

        valueTitle = "longitude";
        expectedValue = expected.longitude;
        value = coord.longitude;
        
        
    } else if(coord.north != expected.north) {

        valueTitle = "north";
        expectedValue = expected.north;
        value = coord.north;
        
        
    } else if(expected.south != coord.south) {

        valueTitle = "south";
        expectedValue = expected.south;
        value = coord.south;
        
    } else if(coord.east != expected.east) {

        valueTitle = "east";
        expectedValue = expected.east;
        value = coord.east;
        
    } else if(expected.west != coord.west) {

        valueTitle = "west";
        expectedValue = expected.west;
        value = coord.west;
        
    }
    
    if(valueTitle)
        printf("Error: Expected %.16f but was %.16f for %s\n", expectedValue, value, valueTitle);
    
}

void checkHashes(char* hash, char* expected) {
    
    if(strcmp(hash, expected) != 0) {
        printf("Error: Expected hash = %s. (%s)\n", hash, expected);
//        abort();
    }
}

void checkNeighbors(char** neighbors, char** expectedNeighbors) {
 
    for(int i = 0; i < 8; i++)
        if(strcmp(neighbors[i], expectedNeighbors[i]) != 0)
            printf("Error: Expected hash = %s at index %i. (%s)\n", expectedNeighbors[i], i, neighbors[i]);

}

int main() {

    // Decoder
    
    SGGeoCoord coord = SGGeohashDecode("ezs42");
    SGGeoCoord expectedCoord = {42.60498046875, -5.60302734375, 42.626953125, -5.5810546875, 42.5830078125, -5.625};
    checkCoords(coord, expectedCoord);

    
    coord = SGGeohashDecode("ezs42gx");
    expectedCoord = (SGGeoCoord){42.602920532226562, -5.5817413330078125, 42.603607177734375, -5.581054687500000, 42.60223388671875, -5.582427978515625};
    checkCoords(coord, expectedCoord);
    
    coord = SGGeohashDecode("9xj5smj4w40");
    expectedCoord = (SGGeoCoord){40.018140748143196, -105.27485780417919, 40.01814141869545, -105.27485713362694, 40.018140077590942, -105.27485847473145};
    checkCoords(coord, expectedCoord);
    
    // Encoder
    
    char* hash = SGGeohashEncode(42.60498046875, -5.60302734375, 5);
    checkHashes(hash, "ezs42");
    
    hash = SGGeohashEncode(40.018141, -105.274858, 12);
    checkHashes(hash, "9xj5smj4w40m");

    hash = SGGeohashEncode(40.018141, -105.274858, 2);
    checkHashes(hash, "9x");
    
    hash = SGGeohashEncode(40.018141, -105.274858, 0);
    checkHashes(hash, "9xj5smj4w40m");
    
    // Neighbors
    
    char** neighbors = SGGeohashNeighbors("ezs42");

    char* expectedNeighbors[8] = {"ezs48", "ezs49", "ezs43", "ezs41", "ezs40", "ezefp", "ezefr", "ezefx"};
    checkNeighbors(neighbors, expectedNeighbors);
    
    expectedNeighbors[0] = "9xj5smj4w40q"; expectedNeighbors[1] = "9xj5smj4w40w";
    expectedNeighbors[2] = "9xj5smj4w40t"; expectedNeighbors[3] = "9xj5smj4w40s";
    expectedNeighbors[4] = "9xj5smj4w40k"; expectedNeighbors[5] = "9xj5smj4w40h";
    expectedNeighbors[6] = "9xj5smj4w40j"; expectedNeighbors[7] = "9xj5smj4w40n";
    
    neighbors = SGGeohashNeighbors("9xj5smj4w40m");
    checkNeighbors(neighbors, expectedNeighbors);
    
    
    return 0;
}








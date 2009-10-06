//
//  SGGeohashTest.c
//  libgeohash
//
//  Created by Derek Smith on 10/6/09.
//  Copyright 2009 CrashCorp. All rights reserved.
//

#include "SGGeohash.h"

#include <string.h>
#include <stdio.h>

void checkCoords(SGGeoCoord coord, SGGeoCoord expected) {
 
    if(coord.latitude != expected.latitude || coord.longitude != expected.longitude)
        printf("Error: Expected lat = %.11f and long = %.11f. (%.11f, %.11f)\n", expected.latitude,
                expected.longitude, coord.latitude, coord.longitude);
    
}

void checkHashes(char* hash, char* expected) {
    
    if(!strcmp(hash, expected))
        printf("Error: Expected hash = %s. (%s)", hash, expected);
}

int main() {

    SGGeoCoord coord = SGGeohashDecode("ezs42");
    SGGeoCoord expectedCoord = {42.60498046875, -5.60302734375};
    checkCoords(coord, expectedCoord);
    
    coord = SGGeohashDecode("ezs42gx");
    expectedCoord = (SGGeoCoord){42.62489318848, -5.58174133301};
    checkCoords(coord, expectedCoord); 
    
    char* hash = SGGeohashEncode(42.60498046875, -5.60302734375, 5);
    checkHashes(hash, "ezs42");

    return 0;
}


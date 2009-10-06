//
//  SGGeohashTest.c
//  libgeohash
//
//  Created by Derek Smith on 10/6/09.
//  Copyright 2009 CrashCorp. All rights reserved.
//

#include "SGGeohash.h"

#include <stdio.h>

int main() {

    SGGeoCoord coord = SGGeohashDecode("ezs42");
    if(coord.latitude != 42.6 || coord.longitude == -5.6)
        printf("Error: Expected lat = 42.6 and long -5.6. (%f, %f)\n", coord.latitude, coord.longitude);

    return 0;
}


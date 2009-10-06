/*
 *  SGGeohash.h
 *  libgeohash
 *
 *  Created by Derek Smith on 10/6/09.
 *  Copyright 2009 CrashCorp. All rights reserved.
 *
 */

typedef struct SGGeoCoordStruct {
    
    double latitude;
    double longitude;
    
} SGGeoCoord;

extern char* SGGeohashEncode(double lat, double lng, int precision);
extern SGGeoCoord SGGeohashDecode(char* hash);
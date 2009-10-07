/*
 *  SGGeohash.h
 *  libgeohash
 *
 *  Created by Derek Smith on 10/6/09.
 *  Copyright 2009 SimpleGeo. All rights reserved.
 *
 */

typedef struct SGGeoCoordStruct {
    
    double latitude;
    double longitude;
    
    double topLeft;
    double topRight;
    double bottomRight;
    double bottomLeft;
    
} SGGeoCoord;

typedef struct SGBoundingBoxStruct {
    
    double north;
    double south;
    double east;
    double west;
        
} SGBoundingBox;

/*
 * Creates a the hash at the specified precision. If precision is set to 0.
 * or less than it defaults to 12.
 */
extern char* SGGeohashEncode(double lat, double lng, int precision);

/* 
 * Returns the latitude and longitude used to create the hash along with
 * the bounding box for the encoded coordinate.
 */
extern SGGeoCoord SGGeohashDecode(char* hash);

/* 
 * Return an array of geohashes that represent the neighbors of the passed
 * in value. The neighbors are indexed as followed:
 *
 *                  N, NE, E, SE, S, SW, W, NW
 */ 
extern char** SGGeohashNeighbors(char* hash);
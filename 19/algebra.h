#ifndef DAY_19_ALGEBRA_INCLUDED_H
#define DAY_19_ALGEBRA_INCLUDED_H

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

/* Do not change! This is defined for readibility,
 * the code is not valid for values other than 3
 */
#define DIM 3 


/**
 *  Vector3D coordinate data type
 */
typedef int Int;

/**
 * Good'ol vectors. Can be accessed with indeces or coordinates:
 *
 *            vector.data[0] == vector.coord.x
 * 
 */
typedef union {
    Int data[3];
    struct {
        Int x,y,z;
    } coord;
} Vector3D;


/* A 3x3 matrix which:
 *  - In every row, and every column, there is a single non-zero value
 *  - All non-zero entries are either +1 or -1
 *  - Has determinant 1
 *  - The following property holds: X=D*Y, where:
 *    > X are the coordinates of a beacons in the reference basis
 *    > Y are the coordinates of a beacon in this scanner's basis
 *    > D is the orientation matrix 
 */
typedef struct { 
    unsigned short sparsity[DIM]; // Which column is non-zero for each colum
    short values[DIM];            // The value of the non-zero entry (+1 or -1)
} Orientation;


Vector3D NewVector3D(Int x, Int y, Int z);
Orientation ConstructOrientation(size_t permutation_id);

Vector3D vecmult(Orientation * D, Vector3D in);
Orientation matmul(Orientation * A, Orientation * B);

Orientation inv(Orientation * D);

Vector3D sub(Vector3D A, Vector3D B);

bool eq(Vector3D A, Vector3D B);

void PrintOrientation(Orientation * D);
void PrintVector3D(Vector3D v);



#endif
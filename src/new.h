#ifndef RD_NEW_H
#define RD_NEW_H

#include <stdlib.h>

#define snew(type)   (malloc(sizeof(type)))
#define mnew(type,x) (malloc(sizeof(type) * (x)))

#endif
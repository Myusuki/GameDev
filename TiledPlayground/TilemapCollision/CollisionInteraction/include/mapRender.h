#ifndef MAPRENDER_H
#define MAPRENDER_H

#include <raylib.h>
#include <tmx.h>

// tmx callback glob funcs
// load 
void* RaylibTexLoad( const char *path );
// free
void RaylibTexFree( void *ptr );

// map render function
void RenderMap( tmx_map *map );

#endif

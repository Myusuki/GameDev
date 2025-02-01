#ifndef MAPRENDER_H
#define MAPRENDER_H

#include <raylib.h>
#include <tmx.h>

// window dimensions
#define windowWidth 800
#define windowHeight 640

// map render functions
// tmx callback glob funcs
// load 
void* RaylibTexLoad( const char *path );
// free
void RaylibTexFree( void *ptr );
// main render function
void RenderMap( tmx_map *map );
// convert background color to raylib color
Color IntToColor( int color );
// draw tile layers
void DrawTileLayer( tmx_map *map, tmx_layer *layer );
// draw tiles
void DrawTile( void *image, Rectangle sourceRect, Vector2 destPos, float opacity, unsigned int flags );

#endif

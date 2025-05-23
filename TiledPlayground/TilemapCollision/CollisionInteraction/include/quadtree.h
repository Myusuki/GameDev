#ifndef QUADTREE_H
#define QUADTREE_H
#include <raylib.h>
#include <tmx.h>
#include "collisionObject.h"

typedef struct TreeNode {
  bool split;
  int maxCollisionAmount;
  int collisionAmountContained;
	int tileWidth;
	int tileHeight;
  Rectangle nodeArea;
  struct TreeNode* childNodes[4];
  CollisionObjNode *wallListHead;
} TreeNode;

void InitQuadTree( TreeNode *rootNode, tmx_map *map );
void InitTreeNode( tmx_map *map, TreeNode *Node, Rectangle mapRect );
void InsertCollisionObjs( TreeNode *node, CollisionObj *collisionObject );
void SplitTreeNode( TreeNode *node ); // splits a quadtree node with a given node

#endif

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>
#include <tmx.h>
#include "quadtree.h"
#include "collisionObject.h"

void InsertRectangleCollision( TreeNode *node, CollisionObj *rectCollision );
CollisionObj** GetCollisionsRect( TreeNode *node, CollisionObj checkSubject, CollisionObj* *collisionArr );
TreeNode* SearchForNode( TreeNode *rootNode, Vector2 checkSubject );

void InitQuadTree( TreeNode *rootNode, tmx_map *map )
{
	// init root node
	Rectangle mapRect = { .x = 0, .y = 0, .width = map->width * map->tile_width, .height = map->height * map->tile_height };
	InitTreeNode(map, rootNode, mapRect);

	// check tile types
	tmx_layer *layer = map->ly_head;
	while( layer != NULL && layer->type == L_LAYER )
	{
		for( int row = 0; row < map->height; row++ )
		{
			for( int column = 0; column < map->width; column++ )
			{
				unsigned int gid = ( layer->content.gids[ (column * map->width) + row] ) & TMX_FLIP_BITS_REMOVAL;
				tmx_tile *targetTile = map->tiles[gid];
				tmx_property *tileProperty = tmx_get_property(targetTile->properties, "uTileType");
				if( tileProperty->value.integer == WALL )
				{
					Rectangle tileRect = { .x = column * targetTile->width, .y = row * targetTile->height, .width = targetTile->width, .height = targetTile->height };
					CollisionObj newCollisionObj = { .visible = true, .collisionType = WALL, .shape = Rect, .collisionShape = tileRect };
					InsertCollisionObjs(rootNode, &newCollisionObj);
				}
			}
		}
		layer = layer->next;
	}
}
void InitTreeNode( tmx_map *map, TreeNode *node, Rectangle boundedArea )
{
  node->split = false;
  node->maxCollisionAmount = 10;
  node->collisionAmountContained = 0;
	node->tileWidth = map->tile_width;
	node->tileHeight = map->tile_height;
  node->nodeArea = (Rectangle){ .x = boundedArea.x, .y = boundedArea.y, .width = boundedArea.width, .height = boundedArea.height };

  int childNodesLength = sizeof(node->childNodes) / sizeof(node->childNodes[0] );
  for( int childNode = 0; childNode < childNodesLength; childNode++ )
    node->childNodes[ childNode ] = NULL;

  node->wallListHead = NULL;
}

void SplitTreeNode( TreeNode *node )
{
	if( node == NULL )
		return;
	// set node to split is true
	node->split = true;
	// set area of containment for child nodes
  int width1 = ( ( (int)node->nodeArea.width / node->tileWidth) / 2);
  int width2 = 0;
  if( ( (int)node->nodeArea.width / node->tileWidth) % 2 != 0 )
    width2 = ( (int)node->nodeArea.width / node->tileWidth) - width1;
  else
    width2 = width1;
  int height1 = ( (int)node->nodeArea.height / node->tileHeight) / 2;
  int height2 = 0;
  if( ( (int)node->nodeArea.height / node->tileHeight ) % 2 != 0 )
    height2 = ( (int)node->nodeArea.height / node->tileHeight) - height1;
  else
    height2 = height1;

	// init child nodes
  for( int child = 0; child < 4; child++ )
  {
    node->childNodes[child] = malloc( sizeof(TreeNode*) );
  }

	// assign areas of containment to child nodes
  // top left child
  node->childNodes[0]->nodeArea = (Rectangle){ .x = node->nodeArea.x, .y = node->nodeArea.y, .width = width1 * node->tileWidth, .height = height1 * node->tileHeight };
  // top right child
  node->childNodes[1]->nodeArea = (Rectangle){ .x = node->nodeArea.x + (width1 * node->tileWidth), .y = node->nodeArea.y, .width = width2 * node->tileWidth, .height = height1 * node->tileHeight };
  // bottom left child
  node->childNodes[2]->nodeArea = (Rectangle){ .x = node->nodeArea.x, .y = node->nodeArea.y + (height1 * node->tileHeight), .width = width1 * node->tileWidth, .height = height2 * node->tileHeight };
  // bottom right child
  node->childNodes[3]->nodeArea = (Rectangle){ .x = node->nodeArea.x + (width1 * node->tileWidth), .y = node->nodeArea.y + (height1 * node->tileHeight), .width = width2 * node->tileWidth, .height = height2 * node->tileHeight };

  // insert collision objects
	CollisionObjNode *wallListHeadNode = node->wallListHead;
	while( wallListHeadNode != NULL )
	{
		InsertCollisionObjs(node, wallListHeadNode->object);
		node->collisionAmountContained--;
		wallListHeadNode = wallListHeadNode->next;
	}
}
void InsertCollisionObjs( TreeNode *node, CollisionObj *collisionObject )
{
	switch( collisionObject->collisionType )
	{
		case Rect:
			InsertRectangleCollision(node, collisionObject);
			break;
	}
	// split if max amount has been reached in a node
	if( node->collisionAmountContained == node->maxCollisionAmount )
		SplitTreeNode(node);
}
void InsertRectangleCollision( TreeNode *node, CollisionObj *rectCollision )
{
	// check if node is NULL
	if( node == NULL )
	{
		printf( "Given Node is NULL!\n" );
		return;
	}
	// check if node is split already
	if( node->split )
	{
		// recurse to respective child node(s)
		for( int childNode = 0; childNode < 4; childNode++ )
		{
			bool xPosCheck, yPosCheck, wPosCheck, hPosCheck;
			xPosCheck = rectCollision->collisionShape.rect.x >= node->childNodes[childNode]->nodeArea.x && rectCollision->collisionShape.rect.x <= node->childNodes[childNode]->nodeArea.width;
			yPosCheck = rectCollision->collisionShape.rect.y >= node->childNodes[childNode]->nodeArea.y && rectCollision->collisionShape.rect.y <= node->childNodes[childNode]->nodeArea.height;
			wPosCheck = ( rectCollision->collisionShape.rect.x + rectCollision->collisionShape.rect.width ) >= node->childNodes[childNode]->nodeArea.x && ( rectCollision->collisionShape.rect.x + rectCollision->collisionShape.rect.width ) <= node->childNodes[childNode]->nodeArea.width;
			hPosCheck = ( rectCollision->collisionShape.rect.y + rectCollision->collisionShape.rect.height ) >= node->childNodes[childNode]->nodeArea.y && ( rectCollision->collisionShape.rect.y + rectCollision->collisionShape.rect.height ) <= node->childNodes[childNode]->nodeArea.height;

			// if whole rect is within a child node
			if( xPosCheck && yPosCheck && wPosCheck && hPosCheck )
				InsertRectangleCollision(node->childNodes[childNode], rectCollision);
			// if ul corner is within a child node
			else if( xPosCheck && yPosCheck )
				InsertRectangleCollision(node->childNodes[childNode], rectCollision);
			// if ur corner is within a child node
			else if( wPosCheck && yPosCheck )
				InsertRectangleCollision(node->childNodes[childNode], rectCollision);
			// if bl corner is within a child node
			else if( hPosCheck && xPosCheck )
				InsertRectangleCollision(node->childNodes[childNode], rectCollision);
			// if br corner is within a child node
			else if( wPosCheck && hPosCheck )
				InsertRectangleCollision(node->childNodes[childNode], rectCollision);
		}
	}

	// check if collisionObj is already in the list
	CollisionObjNode *checkNode = node->wallListHead;
	while( checkNode != NULL )
	{
		bool sameCollisionType, sameShape, sameShapeX, sameShapeY;
		sameCollisionType = rectCollision->collisionType == checkNode->object->collisionType;
		sameShape = rectCollision->shape == checkNode->object->shape;
		sameShapeX = rectCollision->collisionShape.rect.x == checkNode->object->collisionShape.rect.x;
		sameShapeY = rectCollision->collisionShape.rect.y == checkNode->object->collisionShape.rect.y;

		if( sameCollisionType && sameShape && sameShapeX && sameShapeY )
			return;
		else
			checkNode = checkNode->next;
	}

  // insert collision object 
	switch( rectCollision->collisionType )
  {
		case WALL:
			if( node->wallListHead == NULL )
			{
				CollisionObjNode newNode = { .object = rectCollision, .next = NULL };
				node->wallListHead = &newNode;
				node->collisionAmountContained++;
			}
			else if( node->wallListHead != NULL )
			{
				CollisionObjNode *targetNode = node->wallListHead;
				CollisionObjNode newNode = { .object = rectCollision, .next = NULL };
				while( targetNode->next != NULL )
					targetNode = targetNode->next;
				targetNode->next = &newNode;
				node->collisionAmountContained++;
			}
			break;
	}
}

CollisionObj** GetCollisions( TreeNode *rootNode, CollisionObj checkSubject, CollisionObj* *collisionArr )
{
	if( rootNode == NULL )
	{
		printf( "Node is null\n" );
		return NULL;
	}

	if( collisionArr == NULL )
	{
		printf( "Failed to malloc!\n" );
		return NULL;
	}

	switch( checkSubject.shape )
	{
		case Rect:
			collisionArr = GetCollisionsRect(rootNode, checkSubject, collisionArr);
	}

	return collisionArr;
}
CollisionObj** GetCollisionsRect( TreeNode *node, CollisionObj checkSubject, CollisionObj* *collisionArr )
{
	Vector2 ulPoint, urPoint, blPoint, brPoint;
	ulPoint = (Vector2){ .x = checkSubject.collisionShape.rect.x, .y = checkSubject.collisionShape.rect.y };
	urPoint = (Vector2){ .x = checkSubject.collisionShape.rect.x + checkSubject.collisionShape.rect.width, .y = checkSubject.collisionShape.rect.y };
	blPoint = (Vector2){ .x = checkSubject.collisionShape.rect.x, .y = checkSubject.collisionShape.rect.y + checkSubject.collisionShape.rect.height };
	brPoint = (Vector2){ .x = checkSubject.collisionShape.rect.x + checkSubject.collisionShape.rect.width, .y = checkSubject.collisionShape.rect.y + checkSubject.collisionShape.rect.height };

	TreeNode* benchedNodes[4];
	benchedNodes[0] = SearchForNode(node, ulPoint);
	benchedNodes[1] = SearchForNode(node, urPoint);
	benchedNodes[2] = SearchForNode(node, blPoint);
	benchedNodes[3] = SearchForNode(node, brPoint);

	TreeNode* *actualNodes = calloc(4, sizeof(TreeNode*) );
	for( int checkNode = 0; checkNode < 4; checkNode++ )
	{
		if( benchedNodes[checkNode] == NULL )
			continue;
		else
		{
			actualNodes[0] = benchedNodes[checkNode];
			break;
		}
	}
	int amountOfActualNodes = 1;

	for( int checkNode1 = 0; checkNode1 < 4; checkNode1++ )
	{
		if( actualNodes[checkNode1] == NULL )
			continue;
		for( int checkNode2 = 0; checkNode2 < 4; checkNode2++ )
		{
			if( benchedNodes[checkNode2] == NULL )
				continue;
			Rectangle checkRect1 = actualNodes[checkNode1]->nodeArea;
			Rectangle checkRect2 = benchedNodes[checkNode2]->nodeArea;
			if( (checkRect1.x != checkRect2.x) || (checkRect1.y != checkRect2.y) 
					|| (checkRect1.width != checkRect2.width) || (checkRect1.height != checkRect2.height) )
			{
				actualNodes[amountOfActualNodes] = benchedNodes[checkNode2];
				amountOfActualNodes++;
			}
		}
	}

	int amountOfCollisions = 0;
	for( int checkNode = 0; checkNode < 4; checkNode++ )
	{
		if( actualNodes[checkNode] == NULL )
			continue;
		switch( checkSubject.collisionType )
		{
			case WALL:
			{
				CollisionObjNode *listNode = actualNodes[checkNode]->wallListHead;
				while( listNode != NULL )
				{
					bool alreadyExists = false;
					for( int collObj = 0; collObj < amountOfCollisions; collObj++ )
					{
						if( listNode->object->shape == collisionArr[collObj]->shape &&
								listNode->object->collisionType == collisionArr[collObj]->collisionType &&
								listNode->object->collisionShape.rect.x == collisionArr[collObj]->collisionShape.rect.x &&
								listNode->object->collisionShape.rect.y == collisionArr[collObj]->collisionShape.rect.y &&
								listNode->object->collisionShape.rect.width == collisionArr[collObj]->collisionShape.rect.width &&
								listNode->object->collisionShape.rect.height == collisionArr[collObj]->collisionShape.rect.height )
							alreadyExists = true;
					}
					if( !alreadyExists )
					{
						collisionArr[amountOfCollisions] = listNode->object;
						amountOfCollisions++;
					}
					listNode = listNode->next;
				}
			}
		}
	}
	return collisionArr;
}
TreeNode* SearchForNode( TreeNode *rootNode, Vector2 checkSubject )
{
	if( rootNode == NULL )
	{
		printf( "Node is null\n" );
		return NULL;
	}
	TreeNode *foundNode = rootNode;
	if( foundNode->split )
	{
		for( int childNode = 0; childNode < 4; childNode++ )
		{
			bool xPosCheck, yPosCheck;
			xPosCheck = checkSubject.x >= foundNode->childNodes[childNode]->nodeArea.x && checkSubject.x <= foundNode->childNodes[childNode]->nodeArea.width;
			yPosCheck = checkSubject.y >= foundNode->childNodes[childNode]->nodeArea.y && checkSubject.y <= foundNode->childNodes[childNode]->nodeArea.height;
			if( xPosCheck && yPosCheck )
				foundNode = SearchForNode(foundNode->childNodes[childNode], checkSubject);
		}
	}
	return foundNode;
}

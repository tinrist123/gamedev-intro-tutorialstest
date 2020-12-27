#include "ShakeTree.h"

ShakeTree::ShakeTree()
{
	this->category = Category::MAPOBJECT;
}

void ShakeTree::Render()
{
	animation_set->at(SHAKE_TREE_ANI)->Render(x, y);
}

void ShakeTree::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = t = r = b;
}

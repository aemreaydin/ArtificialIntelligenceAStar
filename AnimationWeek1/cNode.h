#pragma once

#include <vector>
#include <string>

class cNode
{
public:
	cNode();
	~cNode();

	void AddConnection(cNode*);

	float f, g, h;
	bool connected;
	int i, j;
	cNode * prev;
};
#include "cNode.h"

cNode::cNode()
{
	this->f = this->g = this->h = -1.f;
	this->i = this->j = 0;
	this->connected = false;
	this->prev = NULL;
}

cNode::~cNode() {}

//void cNode::AddConnection(cNode* node)
//{
//	this->ConnectedNodes.push_back(node);
//}
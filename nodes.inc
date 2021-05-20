#if defined _nodes_included
	#endinput
#endif
#define _nodes_included

#include <a_samp>
#include <vector>
#include <idgen>

#if !defined NODES_DEFAULT_CAPACITY
	#define NODES_DEFAULT_CAPACITY 					(16)
#endif

#if !defined NODES_DEFAULT_GROWTH
	#define NODES_DEFAULT_GROWTH					(16)
#endif

enum e_NODES_DATA
{
	e_NODES_DATA_NODE_COUNT,
	e_NODES_DATA_NODE_SIZE,
	e_NODES_DATA_GENERATOR,
}

enum E_NODES_ERROR
{
	E_NODES_ERROR_OK,
	E_NODES_ERROR_INVALID_NODE_IDX,
	E_NODES_ERROR_INVALID_IDX,
}

#define INVALID_NODE								(Node:-1)

stock NodeManager_GetNodeSize(NodeM:nodem)
{
	return Vec_Get(Vec:nodem, e_NODES_DATA_NODE_SIZE);
}

static stock Idgen:NodeManager_GetNodeGen(NodeM:nodem)
{
	return Idgen:Vec_Get(Vec:nodem, e_NODES_DATA_GENERATOR);
}

static stock NodeManager_SetCount(NodeM:nodem, count)
{
	Vec_Set(Vec:nodem, e_NODES_DATA_NODE_COUNT, count);
}

stock NodeManager_Count(NodeM:nodem)
{
	return Vec_Get(Vec:nodem, e_NODES_DATA_NODE_COUNT);
}

stock NodeM:NodeManager_New(nodeSize = 1, initialCapacity = NODES_DEFAULT_CAPACITY, growth = NODES_DEFAULT_GROWTH)
{
	new Vec:nodeVector = Vec_New((initialCapacity * nodeSize) + _:e_NODES_DATA, false, growth * nodeSize);
	Vec_Set(nodeVector, e_NODES_DATA_NODE_SIZE, nodeSize);
	Vec_Set(nodeVector, e_NODES_DATA_GENERATOR, _:Idgen_New(UNLIMITED_CAPACITY));
	return NodeM:nodeVector;
}

stock Node:NodeManager_AddNode(NodeM:nodem, {_,Float,Vec,Idgen,Node,NodeM}:...)
{
	new
		Idgen:generator = NodeManager_GetNodeGen(nodem),
		size = NodeManager_GetNodeSize(nodem),
		argsCount = numargs() - 1;
	new index = Idgen_NewID(generator);

	for(new i = 0; i < size; ++i)
	{
		if(i > argsCount)
		{
			Vec_Set(Vec:nodem, _:e_NODES_DATA + index*size + i, 0);
		}
		else
		{
			Vec_Set(Vec:nodem, _:e_NODES_DATA + index*size + i, getarg(1 + i));
		}
	}
	NodeManager_SetCount(nodem, NodeManager_Count(nodem) + 1);
	return Node:index;
}

stock bool:NodeManager_IsValidNode(NodeM:nodem, Node:node)
{
	return Idgen_IsValidID(NodeManager_GetNodeGen(nodem), _:node);
}

stock E_NODES_ERROR:NodeManager_SetNodeValue(NodeM:nodem, Node:nodeIndex, dataindex, {_,Float,Vec,Idgen,Node,NodeM}:value)
{
	if(dataindex >= NodeManager_GetNodeSize(nodem))
		return E_NODES_ERROR_INVALID_IDX;
	if(!Idgen_IsValidID(NodeManager_GetNodeGen(nodem), _:nodeIndex))
		return E_NODES_ERROR_INVALID_NODE_IDX;

	new nodeSize = NodeManager_GetNodeSize(nodem);
	Vec_Set(Vec:nodem, _:e_NODES_DATA + _:nodeIndex*nodeSize + dataindex, value);
	return E_NODES_ERROR_OK;
}

stock E_NODES_ERROR:NodeManager_RemoveNode(NodeM:nodem, Node:nodeIndex)
{
	new Idgen:generator = NodeManager_GetNodeGen(nodem);
	if(!Idgen_IsValidID(generator, _:nodeIndex))
		return E_NODES_ERROR_INVALID_NODE_IDX;
	new nodeSize = NodeManager_GetNodeSize(nodem);
	for(new i = 0; i < nodeSize; ++i)
	{
		Vec_Set(Vec:nodem, _:e_NODES_DATA + _:nodeIndex*nodeSize + i, 0);
	}
	Idgen_ReleaseId(generator, _:nodeIndex);
	NodeManager_SetCount(nodem, NodeManager_Count(nodem) - 1);
	return E_NODES_ERROR_OK;
}

stock E_NODES_ERROR:NodeManager_GetNodeValue(NodeM:nodem, Node:nodeIndex, dataIndex, &{_,Float,Vec,Idgen,Node,NodeM}:value)
{
	if(!Idgen_IsValidID(NodeManager_GetNodeGen(nodem), _:nodeIndex))
		return E_NODES_ERROR_INVALID_NODE_IDX;
	new nodeSize = NodeManager_GetNodeSize(nodem);
	value = Vec_Get(Vec:nodem, _:e_NODES_DATA + _:nodeIndex*nodeSize + dataIndex);
	return E_NODES_ERROR_OK;
}

stock E_NODES_ERROR:NodeManager_GetNodeData(NodeM:nodem, Node:nodeIndex,
	{_,Float,Vec,Idgen,Node,NodeM}:node[], arrSize = sizeof node, startIndex = 0, endIndex = 0)
{
	if(!Idgen_IsValidID(NodeManager_GetNodeGen(nodem), _:nodeIndex))
		return E_NODES_ERROR_INVALID_NODE_IDX;
	new nodeSize = NodeManager_GetNodeSize(nodem);
	new size = arrSize <= nodeSize - startIndex ? arrSize + startIndex : nodeSize;
	if(endIndex == 0 || endIndex > size)
	{
		endIndex = size;
	}
	for(new i = startIndex; i < endIndex; ++i)
	{
		node[i - startIndex] = Vec_Get(Vec:nodem, _:e_NODES_DATA + _:nodeIndex * nodeSize + i);
	}
	return E_NODES_ERROR_OK;
}

stock NodeManager_Clear(NodeM:nodem)
{
	memset(Vec_GetAsArray(Vec:nodem, _:e_NODES_DATA), 0, Vec_Len(Vec:nodem));
	Idgen_Clear(NodeManager_GetNodeGen(nodem));
	NodeManager_SetCount(nodem, 0);
}

stock NodeManager_Delete(NodeM:nodem)
{
	Idgen_Delete(NodeManager_GetNodeGen(nodem));
	Vec_Delete(Vec:nodem);
}
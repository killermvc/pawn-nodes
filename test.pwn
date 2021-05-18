#define RUN_TESTS
#define YSI_NO_HEAP_MALLOC
#define YSI_NO_CACHE_MESSAGE
#define YSI_NO_OPTIMISATION_MESSAGE
#define YSI_NO_VERSION_CHECK

#include "nodes.inc"
#include <a_samp>
#include <YSI_Core\y_testing>

Test:AddNode()
{
	new NodeM:nodem = NodeManager_New(2);
	new Node:nodeIdx1 = NodeManager_AddNode(nodem, 1, 2);
	new Node:nodeIdx2 = NodeManager_AddNode(nodem, 3, 4);

	ASSERT(NodeManager_IsValidNode(nodem, nodeIdx1));
	ASSERT(NodeManager_IsValidNode(nodem, nodeIdx2));
	ASSERT_EQ(Vec_Get(Vec:nodem, _:e_NODES_DATA + 0), 1);
	ASSERT_EQ(Vec_Get(Vec:nodem, _:e_NODES_DATA + 1), 2);
	ASSERT_EQ(Vec_Get(Vec:nodem, _:e_NODES_DATA + 2), 3);
	ASSERT_EQ(Vec_Get(Vec:nodem, _:e_NODES_DATA + 3), 4);
}

Test:RemoveNode()
{
	new NodeM:nodem = NodeManager_New(2);
	new Node:nodeIdx1 = NodeManager_AddNode(nodem, 1, 2);
	new Node:nodeIdx2 = NodeManager_AddNode(nodem, 3, 4);

	NodeManager_RemoveNode(nodem, nodeIdx1);

	ASSERT_FALSE(NodeManager_IsValidNode(nodem, nodeIdx1));
	ASSERT(NodeManager_IsValidNode(nodem, nodeIdx2));
	ASSERT_EQ(Vec_Get(Vec:nodem, _:e_NODES_DATA + 0), 0);
	ASSERT_EQ(Vec_Get(Vec:nodem, _:e_NODES_DATA + 1), 0);
	ASSERT_EQ(Vec_Get(Vec:nodem, _:e_NODES_DATA + 2), 3);
	ASSERT_EQ(Vec_Get(Vec:nodem, _:e_NODES_DATA + 3), 4);
}

Test:GetNodeValue()
{
	new NodeM:nodem = NodeManager_New(2);
	new Node:nodeIdx = NodeManager_AddNode(nodem, 1, 2);

	new val1, val2;
	new E_NODES_ERROR:err1 = NodeManager_GetNodeValue(nodem, nodeIdx, 0, val1);
	new E_NODES_ERROR:err2 = NodeManager_GetNodeValue(nodem, nodeIdx, 1, val2);

	ASSERT_EQ(err1, E_NODES_ERROR_OK);
	ASSERT_EQ(err2, E_NODES_ERROR_OK);
	ASSERT_EQ(val1, 1);
	ASSERT_EQ(val2, 2);
}

Test:SetNodeValue()
{
	new NodeM:nodem = NodeManager_New(2);
	new Node:nodeIdx = NodeManager_AddNode(nodem, 1, 2);

	new E_NODES_ERROR:err1 = NodeManager_SetNodeValue(nodem, nodeIdx, 0, 3);
	new E_NODES_ERROR:err2 = NodeManager_SetNodeValue(nodem, nodeIdx, 1, 4);

	new val1, val2;
	NodeManager_GetNodeValue(nodem, nodeIdx, 0, val1);
	NodeManager_GetNodeValue(nodem, nodeIdx, 1, val2);
	ASSERT_EQ(err1, E_NODES_ERROR_OK);
	ASSERT_EQ(err2, E_NODES_ERROR_OK);
	ASSERT_EQ(val1, 3);
	ASSERT_EQ(val2, 4);
}

Test:GetNodeData()
{
	new NodeM:nodem = NodeManager_New(2);
	new Node:nodeIdx = NodeManager_AddNode(nodem, 1, 2);
	new arr[2];

	new E_NODES_ERROR:err = NodeManager_GetNodeData(nodem, nodeIdx, arr);
	ASSERT_EQ(err, E_NODES_ERROR_OK);
	ASSERT_EQ(arr[0], 1);
	ASSERT_EQ(arr[1], 2);
}
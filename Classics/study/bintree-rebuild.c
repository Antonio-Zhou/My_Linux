#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

typedef int ElemType;
typedef struct TreeNode {
	ElemType	data;
	struct TreeNode	*left;
	struct TreeNode	*right;
}TreeNode, *pTreeNode;

pTreeNode ConstructCore(int *startPreorder, int *endPreorder,
		int *startInorder, int *endInorder)
{
	int rootValue = 0;
	int *rootInorder = NULL;
	int leftLength = 0;
	int *leftPreorderEnd = NULL;

	/*root*/
	rootValue = startPreorder[0];
	pTreeNode BTree = (struct TreeNode *)malloc(sizeof(TreeNode));
	BTree->data = rootValue;
	BTree->left = BTree->right = NULL;

	/*只有一个节点*/
	if (startPreorder == endPreorder) {
		if (startInorder == endInorder && *startPreorder == *startInorder)
			return BTree;
		else
			puts("Invalid input.\n");
	}

	rootInorder = startInorder;
	/*找到root在InOrder中的位置*/
	while(rootInorder <= endInorder && *rootInorder != rootValue)
		++rootInorder;
	if(rootInorder == endInorder && *rootInorder != rootValue)
		puts("Invalid input.\n");

	/*左子树的范围*/
	leftLength = rootInorder - startInorder;
	leftPreorderEnd = startPreorder + leftLength;
	if (leftLength > 0) {
		BTree->left = ConstructCore(startPreorder + 1,
						leftPreorderEnd, startInorder, rootInorder - 1);
	}
	if (leftLength < endPreorder - startPreorder) {
		BTree->right = ConstructCore(leftPreorderEnd + 1,
						endPreorder, rootInorder + 1, endInorder);
	}

	return BTree;

}

pTreeNode RebuildTree(int *preorder, int *inorder, int length)
{
	pTreeNode BTree;

	if (preorder == NULL || inorder == NULL || length <= 0)
		return NULL;

	return ConstructCore(preorder, preorder+length-1, inorder, inorder+length-1);
}

int
main(int argc, char **argv)
{
	int n, i = 0;
	pTreeNode root;
	int tmp;
	printf("please input the number of the tree:");
	scanf("%d", &n);
	int prearray[n];
	int inarray[n];
	printf("please input the preorder of the tree:");
	do{
		scanf("%d",&tmp);
		prearray[i] = tmp;
		i++;
	}while(i < n);

	i = 0;
	printf("please input the inorder of the tree:");
	do{
		scanf("%d", &tmp);
		inarray[i] = tmp;
		i++;
	}while(i < n);

	printf("the preorder is :\n");
	for(i = 0; i < n; i++){
		printf("%d\t",prearray[i]);
	}
	putchar('\n');
	printf("the inorder is :\n");
	for(i = 0; i < n; i++){
		printf("%d\t",inarray[i]);
	}
	putchar('\n');

	root = RebuildTree(prearray, inarray, n);
	printf("The root of the tree is %d\n", root->data);

	return 0;
}

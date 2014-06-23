/*
 * 1.查找树的创建(create_tree)
 * 假设有如下数组4,1,45,78,345,23,12,3,6,21
 * 首先选定4为root,然后遍历剩下的数字,如果大于等于4则放到4的右侧,小于4放到4的左侧,最后构建成的树;所有的左孩子都小于父节点,所有的右孩子都大于等于父节点.
 * 2.遍历查找树(display_tree)
 * 按照左中右的顺序遍历树,结果为:1,3,4,5,12,21,23,45,78,345;遍历的结果就是已经排好序的数字.
 * 3.查找树中的节点(search_tree)
 * 从根节点开始，如果大于等于根节点，则查找根节点的右侧；如果小于根节点，则查找根节点的左侧，直到查找到节点
 * 比如要查找12：
 * 	比4大,往右走;
 * 	比45小,往左走;
 * 	比23小,往左走;
 * 	找到12
 * 4.删除树中的节点(delete_node)
 * 这个是最复杂的,因为删除完节点后要重新构建树,涉及到的情况很多:
 * a.要删除的node没有左右孩子,有父节点.
 * 	如果要删除的node为父节点的左孩子,则将父节点的左孩子指针设置为NULL;如果要删除的node为父节点的右孩子,则将父节点的右孩子指针设置为NULL.最后删除node
 * b.要删除的node没有左右孩子,没有父节点(即根节点)
 * 	根节点设为NULL,删除node。
 * c.要删除的node有左孩子没右孩子,有父节点
 * 	如果要删除的node为父节点的左孩子,则将父节点的左孩子设置为要被删除node的左孩子;如果要删除的node为父节点的右孩子,则将父节点的右孩子指针设置为要被删除node的左孩子.最后删除node
 * d.要被删除的node有左孩子没有右孩子,没有父节点
 * 	将要被删除的node的左孩子设置为根节点,删除node
 * e.要删除的node有右孩子没左孩子,有父节点
 * 	如果要删除的node为父节点的左孩子,则将父节点的左孩子设置为要被删除node的右孩子;如果要删除的node为父节点的右孩子,则将父节点的右孩子指针设置为要被删除node的右孩子.最后删除node
 * f.要被删除的node有右孩子没有左孩子,没有父节点
 * 	将要被删除的node的右孩子设置为根节点,删除node。
 * g.要被删除的node左右孩子都有,有父节点
 * 	将要被删除node的右孩子插入到左孩子中去.如果要删除的node为父节点的左孩子,则将父节点的左孩子设置为要被删除node的左孩子;如果要删除的node为父节点的右孩子,则将父节点的右孩子指针设置为要被删除node的左孩子.最后删除node.
 * h.要被删除的node左右孩子都有,无父节点
 * 	将要被删除node的右孩子插入到左孩子中去,父节点修改为要被删除node的左孩子,删除node节点.
 * */

#include <stdio.h>
#include <stdlib.h>

#define SIZE 10

typedef struct tagNode{
        int value;
        struct tagNode* left;
        struct tagNode* right;
}treeNode;

/*
 * 打印数组
 * */
void display_array(int array[], int size)
{
	int i;

	printf("the array is:");
	for (i=0; i<size; i++)
		printf("%d ", array[i]);

	printf("\n");
}

/*
 * 按左中右顺序遍历树
 * */
void display_tree(treeNode *node)
{
	/*树为空*/
	if (node == NULL) {
		printf("NULL\n");
		return;
	}

	if (node->left != NULL)
		display_tree(node->left);

	printf("%d ", node->value);

	if (node->right != NULL)
		display_tree(node->right);
}

/*
 * 查找以node为节点的树中上是否存在vlaue的节点
 * */
treeNode *search_tree(treeNode *node, int value)
{
	if (node->value == value) {
		return node;
	} else if(node->value > value) {
		if(node->left != NULL){
			return search_tree(node->left, value);
		} else {
			return NULL;
		}
	} else {
		if(node->right != NULL) {
			return search_tree(node->right, value);
		} else {
			return NULL;
		}
	}
}

/*
 * 查找以node为节点的树中上是否存在vlaue的节点,parent为查找到的节点的父节点
 * dir为1表示parent节点的左节点为查找结果
 * dir为2表示parent节点的右节点为查找结果
 * */
treeNode *search_tree_with_parent(treeNode *node, treeNode **parent, int *dir, int value)
{
	/*根节点*/
	if (node->value == value) {
		return node;
	/*右子树*/
	} else if (node->value > value) {
		if (node->left != NULL) {
			*dir = 1;
			*parent = node;
			return search_tree_with_parent(node->left, parent, dir, value);
		} else {
			return NULL;
		}
	} else {
		if (node->right != NULL) {
			*dir = 2;
			*parent = node;
			return search_tree_with_parent(node->right, parent, dir, value);
		} else {
			return NULL;
		}
	}
}

/*
 * 将iNode插入到以node为根节点的树中
 * */
void insert_node(treeNode *node, treeNode *iNode)
{
        if (iNode->value >= node->value) {
		if (node->right != NULL) {
			insert_node(node->right, iNode);
			return;
		} else {
                	node->right = iNode;
		}
        } else { 
		if (node->left != NULL){
			insert_node(node->left, iNode);
			return;
		} else {
                	node->left = iNode;
		}
        }
}

/*
 * 从以root为根节点的树中删除值为value的节点
 * */
void delete_node(treeNode **root, int value)
{
	treeNode *parent = NULL;
	int dir = -1;

	treeNode *deleteNode = search_tree_with_parent(*root, &parent, &dir, value);
	if(deleteNode == NULL){
		printf("%s\n", "node not found");
		exit(0);
	}

	if (deleteNode->left == NULL && deleteNode->right == NULL) {
		/*对应说明中的a*/
		if (parent != NULL) {
			if (dir == 1)
				parent->left = NULL;
			else
				parent->right = NULL;
		/*对应说明中的b*/
		} else {
			*root = NULL;
		}
	} else if (deleteNode->left != NULL && deleteNode->right == NULL) {
		/*对应说明中的c*/
		if (parent != NULL) {
			if (dir == 1)
				parent->left = deleteNode->left;
			else
				parent->right = deleteNode->left;
		/*对应说明中的d*/
		} else {
			*root = deleteNode->left;
		}
	} else if (deleteNode->left == NULL && deleteNode->right != NULL) {
		/*对应说明中的e*/
		if (parent != NULL) {
			if (dir == 1)
				parent->left = deleteNode->right;
			else
				parent->right = deleteNode->right;
		/*对应说明中的f*/
		} else {
			*root = deleteNode->right;
		}
	} else {
		insert_node(deleteNode->left, deleteNode->right);
		/*对应说明中的g*/
		if (parent != NULL) {
			if (dir == 1)
				parent->left = deleteNode->left;
			else
				parent->right = deleteNode->left;
		/*对应说明中的h*/
		} else {
			*root = deleteNode->left;
		}
	}
	free(deleteNode);
	deleteNode = NULL;
}

/*
 * 使用array数组中的数，创建以root为根节点的树
 * */
void create_tree(treeNode **root, int array[], int size)
{
	int i;

	*root = (treeNode*)malloc(sizeof(treeNode));
	(*root)->value = array[0];
	(*root)->left = NULL;
	(*root)->right = NULL;

	for (i=1; i<size; i++) {
		treeNode* child = (treeNode*)malloc(sizeof(treeNode));
		child->value = array[i];
		child->left = NULL;
		child->right = NULL;
		insert_node(*root, child);
	}
}

/*
 * 删除以node为根节点的树
 * */
void delete_tree(treeNode *node)
{
	if (node == NULL)
		return;

	if (node->left != NULL)
		delete_tree(node->left);

	if (node->right != NULL)
		delete_tree(node->right);

	if (node->left == NULL && node->right == NULL) {
		free(node);
		node = NULL;
	}
}

int main(int argc, char *argv[])
{

	int value = atoi(argv[1]);
	int array[SIZE] = {4, 1, 45, 78, 345, 23, 12, 3, 6, 21};
	int dir = -1;
	treeNode *root = NULL;
	treeNode *parent = NULL;

	display_array(array, SIZE);

	create_tree(&root, array, SIZE);
	printf("the tree is(left->middle->right):");
	display_tree(root);
	printf("\n");

	printf("search value %d:", value);
	if (search_tree(root, value) != NULL)
		printf("%s\n", "exist");
	else
		printf("%s\n", "not exist");

	printf("delete value:%d\n", value);
	delete_node(&root, value);
	printf("the tree is(left->middle->right):");
	display_tree(root);
	printf("\n");

	delete_tree(root);
	return 0;
}

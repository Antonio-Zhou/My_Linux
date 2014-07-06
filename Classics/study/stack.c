#include <stdio.h>
#include <malloc.h>

#define DEFAULT_SIZE 100

struct Stack {
	int *base;
	int *top;
	int size;
};

int init_stack(struct Stack *S)
{
	S->base = (int *)malloc(DEFAULT_SIZE * sizeof(struct Stack));
	if (!S->base)
		return -1;
	S->top = S->base;
	S->size = DEFAULT_SIZE;
	return 0;
}

void push_stack(struct Stack *S, int push)
{
	*(S->top)++ = push;
}

void pop_stack(struct Stack *S, int *pop)
{
	*pop = *(--S->top);
}

int empty_stack(struct Stack S)
{
	if (S.base == S.top)
		return 0;
	else
		return 1;
}

int destory_statck(struct Stack *S)
{
	S->base = NULL;
	S->top = NULL;
	S->size = 0;
	free(S->base);
	free(S->top);
}

void clear_stack(struct Stack *S)
{
	S->top = S->base;
}
int main(void)
{
	struct Stack S;
	int p, ret;

	printf("init the Stack\n");
	if (init_stack(&S) == 0)
		printf("OK\n");
	else
		printf("ERROR\n");

	push_stack(&S, 6);
	clear_stack(&S);
	if (!empty_stack(S))
		printf("the stack is empty!\n");
	else
		printf("the stack is not empty!\n");

	push_stack(&S, 5);
	pop_stack(&S, &p);
	printf("p=%d\n", p);

	if (!empty_stack(S))
		printf("the stack is empty!\n");
	else
		printf("the stack is not empty!\n");

	return 0;
}

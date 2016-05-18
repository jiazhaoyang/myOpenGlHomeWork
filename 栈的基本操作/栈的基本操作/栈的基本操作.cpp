// 栈的基本操作.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


#include <stdio.h>
#include <stdlib.h>
#define MAX 1024 ///栈使用数组模拟，MAX是最大元素个数
typedef int DataType; ///数据域使用整形

typedef struct _stack
{
	DataType data[MAX]; ///存放数据
	int top; ///栈顶指针
}stack;

///初始化
int initStack(stack(*s))
{
	return emptyStack(s);
}

///数据压栈，成功返回1，失败返回0
int push(stack(*s), DataType d)
{
	if ((*s).top >= MAX - 1) //栈已满
	{
		printf("栈已满，不能压栈\n");
		return 0;
	}

	//数据压栈
	(*s).top++;
	(*s).data[(*s).top] = d;
	return 1;
}

///数据出栈，成功返回1，d指向的区域存储弹出的数据，失败返回0
int pop(stack(*s), DataType *d)
{
	if ((*s).top <= -1)
	{
		printf("栈为空，不能出栈\n");
		return 0;
	}

	//出栈
	*d = (*s).data[(*s).top];
	(*s).top--;
	return 1;
}

///清空栈
int emptyStack(stack(*s))
{
	(*s).top = -1;
	return 1;
}

int main(int argc, char** argv)
{
	stack s;
	int i, d;

	initStack(&s);

	//压栈
	for (i = 0; i < 1025; i++)
	{
		push(&s, i);
	}

	//清空
	emptyStack(&s);

	for (i = 0; i < 10; i++)
	{
		push(&s, i);
	}
	//出栈
	for (i = 0; i < 11; i++)
	{
		pop(&s, &d);
		printf("%d\n", d);
	}

	return 0;
}


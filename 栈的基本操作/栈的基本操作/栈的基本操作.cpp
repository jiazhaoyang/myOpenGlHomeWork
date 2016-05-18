// ջ�Ļ�������.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


#include <stdio.h>
#include <stdlib.h>
#define MAX 1024 ///ջʹ������ģ�⣬MAX�����Ԫ�ظ���
typedef int DataType; ///������ʹ������

typedef struct _stack
{
	DataType data[MAX]; ///�������
	int top; ///ջ��ָ��
}stack;

///��ʼ��
int initStack(stack(*s))
{
	return emptyStack(s);
}

///����ѹջ���ɹ�����1��ʧ�ܷ���0
int push(stack(*s), DataType d)
{
	if ((*s).top >= MAX - 1) //ջ����
	{
		printf("ջ����������ѹջ\n");
		return 0;
	}

	//����ѹջ
	(*s).top++;
	(*s).data[(*s).top] = d;
	return 1;
}

///���ݳ�ջ���ɹ�����1��dָ�������洢���������ݣ�ʧ�ܷ���0
int pop(stack(*s), DataType *d)
{
	if ((*s).top <= -1)
	{
		printf("ջΪ�գ����ܳ�ջ\n");
		return 0;
	}

	//��ջ
	*d = (*s).data[(*s).top];
	(*s).top--;
	return 1;
}

///���ջ
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

	//ѹջ
	for (i = 0; i < 1025; i++)
	{
		push(&s, i);
	}

	//���
	emptyStack(&s);

	for (i = 0; i < 10; i++)
	{
		push(&s, i);
	}
	//��ջ
	for (i = 0; i < 11; i++)
	{
		pop(&s, &d);
		printf("%d\n", d);
	}

	return 0;
}


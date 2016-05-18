// Ï£¶ûÅÅÐò.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


#include <stdio.h>
void shell_sort(int arr[], int size)
{
	int i, j, k, temp;
	for (i = size / 2; i>0; i /= 2)
	{
		for (j = i; j<size; j++)
		{
			temp = arr[j];
			for (k = j - i; k >= 0 && temp<arr[k]; k -= i)
			{
				arr[k + i] = arr[k];
			}
			arr[k + i] = temp;
		}
	}
	for (i = 0; i<size; i++)
	{
		printf("%d ", arr[i]);
	}
}
void main()
{
	int arr[] = { 12, 34, 64, 23, 87, 54, 89, 45, 56, 1, 3, 7, 5, 4, 2 };
	shell_sort(arr, 15);
}


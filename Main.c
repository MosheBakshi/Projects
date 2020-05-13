//VS-19
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


typedef struct Interval
{
	int Strt;
	int End;
	int degree;
	struct Interval* next;
	struct Interval* previous;
} Interval;

typedef struct node
{
	Interval Interval;
	struct node* Same;
	struct node* NSame;
}node;

void mergeSort(Interval*,int,int);
void merge(Interval*,int,int,int);
void EDCMatrix(Interval*,int,int);
void AddNode(node*,Interval);
bool Cuts(Interval,Interval);
void GreedyColoring();
void GEdges(int**,int,Interval*);
void printList(node* );
void freeList(node* );

void main()
{
	GreedyColoring();
}

void GreedyColoring()
{
	int k, i, **NumCol, GEgdes, MaxDeg, MinDeg = 0, GCMaxDeg, GCMinDeg=0;
	Interval* arr = NULL;
	printf("Please input k\n");
	scanf("%d", &k);
	arr = (Interval*)malloc(k * sizeof(Interval));
	if (!arr) exit(1);
	NumCol = calloc(k,sizeof(int*));
	for (i = 0; i < k; i++)
	{
		NumCol[i] = calloc(k,sizeof(int));
	}
	for (i = 0; i < k; i++)
	{
		printf("Please input %dth interval: ", i + 1);
		scanf("%d %d", &arr[i].Strt, &arr[i].End);
	}
	printf("The Intervals family is:\n");
	for (i = 0; i < k; i++)
	{
		printf("[%d,%d]", arr[i].Strt, arr[i].End);
		if (i + 1 != k)
			printf(",");
	}
	printf("\n");
	mergeSort(arr, 0, k - 1); // sort the intervals by start 
	EDCMatrix(arr,NumCol,k); // 
	GEdges(NumCol, k,arr);
	free(arr);
	for (i = 0; i < k; i++)
		free(NumCol[i]);
	free(NumCol);
}

void mergeSort(Interval* this, int from, int to)
{

	int split /*in the middle*/;
	if (from < to)
	{
		split = (from + to) / 2;
		mergeSort(this, from, split);
		mergeSort(this, split + 1, to);
		merge(this, from, split, to);
	}
}

void merge(Interval* arr, int start, int middle, int end)
{
	int st = start, mid = middle + 1, i = 0;
	Interval* temp = (Interval*)calloc(end - start + 1, sizeof(Interval));
	if (!temp) exit(1);

	while ((st <= middle) && (mid <= end))
	{
		if (arr[st].Strt < arr[mid].Strt)
			temp[i++] = arr[st++];
		else {
			if (arr[st].Strt == arr[mid].Strt)
				if (arr[st].End < arr[mid].End)
					temp[i++] = arr[st++];
				else
					temp[i++] = arr[mid++];
			else
				temp[i++] = arr[mid++];
		}
	}
	while (st <= middle) temp[i++] = arr[st++];
	while (mid <= end) temp[i++] = arr[mid++];

	for (st = start, i = 0; st <= end; st++, i++)
		arr[st] = temp[i];

	free(temp);
}

bool Cuts(Interval a, Interval b)
{
	if (a.Strt == b.Strt && a.End == b.End)
		return true;
	if ((a.Strt < b.Strt && b.Strt < a.End) || (a.Strt < b.End && b.End < a.End))
		return false;
	if (!((b.Strt < a.Strt && a.Strt < b.End))) {
		if(!((b.Strt < a.End && a.End < b.End)))
		return true;
	}
	if (a.Strt > b.End || a.End < b.Strt)
		return true;
	else
		return false;
}

void EDCMatrix(Interval* arr, int** CArr,int size)// create MATRIX for EDGES,DEGREES and COLOROING find
{
	int i = 0, k = 0;
	for (i = 0; i < size; i++)
	{
		for (k = 0; k < size; k++)
		{
			if (!Cuts(arr[i], arr[k]))
				CArr[i][k] = 1;
		}
	}
}

void GEdges(int** CArr, int size,Interval* Arr)
{
	int i, j, counterME = 0, counterMaD, counterMiD, counterCMaD, counterCMiD,
		MaxDeg = 0, MinDeg = size,
		counterCME = 0, MaxCDeg = 0, MinCDeg = size,
		chromaticNUM = 0;// chromatic number
	node* list = NULL,*temp = NULL;//root for list
	node* sr, * nsr;//SameRunner //NotSameRunner for optimal coloring
	for (i = 0; i < size;i++)
	{
		counterMaD = 0;// the CArr[i] items Deg counter for - MAX in G
		counterMiD = size-1;// the CArr[i] items Deg counter for - Min in G
		counterCMaD = 0; // the CArr[i] Complement items Deg counter for - Max in ~G
		counterCMiD = size-1;// the CArr[i] Complement items Deg counter for - Min in ~G
		for (j = 0; j < size; j++)
		{
			if (CArr[i][j] == 1 && i!=j)
			{
				counterME++;//G Edges
			}
			if (CArr[i][j] == 0 && i != j)
			{
				counterCME++;//~G Edges
			}
			if (CArr[i][j] == 1)
			{
				counterMaD++;
			}
			if (CArr[i][j] == 0 && i!=j)
			{
				counterCMaD++;
			}
			if(CArr[i][j] == 0 && i!=j)
			{
				counterMiD--;
			}
			if (CArr[i][j] == 1)
			{
				counterCMiD--;
			}
		}
		if (MaxDeg < counterMaD) // check for higher
		{
			MaxDeg = counterMaD;
		}
		if (MinDeg > counterMiD)//check for lower
		{
			MinDeg = counterMiD;
		}
		if (MaxCDeg < counterCMaD) // check for higher complement
		{
			MaxCDeg = counterCMaD;
		}
		if (MinCDeg > counterCMiD)//check for lower complements
		{
			MinCDeg = counterCMiD;
		}
	}
	counterME /= 2;// Edges = sigmaDEG(G)/2
	counterCME /= 2;
	printf("G Edges = %d\n", counterME);
	printf("Maximum Degree of G = %d\n", MaxDeg);
	printf("Minimum Degree of G = %d\n", MinDeg);
	if (list == NULL)//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	{
		list = malloc(sizeof(node));
		list->Interval = Arr[0];
		list->Same = NULL;
		list->NSame = NULL;
		chromaticNUM++;
	}
	sr = list->Same;
	nsr = list->NSame;
	for (i = 1; i < size; i++)//optimal coloring creation 
	{
		temp = list;
		nsr = temp;
		{
			while (temp)
			{
				sr = nsr;
				while (sr)
				{
					if (Cuts(sr->Interval, Arr[i]))// check for same color runner if the oponent cuts with it
					{
						if (sr->Same == NULL)//if next is not exist
						{
							sr->Same = malloc(sizeof(node));
							sr->Same->Interval = Arr[i];
							sr->Same->Same = NULL;
							sr->Same->NSame = NULL;
							temp = NULL;
							break;
						}
						else if (Cuts(sr->Interval, Arr[i]))// if do exist next check if fit after him
						{
							sr = sr->Same;
						}
					}

					else if (!Cuts(sr->Interval, Arr[i])) { break; } // in case not fit break loop for sr
				}
					if (!Cuts(sr->Interval, Arr[i]))// to create another color list
						{
							if (nsr->NSame== NULL)
							{
								nsr->NSame = malloc(sizeof(node));
								nsr->NSame->Interval = Arr[i];
								nsr->NSame->Same = NULL;
								nsr->NSame->NSame = NULL;
								temp = NULL;
								chromaticNUM++;
								break;
							}
							else
							{
								nsr = nsr->NSame;
							}
						}
			}

		}
	}///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	printf("Chromatic Number of G = %d\n", chromaticNUM);
	printf("G's Complement Edges = %d\n", counterCME);
	printf("Maximum Degree of G's Complement = %d\n", MaxCDeg);
	printf("Minimum Degree of G's Complement = %d\n", MinCDeg);
	printList(list);
	freeList(list);
}

void printList(node* list)
{
	node* sr, * nsr;
	int chromatic = 1;
	nsr = list;
	while (nsr)
	{
		sr = nsr->Same;
		printf("{");
		printf("[%d,%d]", nsr->Interval.Strt, nsr->Interval.End);
		while (sr)
		{
			printf("[%d,%d]", sr->Interval.Strt, sr->Interval.End);
			if (sr->Same != NULL) { printf(","); }
			sr = sr->Same;
		}
		printf("} = %d", chromatic);
		if (nsr->NSame != NULL) { printf(","); }
		chromatic++;
		nsr = nsr->NSame;
	}
}

void freeList(node* List)
{
	node* sr, * nsr,*temp;
	nsr = List;
	while (nsr)
	{
		sr = nsr->Same;
		while (sr)
		{
			temp = sr;
			sr = sr->Same;
			free(temp);
		}
		temp = nsr;
		nsr = nsr->NSame;
		free(temp);
	}

}
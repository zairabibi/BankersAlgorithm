#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

// Declare dynamic arrays/vectors and global variables
int m=0, n = 0;

int* resources;
int* available;
int* max_claim;
int* allocated;
int* need;

//*********************************************************
int displayMenu()
{
	printf("Banker's Algorithm\n------------------\n");

	printf("1) Enter claim graph \n2) Request resource \n3) Release resource \n4) Determine safe sequence \n5) Quit program");

	printf("\n\nEnter selection: ");

	int sel;
	scanf("%d", &sel);

	return sel;
}

//*********************************************************
void print(int *vector, char *title) {
	printf("%s:\n", title);

	// for loop j: print each resource index 
	for (int j = 0; j < m; j++)
		printf("\t r%d", j);
	printf("\n");

	// for loop j: print value of vector[j] 
	for (int j = 0; j < m; j++)
		printf("\t %d", vector[j]);
	
	printf("\n\n");
	return;
}

//*************************************************************
void print_max_claim(int *matrix, char *title) {
	
	printf("%s:\n", title);

	// for loop j: print each resource index 
	for (int j = 0; j < m; j++)
	{
		printf("\t r%d", j);
	}
	printf("\n");
	// for each process i: 
		// for each resource j: 
			// print value of matrix[i,j]
	for (int i = 0; i < n; i++)
	{
		printf("p%d", i);
		for (int j = 0; j < m; j++)
			printf("\t %d", matrix[i*m + j]);

		printf("\n");
	}
	printf("\n");
	return;
}


//**************************************************************
void enterGraph()
{
	// prompt for number of resources 
	printf("Enter number of resources: ");
	scanf("%d", &m);

	// allocate memory for vectors 
	resources = (int*)malloc(m * sizeof(int));
	available = (int*)malloc(m * sizeof(int));
	
	// for each resource, prompt for number of units, set resource and available vectors indices
	printf("Enter number of units for resources (r0 to r%d): ", (m - 1));
	for (int i = 0; i < m; i++)
	{
		scanf("%d", &resources[i]);
		available[i] = resources[i];
	}

	// prompt for number of processes
	printf("Enter number of processes: ");
	scanf("%d", &n);

	// allocate memory for arrays
	max_claim = malloc((m * n) * sizeof(int));
	allocated = malloc((m * n) * sizeof(int));
	need = malloc((m * n) * sizeof(int));

	// for each process, for each resource, prompt for maximum number of units requested by process, update maxclaim and need arrays 
	for (int i = 0; i < n; i++)
	{
		printf("Enter maximum number of units process p%d will claim from each resource (r0 to r%d): ", i, (m - 1));

		for (int j = 0; j < m; j++)
			scanf("%d", &max_claim[i*m + j]);
	}
	// for each process, for each resource, prompt for number of resource units allocated to process 
	for (int i = 0; i < n; i++)
	{
		printf("Enter number of units of each resource (r0 to r%d) currently allocated to process p%d: ", (m - 1), i);

		for (int j = 0; j < m; j++)
			scanf("%d", &allocated[i*m + j]);
	}

	//updating the need and available array
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			need[i*m + j] = max_claim[i*m + j] - allocated[i*m + j];
			available[j] -= allocated[i*m + j];
		}
			
	}

	printf("\n");

	// print resource vector, available vector, maxclaim array, allocated array, need array 
	print(resources, "Resources");
	print(available, "Available");
	print_max_claim(max_claim, "Max Claim");
	print_max_claim(allocated, "Allocated");
	print_max_claim(need, "Need");
	return;
}


//**************************************************************
void requestResource() 
{
	int pid, rid, units;
	// prompt for process, resource, and number of units requested
	printf("Enter requesting process: p");
	scanf("%d", &pid);

	printf("Enter requested resource: r");
	scanf("%d", &rid);

	printf("Enter number of units process p%d is requesting from resource r%d: ", pid, rid);
	scanf("%d", &units);

	// if enough units available and request is less than need
	if (units <= need[pid*m + rid] && available[rid] >= units)
	{
		// reduce number of available units
		available[rid] -= units;

		// increase number of allocated units
		allocated[pid*m + rid] += units;

		// reduce number of needed units
		need[pid*m + rid] -= units;

		printf("\n");
		// print updated available, allocated, and need vectors/matrices
		print(available, "Available");
		print_max_claim(allocated, "Allocated");
		print_max_claim(need, "Need");
	}

	else
		printf("**Request was denied\n");
		return;
}


//**************************************************************
void releaseResource() 
{
	// declare local variables
	int pid, rid, units;

	// prompt for process, resource, and number of units released
	printf("Enter releasing process: p");
	scanf("%d", &pid);

	printf("Enter released resource: r");
	scanf("%d", &rid);

	printf("Enter number of units process p%d is releasing from resource r%d: ", pid, rid);
	scanf("%d", &units);

	// if enough units allocated
	if (units <= allocated[pid*m + rid])
	{
		// increase number of available units
		available[rid] += units;

		// reduce number of allocated units
		allocated[pid*m + rid] -= units;

		// increase number of needed units
		need[pid*m + rid] += units;

		printf("\n");
		// print updated available, allocated, and need vectors/matrices
		print(available, "Available");
		print_max_claim(allocated, "Allocated");
		print_max_claim(need, "Need");
	}

	else
		printf("**Release cannot be performed\n");
	
	return;
}


//******************************************************************
void findSafeSequence() 
{
	// declare local variables
	int sequencedProcesses = 0, ifProcessed = 0;
	int *processedArr = (int*)malloc(n * sizeof(int));
	int *safeSequence = (int*)malloc(n * sizeof(int));

	for (int i = 0; i < n; i++)
		processedArr[i] = -1;

	// while not all processes are processed
	while (sequencedProcesses < n)
	{
		// for each process
		for (int i = 0; i < n; i++)
		{
			// if process has not been processed yet
			if (processedArr[i] == -1)
			{
				// print message comparing need vector with available vector
				printf("Comparing: < ");

				for (int j = 0; j < m; j++)
					printf("%d ", need[i*m + j]);

				printf("> <= < ");

				for (int j = 0; j < m; j++)
					printf("%d ", available[j]);

				printf("> : ");

				// for each resource 
				int safeProcessing = -1, j = 0;
				for (j = 0; j < m; j++)
				{
					// check for safe processing by comparing process' need vector to available vector 
					if (need[i*m + j] > available[j])
						break;
				}

				// if each resource is available 
				if (j == m)
				{
					// print message that process can be processed
					printf("Process p%d can be sequenced.\n", i);
					processedArr[i] = 0;
					safeSequence[sequencedProcesses] = i;
					sequencedProcesses++;

					// for each resource 
					for (int j = 0; j < m; j++)
					{
						// update number of available units of resource 
						available[j] += allocated[i*m + j];

						//free all resources allocated to process
						allocated[i*m + j] = 0;
					}
					ifProcessed = 1;
				}

				else
					printf("Process p%d cannot be sequenced.\n", i);
					
			}
		}
	}
		
	//if (no process was processed in the final round of the for-loop)
	if (ifProcessed == 0)
		// print message of deadlock among processes not processed
		printf("**Deadlock among processes.\n");
		
	// else print safe sequence of processes
	else
	{
		printf("Safe sequence of processes: ");

		for (int i = 0; i < n; i++)
			printf("p%d ", safeSequence[i]);
	}
	printf("\n\n");

	free(processedArr);
	free(safeSequence);

	return;
}


//******************************************************************
void quit() 
{
	printf("Quitting program...\n");
	// check if vectors/array are not NULL--if so, free each vector/array
	if (resources != NULL)
		free(resources);

	if (available != NULL)
		free(available);

	if (allocated != NULL)
		free(allocated);

	if (need != NULL)
		free(need);

	if (max_claim != NULL)
		free(max_claim);

	return;
}


//*************************************************************
int main() 
{
	// declare local vars 
	int sel = 0;

	// while user has not chosen to quit 
	do {
		// print menu of options and prompt for menu selection 
		sel = displayMenu();

		// call appropriate procedure based on choice--use switch statement or series of if, else if, else statements 	
		switch (sel)
		{
		case 1:
			enterGraph();
			break;

		case 2:
			requestResource();
			break;

		case 3:
			releaseResource();
			break;

		case 4:
			findSafeSequence();
			break;

		case 5: quit();
			break;

		default: printf("\nInvalid selection. Try Again.\n");
			break;
		}

	} while (sel != 5);
		
	return 1; // indicates success 
} // end of procedure 
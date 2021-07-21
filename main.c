#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include "main.h"
#include "filters.h"

int main(int argc, char** argv) {
	//Check if number of arguments is correct
	readArgs(argc, argv);
	printf("Directory to search: %s\n", argv[1]);
	// Create startDir with a length of the max filepath size plus one (for null terminator)
	char startDir[PATH_MAX + 1];
	strcpy(startDir, argv[1]);
	// Get the filters that the user want to use
	userFilters* filters = getFilters();
	// Run a search using the filters the user has chosen
	genericSearch(startDir, filters);
	exit(0);
}

// This is the generic search function which traverses the directories
// The search criteria is passed as a function pointer this is called on every file
void genericSearch(char* startDir, userFilters* filters) {
	// Open the directory
	DIR* directoryPointer = opendir(startDir);
	// If there was not an error opening the directory
	if(directoryPointer != NULL) {
		// Go through every directory entry
		struct dirent* directoryEntry = readdir(directoryPointer);
		while(directoryEntry != NULL) {
			// If the directoryEntry is not a directory (i.e not a traversible folder))
			if(directoryEntry->d_type != DT_DIR) {
				// Assume the file matches
				int match = 1;
				// Run every filter against the directoryEntry
				int i;
				for(i=0; i<filters->numFilters; i++) {
					// Get the current filter
					userFilter* filter = filters->filters[i];
					// Run a filter and store the result
					int result = (*(filter->filter))(directoryEntry, filter->filterArgument);
					// If there is not a match, set 'match' to zero and exit the loop
					if(result != 1) {
						match = 0;
						i = filters->numFilters + 1;
					}	
				}
				// If the file met all the criteria of the filters then print the path
				if(match == 1) {
					printf("%s/%s\n", startDir, directoryEntry->d_name);
				}	
			// If the directoryEntry is a directory
			}else if(directoryEntry->d_type == DT_DIR) {
				// Make sure that the file isn't '.' or '..'
				if(strcmp(directoryEntry->d_name, ".") != 0 && strcmp(directoryEntry->d_name, "..") != 0) {
					// Create a buffer for the full path of the next directory
					char newDir[PATH_MAX + 1];
					snprintf(newDir, PATH_MAX + 1, "%s/%s", startDir, directoryEntry->d_name);
					//printf("Recursing: %s\n", newDir);
					genericSearch(newDir, filters);
				}
			}
			// Load the next directoryEntry
			directoryEntry = readdir(directoryPointer);
		}
		//Free the directory now that we're done with it
		free(directoryEntry);
	}else{
		printf("Error in opening dirp\n");
	}
	// Free the directoryPointer now that we're done with it
	closedir(directoryPointer);
}

// Gets the users filters for the search and returns an array of the filters
userFilters* getFilters() {
	printf("Here are the filters that you can apply:\n");
	printf("1. filename\n");	
	printf("2. symlink\n");
	// Get the number of filters that will be applied
	printf("How many filters would you like to apply?\n");
	int numFilters;
	scanf("%d", &numFilters);
	// Allocate the memory needed for the userFilters object
	userFilters* filtersStruct = (userFilters*)malloc(sizeof(userFilters)); 
	filtersStruct->filters = (userFilter**)malloc(sizeof(userFilter*)*numFilters);
	filtersStruct->numFilters = numFilters;
	//Loop through until all the filters have been selected by the user
	int i;
	for(i=0; i<numFilters; i++) {
		printf("Select the filter:\n");
		// Get the users choice on what filter to apply
		int userChoice; 
		scanf("%d", &userChoice);
		// Allocate the memory for the userFilter
		userFilter* filter = (userFilter*)malloc(sizeof(userFilter));
		// Bring up the correct menu for the user	
		switch(userChoice){
			case 1:
				// The user chose 'filename'
				filter->filter = &filenameFilter;
				char* filename;
				printf("Filename to filter by: ");
				scanf("%s", filename);
				filter->filterArgument = (void*)filename;
				break;
			case 2:
				// The user chose 'symlink'
				filter->filter = &symlinkFilter;
				int* symlinkFilterType = (int*)malloc(sizeof(int));
				printf("Type \'1\' to search for symlinks only or type \'2\' to exclude symlinks from your search\n");
				scanf("%d", symlinkFilterType);
				filter->filterArgument = (void*)symlinkFilterType;
				break;
		}
		filtersStruct->filters[i] = filter;
	}
	return filtersStruct;
}

// Reads the arguments and creates a struct of the filters needed during the search
void readArgs(int numArgs, char** argv) {
	//Check if the user is trying to access the help menu
	if(numArgs == 2 && strcmp(argv[1], "help") == 0) {
		printf("Usage: ./find <directory_to_search>\n");
		exit(0);
	// Check if the correct number of arguments have been provided
	}else if(numArgs != 2) {
		printf("Incorrect number of arguments. Run ./find help\n");
		exit(0);
	}
}

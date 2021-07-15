#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include <dirent.h>
#include <sys/types.h>
#include <string.h>
#include <limits.h>
#include "filters.h"

int main(int argc, char** argv) {
	//Check if number of arguments is correct
	readArgs(argc, argv);
	printf("Directory to search: %s\nFilename to find: %s\n", argv[1], argv[2]);
	// searchDirMatchingFilename(argv[1], argv[2]);
	genericSearch(argv[1], argv[2]);
	exit(0);
}

// This is the generic search function which traverses the directories
// The search criteria is passed as a function pointer this is called on every file
void genericSearch(char* startDir, char* searchFilename) {
	// Open the directory
	DIR* directoryPointer = opendir(startDir);
	// If there was not an error opening the directory
	if(directoryPointer != NULL) {
		// Go through every directory entry
		struct dirent* directoryEntry = readdir(directoryPointer);
		while(directoryEntry != NULL) {
			// If the directoryEntry is a regular file
			if(directoryEntry->d_type == DT_REG) {
				// If the filename matches the searchFilename
				if(filenameFilter(directoryEntry, searchFilename)){
					// Print the full path to the file that has matched
					printf("%s/%s\n", startDir, directoryEntry->d_name);	
				}
			}else if(directoryEntry->d_type == DT_DIR) {
				// Make sure that the file isn't '.' or '..'
				if(strcmp(directoryEntry->d_name, ".") != 0 && strcmp(directoryEntry->d_name, "..") != 0) {
					// Create a buffer for the full path of the next directory
					char newDir[PATH_MAX + 1];
					snprintf(newDir, PATH_MAX + 1, "%s/%s", startDir, directoryEntry->d_name);
					//printf("Recursing: %s\n", newDir);
					genericSearch(newDir, searchFilename);
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

// Reads the arguments and creates a struct of the filters needed during the search
void readArgs(int numArgs, char** argv) {
	//Check if the user is trying to access the help menu
	if(numArgs == 2 && strcmp(argv[1], "help") == 0) {
		printf("Usage: ./find <directory_to_search> <filename_to_search>\n");
		exit(0);
	// Check if the correct number of arguments have been provided
	}else if(numArgs != 3) {
		printf("Incorrect number of arguments. Run ./find help\n");
	}
}

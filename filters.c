#include <dirent.h>
#include <string.h>
#include <sys/types.h>

// This filter simply checks if searched filename matches the name of the file
// Returns 1 for a match, 0 for no match
int filenameFilter(struct dirent* directoryEntry, void* arg) {
	// Convert the generic argument to char*
	char* filename = (char*)arg;
	// If the filenames match
	if(strcmp(directoryEntry->d_name, filename) == 0) {
		return 1;
	}
	return 0;
}

// This filter handles all searches related to symlinks
// Returns 1 for a match, 0 for no match
int symlinkFilter(struct dirent* directoryEntry, void* arg) {
	// Get the argument from the user
	int filterType = *(int*)arg;
	// Check if the directoryEntry is a symlink
	int isSymlink = 0;
	if(directoryEntry->d_type == DT_LNK) {
		isSymlink = 1;
	}
	int returnValue = 0;
	// If there is a symlink
	if(isSymlink == 1) {
		// If the user wants to find symlinks
		if(filterType == 1) {
			returnValue = 1;
		}
	} else {
		// If the user want to find symlinks
		if(filterType == 2) {
			returnValue = 1;
		}
	}
	return returnValue;
}

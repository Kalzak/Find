#include <dirent.h>
#include <string.h>

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

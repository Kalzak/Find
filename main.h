#include <dirent.h>

// This struct will store the users selected filter and the data needed for the filter
// eg: To search by filename the struct would contain a pointer to the filter function
// 'filenameFilter' and also a char* of the string the user is searching for
typedef struct userFilter {
	int (*filter)(struct dirent*, void*);
	void* filterArgument;
} userFilter;

typedef struct userFilters {
	userFilter** filters;
	int numFilters;
} userFilters;

int main(int argc, char *argv[]);
void readArgs(int numArgs, char** secondArgument);
void genericSearch(char* startDir, userFilters* filters);
int searchDirMatchingFilename(char* dirPath, char* filename);
userFilters* getFilters();

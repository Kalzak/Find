#include <dirent.h>

int filenameFilter(struct dirent* directoryEntry, char* filename);

typedef int (*genericFunctionPointer)(void);

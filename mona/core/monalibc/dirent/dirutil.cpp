#include <monapi/messages.h>
#include "dirent_p.h"

extern "C" size_t strlcpy(char *dst, const char *src, size_t siz);

using namespace MonAPI;

struct dirent* convertFromDirInfo(monapi_directoryinfo* di, struct dirent* ent)
{
	if( ent == NULL || di == NULL ) return NULL;
	ent->d_fileno = -1;
	ent->d_off = 0;
	ent->d_reclen = sizeof(monapi_directoryinfo);
	ent->d_type = di->attr;
	strlcpy(ent->d_name, di->name, 255);
	return ent;
}

monapi_directoryinfo *getDirInfo(SharedMemory& shm, int index)
{
	int num;
	int sc;
	num = *(shm.data());
	if( index >= num ) return NULL;
	sc = sizeof(monapi_directoryinfo)*index+sizeof(int);
	return (monapi_directoryinfo*)&shm.data()[sc];
}

int getDirInfoNum(SharedMemory& shm)
{
	return *shm.data();
}

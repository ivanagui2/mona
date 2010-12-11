#ifndef _SERVER_FILE_
#define _SERVER_FILE_

enum
{
    MSG_FILE_READ_DATA = 0x0400,
//    MSG_FILE_CHANGE_DRIVE,
//    MSG_FILE_GET_CURRENT_DRIVE,
//    MSG_FILE_CHANGE_DIRECTORY,
//    MSG_FILE_GET_CURRENT_DIRECTORY,
    MSG_FILE_DECOMPRESS_BZ2,
    MSG_FILE_DECOMPRESS_BZ2_FILE,
    MSG_FILE_DECOMPRESS_ST5,
    MSG_FILE_DECOMPRESS_ST5_FILE,
    MSG_FILE_READ_DIRECTORY = 0x0410,
    MSG_FILE_OPEN,
    MSG_FILE_CLOSE,
    MSG_FILE_READ,
    MSG_FILE_SEEK,
    MSG_FILE_GET_SIZE,
    MSG_FILE_GET_DATE,
    MSG_FILE_READ_ALL,
    MSG_FILE_WRITE,
    MSG_FILE_DELETE,
};

#endif

#ifndef CGSS_IO_H
#define CGSS_IO_H

#include "tm_api.h"
#include <stdio.h>

DECL_CGSS_API(TM_HFILE, TmOpenFile, (LPCSTR lpcFileName, uint32 dwDesiredAccess, uint32 dwCreationDisposition));
DECL_CGSS_API(size_t, TmGetFileSize, (TM_HFILE hFile));
DECL_CGSS_API(size_t, TmReadFile, (TM_HFILE hFile, uint8 *pBuffer, uint32 dwBufferSize));
DECL_CGSS_API(size_t, TmWriteFile, (TM_HFILE hFile, uint8 *pBuffer, uint32 dwBufferSize));
DECL_CGSS_API(void, TmCloseFile, (TM_HFILE hFile));

#endif //CGSS_IO_H

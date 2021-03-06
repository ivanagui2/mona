/*
Copyright (c) 2005 bayside

Permission is hereby granted, free of charge, to any person 
obtaining a copy of this software and associated documentation files 
(the "Software"), to deal in the Software without restriction, 
including without limitation the rights to use, copy, modify, merge, 
publish, distribute, sublicense, and/or sell copies of the Software, 
and to permit persons to whom the Software is furnished to do so, 
subject to the following conditions:

The above copyright notice and this permission notice shall be 
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF 
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY 
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, 
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#if !defined(_MONAGUI_SDL_H_INCLUDED_)
#define _MONAGUI_SDL_H_INCLUDED_

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

// ==================================================
// SDL依存部分
// ==================================================

#include <SDL.h>
#undef   WIN32
#undef   main

/** スクリーンオブジェクト */
extern SDL_Surface* screen;

// ==================================================
// GUIサーバ依存部分
// ==================================================

/** GUIサーバー内ビットマップ構造体 */
typedef struct {
	dword Handle;
	int Width, Height;
	dword* Data;
} guiserver_bitmap;

/** GUIサーバー内ウィンドウ（部品）構造体 */
typedef struct {
	dword Handle, Parent, Owner, ThreadID;
	int X, Y, Width, Height, OffsetX, OffsetY, Opacity;
	bool Visible, Focused;
	dword Flags, TransparencyKey;
	dword BufferHandle, FormBufferHandle;
	guiserver_bitmap* __internal1;
	bool __internal2;
	char __reserved[64];
} guiserver_window;

/** GUIサーバー内ウィンドウスタイル */
enum
{
	WINDOWFLAGS_NOBORDER = 1,
	WINDOWFLAGS_MODAL = 2,
	WINDOWFLAGS_NOACTIVATE = 4,
	WINDOWFLAGS_TOPMOST = 8,
	WINDOWFLAGS_BOTTOMMOST = 16
};

#define MAKE_DWORD(a, b) (((dword)(unsigned short)(a)) + (((dword)(unsigned short)(b)) << 16))
#define GET_X_DWORD(a) ((int)(short)(a & 0xffff))
#define GET_Y_DWORD(a) ((int)(short)((a) >> 16))

// ==================================================
// MonAPI依存部分
// ==================================================

/** MonAPIダミー定数 */
enum {
	MONAPI_FALSE = 0,
	MONAPI_TRUE  = 1,
	THREAD_UNKNOWN,
	ID_GUI_SERVER,
	MSG_GUISERVER_CREATEWINDOW,
	MSG_GUISERVER_CREATEBITMAP,
	MSG_GUISERVER_ACTIVATEWINDOW,
	MSG_GUISERVER_DISPOSEWINDOW,
	MSG_GUISERVER_MOVEWINDOW,
	MSG_GUISERVER_DRAWWINDOW,
	MSG_GUISERVER_MOUSECAPTURE,
	MSG_GUISERVER_CREATEOVERLAP,
	MSG_GUISERVER_DISPOSEOVERLAP,
	MSG_GUISERVER_MOVEOVERLAP,
	MSG_GUISERVER_DECODEIMAGE,
	MSG_GUISERVER_DISPOSEBITMAP
};

/** MonAPIダミー構造体 */
typedef struct {
	dword header;
	dword arg1;
	dword arg2;
	dword arg3;
	dword from;
	char str[128];
	int length;
} MessageInfo;

namespace MonAPI {
	/** MonAPIダミークラス */
	class MemoryMap {
	public:
		static void* map(dword size)
		{
			char* memory = new char [size];
			memset(memory, 0, size);
			return memory;
		}
	};
	
	/** MonAPIダミークラス */
	class Message {
	public:
		static const int equalsHeader = 0;
		
	public:
		static int lookupMainThread(char *processName)
		{
			return 0;
		}
		
		static int send(dword thread_id, dword msg_id)
		{
			return 0;
		}
		
		static int send(dword thread_id, dword msg_id, dword arg1)
		{
			return 0;
		}
		
		static int reply(MessageInfo* info)
		{
			return 0;
		}
		
		static int receive(MessageInfo* info)
		{
			return 0;
		}
		
		static int receive(MessageInfo* info, MessageInfo* src, int msg_id)
		{
			return 0;
		}
		
		static int sendReceive(MessageInfo* info, dword thread_id, dword msg_id)
		{
			if (msg_id == MSG_GUISERVER_CREATEWINDOW) {
				info->arg2 = sizeof(guiserver_window);
			}
			return 0;
		}
		
		static int sendReceive(MessageInfo* info, dword thread_id, dword msg_id, dword arg1)
		{
			return 0;
		}
		
		static int sendReceive(MessageInfo* info, dword thread_id, dword msg_id, dword arg1, dword arg2)
		{
			return 0;
		}
		
		static int sendReceive(MessageInfo* info, dword thread_id, dword msg_id, dword arg1, dword arg2, dword arg3)
		{
			if (msg_id == MSG_GUISERVER_CREATEBITMAP) {
				info->arg2 = sizeof(guiserver_bitmap);
			}
			return 0;
		}
		
		static int sendReceive(MessageInfo* info, dword thread_id, dword msg_id, dword arg1, dword arg2, dword arg3, const char* str)
		{
			return 0;
		}
	};
	
	/** MonAPIダミークラス */
	class System {
	public:
		static dword getThreadID()
		{
			return 0;
		}
	};
}

/** MonAPIダミーメソッド */
dword monapi_get_server_thread_id(dword serverID);

/** MonAPIダミーメソッド */
bool monapi_register_to_server(int serverID, int isCout);

/** MonAPIダミーメソッド */
dword syscall_get_tid();

/** MonAPIダミーメソッド */
dword syscall_mthread_create(dword funcID);

/** MonAPIダミーメソッド */
void syscall_mthread_join(dword threadID);

/** MonAPIダミーメソッド */
void syscall_kill_thread(dword threadID);

/** MonAPIダミーメソッド */
void kill_timer(dword timerID);

#endif // _MONAGUI_SDL_H_INCLUDED_

#pragma once


#ifdef UNICODE
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#else
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif

static const unsigned int g_iWinCX = 1280;
static const unsigned int g_iWinCY = 720;


extern int	g_eCurScene;
extern HWND		g_hWnd;
extern BOOL		g_Done;
extern BOOL		g_Menu;
extern BOOL		g_AnotherMenu;
extern HINSTANCE g_hInst;
extern double	g_TickLate;
namespace Client {}
using namespace Client;

#include "Engine.h"

#include <thread>


#define NO_EVENT 0
#define CHANGE	 0
#define WINCX	1280
#define WINCY	720

#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"


#include "Log.h"
#define	ADDLOG(log)																	\
	if (CEngine::GetInstance()->GetWindow("Log"))									\
	{																				\
		dynamic_cast<CLog*>(CEngine::GetInstance()->GetWindow("Log"))->AddLog(log); \
	}																				


#include "Node.h"
void SortUp(CNode* item);
void SortDown(CNode* item);
class Heap
{


public:
	vector<CNode*> items;
	_int currentItemCount;

public:
	Heap(int maxHeapSize) {
		items.reserve(maxHeapSize);
	}
	~Heap() {
	}
	void Add(CNode* item) {
		item->heapIndex = currentItemCount;
		items[currentItemCount] = item;
		SortUp(item);
		currentItemCount++;
	}

	CNode* RemoveFirst() {
		CNode* firstItem = items[0];
		currentItemCount--;

		items[0] = items[currentItemCount];
		items[0]->heapIndex = 0;
		SortDown(items[0]);

		return firstItem;
	}

	void UpdateItem(CNode* item) {
		SortUp(item);
	}

	_int Count() {
		return currentItemCount;
	}

	bool Contains(CNode* item) {
		return items[item->heapIndex] == item;
	}

	void SortDown(CNode* item) {
		while (true) {
			int childIndexLeft = item->heapIndex * 2 + 1;
			int childIndexRight = item->heapIndex * 2 + 2;
			int swapIndex = 0;

			if (childIndexLeft < currentItemCount) {
				swapIndex = childIndexLeft;

				if (childIndexRight < currentItemCount) {
					if (items[childIndexLeft]->CompareTo(items[childIndexRight]) < 0) {
						swapIndex = childIndexRight;
					}
				}

				if (item->CompareTo(items[swapIndex]) < 0) {
					Swap(item, items[swapIndex]);
				}
				else {
					return;
				}
			}
			else
				return;
		}
	}

	void SortUp(CNode* item) {
		int parentIndex = (item->heapIndex - 1) / 2;
		while (true) {
			CNode* parentItem = items[parentIndex];
			if (item->CompareTo(parentItem) > 0)
				Swap(item, parentItem);
			else
				break;

			parentIndex = (item->heapIndex - 1) / 2;
		}
	}

	void Swap(CNode* itemA, CNode* itemB) {
		items[itemA->heapIndex] = itemB;
		items[itemB->heapIndex] = itemA;
		int itemAIndex = itemA->heapIndex;
		itemA->heapIndex = itemB->heapIndex;
		itemB->heapIndex = itemAIndex;
	}
};
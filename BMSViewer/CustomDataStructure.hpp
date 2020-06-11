#pragma once
#ifndef __CUSTOM_DATA_STRUCTURE_HPP_
#define __CUSTOM_DATA_STRUCTURE_HPP_

#ifndef _IN_
#define _IN_
#endif

#include <Windows.h>
#include <mutex>
#include <vector>

using std::vector;
using std::mutex;

enum eState {
	GP_NONE,
	GP_MAIN,
	GP_PLAY,
	GP_LOAD,
	GP_SETTING,
	GP_LOADING,
	GP_PAUSE,
	GP_EXITMENU
};

// 스크립트 구조체
typedef struct _SCRIPT_STRUCT
{
	LPTSTR *script;
	int count;
	bool bSelect;
}Script;

typedef struct _Point
{
	double x;
	double y;
}Point;

typedef struct _Rect
{
	double left;
	double top;
	double right;
	double bottom;
}Rect;

typedef struct _Size
{
	double cx;
	double cy;
}Size;

typedef struct _TextureInfo
{
	LPTSTR name = nullptr;
	LPTSTR path = nullptr;
	int width, height;
	int widthCount, heightCount;
	int** data;
} TextureInfo;

// 노드
template <class Data>
class Node
{
private:
	Data * data;
	Node<Data> * next;
	Node<Data> * prev;
	UINT nID;
	BOOL bDeleteData;

public:
	Node(Data * nData = NULL) : data(nData)
	{
		next = NULL;
		prev = NULL;
		nID = 0;
		bDeleteData = FALSE;
	}
	~Node()
	{
		if (data != NULL && bDeleteData == TRUE)
			delete data;
	}

	void operator=(const Data &_data)
	{
		data = _data;
	}

	Data* SetData(Data * nData)
	{
		Data * temp = data;
		data = nData;

		return temp;
	}
	Data* GetData()
	{
		return data;
	}
	void SetNext(Node * node)
	{
		next = node;
	}
	void SetPrev(Node * node)
	{
		prev = node;
	}
	Node<Data>* GetNext()
	{
		return next;
	}
	Node<Data>* GetPrev()
	{
		return prev;
	}
	UINT GetID()
	{
		return nID;
	}
	void SetID(UINT id)
	{
		nID = id;
	}
	void DeleteData(BOOL bDelete)
	{
		switch (bDelete)
		{
		case TRUE:
		case FALSE:
			bDeleteData = bDelete;
			break;
		}
		
	}
};

// 단일링크드 리스트
template <class T>
class SingleLinkedList
{
private:
	mutex mtx;
	Node<T> * root;		// 0번
	INT cnt;

public:
	SingleLinkedList()
	{
		root = NULL;
		cnt = 0;
	}
	SingleLinkedList(const SingleLinkedList<T> &right)
	{
		root = right.root;
		cnt = right.cnt;
	}
	~SingleLinkedList()
	{
		for (INT i = cnt; i > 0; i--)
		{
			DeleteNode();
		}
	}

	T operator[](INT idx)
	{
		return *GetData(idx);
	}

	void operator=(const SingleLinkedList<T> &List)
	{
		this->root = List.root;
		this->cnt = List.cnt;
	}

public:
	BOOL AddNode(T * data, UINT nID = 0, INT idx = 0)
	{
		mtx.lock();
		bool retValue = false;

		if (idx > cnt + 1 || idx < 0) // 들어있는 수 보다 2 이상 클때 혹은 0보다 작을때 작업 없이 종료
		{
			retValue = false;
		}
		else
		{
			Node<T> * newNode = CreateNode(data);
			Node<T> * temp = root;

			if (temp == NULL)
			{
				root = newNode;
				newNode->SetID(nID);
				cnt++;
			}
			else
			{
				if (idx == 0)
				{
					for (INT i = 1; i < cnt; i++)
					{
						temp = temp->GetNext();
					}
				}
				else
				{
					for (INT i = 1; i < idx; i++)
					{
						temp = temp->GetNext();
					}
				}

				if (temp->GetNext() != NULL)
				{
					newNode->SetNext(temp->GetNext());
				}

				temp->SetNext(newNode);

				cnt++;

				if (nID != 0)
				{
					newNode->SetID(nID);
					Sorting(); // sorting from nID
				}
			}

			retValue = true;
		}
		mtx.unlock();
		return retValue;
	}
	
	T* ChangeData(T * data, INT idx)
	{
		mtx.lock();
		T* retValue = nullptr;

		if (idx > cnt + 1 || root == NULL) // 들어있는 수 보다 2 이상 클때 혹은 0보다 작을때 작업 없이 종료
			retValue = nullptr;
		else
		{

			Node<T> * temp = root;

			for (INT i = 0, i <= count; i++)
			{
				temp = temp->GetNext();
			}

			retValue = temp->SetData(data)
		}

		mtx.unlock();
		return retValue;
	}
	// 0번부터 시작(root는 0)
	BOOL DeleteNode(INT idx = 0)
	{
		mtx.lock();
		bool retValue = true;

		if (idx < 0)
			retValue = false;

		if (root == NULL)
			retValue = false;

		Node<T> * temp = root;
		Node<T> * delNode;

		if (idx == 0)
		{
			root = root->GetNext();
			delete temp;
			cnt--;
		}
		else
		{
			for (INT i = 0; i < idx; i++)
			{
				temp = temp->GetNext();
			}


			delNode = temp->GetNext();
			temp->SetNext(delNode->GetNext());

			delete delNode;
			cnt--;
		}

		mtx.unlock();
		return retValue;
	}	// idx로 검색 후 삭제

	T* GetData(INT index)
	{
		Node<T> * temp = root;

		if (Size() == 0)
			return NULL;

		for (INT i = 0; i < index; i++)
		{
			temp = temp->GetNext();
		}

		return temp->GetData();
	}
	INT Size() { return cnt; }
private:
	Node<T>* CreateNode(T * data)
	{
		Node<T> * node = new Node<T>(data);

		return node;
	}
	Node<T>* GetNode(INT count)
	{
		Node<T> * temp = root;

		for (INT i = 0; i < count; i++)
		{
			temp = temp->GetNext();
		}

		return temp;
	}
	void Sorting()
	{
		if (Size() <= 2)
			return;

		Node<T> *temp = GetNode(Size() - 2);
		Node<T> *spare = temp->GetNext();

		temp->SetNext(NULL);
		temp = root;

		if (temp->GetID() > spare->GetID())
		{
			spare->SetNext(temp);
			root = spare;
			return;
		}

		for (; temp->GetNext() != NULL;)
		{
			if (temp->GetNext()->GetID() > spare->GetID())
			{
				spare->SetNext(temp->GetNext());
				temp->SetNext(spare);
				return;
			}
			temp = temp->GetNext();
		}
		temp->SetNext(spare);
	}
};

template <class T>
class Graph
{
private:
	SingleLinkedList<T> m_Vertex;					// 정점들
	vector<vector<int>> m_Edge;			// 간선 - 인접행렬로 표현
	INT count = 0;						// 노드 갯수
	
public:
	Graph()
	{
		
	}
	~Graph()
	{
		
	}
public:
	bool InsertVertex(T vertex, int vertexNumber, int edgeNumber[] = nullptr, int edgeNumberSize = 1)
	{
		m_Vertex.push_back(vertex);

		if (edgeNumber == nullptr)
		{
			return false;
		}
		else
		{
			for (int i = 0; i < edgeNumberSize; i++)
			{
				m_Edge[count][i] = edgeNumber[i];
			}
		}

		count++;

		return true;
	}

	T GetVertex(int vertexNumber)
	{
		return m_Vertex[vertexNumber];
	}
};

template <class T>
class CircleQueue
{
private:
	Node<T> *rear;
	INT nQueCount;
	mutex mtx;

public:
	CircleQueue() : nQueCount(0), rear(nullptr) {}
	~CircleQueue()
	{
		mtx.lock();

		if (rear != nullptr)
		{
			Node<T> *temp = rear->GetNext();
			rear->SetNext(nullptr);

			for (; temp != nullptr;)
			{
				rear = temp;
				temp = temp->GetNext();

				delete rear;
			}
		}
		mtx.unlock();
	}

public:
	void EnQueue(T *data)
	{
		mtx.lock();

		nQueCount++;

		if (rear == nullptr)
		{ // 첫 항이 비어있을 경우
			rear = new Node<T>();
			rear->SetData(data);
			rear->SetNext(rear);
		}
		else
		{
			Node<T> *temp = new Node<T>();
			temp->SetData(data);
			temp->SetNext(rear->GetNext());
			rear->SetNext(temp);
			rear = temp;
		}
		mtx.unlock();
	}

	T* DeQueue()
	{
		mtx.lock();
		T *data;
		T *retValue = nullptr;

		if (rear != nullptr)
		{
			nQueCount--;
			Node<T> *temp = rear->GetNext();

			if (temp == rear)
			{
				rear = nullptr;
				data = temp->GetData();
				temp->SetData(nullptr);

				delete temp;	// 메모리는 바로 반환
				retValue = data;
			}
			else
			{
				rear->SetNext(temp->GetNext());
				data = temp->GetData();
				temp->SetData(nullptr);

				delete temp;	// 메모리는 바로 반환
				retValue = data;
			}
		}
		mtx.unlock();
		return retValue;
	}

	bool Delete(T *data)
	{
		mtx.lock();
		Node<T> *temp = rear;
		Node<T> *del = NULL;
		bool retValue = false;

		if (temp != NULL && data != NULL)
		{
			for (; del != rear;)
			{
				del = temp->GetNext();
				if (del->GetData() == data)
				{
					nQueCount--;

					temp->SetNext(del->GetNext());

					if (del == rear)
						rear = temp;

					delete del->GetData();
					delete del;

					retValue = true;
					break;
				}
				temp = temp->GetNext();
			}
		}

		mtx.unlock();
		return retValue;
	}
	INT GetCount() { return nQueCount; }
};

template <class T>
class Queue
{
private:
	mutex mtx;

public:
	INT nQueCount = 0;
	Node<T> *front = nullptr;
	Node<T> *rear = nullptr;

public:
	Queue() {}
	~Queue()
	{
		if (rear == nullptr)
			return;

		Node<T> *temp = rear->GetNext();
		rear->SetNext(nullptr);

		for (; temp != nullptr;)
		{
			rear = temp;
			temp = temp->GetNext();

			delete rear;
		}
	}

public:
	void EnQueue(T *data)
	{
		mtx.lock();
		Node<T> *temp;
		temp = new Node<T>();
		temp->SetData(data);
		if (front == nullptr)
		{
			front = temp;
			if (rear == nullptr)
				rear = front;
		}
		else
		{
			front->SetNext(temp);
			front = temp;
		}
		nQueCount++;
		mtx.unlock();
	}
	T* DeQueue()
	{
		mtx.lock();
		if (rear == nullptr)
		{
			mtx.unlock();
			return nullptr;
		}
		
		T *data;
		if (rear == front)
		{
			front = nullptr;
			data = rear->GetData();

			delete rear;

			nQueCount--;
			rear = nullptr;
		}
		else
		{
			Node<T> *temp = rear;
			data = rear->GetData();
			rear = rear->GetNext();
			nQueCount--;

			delete temp;
		}
		mtx.unlock();
		return data;
	}
	bool Delete(T *data)
	{
		mtx.lock();
		Node<T> *temp = rear;
		Node<T> *del = nullptr;
		bool retValue = false;

		if (temp == nullptr || data == nullptr)
		{
			mtx.unlock();
			return retValue;
		}
	
		for (; del != nullptr;)
		{
			if (del->GetData() == data)
			{
				nQueCount--;

				temp->SetNext(del->GetNext());

				if (del == front)
					front = temp;

				delete del->GetData();
				delete del;

				retValue = true;
				break;
			}
			temp = temp->GetNext();
			del = temp->GetNext();
		}

		mtx.unlock();
		return retValue;
	}
	void clear()
	{
		for (int i = 0; i < nQueCount; i++)
		{
			T* temp = DeQueue();
			delete temp;
		}
		nQueCount = 0;
	}
	INT GetCount() { return nQueCount; }
};

template <class T>
class Stack
{
private:
	T *data;
	INT nTop;
	INT nCount;
public:
	Stack(INT stack_size)
	{ // 스택 초기화
		nTop = -1;
		nCount = stack_size;

		data = new T[nCount];
	}
	~Stack()
	{
		delete[] data;
	}

public:
	BOOL IsFull() {	return (nTop < nCount - 1) ? FALSE : TRUE; }
	INT GetCount() { return nTop + 1; }
	T Pop()
	{
		T temp = 0;

		// 스택이 비었다면 반환하지 않는다.
		if (nTop < 0)
		{
			return temp;
		}

		temp = data[nTop];
		data[nTop] = 0;
		nTop--;

		return temp;
	}
	// If Fail to add push, return true;
	BOOL Push(T data)
	{
		// 스택이 가득 찼다면 추가하지 않는다
		if (nTop == nCount - 1)
		{
			return  TRUE;
		}

		nTop++;
		this->data[nTop] = data;

		return FALSE;
	}
};

template <class T>
class Heap
{
public:
	static const UINT HEAP_MAX = 0;
	static const UINT HEAP_MIN = 1;

private:
	typedef struct _HeapNode
	{
		T data;
		UINT nID;
	}HeapNode;

	UINT nFlag;	// Select Max heap or Min heap
	HeapNode *ndRoot;
	int nNodeCount;

public:
	Heap(UINT nFlag = HEAP_MAX) : nFlag(nFlag)
	{
		ndRoot = NULL;
		nNodeCount = 0;
	}
	~Heap()
	{
		for (; ndRoot != NULL;)
		{
			Delete();
		}
	}

public:
	void Insert(HeapNode data, UINT nID) // Do Not Insert same ID.
	{
		int idx = ++nNodeCount;

		if (ndRoot == NULL)
		{
			ndRoot = (HeapNode*)malloc(sizeof(HeapNode) * (nNodeCount + 1));
			ndRoot[1].data = data; // [0] doesn't use.
			ndRoot[1].nID = nID;
			return;
		}

		ndRoot = (HeapNode*)realloc(ndRoot, sizeof(HeapNode) * (nNodeCount + 1));
		ndRoot[idx].data = data;
		ndRoot[idx].nID = nID;

		while (true)
		{
			if ((int)(idx / 2) == 0)
				break;

			switch (nFlag)
			{
			case HEAP_MAX:
				if (ndRoot[idx].nID > ndRoot[(int)(idx / 2)].nID)
				{	// Swap data
					HeapNode temp = ndRoot[idx];
					ndRoot[idx] = ndRoot[(int)(idx / 2)];
					ndRoot[(int)(idx / 2)] = temp;
				}
				else
					return;

				break;
			case HEAP_MIN:
				if (ndRoot[idx].nID < ndRoot[(int)(idx / 2)].nID)
				{	// Swap data
					HeapNode temp = ndRoot[idx];
					ndRoot[idx] = ndRoot[(int)(idx / 2)];
					ndRoot[(int)(idx / 2)] = temp;
				}
				else
					return;

				break;
			}

			idx = (int)(idx / 2);
		}
	}
	HeapNode Delete() // if delete root, it will return data.
	{
		int idx = 1;
		HeapNode temp;

		if (ndRoot == NULL)
			return -1;

		HeapNode rtn = ndRoot[idx];	// return value

		ndRoot[idx] = ndRoot[nNodeCount];
		nNodeCount--;

		if (nNodeCount == 0)
		{
			delete ndRoot;
			ndRoot = NULL;
		}
		else
		{
			HeapNode * temp = NULL;
			temp = (HeapNode*)malloc(sizeof(HeapNode) * (nNodeCount + 1));
			memcpy(temp, ndRoot, sizeof(HeapNode) * (nNodeCount + 1));
			delete ndRoot;
			ndRoot = temp;
		}

		for (;;)
		{
			switch (nFlag)
			{
			case HEAP_MAX:
				if ((idx * 2) + 1 > nNodeCount)
				{
					if ((idx * 2) <= nNodeCount)
					{
						if (ndRoot[idx].nID < ndRoot[idx * 2].nID)
						{
							temp = ndRoot[idx];
							ndRoot[idx] = ndRoot[idx * 2];
							ndRoot[idx * 2] = temp;
						}
					}

					return rtn;
				}
				if (ndRoot[idx * 2].nID > ndRoot[(idx * 2) + 1].nID)
				{	// left priority
					if (ndRoot[idx].nID < ndRoot[idx * 2].nID)
					{	// left
						temp = ndRoot[idx];
						ndRoot[idx] = ndRoot[idx * 2];
						ndRoot[idx * 2] = temp;
						idx *= 2; // Child Node
					}
					else if (ndRoot[idx].nID < ndRoot[(idx * 2) + 1].nID)
					{	// right
						temp = ndRoot[idx];
						ndRoot[idx] = ndRoot[(idx * 2) + 1];
						ndRoot[(idx * 2) + 1] = temp;
						idx = (idx * 2) + 1; // Child Node
					}
					else
						return rtn;
				}
				else if (ndRoot[idx * 2].nID < ndRoot[(idx * 2) + 1].nID)
				{	// right priority
					if (ndRoot[idx].nID < ndRoot[(idx * 2) + 1].nID)
					{	// right
						temp = ndRoot[idx];
						ndRoot[idx] = ndRoot[(idx * 2) + 1];
						ndRoot[(idx * 2) + 1] = temp;
						idx = (idx * 2) + 1; // Child Node
					}
					else if (ndRoot[idx].nID < ndRoot[idx * 2].nID)
					{	// left
						temp = ndRoot[idx];
						ndRoot[idx] = ndRoot[idx * 2];
						ndRoot[idx * 2] = temp;
						idx *= 2; // Child Node
					}
					else
						return rtn;
				}
				else
					return rtn;
				break;
			case HEAP_MIN:
				if ((idx * 2) + 1 > nNodeCount)
				{
					if ((idx * 2) <= nNodeCount)
					{
						if (ndRoot[idx].nID > ndRoot[idx * 2].nID)
						{
							temp = ndRoot[idx];
							ndRoot[idx] = ndRoot[idx * 2];
							ndRoot[idx * 2] = temp;
						}
					}

					return rtn;
				}
				if (ndRoot[idx * 2].nID < ndRoot[(idx * 2) + 1].nID)
				{	// left priority
					if (ndRoot[idx].nID > ndRoot[idx * 2].nID)
					{	// left
						temp = ndRoot[idx];
						ndRoot[idx] = ndRoot[idx * 2];
						ndRoot[idx * 2] = temp;
						idx *= 2; // Child Node
					}
					else if (ndRoot[idx].nID > ndRoot[(idx * 2) + 1].nID)
					{	// right
						temp = ndRoot[idx];
						ndRoot[idx] = ndRoot[(idx * 2) + 1];
						ndRoot[(idx * 2) + 1] = temp;
						idx = (idx * 2) + 1; // Child Node
					}
					else
						return rtn;
				}
				else if (ndRoot[idx * 2].nID > ndRoot[(idx * 2) + 1].nID)
				{	// right priority
					if (ndRoot[idx].nID > ndRoot[(idx * 2) + 1].nID)
					{	// right
						temp = ndRoot[idx];
						ndRoot[idx] = ndRoot[(idx * 2) + 1];
						ndRoot[(idx * 2) + 1] = temp;
						idx = (idx * 2) + 1; // Child Node
					}
					else if (ndRoot[idx].nID > ndRoot[idx * 2].nID)
					{	// left
						temp = ndRoot[idx];
						ndRoot[idx] = ndRoot[idx * 2];
						ndRoot[idx * 2] = temp;
						idx *= 2; // Child Node
					}
					else
						return rtn;
				}
				else
					return rtn;
				break;
			}

			if (idx > nNodeCount)
				break;
		}

		return rtn;
	}
	HeapNode Search(UINT nTargetID, UINT nFlag = PREORDER, int index = 1) // return data
	{
		int idx = index;
		HeapNode result;

		if (ndRoot == NULL)
			return result;

		if (!(nFlag >= PREORDER && nFlag <= POSTORDER))
			nFlag = PREORDER;

		switch (nFlag)
		{
		case PREORDER:
			if (ndRoot[idx].nID == nTargetData)
				result = ndRoot[idx];

			if ((idx * 2) <= nNodeCount && result == -1)
				result = Search(nTargetData, nFlag, (idx * 2));

			if (((idx * 2) + 1) <= nNodeCount && result == -1)
				result = Search(nTargetData, nFlag, ((idx * 2) + 1));

			break;

		case INORDER:
			if ((idx * 2) <= nNodeCount)
				result = Search(nTargetData, nFlag, (idx * 2));

			if (ndRoot[idx].nID == nTargetData && result == -1)
				result = ndRoot[idx];

			if (((idx * 2) + 1) <= nNodeCount && result == -1)
				result = Search(nTargetData, nFlag, ((idx * 2) + 1));

			break;

		case  POSTORDER:
			if ((idx * 2) <= nNodeCount)
				result = Search(nTargetData, nFlag, (idx * 2));

			if (((idx * 2) + 1) <= nNodeCount && result == -1)
				result = Search(nTargetData, nFlag, ((idx * 2) + 1));

			if (ndRoot[idx].nID == nTargetData && result == -1)
				result = ndRoot[idx];

			break;
		}

		return result;
	}
	int GetNodeCount() { return nNodeCount; }
};

template <class T>
class Matrix
{
private:
	vector<vector<T>> data;

public:
	Matrix()
	{
	}
	Matrix(int row, int col)
	{
		SetSize(this->row, this->col);
	}

	~Matrix()
	{
		
	}

	vector<T> operator[](int i)
	{
		return data[i];
	}

	void operator=(const Matrix<T> &other)
	{
		data = other.data;
	}

	void operator=(const vector<vector<T>> &other)
	{
		data = other;
	}

public:
	void Clear()
	{
		if (data.size() > 0)
		{
			for (int i = data.size(); i >= 0; i--)
			{
				data[i - 1].clear();
			}
			data.clear();
		}
	}

	int GetItemsCount()
	{
		if (data.size() <= 0)
			return 0;

		if (data[0].size() <= 0)
			return 0;

		return (data.size() * data[0].size());
	}

	// return cx = col = 가로, cy = row = 세로
	Size GetSize()
	{
		return Size{ (double)data[0].size(), (double)data.size() };
	}

	// row = 세로, col = 가로
	void SetSize(int row, int col)
	{
		if (!(row > 0 && col > 0))
			return;

		// resize
		data.resize(row);

		for (int i = 0; i < row; i++)
		{
			data[i].resize(col);
		}
	}

	vector<T> GetVector(int row)
	{
		return data[row];
	}

	void SetData(T** _data, int row, int col)
	{
		SetSize(row, col);

		for (int i = 0; i < row; i++)
		{
			for (int j = 0; j < col; j++)
			{
				data[i][j] = _data[i][j];
			}
		}
	}

	T GetData(int row, int col)
	{
		return data[row][col];
	}
};

#endif
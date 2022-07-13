#ifndef MINHEAP_H
#define MINHEAP_H

#include "PositiveKey.h"

class MinHeap {

public:

	MinHeap(unsigned int s);
	~MinHeap();

	bool empty() const;
	PositiveKey peek() const;
	void insert(PositiveKey x);
	PositiveKey remove();
	void replace(PositiveKey x);

private:

	void swap(int i, int j);
	void shiftUp(int parent, int child);
	void shiftDown(int parent);

	int last, size;
	PositiveKey* data;

};

#endif

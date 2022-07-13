#include <stdexcept>
#include "MinHeap.h"

MinHeap::MinHeap(unsigned int s) {
	if (s == 0)
		throw std::runtime_error("Invalid heap size!");

	last = -1;
	size = s;
	data = new PositiveKey[size];
}

MinHeap::~MinHeap() {
	delete[] data;

	data = nullptr;
}

bool MinHeap::empty() const {
	return (last < 0);
}

PositiveKey MinHeap::peek() const {
	if (empty())
		throw std::runtime_error("Heap is empty!");

	return data[0];
}

void MinHeap::insert(PositiveKey x) {
	if (last == size - 1)
		throw std::runtime_error("Heap is full!");

	data[++last] = x;
	shiftUp((last - 1) / 2, last);
}

PositiveKey MinHeap::remove() {
	if (empty())
		throw std::runtime_error("Heap is empty!");

	PositiveKey min = data[0];
	data[0] = data[last--];
	shiftDown(0);

	return min;
}

void MinHeap::replace(PositiveKey x) {
	if (empty())
		throw std::runtime_error("Heap is empty!");

	data[0] = x;
	shiftDown(0);
}

void MinHeap::swap(int i, int j) {
	PositiveKey v = data[i];
	data[i] = data[j];
	data[j] = v;
}

void MinHeap::shiftUp(int parent, int child) {
	if (data[child].estimate < data[parent].estimate) {
		swap(parent, child);

		if (parent != 0)
			shiftUp((parent - 1) / 2, parent);
	}
}

void MinHeap::shiftDown(int parent) {
	int left = 2 * parent + 1;
	int right = 2 * parent + 2;

	if (left <= last) {
		if (right <= last) {
			if (data[left].estimate < data[right].estimate) {
				if (data[parent].estimate > data[left].estimate) {
					swap(parent, left);
					shiftDown(left);
				}
			}
			else {
				if (data[parent].estimate > data[right].estimate) {
					swap(parent, right);
					shiftDown(right);
				}
			}
		}
		else {
			if (data[parent].estimate > data[left].estimate) {
				swap(parent, left);
			}
		}
	}
}

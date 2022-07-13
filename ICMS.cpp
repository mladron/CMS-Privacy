//
// ICMS.cpp Inverted Count-Min Sketch implemented by J. Martínez
//

#include <iostream>
#include <cstdlib>
#include <ctime>
#include "CMS.h"
#include "ICMS.h"


/*
 * ICMS constructor initializes the count table and the array of hashes
 */

ICMS::ICMS(unsigned int width, short int depth, CMS* cms) {
	table_width = width;
	table_depth = depth;

	items = 0;

	// initialize the counter table

	table = new unsigned int* [table_depth];

	for (int i = 0; i < table_depth; i++)
		table[i] = new unsigned int[table_width];

	for (int i = 0; i < table_depth; i++)
		for (int j = 0; j < table_width; j++) {
			table[i][j] = 0;
		}

	// initialize the linked lists of keys associated to each counter

	keys = new std::deque<uint32_t> *[table_depth];

	for (int i = 0; i < table_depth; i++)
		keys[i] = new std::deque<uint32_t>[table_width];

	for (int i = 0; i < table_depth; i++)
		for (int j = 0; j < table_width; j++) {
			keys[i][j].clear();
		}

	// initialize d pairwise independent hashes

	hashes = new unsigned int* [table_depth];

	for (int i = 0; i < table_depth; i++) {
		hashes[i] = new unsigned int[2];

		hashes[i][0] = cms->hashValue(i, 0);
		hashes[i][1] = cms->hashValue(i, 1);
	}
}


/*
 * ICMS destructor deletes the table of counters, keys, and hashes
 */

ICMS::~ICMS() {

	// deallocate the table of counters

	for (int i = 0; i < table_depth; i++) {
		delete[] table[i];
	}

	delete table;

	// deallocate the table containing the list of keys

	for (int i = 0; i < table_depth; i++) {
		delete[] keys[i];
	}

	delete keys;

	// deallocate the table of hashes

	for (int i = 0; i < table_depth; i++) {
		delete[] hashes[i];
	}

	delete hashes;
}


/*
 * initializes the counters and the adjacency lists
 */

void ICMS::initialize() {

	// initialize the items and the counters

	items = 0;

	for (int i = 0; i < table_depth; i++)
		for (int j = 0; j < table_width; j++) {
			table[i][j] = 0;
		}

	// initialize the linked lists of keys associated to each counter

	for (int i = 0; i < table_depth; i++)
		for (int j = 0; j < table_width; j++) {
			keys[i][j].clear();
		}
}


/*
 * updates the count of a numeric item
 */

void ICMS::update(uint64_t item, const unsigned int counter[]) {
	items = items + 1;

	uint64_t hashval = 0;

	for (int i = 0; i < table_depth; i++) {
		hashval = (((long)hashes[i][0] * item + hashes[i][1]) % LONG_PRIME) % table_width;

		// updates the counter with the input counters from the CMS, and adds the item to the items list

		table[i][hashval] = counter[i];
		keys[i][hashval].push_back(item);
	}
}


/*
 * updates the count of a string item
 */

void ICMS::update(const char* str, const unsigned int counter[]) {
	update(hashstr(str), counter);
}


/*
 * estimates the count of a numeric item
 */

unsigned int ICMS::estimate(uint64_t item) {
	unsigned int min = std::numeric_limits<unsigned int>::max();

	uint64_t hashval = 0;

	for (int i = 0; i < table_depth; i++) {
		hashval = (((long)hashes[i][0] * item + hashes[i][1]) % LONG_PRIME) % table_width;
		min = MIN(min, table[i][hashval]);
	}

	return min;
}


/*
 * estimates the count of a string item
 */

unsigned int ICMS::estimate(const char* str) {
	return estimate(hashstr(str));
}


/*
 * returns the value of a counter
 */

unsigned int ICMS::counter(unsigned int w, short int d) {
	return table[d][w];
}


/*
 * returns the values of the hashes
 */

unsigned int ICMS::hashValue(short int depth, short int h) {
	return hashes[depth][h];
}


/*
 * calculates the hash of an item for a given hash function
 */

uint64_t ICMS::hash(uint64_t item, short int h) {
	return (((long)hashes[h][0] * item + hashes[h][1]) % LONG_PRIME) % table_width;
}


/*
 * generates a hash value of a string
 */

uint64_t ICMS::hashstr(const char* str) {
	uint64_t hash = 5381;

	int c;

	while (c = *str++) {
		hash = ((hash << 5) + hash) + c;
	}

	return hash;
}


/*
 * hash functions
 */

unsigned int ICMS::hash(uint64_t key, int i, int s) {
	unsigned int val = RSHash(key) + i * JSHash(key);

	if (i == 2) val = RSHash(key);
	if (i == 1) val = JSHash(key);

	return (val % s);
}

// RSHash function

unsigned int ICMS::RSHash(uint64_t key) {
	int b = 378551;
	int a = 63689;
	int hash = 0;
	int i = 0;
	char k[8];

	memcpy(k, &key, 8);

	for (i = 0; i < 8; i++)
	{
		hash = hash * a + k[i];
		a = a * b;
	}

	return hash;
}

// JSHash function

unsigned int ICMS::JSHash(uint64_t key) {
	int hash = 1315423911;
	int i = 0;
	char k[8];

	memcpy(k, &key, 8);

	for (i = 0; i < 8; i++)
	{
		hash ^= ((hash << 5) + k[i] + (hash >> 2));
	}

	return hash;
}

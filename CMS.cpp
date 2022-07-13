//
// CMS.cpp Count-Min Sketch implemented by J. Martínez
//

#include <iostream>
#include <cstdlib>
#include <ctime>
#include "CMS.h"


/*
 * CMS constructor initializes the count table and the array of hashes
 */

CMS::CMS(unsigned int width, short int depth, int seed) {

	// width: table width
	// depth: table depth or number of counters checked

	table_width = width;
	table_depth = depth;

	items = 0;

	// initialize the counter table

	table = new unsigned int*[table_depth];

	for (int i = 0; i < table_depth; i++)
		table[i] = new unsigned int[table_width];

	for (int i = 0; i < table_depth; i++)
		for (int j = 0; j < table_width; j++) {
			table[i][j] = 0;
		}

	// initialize d pairwise independent hashes

	srand(seed);

	hashes = new unsigned int*[table_depth];

	for (int i = 0; i < table_depth; i++) {
		hashes[i] = new unsigned int[2];
		genhash(hashes, i);
	}
}


/*
 * CMS destructor deletes count table and the array of hashes
 */

CMS::~CMS() {
	for (int i = 0; i < table_depth; i++) {
		delete[] table[i];
	}

	delete table;

	for (int i = 0; i < table_depth; i++) {
		delete[] hashes[i];
	}

	delete hashes;
}


/*
 * updates the count of a numeric item
 */

void CMS::update(uint64_t item) {
	items = items + 1;

	uint64_t hashval = 0;

	for (int i = 0; i < table_depth; i++) {
		hashval = (((long)hashes[i][0] * item + hashes[i][1]) % LONG_PRIME) % table_width;

		table[i][hashval] = table[i][hashval] + 1;
	}
}


/*
 * updates the count of a string item
 */

void CMS::update(const char *str) {
	update(hashstr(str));
}


/*
 * estimates the count of a numeric item
 */

unsigned int CMS::estimate(uint64_t item) {
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

unsigned int CMS::estimate(const char *str) {
	return estimate(hashstr(str));
}


/*
 * returns the value of a counter
 */

unsigned int CMS::counter(unsigned int w, short int d) {
	return table[d][w];
}


/*
 * returns the values of the counters of a given item  
 */

void CMS::counters(uint64_t item, unsigned int counter[]) {
	uint64_t hashval = 0;

	for (int i = 0; i < table_depth; i++) {
		hashval = (((long)hashes[i][0] * item + hashes[i][1]) % LONG_PRIME) % table_width;

		counter[i] = table[i][hashval];
	}
}


/*
 * returns the values of the hashes
 */

unsigned int CMS::hashValue(short int depth, short int h) {
	return hashes[depth][h];
}


/*
 * calculates the hash of an item for a given hash function
 */

uint64_t CMS::hash(uint64_t item, short int h) {
	return (((long)hashes[h][0] * item + hashes[h][1]) % LONG_PRIME) % table_width;
}


/*
 * generates hash functions
 */

void CMS::genhash(unsigned int** hashes, int i) {
	hashes[i][0] = JSHash((uint64_t)(float(rand()) * float(LONG_PRIME) / float(RAND_MAX) + 1));
	hashes[i][1] = RSHash((uint64_t)(float(rand()) * float(LONG_PRIME) / float(RAND_MAX) + 1));
}


/*
 * generates a hash value of a string
 */

uint64_t CMS::hashstr(const char *str) {
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

unsigned int CMS::hash(uint64_t key, int i, int s) {
	unsigned int val = RSHash(key) + i * JSHash(key);

	if (i == 2) val = RSHash(key);
	if (i == 1) val = JSHash(key);

	return (val % s);
}

// RSHash function

unsigned int CMS::RSHash(uint64_t key) {
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

unsigned int CMS::JSHash(uint64_t key) {
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

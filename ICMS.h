#ifndef ICMS_H
#define ICMS_H

//
// ICMS.hpp Inverted Count-Min Sketch implemented by J. Martínez
//

#include <deque>

#define LONG_PRIME 4294967311l
#define MIN(a, b) (a < b ? a : b)

class ICMS {
public:

	ICMS(unsigned int width, short int depth, CMS* cms);				// class constructor 
	~ICMS();															// class destructor

	void initialize();													// initializes the counters and the adjacency lists
	unsigned int width() { return table_width; };						// gets table width
	unsigned int depth() { return table_depth; };						// gets table depth

	void update(uint64_t item, const unsigned int counter[]);			// updates the counters of a numeric item
	void update(const char* str, const unsigned int counter[]);			// updates the counters of a string item

	unsigned int estimate(uint64_t item);								// estimates the count of a numeric item
	unsigned int estimate(const char* str);								// estimates the count of a string item

	unsigned int totalItems() { return items; };						// gets the count of all the items stored in the sketch		

	unsigned int** table;												// counter table
	std::deque<uint32_t>** keys;										// linked list with the keys associated to a counter

	unsigned int counter(unsigned int w, short int d);					// returns the value of a counter
	unsigned int hashValue(short int depth, short int h);				// returns the hash value

	uint64_t hash(uint64_t item, short int h);							// calculates the hash of an item for a given hash function

private:

	unsigned int items;													// total items stored
	unsigned int table_width, table_depth;								// width and depth of the count table

	unsigned int** hashes;												// hash values

	unsigned int hash(uint64_t key, int i, int s);
	unsigned int RSHash(uint64_t key);
	unsigned int JSHash(uint64_t key);

	uint64_t hashstr(const char* str);									// generates a hash value of a string

};

#endif
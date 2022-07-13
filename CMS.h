#ifndef CMS_H
#define CMS_H

//
// CMS.hpp Count-Min Sketch implemented by J. Martínez
//

#define LONG_PRIME 4294967311l
#define MIN(a, b) (a < b ? a : b)

class CMS {
public:

	CMS(unsigned int width, short int depth, int seed);				// class constructor 
	~CMS();															// class destructor

	unsigned int width() { return table_width; };					// gets table width
	unsigned int depth() { return table_depth; };					// gets table depth

	void update(uint64_t item);										// updates the count of a numeric item
	void update(const char *str);									// updates the count of a string item

	unsigned int estimate(uint64_t item);							// estimates the count of a numeric item
	unsigned int estimate(const char *str);							// estimates the count of a string item

	unsigned int counter(unsigned int w, short int d);				// returns the value of a counter
	void counters(uint64_t item, unsigned int counter[]);			// returns the value of the counters of a numeric item
	unsigned int hashValue(short int depth, short int h);			// returns the hash value

	unsigned int totalItems() { return items; };					// gets the count of all the items stored in the sketch	

	uint64_t hash(uint64_t item, short int h);						// calculates the hash of an item for a given hash function

private:

	unsigned int items;												// total items stored
	unsigned int table_width, table_depth;							// width and depth of the count table

	unsigned int **table;											// counter table
	unsigned int **hashes;											// hash values

	unsigned int hash(uint64_t key, int i, int s);
	unsigned int RSHash(uint64_t key);
	unsigned int JSHash(uint64_t key);

	uint64_t hashstr(const char *str);							// generates a hash value of a string
	void genhash(unsigned int **hashes, int i);					// generates hashes

};

#endif
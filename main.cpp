/*
 * On the Privacy of the Count-Min Sketch 
 */

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include "PositiveKey.h"
#include "KeyFrequency.h"
#include "PeelingThreshold.h"
#include "PeelingOutput.h"
#include "HeavyHitterEstimate.h"
#include "MinHeap.h"
#include "CMS.h"
#include "ICMS.h"


int POSITIVES_MAX_SIZE;


/*
 * testSetup stores the command line arguments
 */

struct testSetup {
	unsigned int width;
	short int depth;
	std::string inputKeys;
	int p;
	int step;
	int uniqueKeys;
	int seed;
	int algorithm;
};


/*
 * setupICMSTest initializes the test setup with the command line arguments
 */

testSetup setupICMSTest(int argc, char* argv[]) {
	testSetup setup;

	setup.width = -1;
	setup.depth = -1;
	setup.inputKeys = "";
	setup.p = -1;
	setup.step = -1;
	setup.seed = -1;
	setup.algorithm = -1;

	for (int i = 1; i < argc; i++) {
		std::stringstream str;
		str << argv[i];

		std::string argument = str.str();

		// argument 'w' the table width, default value is w=4096 

		if (argument[0] == 'w' || argument[0] == 'W') {
			int len = argument.size();
			setup.width = std::stoi(argument.substr(2, len - 1).c_str());
		}

		// argument 'd' is the table depth, default value is d=4

		if (argument[0] == 'd' || argument[0] == 'D') {
			int len = argument.size();
			setup.depth = std::stoi(argument.substr(2, len - 1).c_str());
		}

		// argument 'f' is the input file name

		if (argument[0] == 'f' || argument[0] == 'F') {
			int len = argument.size();
			setup.inputKeys = argument.substr(2, len - 1).c_str();
		}

		// argument 'p' is the minimum value of p

		if (argument[0] == 'p' || argument[0] == 'P') {
			int len = argument.size();
			setup.p = std::stoi(argument.substr(2, len - 1).c_str());
		}

		// argument 'i' is the increment/delta of p

		if (argument[0] == 'i' || argument[0] == 'I') {
			int len = argument.size();
			setup.step = std::stoi(argument.substr(2, len - 1).c_str());
		}

		// argument 's' is the CMS seed

		if (argument[0] == 's' || argument[0] == 'S') {
			int len = argument.size();
			setup.seed = std::stoi(argument.substr(2, len - 1).c_str());
		}

		// argument 'a' is the strategy to pick a "heavy hitter" from adjacency lists

		if (argument[0] == 'a' || argument[0] == 'A') {
			int len = argument.size();
			setup.algorithm = std::stoi(argument.substr(2, len - 1).c_str());
		}
	}

	// set default values for command line arguments 

	if (setup.width == -1)
		setup.width = 16384;

	if (setup.depth == -1)
		setup.depth = 4;

	if (setup.inputKeys.compare("") == 0) {
		setup.inputKeys = "sj.txt";
	}

	if (setup.p == -1)
		setup.p = 256;

	if (setup.step == -1) {
		setup.step = 128;
	}

	if (setup.seed == -1)
		setup.seed = 45687;

	if (setup.algorithm == -1)
		setup.algorithm = 2;

	POSITIVES_MAX_SIZE = (setup.p + setup.step * 24) + 4096;

	return setup;
}


/*
 * displayICMParameters shows the test setup on the console
 */

void showICMSParameters(testSetup setup) {
	std::cout << "Inverting Count-Min Sketch \n\n";

	std::cout << "Table depth      d = " << setup.depth << "\n";
	std::cout << "Table width      w = " << setup.width << "\n";
	std::cout << "Input keys       f = '" << setup.inputKeys << "' \n";
	std::cout << "CMS seed         s = " << setup.seed << "\n";
	std::cout << "Range of P       p = [" << setup.p << "," << (setup.p + setup.step * 24) << "] \n";
	std::cout << "Hitter selection a = " << setup.algorithm << " (" << ((setup.algorithm == 1) ? "First" : ((setup.algorithm == 2) ? "Maximum" : "Minimum")) << " estimate) \n";
	std::cout << "\n";
}


/*
 * ip2int converts a string ip into a number
 */

uint32_t ip2int(const std::string& str) {
	std::istringstream iss(str);

	uint32_t ip = 0, octet;

	for (unsigned short int i = 0; i < 4; i++) {
		iss >> octet;

		ip = ip | octet << (24 - 8 * i);

		if (i <= 2) {
			char delimiter;
			iss >> delimiter;
		}
	}

	return ip;
}


/*
 * int2ip converts a number into a string ip
 */

std::string int2ip(uint32_t ip) {
	std::stringstream ss;

	ss.str("");
	ss << (ip >> 24 & 0xFF) << "." << (ip >> 16 & 0xFF) << "." << (ip >> 8 & 0xFF) << "." << (ip & 0xFF);

	return ss.str();
}


/*
 * compareKey and compareFrequency are used by qsort
 */

int compareKey(const void* a, const void* b) {
	const KeyFrequency* x = (KeyFrequency*)a;
	const KeyFrequency* y = (KeyFrequency*)b;

	if (x->ip > y->ip)
		return 1;
	else if (x->ip < y->ip)
		return -1;

	return 0;
}

int compareFrequency(const void* a, const void* b) {
	const PeelingOutput* x = (PeelingOutput*)a;
	const PeelingOutput* y = (PeelingOutput*)b;

	if (x->frequency < y->frequency)
		return 1;
	else if (x->frequency > y->frequency)
		return -1;

	return 0;
}


/*
 * frequencyOfIP calculates the real frequency of an ip from the input trace file
 */

unsigned int frecuencyOfIP(KeyFrequency realFrequencies[], int size, unsigned int key) {
	int mid, low = 0, high = size - 1;

	while (low <= high) {
		mid = (low + high) / 2;

		if (realFrequencies[mid].ip == key) {
			return realFrequencies[mid].frequency;
		}
		else if (realFrequencies[mid].ip > key) {
			high = mid - 1;
		}
		else {
			low = mid + 1;
		}
	}

	return 0;
}


/*
 * uniqueKeysFromTrace returns the size of the file of unique keys and real frequencies
 */

int uniqueKeysFromTrace(std::string keys) {
	std::stringstream ss;

	ss.str("");
	ss << keys.substr(0, keys.length() - 4) << "_freq.txt";
	std::string frequencies = ss.str();

	std::string data, inputLine;
	std::ifstream inputFile;

	int uniqueKeys = 7208399;

	inputFile.open(frequencies);

	if (inputFile.is_open()) {
		// read the header line containing the size of the file, skip frequency and ip columns

		getline(inputFile, inputLine);

		std::stringstream tokenizer(inputLine);

		getline(tokenizer, data, '\t');
		getline(tokenizer, data, '\t');
		getline(tokenizer, data);

		uniqueKeys = stoi(data);

		inputFile.close();
	}

	return uniqueKeys;
}


/*
 * initializeUniqueKeysAndRealFrequencies loads the unique keys and their real frequencies, qsort uses compareKey
 */

void initializeUniqueKeysAndRealFrequencies(KeyFrequency realFrequencies [], int maxKeys, std::string keys) {
	std::stringstream ss;

	ss.str("");
	ss << keys.substr(0, keys.length() - 4) << "_freq.txt";
	std::string frequencies = ss.str();

	std::string data, inputLine;
	std::ifstream inputFile;

	uint32_t ip;
	unsigned int frequency;

	int currentKey = 0;

	inputFile.open(frequencies);

	if (inputFile.is_open()) {
		// skip the header line

		getline(inputFile, inputLine);

		while (getline(inputFile, inputLine)) {
			// extract the IP address and its real frequency

			std::stringstream tokenizer(inputLine);

			getline(tokenizer, data, '\t');

			frequency = stoi(data);

			getline(tokenizer, data, '\t');

			ip = ip2int(data);

			if (currentKey < maxKeys) {
				realFrequencies[currentKey].ip = ip;
				realFrequencies[currentKey].frequency = frequency;

				currentKey++;
			}
			else
				break;
		}

		// sort the array by ip to use binary search

		qsort(realFrequencies, maxKeys, sizeof(KeyFrequency), compareKey);

		inputFile.close();
	}
}


/*
 * keysWithHighestEstimate finds the p ips of the universe that have the highest CMS estimate
 */

void keysWithHighestEstimate(CMS* cms, PositiveKey positives[]) {
	PositiveKey key, min;

	// calculate the P set checking IPs of the IPv4 universe, from 0 to 4294967295 (0.0.0.0 to 255.255.255.255)
	// using a min heap to store the p keys with highest frequencies

	MinHeap heap = MinHeap(POSITIVES_MAX_SIZE);

	// insert the first POSITIVES_MAX_SIZE IPs in the heap

	for (uint64_t ip = 0; ip < POSITIVES_MAX_SIZE; ip++) {
		key.ip = ip;
		key.estimate = cms->estimate(ip);

		heap.insert(key);
	}

	// keep the set of p keys with the highests estimates (frequencies)

	for (uint64_t ip = POSITIVES_MAX_SIZE; ip <= 4294967295; ip++) {
		key.ip = ip;
		key.estimate = cms->estimate(ip);

		// if the estimate of the current key is greater than the minimum estimate in the heap, replace the minimum estimate with the new estimate
		// the replace operation is equivalent to remove the minimum estimate, and insert the estimate of the current key

		min = heap.peek();

		if (key.estimate > min.estimate) {
			heap.replace(key);
		}
	}

	// get the set of positives with the highest frequencies (heavy hitters)

	for (int i = POSITIVES_MAX_SIZE - 1; !heap.empty(); i--) {
		positives[i] = heap.remove();
	}

	heap.~MinHeap();
}


/*
 * sizeOfPositivesAndThreshold finds the real size of the set P and the threshold for the input p
 */

PeelingThreshold sizeOfPositivesAndThreshold(PositiveKey positives[], int size, int p) {
	PeelingThreshold peeling;

	// set the actual size of the set P

	for (int i = p; i < size; i++) {
		if (positives[i].estimate < positives[i - 1].estimate) {
			peeling.positives = i;
			peeling.threshold = positives[i - 1].estimate;

			break;
		}
	}

	return peeling;
}


/*
 * initializeCMS() initializes the CMS with the keys in the input trace
 */

void initializeCMS(CMS* cms, unsigned int width, short int depth, std::string keys) {
	std::string address, inputLine;
	std::ifstream inputFile;

	uint32_t ip;

	inputFile.open(keys);

	if (inputFile.is_open()) {
		while (getline(inputFile, inputLine)) {
			// extract the IP address from the input line

			std::stringstream tokenizer(inputLine);
			getline(tokenizer, address, ' ');

			// convert the ip address into a number

			ip = ip2int(address);

			// update the CMS counters

			cms->update(ip);
		}

		inputFile.close();
	}
}


/*
 * isHeavyHitter finds an adjacency list with size 1, and returns its indexes d and w in the ICMS
 */

bool isHeavyHitter(ICMS* icms, unsigned int width, short int depth, int hitterSelection, unsigned int& w, short int& d) {
	if (hitterSelection == 1) {

		// first heavy hitter

		for (int i = 0; i < depth; i++)
			for (int j = 0; j < width; j++)
				if (icms->keys[i][j].size() == 1) {
					d = i;
					w = j;

					return true;
				}
	}
	else {
		HeavyHitterEstimate hitter;
		std::vector<HeavyHitterEstimate> hitters;

		// find all the heavy hitters 

		for (int i = 0; i < depth; i++)
			for (int j = 0; j < width; j++)
				if (icms->keys[i][j].size() == 1) {

					hitter.d = i;
					hitter.w = j;
					hitter.estimate = icms->counter(hitter.w, hitter.d);

					hitters.push_back(hitter);

				}

		if (hitters.empty())
			return false;

		if (hitterSelection == 2) {

			// heavy hitter with the highest estimate

			HeavyHitterEstimate max = hitters.front();

			for (auto& h : hitters) 
				if (h.estimate > max.estimate) {
					max.d = h.d;
					max.w = h.w;
					max.estimate = h.estimate;
				}

			d = max.d;
			w = max.w;

			return true;
		}
		else {

			// heavy hitter with the lowest estimate

			HeavyHitterEstimate min = hitters.front();

			for (auto& h : hitters)
				if (h.estimate < min.estimate) {
					min.d = h.d;
					min.w = h.w;
					min.estimate = h.estimate;
				}

			d = min.d;
			w = min.w;

			return true;
		}
	}

	return false;
}


/*
 * sketchIsEmptyAfterPeeling returns true if the sketch is empty and false otherwise
 */

bool sketchIsEmptyAfterPeeling(ICMS* icms, unsigned int width, short int depth) {
	for (int i = 0; i < depth; i++)
		for (int j = 0; j < width; j++)
			if (!icms->keys[i][j].empty())
				return false;

	return true;
}


/*
 * findKey finds the position of the key equal to the input value
 */

int findKey(std::deque<uint32_t> keys, uint32_t value) {
	int p = 0;

	for (auto it = keys.begin(); it != keys.end(); it++) {
		if (value == *it)
			return p;

		p++;
	}

	return -1;
}


/*
 * validateInputFiles validates the files used by the test
 */

bool validateInputFiles(std::string keys) {
	std::ifstream inputFile;

	inputFile.open(keys);

	if (inputFile.is_open())
		inputFile.close();
	else {
		std::cout << "Input file '" << keys << "' not found!\n";

		return false;
	}

	std::stringstream ss;

	ss.str("");
	ss << keys.substr(0, keys.length() - 4) << "_freq.txt";
	std::string frequencies = ss.str();

	inputFile.open(frequencies);

	if (inputFile.is_open())
		inputFile.close();
	else {
		std::cout << "Input file '" << frequencies << "' not found!\n";

		return false;
	}

	return true;
}


/*
 * round round a double number to two decimals
 */

double round(double number) {
	return (double)((int)(number * 100 + 0.5)) / 100;
}


/*
 * testICMS finds the most likely true positive keys of the CMS
 */

void testICMS(unsigned int width, short int depth, int seed, std::string keys, int pMin, int pMax, int hitterSelection) {
	CMS* cms = new CMS(width, depth, seed);

	// initialize the CMS 

	std::cout << "Initializing the CMS \n";

	initializeCMS(cms, width, depth, keys);

	std::cout << "Keys in the CMS " << cms->totalItems() << "\n";
	std::cout << "Calculating P with max size " << pMax << "\n";

	// initialize the set P with the IPs of the IPv4 universe, from 0 to 4294967295 (0.0.0.0 to 255.255.255.255)

	PositiveKey* positives = new PositiveKey[POSITIVES_MAX_SIZE];

	keysWithHighestEstimate(cms, positives);

	// read the real frequencies of the IPs

	int uniqueKeys = uniqueKeysFromTrace(keys);

	KeyFrequency* realFrequencies = new KeyFrequency[uniqueKeys];

	initializeUniqueKeysAndRealFrequencies(realFrequencies, uniqueKeys, keys);
	
	// the array hitters contains the keys selected during the peeling

	PeelingOutput* hitters = new PeelingOutput[POSITIVES_MAX_SIZE];

	// array of counters to initialize the ICMS with the counters of the CMS

	unsigned int* counters = new unsigned int[depth];

	// hashes, keys, estimates, output files 

	std::stringstream ss;
	std::string filename;
	std::ofstream hittersOutputFile, summaryOutputFile;

	PeelingThreshold peeling;

	// create the output file with the summary of the simulation

	ss.str("");
	ss << keys.substr(0, keys.length() - 4) << "_d" << depth << "_w" << width << "_p" << pMin << "_p" << pMax << "_a" << hitterSelection << "_summary.txt";
	filename = ss.str();

	summaryOutputFile.open(filename);

	summaryOutputFile << "%keys\tTP\tFP\tMFP\tMFN \n";

	// initialize the ICMS

	ICMS* icms = new ICMS(width, depth, cms);

	uint64_t hash1, hash2;
	uint32_t key1, key2;

	int totalHitters, truePositives, misclassifiedFalsePositives, misclassifiedFalseNegatives;

	short int d;
	int step = (pMax - pMin) / 24;
	unsigned int w, estimate1, estimate2;

	for (int p = pMin; p <= pMax; p = p + step) {

		// initialize the size of p and the threshold

		peeling = sizeOfPositivesAndThreshold(positives, POSITIVES_MAX_SIZE, p);

		// initialize the ICMS with the keys in P and the counters of the CMS

		std::cout << "\nTesting p = " << p << "\n";
		std::cout << "P contains " << peeling.positives << " keys, the peeling threshold is " << peeling.threshold << "\n";
		std::cout << "Initializing the ICMS \n";

		icms->initialize();

		// each key in P has the counters of the CMS

		for (int i = 0; i < peeling.positives; i++) {
			cms->counters(positives[i].ip, counters);
			icms->update(positives[i].ip, counters);
		}

		std::cout << "Total items in the ICMS " << icms->totalItems() << "\n";

		// peel the ICMS to obtain the set H, which contains the most likely heavy hitters

		std::cout << "Peeling the ICMS with " << peeling.positives << " keys \n";

		totalHitters = 0;

		while (isHeavyHitter(icms, width, depth, hitterSelection, w, d)) {
			if (icms->keys[d][w].size() == 1) {

				// if the estimate is less than the peeling threshold, the key is a false positive (peeling = 0), otherwise, if the key is very likely to be true positive (peeling = 1)

				key1 = icms->keys[d][w].front();

				// get the estimate of the key

				estimate1 = icms->estimate(key1);

				hitters[totalHitters].ip = key1;
				hitters[totalHitters].frequency = frecuencyOfIP(realFrequencies, uniqueKeys, key1);
				hitters[totalHitters].estimate = cms->estimate(key1);
				hitters[totalHitters].peeling = (estimate1 < peeling.threshold) ? '0' : '1';

				totalHitters++;

				// decrement the counters of the key and remove it from the adjacency list

				for (int d1 = 0; d1 < depth; d1++) {
					// calculate the hash (index) in [0, depth - 1]

					hash1 = icms->hash(key1, d1);

					// decrement the counter

					icms->table[d1][hash1] = icms->table[d1][hash1] - estimate1;

					// remove the key from the adjacency list using find, since the position of the key in the list is unknown

					int pos = findKey(icms->keys[d1][hash1], key1);

					if (pos != -1)
						icms->keys[d1][hash1].erase(icms->keys[d1][hash1].begin() + pos);

					// if after the removal the counter is less than the peeling threshold, the remaining keys in the list are false positives (peeling = 0)

					if (icms->table[d1][hash1] < peeling.threshold) {

						// add the keys in the list as false positives and remove them from the list

						while (!icms->keys[d1][hash1].empty()) {

							// add the key as a false positive heavy hitter (peeling = 0)

							key2 = icms->keys[d1][hash1].front();

							// get the estimate of the key

							estimate2 = icms->estimate(key2);

							hitters[totalHitters].ip = key2;
							hitters[totalHitters].frequency = frecuencyOfIP(realFrequencies, uniqueKeys, key2);
							hitters[totalHitters].estimate = cms->estimate(key2);
							hitters[totalHitters].peeling = '0';

							totalHitters++;

							// remove the key from the list

							icms->keys[d1][hash1].pop_front();

							// remove the key from all the adjacency lists

							for (int d2 = 0; d2 < depth; d2++) {

								// calculate the hash (index) in [0, depth - 1]

								hash2 = icms->hash(key2, d2);

								// remove the key from the adjacency list using find, since the position of the key in the list is unknown

								int pos = findKey(icms->keys[d2][hash2], key2);

								if (pos != -1)
									icms->keys[d2][hash2].erase(icms->keys[d2][hash2].begin() + pos);

							}

						}

					}

				}
			}
		}

		// check if the peeling removed all the keys from the sketch

		if (sketchIsEmptyAfterPeeling(icms, width, depth)) {

			// sort the heavy hitters by their real frequency of the key in descending order

			qsort(hitters, peeling.positives, sizeof(PeelingOutput), compareFrequency);

			// hitters output file with heavy hitters, real frequency, cms estimate, and peeling

			ss.str("");
			ss << keys.substr(0, keys.length() - 4) << "_d" << depth << "_w" << width << "_p" << peeling.positives << "_t" << peeling.threshold << "_a" << hitterSelection << ".txt";
			filename = ss.str();

			hittersOutputFile.open(filename);

			hittersOutputFile << "%ip\t\tfreq.\tcms est\tpeeling \n";

			truePositives = 0;
			misclassifiedFalsePositives = 0;
			misclassifiedFalseNegatives = 0;

			for (int i = 0; i < peeling.positives; i++) {
				ss.str("");
				ss << int2ip(hitters[i].ip) << "\t" << hitters[i].frequency << "\t" << hitters[i].estimate << "\t" << hitters[i].peeling;

				if (hitters[i].frequency >= peeling.threshold)
					truePositives++;

				if (hitters[i].frequency < peeling.threshold && hitters[i].peeling == '1')
					misclassifiedFalsePositives++;

				if (hitters[i].frequency >= peeling.threshold && hitters[i].peeling == '0')
					misclassifiedFalseNegatives++;

				hittersOutputFile << ss.str() << "\n";
			}

			hittersOutputFile << "\n%Peeling threshold " << peeling.threshold << ", "
				<< misclassifiedFalsePositives << " MFP of " << peeling.positives << ", " << round((100 * ((double)misclassifiedFalsePositives / (double)peeling.positives))) << "%, "
				<< misclassifiedFalseNegatives << " MFN ";

			hittersOutputFile.close();

			// summary output: keys TP FP MFP MFN (the peeling removed all the keys from the sketch)

			summaryOutputFile << peeling.positives << "\t" << truePositives << "\t" << (peeling.positives - truePositives) << "\t" << misclassifiedFalsePositives << "\t" << misclassifiedFalseNegatives << "\n";
		}
		else {

			// summary output: keys TP FP MFP MFN (the peeling did not remove all the keys from the sketch

			summaryOutputFile << "%" << peeling.positives << "\tnull\tnull\tnull\tnull\n";

			// interrupt the test since the peeling failed

			break;
		}
	}

	summaryOutputFile.close();

	// deallocate the dynamic memory

	delete[] counters;
	delete[] realFrequencies;
	delete[] positives;
	delete[] hitters;

	delete icms;
	delete cms;
}


/*
 * main program
 */

int main(int argc, char** argv) {
	testSetup setup;

	// command line arguments for Inverting Count-Min Sketch
	//
	// w: table width, default value is 16384
	// d: table depth or number of hash tables, default value is 4
	// f: input keys, default value is sj.txt
	// p: minimum value of P, default value is 256
	// i: increment of p, default value is 128
	// s: CMS seed, default value is 45687
	// a: Hitter selection 1:first, 2:highest estimate, 3:lowest estimate, default value is 2
	// 
	// example for the trace sj12: icms f=sj12.txt p=256 i=128

	setup = setupICMSTest(argc, argv);

	showICMSParameters(setup);

	if (validateInputFiles(setup.inputKeys))
		testICMS(setup.width, setup.depth, setup.seed, setup.inputKeys, setup.p, (setup.p + setup.step * 24), setup.algorithm);

	return 0;
}

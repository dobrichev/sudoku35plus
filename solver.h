#ifndef SOLVER_H_INCLUDED

#define SOLVER_H_INCLUDED

#include <stddef.h> //NULL
#include <malloc.h>
#include <algorithm>    // std::sort
#include "ch81.h"
#include "fsss2.h"
#include "rowminlex.h"

//extern unsigned long long solve(const char* in, const unsigned long long maxSolutions);
//extern unsigned long long solve(const int* gridBM, const char* in, const unsigned long long maxSolutions);
//extern unsigned long long solve(const char* const restrict in, const unsigned long long maxSolutions, char* restrict out);

//extern bool solverHasSolutionsByPM(const int* in);
extern int solverPlus1(const char* const restrict in, char* restrict out);
//extern int solverIsIrreducibleByProbing(char *puz, int skipPos = -1);
//extern int solverIsIrreducibleByPM(const char* const restrict puz, const int * const restrict pm, int skipPos = -1);

struct solverPlus {
	static const int maxSol = 350000;
	char (*mainSolBuf)[81];
	subCanoner* sc;
	fsss2* psolver;
	int startSol[82];
	int numGivens;
	solverPlus() {
		sc = new subCanoner();
		psolver = new fsss2();
		mainSolBuf = (char(*)[81])malloc(maxSol * 81);
	}
	~solverPlus() {
		free(mainSolBuf);
		delete psolver;
		delete sc;
	}
	int doPlus(const char* const restrict ms, char* restrict out);

	//for a fixed solution check which combinations of survived 2+ clues lead to minimal unique solutions
	void combineFloating(const char* sol, const bm128& allFloating, const bm128* allUA, const int numUA, int& nSol, const char* in, char* out) {
		int bm2pos[81]; //bit number to puzzle cell number map
		uint32_t ua[500]; //compressed UA
		int numFloating = 0;

		if(numUA > 500) {
			nSol = -10;
			return;
		}

		//compose maps for compression/decompression
		for(int c = 0; c < 81; c++) {
			if(allFloating.isBitSet(c)) {
				bm2pos[numFloating] = c;
				numFloating++;
			}
		}

		if(numFloating > 30) {
			nSol = -11;
			return;
		}

		//compress the UA
		for(int i = 0; i < numUA; i++) {
			register uint32_t theUA = 0;
			for(int c = 0; c < numFloating; c++) {
				if(allUA[i].isBitSet(bm2pos[c])) {
					theUA |= (1 << c);
				}
			}
			ua[i] = theUA;
			//fprintf(stderr,"%d ", theUA);
		}

		uint32_t size = 1 << numFloating;	//the number of combinations
		bool *invalids = new bool[size];
		memset(invalids, 0, size * sizeof(bool));

		//stage 1: check for uniqueness in decreasing clues addition number, mark multiple-solutions puzzles as invalid
		for(int numBits = 2; numBits <= numFloating; numBits++) {
			//loop over all sets of size numBits, mask = bits to add
			for(uint32_t mask = (1 << numBits) - 1; mask < size; mask = nextPerm(mask)) {
				if(invalids[mask]) {
					//this puzzle is a superset of some previously checked unique puzzle
					continue;
				}
				for(int n = 0; n < numUA; n++) {
					if(ua[n] & mask) continue; //hits the UA
					//Multiple solutions, just mark as invalid
					invalids[mask] = true;
					goto next_mask1;
				}
				//unique, mark all supersets as redundant
				{
					uint32_t nmask = (~mask) & (size - 1); //non-givens
					//http://community.topcoder.com/tc?module=Static&d1=tutorials&d2=bitManipulation
					//i = subsets of the non-givens, supersets of the givens
					uint32_t i = nmask;
					do {
						i = (i - 1) & nmask;
						uint32_t larger = (~i) & (size - 1);
						if(!invalids[larger]) {
							invalids[larger] = true;
						}
					} while(i);
					//check whether these locally non-redundant givens don't make some of the fixed givens redundant
					int positions[32];
					int *p = positions;
					for(int i = 0; i < numFloating; i++) {
						if(mask & (1 << i)) { //bits to add as givens
							*p = bm2pos[i];
							p++;
						}
					}
					if(isGloballyMinimal(sol, numBits, positions)) {
						//globally non-redundant minimal is found
						char *res = &out[81 * nSol];
						nSol++;
						if(nSol >= 1000) { //the buffer size
							nSol = -4;
							goto combined;
						}
						memcpy(res, in, 81);
						for(int i = 0; i < numBits; i++) {
							res[positions[i]] = sol[positions[i]];
						}
					}
				}
next_mask1:
				;
			}
		}
combined:
		delete [] invalids;
	}

	static inline uint32_t nextPerm(const uint32_t prev) {
		//http://graphics.stanford.edu/~seander/bithacks.html#NextBitPermutation
		uint32_t lo = prev & ~(prev - 1);	// lowest one bit
		uint32_t lz = (prev + lo) & ~prev;	// lowest zero bit above lo
		uint32_t next = prev | lz;		// add lz to the set
		next &= ~(lz - 1);				// reset bits below lz
		next |= (lz / lo / 2) - 1;		// put back right number of bits at end
		return next;
	}

	//check whether this pair makes some of the initial givens redundant
	bool isGloballyMinimal(const char* sol, const int c1, const int c2) {
		char v1 = sol[c1];
		char v2 = sol[c2];
		for(int testGiven = 0; testGiven < numGivens; testGiven++) {
			for(int testSolution = startSol[testGiven]; testSolution < startSol[testGiven + 1]; testSolution++) {
				if((mainSolBuf[testSolution][c1] == v1) && (mainSolBuf[testSolution][c2] == v2)) {
					//this pair has at least one occurrence in the inverse solutions for the tested initial given
					goto nextTestGiven;
				}
			}
			//pair {c1,c2} makes testGiven redundant because there are no solutions with both values given AND testGiven != its value
			return false;
nextTestGiven:
			;
		}
		return true; //it is minimal
	}
	bool isGloballyMinimal(const char* sol, const int c1, const int c2, const int c3) {
		char v1 = sol[c1];
		char v2 = sol[c2];
		char v3 = sol[c3];
		for(int testGiven = 0; testGiven < numGivens; testGiven++) {
			for(int testSolution = startSol[testGiven]; testSolution < startSol[testGiven + 1]; testSolution++) {
				if((mainSolBuf[testSolution][c1] == v1) && (mainSolBuf[testSolution][c2] == v2) && (mainSolBuf[testSolution][c3] == v3)) {
					goto nextTestGiven;
				}
			}
			return false;
nextTestGiven:
			;
		}
		return true; //it is minimal
	}
	bool isGloballyMinimal(const char* sol, const int c1, const int c2, const int c3, const int c4) {
		char v1 = sol[c1];
		char v2 = sol[c2];
		char v3 = sol[c3];
		char v4 = sol[c4];
		for(int testGiven = 0; testGiven < numGivens; testGiven++) {
			for(int testSolution = startSol[testGiven]; testSolution < startSol[testGiven + 1]; testSolution++) {
				if((mainSolBuf[testSolution][c1] == v1) && (mainSolBuf[testSolution][c2] == v2) && (mainSolBuf[testSolution][c3] == v3) && (mainSolBuf[testSolution][c4] == v4)) {
					goto nextTestGiven;
				}
			}
			return false;
nextTestGiven:
			;
		}
		return true; //it is minimal
	}
	bool isGloballyMinimal(const char* sol, const int numPositions, const int* positions) {
		switch(numPositions) {
		case 0:
			break;
		case 2:
			return isGloballyMinimal(sol, positions[0], positions[1]);
		case 3:
			return isGloballyMinimal(sol, positions[0], positions[1], positions[2]);
		case 4:
			return isGloballyMinimal(sol, positions[0], positions[1], positions[2], positions[3]);
		default:
			for(int testGiven = 0; testGiven < numGivens; testGiven++) {
				for(int testSolution = startSol[testGiven]; testSolution < startSol[testGiven + 1]; testSolution++) {
					for(int pos = 0; pos < numPositions; pos++) {
						if(mainSolBuf[testSolution][positions[pos]] != sol[positions[pos]]) {
							goto nextTestSolution;
						}
					}
					//a testSolution with matched values on all positions is found
					goto nextTestGiven;
nextTestSolution:
					;
				}
				//none of the solutions for the testGiven has exact match on all positions
				return false;
nextTestGiven:
				;
			}
		}
		return true; //it is minimal
	}

};

#endif // SOLVER_H_INCLUDED

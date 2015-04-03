#include "rowminlex.h" //for essentially different solutions
#include "solver.h" //the new solver

const int BitCount[512] =
{
        0,1,1,2,1,2,2,3,1,2,2,3,2,3,3,4,1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,
        1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
        1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
        2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,
        1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
        2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,
        2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,
        3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,4,5,5,6,5,6,6,7,5,6,6,7,6,7,7,8,
        1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
        2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,
        2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,
        3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,4,5,5,6,5,6,6,7,5,6,6,7,6,7,7,8,
        2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,
        3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,4,5,5,6,5,6,6,7,5,6,6,7,6,7,7,8,
        3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,4,5,5,6,5,6,6,7,5,6,6,7,6,7,7,8,
        4,5,5,6,5,6,6,7,5,6,6,7,6,7,7,8,5,6,6,7,6,7,7,8,6,7,7,8,7,8,8,9
};

//Convert mask to a number (1 to 9) if only the appropriate bit is set.
//Convert zero to 9, allowing cellDigit to char conversion using the same table.
//Convert all the rest to zero.
extern const unsigned int Bitmap2Digit[512] =
{
        0,1,2,0,3,0,0,0,4,0,0,0,0,0,0,0,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};


//	if(nSol > 2000) { //...........1..2.34.231546............7241..531.852..46..........872.1.652.674538. = 6164
//		return -2;
//	}
	//........1.....2.3.....14.25...1.6.5...5.47.68.67.85143...6....2.7..28.1.52.471.86 > 20
	//...........1..2..3..2..1.4...312..56.156.7.3.2.6.53.7..572.6...1287.4.656...15... = 22, 1x37
	//...........1..2..3..2..1.4...312..56.165.3.7.2.5.67.3..6.2.5...128.74.655.7.16... = 23, 1x37
	//...........1..2.34.23.45..1.......56..5.643.7.365.741......67...627.8143.784...6. = 23, 1x37
	//...........1..2.34.32.45..1.......56..5.643.7.635.741......67...267.8143.784...6. = 23, 1x37


//#include <stdio.h>

bool bm128BySize(const bm128& a, const bm128& b) {
	int ac = a.popcount_128();
	int bc = b.popcount_128();
	if(ac < bc) return true;
	if(bc < ac) return false;
	uint64_t aa = a.toInt64();
	uint64_t bb = b.toInt64();
	if(aa < bb) return true;
	if(bb < aa) return false;
	aa = a.toInt64_1();
	bb = b.toInt64_1();
	return (aa < bb);
}

int solverPlus::doPlus(const char* const restrict ms, char* restrict out) {
	//fsss2 psolver;
	//const int maxSol = 10000;
	int ret = 0;
	//char sol[maxSol][81]; //all possible additional givens leading to valid solution grids
	int nSol = (int)psolver->solve(ms, maxSol, mainSolBuf[0]);
	if(nSol >= maxSol) {
		return -1;
	}
	char nonGivens[81];
	int numNonGivens = 0;
	unsigned short pencilmarks[81];
	//unsigned short pencilmarksDuplicates[81]; //bits with occurrences in more than one solution
	//unsigned short relaxedPencilmarks[81]; //initial givens with single bit set, the rest = 511
	unsigned short reducedPencilmarks[81]; //only additional bits leading to minimal puzzle, possibly multiple-solution
	char nonSolved[81]; //index of non-solved cells
	int numNonSolved = 0;
	char givensPerDigit[9] = {0,0,0,0,0,0,0,0,0}; //the seventh given for a digit is always redundant
	//find the initial list of non-givens
	for(int j = 0; j < 81; j++) {
		//pencilmarksDuplicates[j] = 0;
		//pencilmarks[j] = Digit2Bitmap[ms[j]];
		pencilmarks[j] = (1 << ms[j]) >> 1;
		if(0 == ms[j]) {
			nonGivens[numNonGivens++] = j;
			//relaxedPencilmarks[j] = 511;
		}
		else {
			givensPerDigit[ms[j] - 1]++;
			//relaxedPencilmarks[j] = pencilmarks[j];
		}
	}
	//compose pencilmarks for non-givens
	//Non-duplicates are actually cell/value pairs making puzzle unique when set alone. (are they of particular interest at all?)
	for(int i = 0; i < nSol; i++) {
		for(int j = 0; j < numNonGivens; j++) {
			register unsigned short thisBitmap = 1 << (mainSolBuf[i][(int)nonGivens[j]] - 1);
			//pencilmarksDuplicates[(int)nonGivens[j]] |= (pencilmarks[(int)nonGivens[j]] & thisBitmap);
			pencilmarks[(int)nonGivens[j]] |= (thisBitmap);
		}
	}
	//compose a list of non-solved cells (those with more than one valid value). They are candidates for new clues.
	for(int j = 0; j < numNonGivens; j++) {
		if(BitCount[pencilmarks[(int)nonGivens[j]]] > 1) {
			nonSolved[numNonSolved++] = nonGivens[j];
		}
	}
	//printf("%d\n",numNonSolved);
	if(numNonSolved > 48)
		return -5;

	startSol[0] = nSol; //store these solutions next to the main givens solutions
	{
		//compose pencilmarks of all candidates for givens and none of the fixed givens
		int xxl[81];
		for(int j = 0; j < 81; j++) {
			xxl[j] = 511;
			if(ms[j]) xxl[j] = 1 << (ms[j] - 1);
		}
		for(int j = 0, k = 0; j < 81; j++) {
			if(ms[j]) {
				//try solving with inverse of this given
				xxl[j] ^= 511;
				int numInverseSolutions = (int)psolver->solve(xxl, maxSol - startSol[k], mainSolBuf[startSol[k]]);
				if(numInverseSolutions == maxSol - startSol[k])
					return -6; //maximum solutions reached, the processing of this partial result leads to wrong results
				startSol[k + 1] = startSol[k] + numInverseSolutions;
				k++;
				xxl[j] ^= 511;
			}
		}
	}

	numGivens = 81 - numNonGivens;

	//find non-redundant candidates
	for(int j = 0; j < numNonSolved; j++) {
		int c = nonSolved[j];
		int pm = pencilmarks[c];
		int reducedCandidates = 0;
		int valueBM;
		while((valueBM = pm & -pm)) { //take the rightmost nonzero bit
			pm ^= valueBM; //clear this bit from the cloned pencilmarks
			char testValue = Bitmap2Digit[valueBM];
			for(int testGiven = 0; testGiven < numGivens; testGiven++) {
				for(int testSolution = startSol[testGiven]; testSolution < startSol[testGiven + 1]; testSolution++) {
					if(mainSolBuf[testSolution][c] == testValue) {
						//this value has at least one occurrence in the inverse solutions for the tested initial given
						goto nextTestGiven;
					}
				}
				//testValue at c makes testGiven redundant because there are no solutions with testValue given AND testGiven != its value
				goto nextValue;
nextTestGiven:
				;
			}
			reducedCandidates |= valueBM; //it doesn't make any given redundant
nextValue:
			;
		}
		reducedPencilmarks[c] = reducedCandidates; //only candidates that lead to minimality
	}

	const int maxUA = 24000;
	if(nSol >= maxUA)
		return -7;
//	...........1.23.45.42.5..31.........1.46..35.6.753.418.........2.63.518441.26.5.3	35	23268
//	...........1.23.45.42.5..31.........1.46..35.6..53.417.........2.63.517441.26.583	35	23268
//	...........1.23.45.42.5..31.........1..63.45.6.457.318.........2.63.51.441.26.583	35	23268
//	...........1.23.45.42.5..31.........1..63.45.6.45..317.........2.63.518441.26.573	35	23268
//	.............12.34.34.56712...........32.814.4.8..132...........8512.46334..85271	35	21784
//	...........1.23.45.42.5..31.........1.46..35.6.753.41..........2.63.518441.26.573	35	19896
//	...........1.23.45.42.5..31.........1..63.45.6.457.31..........2.63.517441.26.583	35	19896
//	...........1..2.34.35.46721...........348.21.1.8.2.34...........862.417331..68452	35	19296
//	.............12.34.34..5612...........32.714.4.7.5132...........7812.45334..78261	35	19284

	bm128 ua[maxUA];
	//here we have a list of non-redundant cell/values which also have to be combined
	for(int sol = 0; sol < nSol; sol++) {
		//compose a set of possible non-redundant additional clues
		int alive[81];
		int nAlive = 0;
		for(int j = 0; j < numNonSolved; j++) {
			int c = nonSolved[j];
			if((1 << (mainSolBuf[sol][c] - 1)) & reducedPencilmarks[c]) {
				alive[nAlive++] = c;
			}
		}
		//compose unavoidable sets (UA)
		int nUA = 0;
		for(int testSol = 0; testSol < nSol; testSol++) {
			if(testSol == sol) continue; //don't compare solution to itself
			bm128 u;
			u.clear();
			for(int j = 0; j < nAlive; j++) {
				int c = alive[j];
				if(mainSolBuf[sol][c] != mainSolBuf[testSol][c])
					u.setBit(c);
			}
			if(u.isZero()) {
				//to distinguish sol from testSol only redundant clues are required
				//abandon sol
				goto nextSolUA;
			}
			ua[nUA++] = u;
		}
		//sort the UA by size
		std::sort(&ua[0], &ua[nUA], bm128BySize);
		//remove all non-minimal UA
		{
			int k = 1;
			for(int i = 1; i < nUA; i++) {
				for(int j = 0; j < k; j++) {
					if(ua[j].isSubsetOf(ua[i])) goto next_minimal_ua; //skip i-th ua
				}
				ua[k++] = ua[i];
next_minimal_ua:
				;
			}
			nUA = k;
		}
		//combine all survived minimal UA
		{
			//first process some trivial cases
			//find clues common to all UA
			bm128 p1 = ua[0];
			bm128 pAll = p1;
			for(int i = 1; i < nUA; i++) {
				p1 &= ua[i];
				pAll |= ua[i];
			}
			if(! p1.isZero()) {
				//export +1 puzzles if any
				for(int c = 0; c < 81; c++) {
					if(p1.isBitSet(c)) {
						char *res = &out[81 * ret];
						ret++;
						if(ret >= 1000) { //the buffer size
							return -4;
						}
						memcpy(res, ms, 81);
						res[c] = mainSolBuf[sol][c];
					}
				}
				if(nUA == 1) continue; // only +1 puzzles there
				//reduce further the candidates
				pAll.clearBits(p1);
				for(int i = 0; i < nUA; i++) {
					//ua[i] &= p1;
					ua[i].clearBits(p1);
				}
				//sort again the UA by size - unnecessary, we removed equal number of common candidates from all UA and therefore the UA order remains valid
				//std::sort(&ua[0], &ua[nUA], bm128BySize);
			}
			//15 38s, 71 37+ in the test case, 207 all, 207 - 71 = 136 36s, OK so far

			//at this point we have
			//1. Chosen solution
			//2. All +1 puzzles exported
			//3. List of non-redundant clues to iterate
			//4. List of unhit minimal UA sets
			//and we must
			//1. Add 2+ clues combinations
			//2. Check for redundancy
			//2.a. whether the whole combination doesn't make any of the fixed givens redundant
			//2.b. whether the added clues don't make each other redundant
			//3. Export the eventual +2, +3, etc. minimal unique puzzles
			combineFloating(mainSolBuf[sol], pAll, ua, nUA, ret, ms, out);
			if(ret < 0) return ret;
		}
nextSolUA:
		;
	}
	return ret;
}


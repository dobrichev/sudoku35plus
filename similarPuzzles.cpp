#include "ch81.h"
#include "solver.h"

#include <malloc.h>

inline void addIt(const ch81& puz, puzzleSet& res, subCanoner& sc) {
	ch81 puzCanon;
	sc.canon(puz.chars, puzCanon.chars);
#ifdef _OPENMP
#pragma omp critical
#endif //_OPENMP
	res.insert(puzCanon);
}

void minus5(const ch81 &src, puzzleSet &res, subCanoner& sc) {
	ch81 puz(src); //structure copy
	int t;
	for(int i = 0; i < 81 - 4; i++) {
		//apply {-1}
		if(0 != (t = puz.chars[i])) {
			puz.chars[i] = 0; //clear the given
			//apply {-2}
			for(int j = i + 1; j < 81 - 3; j++) {
				int tt = puz.chars[j];
				if(tt) {
					puz.chars[j] = 0;
					//apply {-3}
					for(int k = j + 1; k < 81 - 2; k++) {
						int ttt = puz.chars[k];
						if(ttt) {
							puz.chars[k] = 0;
							//apply {-4}
							for(int l = k + 1; l < 81 - 1; l++) {
								int tttt = puz.chars[l];
								if(tttt) {
									puz.chars[l] = 0;
									//apply {-5}
									for(int m = l + 1; m < 81; m++) {
										int ttttt = puz.chars[m];
										if(ttttt) {
											puz.chars[m] = 0;
											/////
											addIt(puz, res, sc);
											/////
											puz.chars[m] = ttttt; // restore the {-5} given
										}
									}
									puz.chars[l] = tttt; // restore the {-4} given
								}
							}
							puz.chars[k] = ttt; // restore the {-3} given
						}
					}
					puz.chars[j] = tt; // restore the {-2} given
				}
			}
			puz.chars[i] = t; //restore the {-1} given
		}
	}
}
void minus4(const ch81 &src, puzzleSet &res, subCanoner& sc) {
	ch81 puz(src); //structure copy
	int t;
	for(int i = 0; i < 81 - 3; i++) {
		//apply {-1}
		if(0 != (t = puz.chars[i])) {
			puz.chars[i] = 0; //clear the given
			//apply {-2}
			for(int j = i + 1; j < 81 - 2; j++) {
				int tt = puz.chars[j];
				if(tt) {
					puz.chars[j] = 0;
					//apply {-3}
					for(int k = j + 1; k < 81 - 1; k++) {
						int ttt = puz.chars[k];
						if(ttt) {
							puz.chars[k] = 0;
							//apply {-4}
							for(int l = k + 1; l < 81; l++) {
								int tttt = puz.chars[l];
								if(tttt) {
									puz.chars[l] = 0;
									/////
									addIt(puz, res, sc);
									/////
									puz.chars[l] = tttt; // restore the {-4} given
								}
							}
							puz.chars[k] = ttt; // restore the {-3} given
						}
					}
					puz.chars[j] = tt; // restore the {-2} given
				}
			}
			puz.chars[i] = t; //restore the {-1} given
		}
	}
}
void minus3(const ch81 &src, puzzleSet &res, subCanoner& sc) {
	ch81 puz(src); //structure copy
	int t;
	for(int i = 0; i < 81 - 2; i++) {
		//apply {-1}
		if(0 != (t = puz.chars[i])) {
			puz.chars[i] = 0; //clear the given
			//apply {-2}
			for(int j = i + 1; j < 81 - 1; j++) {
				int tt = puz.chars[j];
				if(tt) {
					puz.chars[j] = 0;
					//apply {-3}
					for(int k = j + 1; k < 81; k++) {
						int ttt = puz.chars[k];
						if(ttt) {
							puz.chars[k] = 0;
							/////
							addIt(puz, res, sc);
							/////
							puz.chars[k] = ttt; // restore the {-3} given
						}
					}
					puz.chars[j] = tt; // restore the {-2} given
				}
			}
			puz.chars[i] = t; //restore the {-1} given
		}
	}
}
void minus2(const ch81 &src, puzzleSet &res, subCanoner& sc) {
	ch81 puz(src); //structure copy
	int t;
	for(int i = 0; i < 81 - 1; i++) {
		//apply {-1}
		if(0 != (t = puz.chars[i])) {
			puz.chars[i] = 0; //clear the given
			//apply {-2}
			for(int j = i + 1; j < 81; j++) {
				int tt = puz.chars[j];
				if(tt) {
					puz.chars[j] = 0;
					/////
					addIt(puz, res, sc);
					/////
					puz.chars[j] = tt; // restore the {-2} given
				}
			}
			puz.chars[i] = t; //restore the {-1} given
		}
	}
}
void minus1(const ch81 &src, puzzleSet &res, subCanoner& sc) {
	ch81 puz(src); //structure copy
	int t;
	for(int i = 0; i < 81; i++) {
		//apply {-1}
		if(0 != (t = puz.chars[i])) {
			puz.chars[i] = 0; //clear the given
			/////
			addIt(puz, res, sc);
			/////
			puz.chars[i] = t; //restore the {-1} given
		}
	}
}
void minus0(const ch81 &src, puzzleSet &res, subCanoner& sc) {
	ch81 puz(src); //structure copy
	/////
	addIt(puz, res, sc);
	/////
}

void doPlusMinimalUnique(const puzzleSet &todo, puzzleSet& done) {
	const size_t chunk_size = 100000;
	size_t nPuzzles;
	ch81 const ** a = (ch81 const **)malloc(chunk_size * sizeof(ch81*));
	if(a == NULL) {
		fprintf(stderr, "Error allocating memory for %d puzzles\n", (int)chunk_size);
		return;
	}
	puzzleSet::const_iterator p = todo.begin();
	//subCanoner sc;
	solverPlus sp;
	do {
#ifdef _OPENMP
#pragma noparallel
#endif //_OPENMP
		for(nPuzzles = 0; (nPuzzles < chunk_size) && (p != todo.end()); nPuzzles++, p++) {
			a[nPuzzles] = &(*p);
		}
		//suitable for parallelization
#ifdef _OPENMP
#pragma omp parallel for schedule(dynamic, 1) private(sp)
#endif //_OPENMP
		for(size_t i = 0; i < nPuzzles; i++) {
			ch81 p1[1000]; //return max 1000 puzzles
			int p1Size = sp.doPlus(a[i]->chars, p1[0].chars);
			if(p1Size > 0) {
				for(int j = 0; j < p1Size; j++) {
					ch81 pcan;
					sp.sc->canon(p1[j].chars, pcan.chars);
					p1[j] = pcan;
				}
	#ifdef _OPENMP
	#pragma omp critical (ss)
	#endif //_OPENMP
				for(int j = 0; j < p1Size; j++) {
					done.insert(p1[j]);
				}
			}
			else if(p1Size < 0) {
				a[i]->toString(p1[0].chars);
				fprintf(stderr, "%81.81s\tError\t%d\n", p1[0].chars, p1Size);
			}
		}
	} while(p != todo.end());
	free(a);
}

void doMinus35(puzzleSet &src) { //down to 35 givens
	puzzleSet dest;
	size_t nPuzzles = src.size();
	const ch81 ** const index = (const ch81 **)malloc(nPuzzles * sizeof(ch81 *));
	nPuzzles = 0;
	for(puzzleSet::const_iterator p = src.begin(); p != src.end(); p++) {
		index[nPuzzles++] = &(*p);
	}
	//suitable for parallelization
	subCanoner sc;
#ifdef _OPENMP
#pragma omp parallel for schedule(static, 1) private (sc)
#endif //_OPENMP
	for(size_t i = 0; i < nPuzzles; i++) {
		ch81 puzModified = *(index[i]);
		int puzSize = 0;
		for(int j = 0; j < 81; j++) {
			if(puzModified.chars[j]) {
				puzSize++;
			}
		}
		switch (puzSize - 35) {
			case 0:
				minus0(puzModified, dest, sc);
				break;
			case 1:
				minus1(puzModified, dest, sc);
				break;
			case 2:
				minus2(puzModified, dest, sc);
				break;
			case 3:
				minus3(puzModified, dest, sc);
				break;
			case 4:
				minus4(puzModified, dest, sc);
				break;
			case 5:
				minus5(puzModified, dest, sc);
				break;
			default:
				//silently ignore input of size < 35 or > 40
				;
		}
	}
	free(index);
	src.clear();
	src.swap(dest);
}

extern int doMinusPlusByDepth() {
	puzzleSet puzzles, done;
	puzzles.loadFromFile(stdin, false); // don't canonicalize the input at this stage
	doMinus35(puzzles); //apply {-n} down to 35 givens, works for n between 0 and 5
	doPlusMinimalUnique(puzzles, done);
	done.saveToFile(stdout);
	return 0;
}

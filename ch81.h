#ifndef CH81_H_INCLUDED

#define CH81_H_INCLUDED 1

#define _CRT_SECURE_NO_DEPRECATE 1

#include <memory.h>
#include <stdio.h>
#include <vector>
#include <set>
#include <map>
#include "rowminlex.h"

//extern void subcanon(const char* restrict puz, char* restrict can);

using namespace std;

struct ch81 {
	char chars[81];
	bool operator < (const ch81 &rhs) const {
		return (memcmp(this, &rhs, sizeof(ch81)) < 0);
	}
	static int toString(const void* c, char *p) {
		int nClues = 0;
		for(int i = 0; i < 81; i++) {
			if(((const char*)c)[i]) {
				p[i] = ((const char*)c)[i] + '0';
				nClues++;
			}
			else
				p[i] = '.';
		}
		//p[81] = 0;
		return nClues;
	}
	int toString(char *p) const {
		int nClues = 0;
		for(int i = 0; i < 81; i++) {
			if(chars[i]) {
				p[i] = chars[i] + '0';
				nClues++;
			}
			else
				p[i] = '.';
		}
		//p[81] = 0;
		return nClues;
	}
	int fromString(const char *p) {
		int nClues = 81;
		for(int i = 0; i < 81; i++) {
			chars[i] = p[i] - '0';
			if(chars[i] <= 0 || chars[i] > 9) {
				chars[i] = 0;
				nClues--;
			}
		}
		return nClues;
	}
	void clear() {
		memset(chars, 0, 81);
	}
};

struct puzzleSet : public set<ch81> {
	void loadFromFile(FILE *f, const bool canonicalize = true, const int maxSize = 100) {
		char buf[1000];
		puzzleSet::size_type n = 0;
		while(fgets(buf, sizeof(buf), f)) {
			ch81 puz, puzCanon;
			int puzSize = puz.fromString(buf);
			if(puzSize > maxSize)
				continue;
			n++;
			if(canonicalize) {
				subcanon(puz.chars, puzCanon.chars);
				insert(puzCanon);
			}
			else {
				insert(puz);
			}
		}
		n -= size();
		if(n) {
			fprintf(stderr, "%d duplicates found.\n", (int)n);
		}
	}
	void loadFromFile(const char *pFName, const bool canonicalize = true) {
		FILE *pfile;
		if(pFName == NULL)
			return;
		pfile = fopen(pFName, "rt");
		if(pfile == NULL) {
			fprintf(stderr, "error: Can't open file %s", pFName);
			return;
		}
		loadFromFile(pfile, canonicalize);
		fclose(pfile);
	}
	void saveToFile(FILE *pfile) const {
		if(pfile == NULL) {
			return;
		}
		for(const_iterator p = begin(); p != end(); p++) {
			ch81 pText;
			int size = p->toString(pText.chars);
			fprintf(pfile, "%81.81s\t%d\n", pText.chars, size);
		}
	}
	void appendToFile(const char *pFName) const {
		FILE *pfile;
		if(pFName == NULL)
			return;
		pfile = fopen(pFName, "at");
		if(pfile == NULL) {
			fprintf(stderr, "error: Can't open file %s", pFName);
			return;
		}
		saveToFile(pfile);
		fclose(pfile);
	}
	void saveToFile(const char *pFName) const {
		FILE *pfile;
		if(pFName == NULL)
			return;
		pfile = fopen(pFName, "wt");
		if(pfile == NULL) {
			fprintf(stderr, "error: Can't open file %s", pFName);
			return;
		}
		saveToFile(pfile);
		fclose(pfile);
	}
};
//struct puzzleVector : public vector<ch81> {
//	void loadFromFile(FILE *f, int downTo) {
//		char buf[1000];
//		while(fgets(buf, sizeof(buf), f)) {
//			ch81 puz;
//			int puzSize = puz.fromString(buf);
//			push_back(puz);
//		}
//	}
//};

#endif // CH81_H_INCLUDED

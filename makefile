#gnu compiler
GCXX = g++ 

#intel compiler
ICXX = /opt/intel/bin/icc

FILELIST = main.cpp t_128.cpp fsss2.cpp solver.cpp similarPuzzles.cpp subcanon.cpp
TARGET = p35

help:
	@echo 'Usage:'
	@echo 'make { gcc_debug | gcc_profiling | gcc_release | intel_debug | intel_profiling | intel_release | clean }'
	@echo 'Where'
	@echo ' gcc = GNU C++ Compiler g++'
	@echo ' intel = Intel C++ Compiler icc'
	@echo ' debug = non-optimized debug binary'
	@echo ' profiling = non-optimized binary with profiling code injected'
	@echo ' release = optimized binary based on previous run on a test case with profiling code injected'
	@echo ''
	@echo 'Example for comiling to optimized binary using GNU C++ Compiler and test file input.txt:'
	@echo 'make clean'
	@echo 'make gcc_profiling'
	@echo './$(TARGET) < input.txt'
	@echo 'make gcc_release'
	@echo 'make clean'
	@echo './$(TARGET) < input.txt'

gcc_debug:
	@echo 'Building target $(TARGET) using Gnu C++ Debug settings'
	$(GCXX) -O0 -march=native -msse4.2 -mavx -Drestrict=__restrict__ -g -Wall -o $(TARGET) $(FILELIST)
	@echo 'Done'

gcc_profiling:
	@echo 'Building target $(TARGET) using Gnu C++ Profile Generate settings'
	$(GCXX) -O3 -march=native -msse4.2 -mavx -Drestrict=__restrict__ -g -Wall -fprofile-generate -o $(TARGET) $(FILELIST)
	@echo 'Done'

gcc_release:
	@echo 'Building target $(TARGET) using Gnu C++ Profile Use settings'
	$(GCXX) -flto -O3 -march=native -msse4.2 -mavx -Drestrict=__restrict__ -fno-strict-aliasing -g -Wall -fprofile-use -Wno-error=coverage-mismatch -fopenmp -o $(TARGET) $(FILELIST)
	@echo 'Done'

intel_profiling:
	@echo 'Building target $(TARGET) using Intel C++ Debug settings'
	$(ICXX) -O3 -inline-level=2 -prof-gen -I/usr/include/x86_64-linux-gnu/c++/4.8 -use-intel-optimized-headers -restrict -fargument-noalias -alias-const -fno-alias -xAVX -fopenmp -o $(TARGET) $(FILELIST)
	@echo 'Done'

intel_release:
	@echo 'Building target $(TARGET) using Intel C++ Debug settings'
	$(ICXX) -g -O3 -ipo -inline-level=2 -prof-use -I/usr/include/x86_64-linux-gnu/c++/4.8 -use-intel-optimized-headers -restrict -fargument-noalias -alias-const -fno-alias -xAVX -fopenmp -o $(TARGET) $(FILELIST)
	@echo 'Done'

intel_debug:
	@echo 'Building target $(TARGET) using Intel C++ Debug settings'
	$(ICXX) -g -O0 -I/usr/include/x86_64-linux-gnu/c++/4.8 -use-intel-optimized-headers -restrict -fargument-noalias -alias-const -fno-alias -xAVX -o $(TARGET) $(FILELIST)
	@echo 'Done'

clean:
	rm -f *.o *.gcda *.dyn pgopti.*

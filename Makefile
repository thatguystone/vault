#
# Hide annoying messages
#
MAKEFLAGS += --no-print-directory --no-builtin-rules

CXX = clang++
NAME = vault
VERSION = 0.1.0

ifneq (,$(findstring clang++,$(CXX)))
	export GCOV = llvm-cov
endif

PKG_CFG_LIBS = \
	libcrypto \
	libcryptsetup \
	tclap

PKG_CFG_TEST_LIBS = \
	libparatec

BIN = $(NAME)
BIN_TEST= $(BIN).test

OBJECTS = $(patsubst %.cpp, %.o, $(wildcard src/*.cpp))
BOBJECTS = $(filter-out %_test.o, $(OBJECTS))
TOBJECTS = $(patsubst %.o, %.to, $(filter-out %main.o, $(OBJECTS)))

CXXFLAGS = \
	-g \
	-fPIC \
	-Wall \
	-Wextra \
	-Wcast-qual \
	-Wdisabled-optimization \
	-Wformat=2 \
	-Wredundant-decls \
	-Wundef \
	-Wwrite-strings \
	-Werror \
	-fstack-protector \
	--param=ssp-buffer-size=4 \
	-D_FORTIFY_SOURCE=2 \
	-std=gnu++14 \
	-march=native \
	-mfpmath=sse \
	-msse \
	-msse2 \
	-MMD \
	-DVERSION=\"$(VERSION)\" \
	$(shell pkg-config --cflags $(PKG_CFG_LIBS))

CXXFLAGS_RELEASE = \
	-O3 \
	$(CXXFLAGS)

CXXFLAGS_TEST = \
	-O0 \
	--coverage \
	-fno-inline \
	$(CXXFLAGS) \
	$(shell pkg-config --cflags $(PKG_CFG_TEST_LIBS))

LDFLAGS = \
	-rdynamic \
	-pthread \
	$(shell pkg-config --libs $(PKG_CFG_LIBS))

# The following flags only work with GNU's ld
ifneq (,$(findstring GNU ld,$(shell ld -v 2>&1)))
	LDFLAGS += \
		-Wl,-z,now \
		-Wl,-z,relro
endif

LDFLAGS_TEST = \
	$(LDFLAGS) \
	-g \
	--coverage \
	$(shell pkg-config --libs $(PKG_CFG_TEST_LIBS))

CLANG_FORMAT = clang-format

all:
	@echo "Choose one of the following:"
	@echo "    make clean      clean everything"
	@echo "    make format     clang-format the source"
	@echo "    make test       run the test suite"
	@echo "    make vault      build the binary"

clean:
	@rm -f $(BIN)
	@rm -f $(BIN_TEST)
	@rm -f $(OBJECTS)
	@rm -f $(TOBJECTS)
	@rm -f $(OBJECTS:.o=.d)
	@rm -f $(TOBJECTS:.to=.td)
	@rm -f $(OBJECTS:.o=.gcno)
	@rm -f $(OBJECTS:.o=.gcda)
	@sudo rm -rf test_data/

format:
	@$(CLANG_FORMAT) -i \
		$(wildcard src/*.cpp) \
		$(wildcard src/*.h)

test: $(BIN_TEST)
	@# There's a bug with llvm coverage that causes segfaults when forking
	@# with coverage, but only on the first run, before the coverage files are
	@# populated. So do a quick run with nothing rist, then run the actual tests.
	@./$(BIN_TEST) -f __NONE__ > /dev/null

	sudo -E ./$(BIN_TEST)
	gcovr \
		--root=src/ \
		--exclude=.*_test.*

$(BIN): $(BOBJECTS)
	@echo '--- LD $@'
	@$(CXX) -o $@ $^ $(LDFLAGS)

$(BIN_TEST): $(TOBJECTS)
	@echo '--- LD $@'
	@$(CXX) -o $@ $^ $(LDFLAGS_TEST)

.PRECIOUS: %.o
%.o: %.cpp | format
	@echo '--- CXX $@'
	@$(CXX) -c $(CXXFLAGS_RELEASE) -MF $*.d $< -o $@

.PRECIOUS: %.to
%.to: %.cpp | format
	@echo '--- CXX $@'
	@rm -f $*.gcda $*.gcno
	@$(CXX) -c $(CXXFLAGS_TEST) -MF $*.td $< -o $@

ifeq (,$(findstring clean,$(MAKECMDGOALS)))
-include $(OBJECTS:.o=.d)
-include $(TOBJECTS:.to=.td)
endif

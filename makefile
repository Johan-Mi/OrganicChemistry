CXXFLAGS := -Wall -Wextra -Wpedantic

all: learn test
.PHONY: all

compile_flags.txt: makefile
	$(file >$@)
	$(foreach O,$(CXXFLAGS),$(file >>$@,$O))

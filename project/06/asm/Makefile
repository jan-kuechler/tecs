COMMON = ../../common

HEADERS = Code.h Command.h DiagCodes.h Parser.h stdafx.h targetver.h $(COMMON)/Diag.h
OBJECTS = asm.o Code.o DiagCodes.o Parser.o $(COMMON)/Diag.o
TARGET = hack-asm

CPP = g++-4.5
CPPFLAGS = -std=c++0x -DLINUX -I. -I$(COMMON)

LD = g++-4.5
LDFLAGS = 
LIBS = -lboost_system -lboost_program_options -lboost_regex

$(TARGET): $(OBJECTS)
	@$(LD) $(LDFLAGS) -o$(TARGET) $(OBJECTS) $(LIBS)

asm.o: asm.cpp $(HEADERS)
	@$(CPP) $(CPPFLAGS) -o asm.o -c asm.cpp

Code.o: Code.cpp $(HEADERS)
	@$(CPP) $(CPPFLAGS) -o Code.o -c Code.cpp

DiagCodes.o: DiagCodes.cpp $(HEADERS)
	@$(CPP) $(CPPFLAGS) -o DiagCodes.o -c DiagCodes.cpp

Parser.o: Parser.cpp $(HEADERS)
	@$(CPP) $(CPPFLAGS) -o Parser.o -c Parser.cpp

$(COMMON)/Diag.o: $(COMMON)/Diag.cpp $(COMMON)/Diag.h
	@$(CPP) $(CPPFLAGS) -o $(COMMON)/Diag.o -c $(COMMON)/Diag.cpp

.PHONY: clean clean-common clean-all
clean:
	@rm -f *.o

clean-common:
	@rm -f $(COMMON)/*.o

clean-all: clean clean-common
	@rm -f $(TARGET)


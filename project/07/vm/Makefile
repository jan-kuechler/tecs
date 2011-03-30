COMMON = ../../common

HEADERS = CodeGen.h Command.h DiagCodes.h Parser.h stdafx.h targetver.h $(COMMON)/Diag.h
OBJECTS = vm.o CodeGen.o DiagCodes.o Parser.o $(COMMON)/Diag.o
TARGET = hack-vm

CPP = g++-4.5
CPPFLAGS = -std=c++0x -DLINUX -I. -I$(COMMON)

LD = g++-4.5
LDFLAGS = 
LIBS = -lboost_system -lboost_filesystem -lboost_program_options

$(TARGET): $(OBJECTS)
	@$(LD) $(LDFLAGS) -o$(TARGET) $(OBJECTS) $(LIBS)

asm.o: asm.cpp $(HEADERS)
	@$(CPP) $(CPPFLAGS) -o asm.o -c asm.cpp

CodeGen.o: CodeGen.cpp $(HEADERS)
	@$(CPP) $(CPPFLAGS) -o CodeGen.o -c CodeGen.cpp

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


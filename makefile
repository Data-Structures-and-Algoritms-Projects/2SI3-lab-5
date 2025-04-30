CC=g++
CFLAGS=-I. -lm -g -std=c++11
DEPS = *.h
OBJ = objPos.o objPosHashTable.o objPosLinearHashing.o objPosQuadHashing.o objPosDoubleHashing.o Test.o  

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

Lab5: $(OBJ)
	$(CC) -o $@ ${OBJ} $(CFLAGS)

test:	testLab5
testLab5:
	./run.sh

.PHONY: clean

clean:
	$(RM) ${OBJ} *.exe *.xml Lab5

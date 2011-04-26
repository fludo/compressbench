CC=g++
CFLAGS=-W -Wall -ansi -pedantic
LDFLAGS=-lboost_unit_test_framework
EXEC=testHNode testHuffman testBinaryIO testMTF testRLE testBWT testHuffBWT testBWT2 clean
SRC= TestHNode.cpp ../codecs/huffman/HNode.cpp
OBJ= $(SRC:.c=.o)

all: $(EXEC)
testBinaryIO: TestBinIO.o BinaryIO.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

testMTF: TestMTF.o MTF.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

testBWT: TestBWT.o BWT.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)  

testBWT2: TestBWT2.o BWT.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)  

testRLE: TestRLE.o RLE.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

testHuffman:testHuffman.o Huffman.o HNode.o BinaryIO.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

testHNode: testHNode.o HNode.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

testHuffBWT:testHuffBWT.o HuffBWT.o Huffman.o BWT.o BinaryIO.o HNode.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

testHNode.o: TestHNode.cpp HNode.o ../codecs/huffman/HNode.hpp
	$(CC) -c TestHNode.cpp -o testHNode.o

testBWT.o: TestBWT.cpp BWTOld.o ../codecs/BWTOld.hpp
	$(CC) -c TestBWT.cpp -o testBWT.o

testBWT2.o: TestBWT2.cpp BWT.o ../codecs/BWT.hpp
	$(CC) -c TestBWT2.cpp -o testBWT2.o

testHuffman.o: Huffman.o ../codecs/Huffman.hpp BinaryIO.o ../codecs/huffman/BinaryIO.hpp
	$(CC) -c TestHuffman.cpp -o testHuffman.o

testHuffBWT.o: Huffman.o ../codecs/Huffman.hpp BWT.o ../codecs/BWT.hpp
	$(CC) -c TestHuffBWT.cpp -o testHuffBWT.o

testMTF.o: testMTF.cpp MTF.o ../codecs/MTF.hpp
	$(CC) -c testMTF.cpp -o testMTF.o

testRLE.o: testRLE.cpp RLE.o ../codecs/RLE.hpp
	$(CC) -c testRLE.cpp -o testRLE.o

TestBinIO.o: TestBinIO.cpp BinaryIO.o ../codecs/huffman/BinaryIO.hpp
	$(CC) -c TestBinIO.cpp -o TestBinIO.o	

BinaryIO.o: ../codecs/huffman/BinaryIO.cpp ../codecs/huffman/BinaryIO.hpp
	$(CC) -c ../codecs/huffman/BinaryIO.cpp -o BinaryIO.o	

MTF.o: ../codecs/MTF.cpp ../codecs/MTF.hpp
	$(CC) -c ../codecs/MTF.cpp -o MTF.o

BWTOld.o: ../codecs/BWTOld.cpp ../codecs/BWTOld.hpp
	$(CC) $(CFLAGS) -c ../codecs/BWTOld.cpp -o BWTOld.o

BWT.o: ../codecs/BWT.cpp ../codecs/BWT.hpp
	$(CC) $(CFLAGS) -c ../codecs/BWT.cpp -o BWT.o

RLE.o: ../codecs/RLE.cpp ../codecs/RLE.hpp
	$(CC) -c ../codecs/RLE.cpp -o RLE.o

HNode.o:  ../codecs/huffman/HNode.cpp ../codecs/huffman/HNode.hpp
	$(CC) -c ../codecs/huffman/HNode.cpp -o HNode.o

Huffman.o:  ../codecs/Huffman.cpp ../codecs/Huffman.hpp HNode.o BinaryIO.o
	$(CC) -c ../codecs/Huffman.cpp -o Huffman.o


HuffBWT.o: ../codeschemes/HuffBwt.cpp ../codeschemes/HuffBwt.hpp
	$(CC) -c ../codeschemes/HuffBwt.cpp -o HuffBWT.o
#%.o: %.c
#	$(CC) -o $@ -c $< $(CFLAGS)

.PHONY: clean mrproper

clean:
	rm -rf *.o

mrproper: clean
	rm -rf $(EXEC)



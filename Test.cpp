#include "objPos.h"
#include "objPosLinearHashing.h"
#include "objPosQuadHashing.h"
#include "objPosDoubleHashing.h"

#include <cstdlib>
#include <ctime>

#define TABLE_CAPACITY 37				// 36 is a better number, but not prime
#define ELEMENT_COUNT 30
#define TOTAL_ASSERT_COUNT 639
#define TOTAL_TEST_CASES 15
#define TOLERANCE_RANGE 0.005   // +/- 0.5%

#include <iostream>
using namespace std;

objPosHashTable *HTableUnderTest;
int successCount;
int passCount;

void createLinearHashingTable(int);
void createQuadHashingTable(int);
void createDoubleHashingTable(int);
void deleteHashingTable();

objPos generateRandomPos();
char generateRandomChar();

objPos *testArray1, *testArray2;
objPos *LHResult1, *LHResult2;
objPos *QHResult1, *QHResult2;
objPos *DHResult1, *DHResult2;
void initTestArrays();
void initLHResults();
void initQHResults();
void initDHResults();

bool assert_equal(int, int);
bool assert_equal(char, char);
bool assert_equal(bool, bool);
bool assert_equal(double, double);
bool assert_equal(objPos a, objPos b);

// **** Enable This Flag for Verbose Hash Table Printout for Debugging **** //
// **** Enable This Flag for Verbose Hash Table Printout for Debugging **** //
bool verboseMode = false;
// **** Enable This Flag for Verbose Hash Table Printout for Debugging **** //
// **** Enable This Flag for Verbose Hash Table Printout for Debugging **** //


/////////////////////////////////////////////
/////////////////////////////////////////////
////////   LINEAR HASHING TESTERS   /////////
/////////////////////////////////////////////
/////////////////////////////////////////////

// Part I - Constructor
// Required Method - Constructor, Destructor, getLambda

void testLHConstructorGetLambda() {
	
	bool result = true;

	cout << "TEST: testLHConstructorGetLambda" << endl;
	createLinearHashingTable(TABLE_CAPACITY);
	double expected = 0.0;
	double actual = HTableUnderTest->getLambda();	

	result = result & assert_equal(expected, actual);
	deleteHashingTable();
	if(result) passCount++;
}


// // Part II - Insertion
// // Required Method - insert, printMe

void testLHInsertPrintCase1() {
	
	cout << "TEST: testLHInsertPrintCase1" << endl;

	bool result = true;	
	createLinearHashingTable(TABLE_CAPACITY);
	objPos* tableRef = HTableUnderTest->getTableRef();

	for(int i = 0; i < ELEMENT_COUNT; i++)
	{
		HTableUnderTest->insert(testArray1[i]);
	}
	
	if(verboseMode) 
		HTableUnderTest->printMe();
	
	cout << "\t Testing Table Consistency..." << endl;
	for(int i = 0; i < TABLE_CAPACITY; i++)
	{
		result &= assert_equal(LHResult1[i], tableRef[i]);
	}
	
	deleteHashingTable();
	if(result) passCount++;
}

void testLHInsertPrintCase2() {
	
	cout << "TEST: testLHInsertPrintCase2" << endl;

	bool result = true;	
	createLinearHashingTable(TABLE_CAPACITY);
	objPos* tableRef = HTableUnderTest->getTableRef();

	for(int i = 0; i < ELEMENT_COUNT; i++)
	{
		HTableUnderTest->insert(testArray2[i]);
	}
	
	if(verboseMode) 
		HTableUnderTest->printMe();
	
	cout << "\t Testing Table Consistency..." << endl;
	for(int i = 0; i < TABLE_CAPACITY; i++)
	{
		result &= assert_equal(LHResult2[i], tableRef[i]);
	}
	
	deleteHashingTable();
	if(result) passCount++;
}

// // Part II - Removal
// // Required Method - remove, getLambda, isInTable, printMe

#define REMOVAL_COUNT 10
void testLHRemove10GetLambda() {
	
	cout << "TEST: testLHRemove10GetLambda" << endl;
	bool result = true;	
	double expectedLambda, actualLambda;	
	bool bitVec[ELEMENT_COUNT] = {0};  // all false
	
	createLinearHashingTable(TABLE_CAPACITY);
	objPos* tableRef = HTableUnderTest->getTableRef();		
	
	for(int i = 0; i < ELEMENT_COUNT; i++)
	{
		HTableUnderTest->insert(testArray1[i]);
	}
	
	if(verboseMode) HTableUnderTest->printMe();
	
	cout << "\t Testing Lambda Before Removal..." << endl;
	expectedLambda = (double)ELEMENT_COUNT / TABLE_CAPACITY;
	actualLambda = HTableUnderTest->getLambda();	
	result &= assert_equal(expectedLambda, actualLambda);
	
	cout << "\t Randomly Removing " << REMOVAL_COUNT << " Elements..." << endl;
	for(int i = 0, count = REMOVAL_COUNT; count > 0; i = (i + rand() % 10) % ELEMENT_COUNT)
	{
		if(!bitVec[i])
		{
			bitVec[i] = true;
			HTableUnderTest->remove(testArray1[i]);
			count--;

			for(int j = 0; j < TABLE_CAPACITY; j++)
			{
				if(LHResult1[j].getPF() == testArray1[i].getPF() &&
				   LHResult1[j].getNum() == testArray1[i].getNum())
				   {
						LHResult1[j].setSym(0);
						break;
				   }					
			}
		}
	}
	
	if(verboseMode) 
	{
		cout << endl << endl << "[After Removal]" << endl;
		HTableUnderTest->printMe();
	}
	
	cout << "\t Testing Table Consistency using Brute Force..." << endl;
	for(int i = 0; i < TABLE_CAPACITY; i++)
	{
		result &= assert_equal(LHResult1[i], tableRef[i]);
	}

	cout << "\t Testing Table Consistency using isInTable()..." << endl;
	for(int i = 0; i < ELEMENT_COUNT; i++)
	{
		bool expected = false;
		for(int j = 0; j < TABLE_CAPACITY; j++)
		{
			if(LHResult1[j].getPF() == testArray1[i].getPF() &&
			   LHResult1[j].getNum() == testArray1[i].getNum() &&
			   LHResult1[j].getSym() == 'v')
			{
				expected = true;
				break;
			}					
		}

		result &= assert_equal(HTableUnderTest->isInTable(testArray1[i]), 
							   expected);
	}

	cout << "\t Testing Lambda After Removal..." << endl;
	expectedLambda = (double)(ELEMENT_COUNT - REMOVAL_COUNT) / TABLE_CAPACITY;
	actualLambda = HTableUnderTest->getLambda();	
	result = result & assert_equal(expectedLambda, actualLambda);
		
	deleteHashingTable();
	if(result) passCount++;
}


#define REMOVAL_COUNT2 15
void testLHRemove15GetLambda() {
	
	cout << "TEST: testLHRemove15GetLambda" << endl;
	bool result = true;	
	double expectedLambda, actualLambda;	
	bool bitVec[ELEMENT_COUNT] = {0};  // all false
	
	createLinearHashingTable(TABLE_CAPACITY);
	objPos* tableRef = HTableUnderTest->getTableRef();		
	
	for(int i = 0; i < ELEMENT_COUNT; i++)
	{
		HTableUnderTest->insert(testArray2[i]);
	}
	
	if(verboseMode) HTableUnderTest->printMe();
	
	cout << "\t Testing Lambda Before Removal..." << endl;
	expectedLambda = (double)ELEMENT_COUNT / TABLE_CAPACITY;
	actualLambda = HTableUnderTest->getLambda();	
	result &= assert_equal(expectedLambda, actualLambda);
	
	cout << "\t Randomly Removing " << REMOVAL_COUNT << " Elements..." << endl;
	for(int i = 0, count = REMOVAL_COUNT2; count > 0; i = (i + rand() % 10) % ELEMENT_COUNT)
	{
		if(!bitVec[i])
		{
			bitVec[i] = true;
			HTableUnderTest->remove(testArray2[i]);
			count--;

			for(int j = 0; j < TABLE_CAPACITY; j++)
			{
				if(LHResult2[j].getPF() == testArray2[i].getPF() &&
				   LHResult2[j].getNum() == testArray2[i].getNum())
				   {
					    LHResult2[j].setSym(0);
						break;
				   }					
			}
		}
	}
	
	if(verboseMode) 
	{
		cout << endl << endl << "[After Removal]" << endl;
		HTableUnderTest->printMe();
	}
	
	cout << "\t Testing Table Consistency using Brute Force..." << endl;
	for(int i = 0; i < TABLE_CAPACITY; i++)
	{
		result &= assert_equal(LHResult2[i], tableRef[i]);
	}

	cout << "\t Testing Table Consistency using isInTable()..." << endl;
	for(int i = 0; i < ELEMENT_COUNT; i++)
	{
		bool expected = false;
		for(int j = 0; j < TABLE_CAPACITY; j++)
		{
			if(LHResult2[j].getPF() == testArray2[i].getPF() &&
			   LHResult2[j].getNum() == testArray2[i].getNum() &&
			   LHResult2[j].getSym() == 'v')
			{
				expected = true;
				break;
			}					
		}

		result &= assert_equal(HTableUnderTest->isInTable(testArray2[i]), 
							   expected);
	}

	cout << "\t Testing Lambda After Removal..." << endl;
	expectedLambda = (double)(ELEMENT_COUNT - REMOVAL_COUNT2) / TABLE_CAPACITY;
	actualLambda = HTableUnderTest->getLambda();	
	result = result & assert_equal(expectedLambda, actualLambda);
		
	deleteHashingTable();
	if(result) passCount++;
}


// ////////////////////////////////////////////////
// ////////////////////////////////////////////////
// ////////   QUADRATIC HASHING TESTERS   /////////
// ////////////////////////////////////////////////
// ////////////////////////////////////////////////

// Part I - Constructor
// Required Method - Constructor, Destructor, getLambda

void testQHConstructorGetLambda() {
	
	bool result = true;

	cout << "TEST: testQHConstructorGetLambda" << endl;
	createQuadHashingTable(TABLE_CAPACITY);
	double expected = 0.0;
	double actual = HTableUnderTest->getLambda();	

	result = result & assert_equal(expected, actual);
	deleteHashingTable();
	if(result) passCount++;
}


// // Part II - Insertion
// // Required Method - insert, printMe

void testQHInsertPrintCase1() {
	
	cout << "TEST: testQHInsertPrintCase1" << endl;

	bool result = true;	
	createQuadHashingTable(TABLE_CAPACITY);
	objPos* tableRef = HTableUnderTest->getTableRef();

	for(int i = 0; i < ELEMENT_COUNT; i++)
	{
		HTableUnderTest->insert(testArray1[i]);
	}
	
	if(verboseMode) HTableUnderTest->printMe();
	
	cout << "\t Testing Table Consistency..." << endl;
	for(int i = 0; i < TABLE_CAPACITY; i++)
	{
		result &= assert_equal(QHResult1[i], tableRef[i]);
	}
	
	deleteHashingTable();
	if(result) passCount++;
}

void testQHInsertPrintCase2() {
	
	cout << "TEST: testQHInsertPrintCase2" << endl;

	bool result = true;	
	createQuadHashingTable(TABLE_CAPACITY);
	objPos* tableRef = HTableUnderTest->getTableRef();

	for(int i = 0; i < ELEMENT_COUNT; i++)
	{
		HTableUnderTest->insert(testArray2[i]);
	}
	
	if(verboseMode) HTableUnderTest->printMe();
	
	cout << "\t Testing Table Consistency..." << endl;
	for(int i = 0; i < TABLE_CAPACITY; i++)
	{
		result &= assert_equal(QHResult2[i], tableRef[i]);
	}
	
	deleteHashingTable();
	if(result) passCount++;
}


// // Part II - Removal
// // Required Method - remove, getLambda, isInTable, printMe

#define REMOVAL_COUNT 10
void testQHRemove10GetLambda() {
	
	cout << "TEST: testQHRemove10GetLambda" << endl;
	bool result = true;	
	double expectedLambda, actualLambda;	
	bool bitVec[ELEMENT_COUNT] = {0};  // all false
	
	createQuadHashingTable(TABLE_CAPACITY);
	objPos* tableRef = HTableUnderTest->getTableRef();		
	
	for(int i = 0; i < ELEMENT_COUNT; i++)
	{
		HTableUnderTest->insert(testArray1[i]);
	}
	
	if(verboseMode) HTableUnderTest->printMe();
	
	cout << "\t Testing Lambda Before Removal..." << endl;
	expectedLambda = (double)ELEMENT_COUNT / TABLE_CAPACITY;
	actualLambda = HTableUnderTest->getLambda();	
	result &= assert_equal(expectedLambda, actualLambda);
	
	cout << "\t Randomly Removing " << REMOVAL_COUNT << " Elements..." << endl;
	for(int i = 0, count = REMOVAL_COUNT; count > 0; i = (i + rand() % 10) % ELEMENT_COUNT)
	{
		if(!bitVec[i])
		{
			bitVec[i] = true;
			HTableUnderTest->remove(testArray1[i]);
			count--;

			for(int j = 0; j < TABLE_CAPACITY; j++)
			{
				if(QHResult1[j].getPF() == testArray1[i].getPF() &&
				   QHResult1[j].getNum() == testArray1[i].getNum())
				   {
						QHResult1[j].setSym(0);
						break;
				   }					
			}
		}
	}
	
	if(verboseMode) 
	{
		cout << endl << endl << "[After Removal]" << endl;
		HTableUnderTest->printMe();
	}
	
	cout << "\t Testing Table Consistency using Brute Force..." << endl;
	for(int i = 0; i < TABLE_CAPACITY; i++)
	{
		result &= assert_equal(QHResult1[i], tableRef[i]);
	}

	cout << "\t Testing Table Consistency using isInTable()..." << endl;
	for(int i = 0; i < ELEMENT_COUNT; i++)
	{
		bool expected = false;
		for(int j = 0; j < TABLE_CAPACITY; j++)
		{
			if(QHResult1[j].getPF() == testArray1[i].getPF() &&
			   QHResult1[j].getNum() == testArray1[i].getNum() &&
			   QHResult1[j].getSym() == 'v')
			{
				expected = true;
				break;
			}					
		}

		result &= assert_equal(HTableUnderTest->isInTable(testArray1[i]), 
							   expected);
	}

	cout << "\t Testing Lambda After Removal..." << endl;
	expectedLambda = (double)(ELEMENT_COUNT - REMOVAL_COUNT) / TABLE_CAPACITY;
	actualLambda = HTableUnderTest->getLambda();	
	result = result & assert_equal(expectedLambda, actualLambda);
		
	deleteHashingTable();
	if(result) passCount++;
}


#define REMOVAL_COUNT2 15
void testQHRemove15GetLambda() {
	
	cout << "TEST: testQHRemove15GetLambda" << endl;
	bool result = true;	
	double expectedLambda, actualLambda;	
	bool bitVec[ELEMENT_COUNT] = {0};  // all false
	
	createQuadHashingTable(TABLE_CAPACITY);
	objPos* tableRef = HTableUnderTest->getTableRef();		
	
	for(int i = 0; i < ELEMENT_COUNT; i++)
	{
		HTableUnderTest->insert(testArray2[i]);
	}
	
	if(verboseMode) HTableUnderTest->printMe();
	
	cout << "\t Testing Lambda Before Removal..." << endl;
	expectedLambda = (double)ELEMENT_COUNT / TABLE_CAPACITY;
	actualLambda = HTableUnderTest->getLambda();	
	result &= assert_equal(expectedLambda, actualLambda);
	
	cout << "\t Randomly Removing " << REMOVAL_COUNT << " Elements..." << endl;
	for(int i = 0, count = REMOVAL_COUNT2; count > 0; i = (i + rand() % 10) % ELEMENT_COUNT)
	{
		if(!bitVec[i])
		{
			bitVec[i] = true;
			HTableUnderTest->remove(testArray2[i]);
			count--;

			for(int j = 0; j < TABLE_CAPACITY; j++)
			{
				if(QHResult2[j].getPF() == testArray2[i].getPF() &&
				   QHResult2[j].getNum() == testArray2[i].getNum())
				   {
						QHResult2[j].setSym(0);
						break;
				   }					
			}
		}
	}
	
	if(verboseMode) 
	{
		cout << endl << endl << "[After Removal]" << endl;
		HTableUnderTest->printMe();
	}
	
	cout << "\t Testing Table Consistency using Brute Force..." << endl;
	for(int i = 0; i < TABLE_CAPACITY; i++)
	{
		result &= assert_equal(QHResult2[i], tableRef[i]);
	}

	cout << "\t Testing Table Consistency using isInTable()..." << endl;
	for(int i = 0; i < ELEMENT_COUNT; i++)
	{
		bool expected = false;
		for(int j = 0; j < TABLE_CAPACITY; j++)
		{
			if(QHResult2[j].getPF() == testArray2[i].getPF() &&
			   QHResult2[j].getNum() == testArray2[i].getNum() &&
			   QHResult2[j].getSym() == 'v')
			{
				expected = true;
				break;
			}					
		}

		result &= assert_equal(HTableUnderTest->isInTable(testArray2[i]), 
							   expected);
	}

	cout << "\t Testing Lambda After Removal..." << endl;
	expectedLambda = (double)(ELEMENT_COUNT - REMOVAL_COUNT2) / TABLE_CAPACITY;
	actualLambda = HTableUnderTest->getLambda();	
	result = result & assert_equal(expectedLambda, actualLambda);
		
	deleteHashingTable();
	if(result) passCount++;
}



// /////////////////////////////////////////////
// /////////////////////////////////////////////
// ////////   DOUBLE PROBING TESTERS   /////////
// /////////////////////////////////////////////
// /////////////////////////////////////////////

void testDHConstructorGetLambda() {
	
	bool result = true;

	cout << "TEST: testDHConstructorGetLambda" << endl;
	createDoubleHashingTable(TABLE_CAPACITY);
	double expected = 0.0;
	double actual = HTableUnderTest->getLambda();	

	result = result & assert_equal(expected, actual);
	deleteHashingTable();
	if(result) passCount++;
}


// // Part II - Insertion
// // Required Method - insert, printMe

void testDHInsertPrintCase1() {
	
	cout << "TEST: testDHInsertPrintCase1" << endl;

	bool result = true;	
	createDoubleHashingTable(TABLE_CAPACITY);
	objPos* tableRef = HTableUnderTest->getTableRef();

	for(int i = 0; i < ELEMENT_COUNT; i++)
	{
		HTableUnderTest->insert(testArray1[i]);
	}
	
	if(verboseMode) 
		HTableUnderTest->printMe();
	
	cout << "\t Testing Table Consistency..." << endl;
	for(int i = 0; i < TABLE_CAPACITY; i++)
	{
		result &= assert_equal(DHResult1[i], tableRef[i]);
	}
	
	deleteHashingTable();
	if(result) passCount++;
}

void testDHInsertPrintCase2() {
	
	cout << "TEST: testDHInsertPrintCase2" << endl;

	bool result = true;	
	createDoubleHashingTable(TABLE_CAPACITY);
	objPos* tableRef = HTableUnderTest->getTableRef();

	for(int i = 0; i < ELEMENT_COUNT; i++)
	{
		HTableUnderTest->insert(testArray2[i]);
	}
	
	if(verboseMode) 
		HTableUnderTest->printMe();
	
	cout << "\t Testing Table Consistency..." << endl;
	for(int i = 0; i < TABLE_CAPACITY; i++)
	{
		result &= assert_equal(DHResult2[i], tableRef[i]);
	}
	
	deleteHashingTable();
	if(result) passCount++;
}

// // Part II - Removal
// // Required Method - remove, getLambda, isInTable, printMe

#define REMOVAL_COUNT 10
void testDHRemove10GetLambda() {
	
	cout << "TEST: testDHRemove10GetLambda" << endl;
	bool result = true;	
	double expectedLambda, actualLambda;	
	bool bitVec[ELEMENT_COUNT] = {0};  // all false
	
	createDoubleHashingTable(TABLE_CAPACITY);
	objPos* tableRef = HTableUnderTest->getTableRef();		
	
	for(int i = 0; i < ELEMENT_COUNT; i++)
	{
		HTableUnderTest->insert(testArray1[i]);
	}
	
	if(verboseMode) HTableUnderTest->printMe();
	
	cout << "\t Testing Lambda Before Removal..." << endl;
	expectedLambda = (double)ELEMENT_COUNT / TABLE_CAPACITY;
	actualLambda = HTableUnderTest->getLambda();	
	result &= assert_equal(expectedLambda, actualLambda);
	
	cout << "\t Randomly Removing " << REMOVAL_COUNT << " Elements..." << endl;
	for(int i = 0, count = REMOVAL_COUNT; count > 0; i = (i + rand() % 10) % ELEMENT_COUNT)
	{
		if(!bitVec[i])
		{
			bitVec[i] = true;
			HTableUnderTest->remove(testArray1[i]);
			count--;

			for(int j = 0; j < TABLE_CAPACITY; j++)
			{
				if(DHResult1[j].getPF() == testArray1[i].getPF() &&
				   DHResult1[j].getNum() == testArray1[i].getNum())
				   {
					    DHResult1[j].setSym(0);
						break;
				   }					
			}
		}
	}
	
	if(verboseMode) 
	{
		cout << endl << endl << "[After Removal]" << endl;
		HTableUnderTest->printMe();
	}
	
	cout << "\t Testing Table Consistency using Brute Force..." << endl;
	for(int i = 0; i < TABLE_CAPACITY; i++)
	{
		result &= assert_equal(DHResult1[i], tableRef[i]);
	}

	cout << "\t Testing Table Consistency using isInTable()..." << endl;
	for(int i = 0; i < ELEMENT_COUNT; i++)
	{
		bool expected = false;
		for(int j = 0; j < TABLE_CAPACITY; j++)
		{
			if(DHResult1[j].getPF() == testArray1[i].getPF() &&
			   DHResult1[j].getNum() == testArray1[i].getNum() &&
			   DHResult1[j].getSym() == 'v')
			{
				expected = true;
				break;
			}					
		}

		result &= assert_equal(HTableUnderTest->isInTable(testArray1[i]), 
							   expected);
	}

	cout << "\t Testing Lambda After Removal..." << endl;
	expectedLambda = (double)(ELEMENT_COUNT - REMOVAL_COUNT) / TABLE_CAPACITY;
	actualLambda = HTableUnderTest->getLambda();	
	result = result & assert_equal(expectedLambda, actualLambda);
		
	deleteHashingTable();
	if(result) passCount++;
}


#define REMOVAL_COUNT2 15
void testDHRemove15GetLambda() {
	
	cout << "TEST: testDHRemove15GetLambda" << endl;
	bool result = true;	
	double expectedLambda, actualLambda;	
	bool bitVec[ELEMENT_COUNT] = {0};  // all false
	
	createDoubleHashingTable(TABLE_CAPACITY);
	objPos* tableRef = HTableUnderTest->getTableRef();		
	
	for(int i = 0; i < ELEMENT_COUNT; i++)
	{
		HTableUnderTest->insert(testArray2[i]);
	}
	
	if(verboseMode) HTableUnderTest->printMe();
	
	cout << "\t Testing Lambda Before Removal..." << endl;
	expectedLambda = (double)ELEMENT_COUNT / TABLE_CAPACITY;
	actualLambda = HTableUnderTest->getLambda();	
	result &= assert_equal(expectedLambda, actualLambda);
	
	cout << "\t Randomly Removing " << REMOVAL_COUNT << " Elements..." << endl;
	for(int i = 0, count = REMOVAL_COUNT2; count > 0; i = (i + rand() % 10) % ELEMENT_COUNT)
	{
		if(!bitVec[i])
		{
			bitVec[i] = true;
			HTableUnderTest->remove(testArray2[i]);
			count--;

			for(int j = 0; j < TABLE_CAPACITY; j++)
			{
				if(DHResult2[j].getPF() == testArray2[i].getPF() &&
				   DHResult2[j].getNum() == testArray2[i].getNum())
				   {
					    DHResult2[j].setSym(0);
						break;
				   }					
			}
		}
	}
	
	if(verboseMode) 
	{
		cout << endl << endl << "[After Removal]" << endl;
		HTableUnderTest->printMe();
	}
	
	cout << "\t Testing Table Consistency using Brute Force..." << endl;
	for(int i = 0; i < TABLE_CAPACITY; i++)
	{
		result &= assert_equal(DHResult2[i], tableRef[i]);
	}

	cout << "\t Testing Table Consistency using isInTable()..." << endl;
	for(int i = 0; i < ELEMENT_COUNT; i++)
	{
		bool expected = false;
		for(int j = 0; j < TABLE_CAPACITY; j++)
		{
			if(DHResult2[j].getPF() == testArray2[i].getPF() &&
			   DHResult2[j].getNum() == testArray2[i].getNum() &&
			   DHResult2[j].getSym() == 'v')
			{
				expected = true;
				break;
			}					
		}

		result &= assert_equal(HTableUnderTest->isInTable(testArray2[i]), 
							   expected);
	}

	cout << "\t Testing Lambda After Removal..." << endl;
	expectedLambda = (double)(ELEMENT_COUNT - REMOVAL_COUNT2) / TABLE_CAPACITY;
	actualLambda = HTableUnderTest->getLambda();	
	result = result & assert_equal(expectedLambda, actualLambda);
		
	deleteHashingTable();
	if(result) passCount++;
}

// ///////////////////////////////////////
// ///////////////////////////////////////
// ////////   TEST BENCH BELOW   /////////
// ///////////////////////////////////////
// ///////////////////////////////////////

bool runAllTests(int argc, char const *argv[]) {

	cout << "[Setting Up Models]" << endl;
	initTestArrays();	
	initLHResults();
	initQHResults();
	initDHResults();
	
	cout << "[LINEAR PROBING HASH TABLE TESTER STARTED]" << endl;
	testLHConstructorGetLambda();
	testLHInsertPrintCase1();
	testLHInsertPrintCase2();
	testLHRemove10GetLambda();
	testLHRemove15GetLambda();
	cout << "[LINEAR PROBING HASH TABLE TESTER ENDED]" << endl;


	cout << "[QUADRATIC PROBING HASH TABLE TESTER STARTED]" << endl;
    testQHConstructorGetLambda();
	testQHInsertPrintCase1();
	testQHInsertPrintCase2();
	testQHRemove10GetLambda();
	testQHRemove15GetLambda();
	cout << "[QUADRATIC PROBING HASH TABLE TESTER ENDED]" << endl << endl;
	
	cout << "[DOUBLE HASHING HASH TABLE TESTER STARTED]" << endl;
	testDHConstructorGetLambda();
	testDHInsertPrintCase1();
	testDHInsertPrintCase2();
	testDHRemove10GetLambda();
	testDHRemove15GetLambda();
	cout << "[DOUBLE HASHING HASH TABLE TESTER ENDED]" << endl;


	if(successCount != TOTAL_ASSERT_COUNT) 
		return false;
	else 
		return true;
}

int main(int argc, char const *argv[]) {
  	
	srand(time(NULL));
	successCount = 0;
	passCount = 0;
	bool successCode = runAllTests(argc, argv);
	if(successCode)	cout << endl << "Passed All Tests" << endl;
	else			cout << endl << "!! Failed Tests, Check Failure !!" << endl;	
	cout << "Assertion Score: " << successCount << " / " << TOTAL_ASSERT_COUNT << endl;	
	cout << "Test Case Score: " << passCount << " / " << TOTAL_TEST_CASES << endl;	

	return successCode? EXIT_SUCCESS : EXIT_FAILURE;
}

void createLinearHashingTable(int size)
{
	deleteHashingTable();
	HTableUnderTest = new objPosLinearHashing(size);
}

void createQuadHashingTable(int size)
{
	deleteHashingTable();
	HTableUnderTest = new objPosQuadHashing(size);
}

void createDoubleHashingTable(int size)
{
	deleteHashingTable();
	HTableUnderTest = new objPosDoubleHashing(size);
}

void deleteHashingTable()
{
	if(HTableUnderTest != NULL)
	{
		delete HTableUnderTest;
		HTableUnderTest = NULL;
	}
	
}


objPos generateRandomPos()
{
	objPos temp;

	temp.setX(rand() % 100);
	temp.setY(rand() % 100);
	temp.setNum(rand() % 100);
	temp.setPF(rand() % 26 + 'A');
	temp.setSym(0);

	return temp;
}

char generateRandomChar()
{
	char candidate;

	if(rand() % 2 == 0)	candidate = rand() % 26 + 'a';
	else				candidate = rand() % 26 + 'A';

	return candidate;
}


bool assert_equal(int a, int b)
{
	bool result = (a == b);
	if(result)	
		successCount++;
	else
	{
		cout << "\t\t[ASSERTION] Expected: " << a << ", but Actual: " << b << endl;		
	}
	return result;
}

bool assert_equal(char a, char b)
{
	bool result = (a == b);
	if(result)	
		successCount++;
	else
	{
		cout << "\t\tExpected: " << a << ", but Actual: " << b << endl;		
	}
	return result;
}

bool assert_equal(bool a, bool b)
{
	bool result = (a == b);
	if(result)	
		successCount++;
	else
	{
		if(a)
			cout << "\t\t[ASSERTION] Expected: TRUE, but Actual: FALSE" << endl;		
		else
			cout << "\t\t[ASSERTION] Expected: FALSE, but Actual: TRUE" << endl;		
	}
	return result;
}

bool assert_equal(double a, double b)
{
	bool result = ((a <= (b+TOLERANCE_RANGE)) && (a >= (b-TOLERANCE_RANGE)));
	if(result)	
		successCount++;
	else
		cout << "\t\tExpected: " << a << ", but Actual: " << b << endl;		
	
	return result;
}

bool assert_equal(objPos a, objPos b)
{
	bool result = (a.getNum() == b.getNum());
	result &= (a.getPF() == b.getPF());
	result &= (a.getSym() == b.getSym());
	result &= (a.getX() == b.getX());
	result &= (a.getY() == b.getY());

	if(result)	
	{
		successCount++;
	}
	else
	{
		cout << "\t\tExpected: ";
		a.printObjPos();
		cout << "\t\tBut Actual: ";		
		b.printObjPos();
	}
	return result;
}



void initTestArrays()
{
	int size = ELEMENT_COUNT;

	testArray1 = new objPos[size];
	testArray1[0].setPF('A');  testArray1[0].setNum(25);
	testArray1[1].setPF('c');  testArray1[1].setNum(42);
	testArray1[2].setPF('K');  testArray1[2].setNum(61);
	testArray1[3].setPF('m');  testArray1[3].setNum(13);
	testArray1[4].setPF('j');  testArray1[4].setNum(4);
	testArray1[5].setPF('z');  testArray1[5].setNum(22);
	testArray1[6].setPF('F');  testArray1[6].setNum(6);
	testArray1[7].setPF('o');  testArray1[7].setNum(75);
	testArray1[8].setPF('P');  testArray1[8].setNum(54);
	testArray1[9].setPF('p');  testArray1[9].setNum(28);
	testArray1[10].setPF('G'); testArray1[10].setNum(59);
	testArray1[11].setPF('i'); testArray1[11].setNum(18);
	testArray1[12].setPF('H'); testArray1[12].setNum(35);
	testArray1[13].setPF('R'); testArray1[13].setNum(49);
	testArray1[14].setPF('d'); testArray1[14].setNum(94);
	testArray1[15].setPF('M'); testArray1[15].setNum(61);
	testArray1[16].setPF('C'); testArray1[16].setNum(42);
	testArray1[17].setPF('O'); testArray1[17].setNum(1);
	testArray1[18].setPF('u'); testArray1[18].setNum(14);
	testArray1[19].setPF('J'); testArray1[19].setNum(76);	
	testArray1[20].setPF('T'); testArray1[20].setNum(66);
	testArray1[21].setPF('a'); testArray1[21].setNum(18);
	testArray1[22].setPF('E'); testArray1[22].setNum(9);
	testArray1[23].setPF('w'); testArray1[23].setNum(51);
	testArray1[24].setPF('V'); testArray1[24].setNum(98);
	testArray1[25].setPF('q'); testArray1[25].setNum(27);
	testArray1[26].setPF('s'); testArray1[26].setNum(37);
	testArray1[27].setPF('S'); testArray1[27].setNum(81);
	testArray1[28].setPF('U'); testArray1[28].setNum(5);
	testArray1[29].setPF('B'); testArray1[29].setNum(77);

	testArray2 = new objPos[size];
	testArray2[0].setPF('G');  testArray2[0].setNum(10);
	testArray2[1].setPF('h');  testArray2[1].setNum(11);
	testArray2[2].setPF('W');  testArray2[2].setNum(3);
	testArray2[3].setPF('Q');  testArray2[3].setNum(83);
	testArray2[4].setPF('z');  testArray2[4].setNum(46);
	testArray2[5].setPF('N');  testArray2[5].setNum(71);
	testArray2[6].setPF('y');  testArray2[6].setNum(24);
	testArray2[7].setPF('J');  testArray2[7].setNum(48);
	testArray2[8].setPF('j');  testArray2[8].setNum(49);
	testArray2[9].setPF('q');  testArray2[9].setNum(11);
	testArray2[10].setPF('T'); testArray2[10].setNum(35);
	testArray2[11].setPF('r'); testArray2[11].setNum(79);
	testArray2[12].setPF('p'); testArray2[12].setNum(91);
	testArray2[13].setPF('R'); testArray2[13].setNum(5);
	testArray2[14].setPF('S'); testArray2[14].setNum(66);
	testArray2[15].setPF('d'); testArray2[15].setNum(42);
	testArray2[16].setPF('I'); testArray2[16].setNum(12);
	testArray2[17].setPF('n'); testArray2[17].setNum(57);
	testArray2[18].setPF('l'); testArray2[18].setNum(33);
	testArray2[19].setPF('n'); testArray2[19].setNum(58);	
	testArray2[20].setPF('A'); testArray2[20].setNum(11);
	testArray2[21].setPF('m'); testArray2[21].setNum(63);
	testArray2[22].setPF('x'); testArray2[22].setNum(8);
	testArray2[23].setPF('w'); testArray2[23].setNum(42);
	testArray2[24].setPF('B'); testArray2[24].setNum(63);
	testArray2[25].setPF('o'); testArray2[25].setNum(61);
	testArray2[26].setPF('L'); testArray2[26].setNum(18);
	testArray2[27].setPF('V'); testArray2[27].setNum(99);
	testArray2[28].setPF('H'); testArray2[28].setNum(2);
	testArray2[29].setPF('u'); testArray2[29].setNum(47);
}

void initLHResults()
{
	int size = TABLE_CAPACITY;

	LHResult1 = new objPos[size];
	LHResult1[0].setPF('s');  LHResult1[0].setNum(37);  LHResult1[0].setSym('v');
	LHResult1[1].setPF('S');  LHResult1[1].setNum(81);  LHResult1[1].setSym('v');
	LHResult1[2].setPF('U');  LHResult1[2].setNum(5);   LHResult1[2].setSym('v');
	LHResult1[3].setPF('B');  LHResult1[3].setNum(77);  LHResult1[3].setSym('v');
	LHResult1[4].setPF(0);    LHResult1[4].setNum(0);   LHResult1[4].setSym(0);
	LHResult1[5].setPF(0);    LHResult1[5].setNum(0);   LHResult1[5].setSym(0);
	LHResult1[6].setPF(0);    LHResult1[6].setNum(0);   LHResult1[6].setSym(0);
	LHResult1[7].setPF(0);    LHResult1[7].setNum(0);   LHResult1[7].setSym(0);
	LHResult1[8].setPF(0);    LHResult1[8].setNum(0);   LHResult1[8].setSym(0);
	LHResult1[9].setPF('z');  LHResult1[9].setNum(22);  LHResult1[9].setSym('v');
	LHResult1[10].setPF(0);   LHResult1[10].setNum(0);  LHResult1[10].setSym(0);
	LHResult1[11].setPF('j'); LHResult1[11].setNum(4);  LHResult1[11].setSym('v');
	LHResult1[12].setPF(0);   LHResult1[12].setNum(0);  LHResult1[12].setSym(0);
	LHResult1[13].setPF('F'); LHResult1[13].setNum(6);  LHResult1[13].setSym('v');
	LHResult1[14].setPF('m'); LHResult1[14].setNum(13); LHResult1[14].setSym('v');
	LHResult1[15].setPF('o'); LHResult1[15].setNum(75); LHResult1[15].setSym('v');
	LHResult1[16].setPF('p'); LHResult1[16].setNum(28); LHResult1[16].setSym('v');
	LHResult1[17].setPF('P'); LHResult1[17].setNum(54); LHResult1[17].setSym('v');
	LHResult1[18].setPF('A'); LHResult1[18].setNum(25); LHResult1[18].setSym('v');
	LHResult1[19].setPF('K'); LHResult1[19].setNum(61); LHResult1[19].setSym('v');	
	LHResult1[20].setPF('i'); LHResult1[20].setNum(18); LHResult1[20].setSym('v');
	LHResult1[21].setPF('H'); LHResult1[21].setNum(35); LHResult1[21].setSym('v');
	LHResult1[22].setPF('G'); LHResult1[22].setNum(59); LHResult1[22].setSym('v');
	LHResult1[23].setPF('R'); LHResult1[23].setNum(49); LHResult1[23].setSym('v');
	LHResult1[24].setPF('c'); LHResult1[24].setNum(42); LHResult1[24].setSym('v');
	LHResult1[25].setPF('d'); LHResult1[25].setNum(94); LHResult1[25].setSym('v');
	LHResult1[26].setPF('M'); LHResult1[26].setNum(61); LHResult1[26].setSym('v');
	LHResult1[27].setPF('C'); LHResult1[27].setNum(42); LHResult1[27].setSym('v');
	LHResult1[28].setPF('O'); LHResult1[28].setNum(1);  LHResult1[28].setSym('v');
	LHResult1[29].setPF('u'); LHResult1[29].setNum(14); LHResult1[29].setSym('v');
	LHResult1[30].setPF('J'); LHResult1[30].setNum(76); LHResult1[30].setSym('v');
	LHResult1[31].setPF('T'); LHResult1[31].setNum(66); LHResult1[31].setSym('v');
	LHResult1[32].setPF('a'); LHResult1[32].setNum(18); LHResult1[32].setSym('v');
	LHResult1[33].setPF('E'); LHResult1[33].setNum(9);  LHResult1[33].setSym('v');
	LHResult1[34].setPF('w'); LHResult1[34].setNum(51); LHResult1[34].setSym('v');
	LHResult1[35].setPF('V'); LHResult1[35].setNum(98); LHResult1[35].setSym('v');
	LHResult1[36].setPF('q'); LHResult1[36].setNum(27); LHResult1[36].setSym('v');

	LHResult2 = new objPos[size];
	LHResult2[0].setPF(0);    LHResult2[0].setNum(0);   LHResult2[0].setSym(0);
	LHResult2[1].setPF(0);    LHResult2[1].setNum(0);   LHResult2[1].setSym(0);
	LHResult2[2].setPF(0);    LHResult2[2].setNum(0);   LHResult2[2].setSym(0);
	LHResult2[3].setPF(0);    LHResult2[3].setNum(0);   LHResult2[3].setSym(0);
	LHResult2[4].setPF(0);    LHResult2[4].setNum(0);   LHResult2[4].setSym(0);
	LHResult2[5].setPF(0);    LHResult2[5].setNum(0);   LHResult2[5].setSym(0);
	LHResult2[6].setPF(0);    LHResult2[6].setNum(0);   LHResult2[6].setSym(0);
	LHResult2[7].setPF('h');  LHResult2[7].setNum(11);  LHResult2[7].setSym('v');
	LHResult2[8].setPF('q');  LHResult2[8].setNum(11);  LHResult2[8].setSym('v');
	LHResult2[9].setPF('G');  LHResult2[9].setNum(10);  LHResult2[9].setSym('v');
	LHResult2[10].setPF('y'); LHResult2[10].setNum(24); LHResult2[10].setSym('v');
	LHResult2[11].setPF('d'); LHResult2[11].setNum(42); LHResult2[11].setSym('v');
	LHResult2[12].setPF('x'); LHResult2[12].setNum(8);  LHResult2[12].setSym('v');
	LHResult2[13].setPF('I'); LHResult2[13].setNum(12); LHResult2[13].setSym('v');
	LHResult2[14].setPF('p'); LHResult2[14].setNum(91); LHResult2[14].setSym('v');
	LHResult2[15].setPF('z'); LHResult2[15].setNum(46); LHResult2[15].setSym('v');
	LHResult2[16].setPF('R'); LHResult2[16].setNum(5);  LHResult2[16].setSym('v');
	LHResult2[17].setPF('n'); LHResult2[17].setNum(57); LHResult2[17].setSym('v');
	LHResult2[18].setPF('W'); LHResult2[18].setNum(3);  LHResult2[18].setSym('v');
	LHResult2[19].setPF('l'); LHResult2[19].setNum(33); LHResult2[19].setSym('v');	
	LHResult2[20].setPF('Q'); LHResult2[20].setNum(83); LHResult2[20].setSym('v');
	LHResult2[21].setPF('j'); LHResult2[21].setNum(49); LHResult2[21].setSym('v');
	LHResult2[22].setPF('T'); LHResult2[22].setNum(35); LHResult2[22].setSym('v');
	LHResult2[23].setPF('N'); LHResult2[23].setNum(71); LHResult2[23].setSym('v');
	LHResult2[24].setPF('J'); LHResult2[24].setNum(48); LHResult2[24].setSym('v');
	LHResult2[25].setPF('r'); LHResult2[25].setNum(79); LHResult2[25].setSym('v');
	LHResult2[26].setPF('S'); LHResult2[26].setNum(66); LHResult2[26].setSym('v');
	LHResult2[27].setPF('n'); LHResult2[27].setNum(58); LHResult2[27].setSym('v');
	LHResult2[28].setPF('A'); LHResult2[28].setNum(11); LHResult2[28].setSym('v');
	LHResult2[29].setPF('m'); LHResult2[29].setNum(63); LHResult2[29].setSym('v');
	LHResult2[30].setPF('w'); LHResult2[30].setNum(42); LHResult2[30].setSym('v');
	LHResult2[31].setPF('B'); LHResult2[31].setNum(63); LHResult2[31].setSym('v');
	LHResult2[32].setPF('o'); LHResult2[32].setNum(61); LHResult2[32].setSym('v');
	LHResult2[33].setPF('L'); LHResult2[33].setNum(18); LHResult2[33].setSym('v');
	LHResult2[34].setPF('V'); LHResult2[34].setNum(99); LHResult2[34].setSym('v');
	LHResult2[35].setPF('H'); LHResult2[35].setNum(2);  LHResult2[35].setSym('v');
	LHResult2[36].setPF('u'); LHResult2[36].setNum(47); LHResult2[36].setSym('v');

}

void initQHResults()
{
	int size = TABLE_CAPACITY;

	QHResult1 = new objPos[size];
	QHResult1[0].setPF(0);    QHResult1[0].setNum(0);   QHResult1[0].setSym(0);
	QHResult1[1].setPF(0);    QHResult1[1].setNum(0);   QHResult1[1].setSym(0);
	QHResult1[2].setPF('u');  QHResult1[2].setNum(14);  QHResult1[2].setSym('v');
	QHResult1[3].setPF('E');  QHResult1[3].setNum(9);   QHResult1[3].setSym('v');
	QHResult1[4].setPF('q');  QHResult1[4].setNum(27);  QHResult1[4].setSym('v');
	QHResult1[5].setPF('w');  QHResult1[5].setNum(51);  QHResult1[5].setSym('v');
	QHResult1[6].setPF(0);    QHResult1[6].setNum(0);   QHResult1[6].setSym(0);
	QHResult1[7].setPF('s');  QHResult1[7].setNum(37);  QHResult1[7].setSym('v');
	QHResult1[8].setPF(0);    QHResult1[8].setNum(0);   QHResult1[8].setSym(0);
	QHResult1[9].setPF('z');  QHResult1[9].setNum(22);  QHResult1[9].setSym('v');
	QHResult1[10].setPF(0);   QHResult1[10].setNum(0);  QHResult1[10].setSym(0);
	QHResult1[11].setPF('j'); QHResult1[11].setNum(4);  QHResult1[11].setSym('v');
	QHResult1[12].setPF(0);   QHResult1[12].setNum(0);  QHResult1[12].setSym(0);
	QHResult1[13].setPF('F'); QHResult1[13].setNum(6);  QHResult1[13].setSym('v');
	QHResult1[14].setPF('m'); QHResult1[14].setNum(13); QHResult1[14].setSym('v');
	QHResult1[15].setPF('o'); QHResult1[15].setNum(75); QHResult1[15].setSym('v');
	QHResult1[16].setPF('i'); QHResult1[16].setNum(18); QHResult1[16].setSym('v');
	QHResult1[17].setPF('P'); QHResult1[17].setNum(54); QHResult1[17].setSym('v');
	QHResult1[18].setPF('A'); QHResult1[18].setNum(25); QHResult1[18].setSym('v');
	QHResult1[19].setPF('K'); QHResult1[19].setNum(61); QHResult1[19].setSym('v');	
	QHResult1[20].setPF('C'); QHResult1[20].setNum(42); QHResult1[20].setSym('v');
	QHResult1[21].setPF('H'); QHResult1[21].setNum(35); QHResult1[21].setSym('v');
	QHResult1[22].setPF('G'); QHResult1[22].setNum(59); QHResult1[22].setSym('v');
	QHResult1[23].setPF('p'); QHResult1[23].setNum(28); QHResult1[23].setSym('v');
	QHResult1[24].setPF('c'); QHResult1[24].setNum(42); QHResult1[24].setSym('v');
	QHResult1[25].setPF('M'); QHResult1[25].setNum(61); QHResult1[25].setSym('v');
	QHResult1[26].setPF('O'); QHResult1[26].setNum(1);  QHResult1[26].setSym('v');
	QHResult1[27].setPF('R'); QHResult1[27].setNum(49); QHResult1[27].setSym('v');
	QHResult1[28].setPF('J'); QHResult1[28].setNum(76); QHResult1[28].setSym('v');
	QHResult1[29].setPF('a'); QHResult1[29].setNum(18); QHResult1[29].setSym('v');
	QHResult1[30].setPF('d'); QHResult1[30].setNum(94); QHResult1[30].setSym('v');
	QHResult1[31].setPF('V'); QHResult1[31].setNum(98); QHResult1[31].setSym('v');
	QHResult1[32].setPF(0);   QHResult1[32].setNum(0);  QHResult1[32].setSym(0);
	QHResult1[33].setPF('T'); QHResult1[33].setNum(66); QHResult1[33].setSym('v');
	QHResult1[34].setPF('U'); QHResult1[34].setNum(5);  QHResult1[34].setSym('v');
	QHResult1[35].setPF('B'); QHResult1[35].setNum(77); QHResult1[35].setSym('v');
	QHResult1[36].setPF('S'); QHResult1[36].setNum(81); QHResult1[36].setSym('v');

	QHResult2 = new objPos[size];
	QHResult2[0].setPF(0);    QHResult2[0].setNum(0);   QHResult2[0].setSym(0);
	QHResult2[1].setPF('L');  QHResult2[1].setNum(18);  QHResult2[1].setSym('v');
	QHResult2[2].setPF(0);    QHResult2[2].setNum(0);   QHResult2[2].setSym(0);
	QHResult2[3].setPF(0);    QHResult2[3].setNum(0);   QHResult2[3].setSym(0);
	QHResult2[4].setPF('u');  QHResult2[4].setNum(47);  QHResult2[4].setSym('v');
	QHResult2[5].setPF(0);    QHResult2[5].setNum(0);   QHResult2[5].setSym(0);
	QHResult2[6].setPF(0);    QHResult2[6].setNum(0);   QHResult2[6].setSym(0);
	QHResult2[7].setPF('h');  QHResult2[7].setNum(11);  QHResult2[7].setSym('v');
	QHResult2[8].setPF('q');  QHResult2[8].setNum(11);  QHResult2[8].setSym('v');
	QHResult2[9].setPF('G');  QHResult2[9].setNum(10);  QHResult2[9].setSym('v');
	QHResult2[10].setPF('y'); QHResult2[10].setNum(24); QHResult2[10].setSym('v');
	QHResult2[11].setPF('d'); QHResult2[11].setNum(42); QHResult2[11].setSym('v');
	QHResult2[12].setPF('x'); QHResult2[12].setNum(8);  QHResult2[12].setSym('v');
	QHResult2[13].setPF('I'); QHResult2[13].setNum(12); QHResult2[13].setSym('v');
	QHResult2[14].setPF('p'); QHResult2[14].setNum(91); QHResult2[14].setSym('v');
	QHResult2[15].setPF('z'); QHResult2[15].setNum(46); QHResult2[15].setSym('v');
	QHResult2[16].setPF('R'); QHResult2[16].setNum(5);  QHResult2[16].setSym('v');
	QHResult2[17].setPF('A'); QHResult2[17].setNum(11); QHResult2[17].setSym('v');
	QHResult2[18].setPF('W'); QHResult2[18].setNum(3);  QHResult2[18].setSym('v');
	QHResult2[19].setPF('l'); QHResult2[19].setNum(33); QHResult2[19].setSym('v');	
	QHResult2[20].setPF('Q'); QHResult2[20].setNum(83); QHResult2[20].setSym('v');
	QHResult2[21].setPF('j'); QHResult2[21].setNum(49); QHResult2[21].setSym('v');
	QHResult2[22].setPF('r'); QHResult2[22].setNum(79); QHResult2[22].setSym('v');
	QHResult2[23].setPF('N'); QHResult2[23].setNum(71); QHResult2[23].setSym('v');
	QHResult2[24].setPF('J'); QHResult2[24].setNum(48); QHResult2[24].setSym('v');
	QHResult2[25].setPF('B'); QHResult2[25].setNum(63); QHResult2[25].setSym('v');
	QHResult2[26].setPF('w'); QHResult2[26].setNum(42); QHResult2[26].setSym('v');
	QHResult2[27].setPF('S'); QHResult2[27].setNum(66); QHResult2[27].setSym('v');
	QHResult2[28].setPF('m'); QHResult2[28].setNum(63); QHResult2[28].setSym('v');
	QHResult2[29].setPF('T'); QHResult2[29].setNum(35); QHResult2[29].setSym('v');
	QHResult2[30].setPF('n'); QHResult2[30].setNum(57); QHResult2[30].setSym('v');
	QHResult2[31].setPF('n'); QHResult2[31].setNum(58); QHResult2[31].setSym('v');
	QHResult2[32].setPF('V'); QHResult2[32].setNum(99); QHResult2[32].setSym('v');
	QHResult2[33].setPF(0);   QHResult2[33].setNum(0);  QHResult2[33].setSym(0);
	QHResult2[34].setPF(0);   QHResult2[34].setNum(0);  QHResult2[34].setSym(0);
	QHResult2[35].setPF('o'); QHResult2[35].setNum(61); QHResult2[35].setSym('v');
	QHResult2[36].setPF('H'); QHResult2[36].setNum(2);  QHResult2[36].setSym('v');

}

void initDHResults()
{
	int size = TABLE_CAPACITY;

	DHResult1 = new objPos[size];
	DHResult1[0].setPF('s');  DHResult1[0].setNum(37);  DHResult1[0].setSym('v');
	DHResult1[1].setPF(0);    DHResult1[1].setNum(0);   DHResult1[1].setSym(0);
	DHResult1[2].setPF('C');  DHResult1[2].setNum(42);  DHResult1[2].setSym('v');
	DHResult1[3].setPF('E');  DHResult1[3].setNum(9);   DHResult1[3].setSym('v');
	DHResult1[4].setPF(0);    DHResult1[4].setNum(0);   DHResult1[4].setSym(0);
	DHResult1[5].setPF(0);    DHResult1[5].setNum(0);   DHResult1[5].setSym(0);
	DHResult1[6].setPF(0);    DHResult1[6].setNum(0);   DHResult1[6].setSym(0);
	DHResult1[7].setPF('u');  DHResult1[7].setNum(14);  DHResult1[7].setSym('v');
	DHResult1[8].setPF(0);    DHResult1[8].setNum(0);   DHResult1[8].setSym(0);
	DHResult1[9].setPF('z');  DHResult1[9].setNum(22);  DHResult1[9].setSym('v');
	DHResult1[10].setPF(0);   DHResult1[10].setNum(0);  DHResult1[10].setSym(0);
	DHResult1[11].setPF('j'); DHResult1[11].setNum(4);  DHResult1[11].setSym('v');
	DHResult1[12].setPF('q'); DHResult1[12].setNum(27); DHResult1[12].setSym('v');
	DHResult1[13].setPF('F'); DHResult1[13].setNum(6);  DHResult1[13].setSym('v');
	DHResult1[14].setPF('m'); DHResult1[14].setNum(13); DHResult1[14].setSym('v');
	DHResult1[15].setPF('o'); DHResult1[15].setNum(75); DHResult1[15].setSym('v');
	DHResult1[16].setPF(0);   DHResult1[16].setNum(0);  DHResult1[16].setSym(0);
	DHResult1[17].setPF('P'); DHResult1[17].setNum(54); DHResult1[17].setSym('v');
	DHResult1[18].setPF('A'); DHResult1[18].setNum(25); DHResult1[18].setSym('v');
	DHResult1[19].setPF('K'); DHResult1[19].setNum(61); DHResult1[19].setSym('v');	
	DHResult1[20].setPF('S'); DHResult1[20].setNum(81); DHResult1[20].setSym('v');
	DHResult1[21].setPF('H'); DHResult1[21].setNum(35); DHResult1[21].setSym('v');
	DHResult1[22].setPF('G'); DHResult1[22].setNum(59); DHResult1[22].setSym('v');
	DHResult1[23].setPF('i'); DHResult1[23].setNum(18); DHResult1[23].setSym('v');
	DHResult1[24].setPF('c'); DHResult1[24].setNum(42); DHResult1[24].setSym('v');
	DHResult1[25].setPF('M'); DHResult1[25].setNum(61); DHResult1[25].setSym('v');
	DHResult1[26].setPF('U'); DHResult1[26].setNum(5);  DHResult1[26].setSym('v');
	DHResult1[27].setPF('O'); DHResult1[27].setNum(1);  DHResult1[27].setSym('v');
	DHResult1[28].setPF('R'); DHResult1[28].setNum(49); DHResult1[28].setSym('v');
	DHResult1[29].setPF('p'); DHResult1[29].setNum(28); DHResult1[29].setSym('v');
	DHResult1[30].setPF('B'); DHResult1[30].setNum(77); DHResult1[30].setSym('v');
	DHResult1[31].setPF('a'); DHResult1[31].setNum(18); DHResult1[31].setSym('v');
	DHResult1[32].setPF('J'); DHResult1[32].setNum(76); DHResult1[32].setSym('v');
	DHResult1[33].setPF('w'); DHResult1[33].setNum(51); DHResult1[33].setSym('v');
	DHResult1[34].setPF('d'); DHResult1[34].setNum(94); DHResult1[34].setSym('v');
	DHResult1[35].setPF('V'); DHResult1[35].setNum(98); DHResult1[35].setSym('v');
	DHResult1[36].setPF('T'); DHResult1[36].setNum(66); DHResult1[36].setSym('v');

	DHResult2 = new objPos[size];
	DHResult2[0].setPF('V');  DHResult2[0].setNum(99);  DHResult2[0].setSym('v');
	DHResult2[1].setPF('H');  DHResult2[1].setNum(2);   DHResult2[1].setSym('v');
	DHResult2[2].setPF(0);    DHResult2[2].setNum(0);   DHResult2[2].setSym(0);
	DHResult2[3].setPF(0);    DHResult2[3].setNum(0);   DHResult2[3].setSym(0);
	DHResult2[4].setPF('u');  DHResult2[4].setNum(47);  DHResult2[4].setSym('v');
	DHResult2[5].setPF(0);    DHResult2[5].setNum(0);   DHResult2[5].setSym(0);
	DHResult2[6].setPF(0);    DHResult2[6].setNum(0);   DHResult2[6].setSym(0);
	DHResult2[7].setPF('h');  DHResult2[7].setNum(11);  DHResult2[7].setSym('v');
	DHResult2[8].setPF(0);    DHResult2[8].setNum(0);   DHResult2[8].setSym(0);
	DHResult2[9].setPF('G');  DHResult2[9].setNum(10);  DHResult2[9].setSym('v');
	DHResult2[10].setPF('y'); DHResult2[10].setNum(24); DHResult2[10].setSym('v');
	DHResult2[11].setPF('x'); DHResult2[11].setNum(8);  DHResult2[11].setSym('v');
	DHResult2[12].setPF(0);   DHResult2[12].setNum(0);  DHResult2[12].setSym(0);
	DHResult2[13].setPF('q'); DHResult2[13].setNum(11); DHResult2[13].setSym('v');
	DHResult2[14].setPF('p'); DHResult2[14].setNum(91); DHResult2[14].setSym('v');
	DHResult2[15].setPF('z'); DHResult2[15].setNum(46); DHResult2[15].setSym('v');
	DHResult2[16].setPF('d'); DHResult2[16].setNum(42); DHResult2[16].setSym('v');
	DHResult2[17].setPF('I'); DHResult2[17].setNum(12); DHResult2[17].setSym('v');
	DHResult2[18].setPF('W'); DHResult2[18].setNum(3);  DHResult2[18].setSym('v');
	DHResult2[19].setPF('R'); DHResult2[19].setNum(5);  DHResult2[19].setSym('v');	
	DHResult2[20].setPF('Q'); DHResult2[20].setNum(83); DHResult2[20].setSym('v');
	DHResult2[21].setPF('A'); DHResult2[21].setNum(11); DHResult2[21].setSym('v');
	DHResult2[22].setPF('r'); DHResult2[22].setNum(79); DHResult2[22].setSym('v');
	DHResult2[23].setPF('N'); DHResult2[23].setNum(71); DHResult2[23].setSym('v');
	DHResult2[24].setPF('n'); DHResult2[24].setNum(57); DHResult2[24].setSym('v');
	DHResult2[25].setPF('w'); DHResult2[25].setNum(42); DHResult2[25].setSym('v');
	DHResult2[26].setPF('j'); DHResult2[26].setNum(49); DHResult2[26].setSym('v');
	DHResult2[27].setPF('l'); DHResult2[27].setNum(33); DHResult2[27].setSym('v');
	DHResult2[28].setPF('J'); DHResult2[28].setNum(48); DHResult2[28].setSym('v');
	DHResult2[29].setPF('T'); DHResult2[29].setNum(35); DHResult2[29].setSym('v');
	DHResult2[30].setPF('o'); DHResult2[30].setNum(61); DHResult2[30].setSym('v');
	DHResult2[31].setPF('n'); DHResult2[31].setNum(58); DHResult2[31].setSym('v');
	DHResult2[32].setPF('L'); DHResult2[32].setNum(18); DHResult2[32].setSym('v');
	DHResult2[33].setPF('S'); DHResult2[33].setNum(66); DHResult2[33].setSym('v');
	DHResult2[34].setPF('m'); DHResult2[34].setNum(63); DHResult2[34].setSym('v');
	DHResult2[35].setPF('B'); DHResult2[35].setNum(63); DHResult2[35].setSym('v');
	DHResult2[36].setPF(0);   DHResult2[36].setNum(0);  DHResult2[36].setSym(0);

}
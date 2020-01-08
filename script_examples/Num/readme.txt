There is command-line calculator, which demonstrates attached lightweight big number library.
The library consists of the 8 header files and it can be used in other software to use big numbers.

The library are the files mentioned below, for almost each file there are listed methods intended to use outside this library.

numdef.h - Parameter value definition. This is configuration file, where programmer can affect the featuses such as the maximum number value, the variable types, which are used etc.
numcore.h - The main core file.
 void numInit() - Core initialization - it must be performed before any operation, once in whole program
 void numZero(numPtr(N)) - Set 0 as number value
 void numCopy(numPtr(NI), numPtr(NO)) - Copy number value from NI to NO
 void numSignSet(numPtr(N), numByte S) - Set the number value sign (0 - positive, 1 - negative)
 numByte numSignGet(numPtr(N)) - Get the number value sign
 numSignGetAbs(numPtr(N)) - Get the number value sign and remove sign (convert to absolute value)
 numSignGetInv(numPtr(N) - Get the number value sign and invert sign
numbase.h - The base number operations
 numByte numCompare(numPtr(N1), numPtr(N2)) - Number compare (N1<N2 ==> -1; N1=N2 ==> 0; N1>N2 ==> 1)
 void numAdd(numPtr(N1), numPtr(N2), numPtr(N3)) - Number add (N3=N1+N2)
 void numSub(numPtr(N1), numPtr(N2), numPtr(N3)) - Number subtract (N3=N1-N2)
 void numMul(numPtr(N1), numPtr(N2), numPtr(N3)) - Num multiply (N3=N1*N2)
 void numDiv(numPtr(N1), numPtr(N2), numPtr(N3)) - Num divide (N3=N1/N2), division be 0 returns the minimum possible number value or the maximum possible number value depending on dividend sign
numio.h - Convert between number value and text to input or output number value
 void numSet(numPtr(N), numChar * Val) - Set number value based on text
 void numGet(numPtr(N), numChar * Val) - Print number value as text
 void numSetArray(numPtr(N), numElementType * NumA, numElementType Weight, numIOArrayIterator ElementCount) - Set number value based on array
 numByte numGetArray(numPtr(N), numElementType * NumA, numElementType Weight, numIOArrayIterator Count) - Read number sign and absolute value into array
numtaylor.h - Functions using Taylor series
 void numInitTaylor() - Structure initialization - it must be performed before any operation using Taylor serie, once in whole program
 void numSin(numPtr(Arg), numPtr(Val)) - Sine (Val=sin(Arg))
 void numCos(numPtr(Arg), numPtr(Val)) - Cosine (Val=cos(Arg))
 void numTan(numPtr(Arg), numPtr(Val)) - Tangent (Val=tan(Arg)=sin(Arg)/cos(Arg))
 void numCot(numPtr(Arg), numPtr(Val)) - Cotangent (Val=cot(Arg)=cos(Arg)/sin(Arg))
 void numSec(numPtr(Arg), numPtr(Val)) - Secant (Val=sec(Arg)=1/cos(Arg))
 void numCsc(numPtr(Arg), numPtr(Val)) - Cosecant (Val=csc(Arg)=1/sin(Arg))
numround.h - Rounding numbers and multiply/divide by multiply of 10
 void numMul10(numPtr(N), numIteratorSignedType I) - Multiply number by 10^I (I > 0)
 void numDiv10(numPtr(N), numIteratorSignedType I) - Divide number by 10^I (I > 0)
 void numRound(numPtr(N), numIteratorSignedType I, numByte Corr) - Round number
nummath.h - Other mathematical functions
 void numExp(numPtr(X), numPtr(N), numPtr(Val)) - Power of integer exponent (Val=X^N)
 void numRoot(numPtr(X), numPtr(N), numPtr(Val)) - Root of integer degree other than 0 (Val^N=X)
numdebug.h - Functions usable in testing and debugging this library
 void numGetRaw(numPtr(N), numByte NoSplit, numChar * Val) - Print number raw value
 void numSetBits(numPtr(N), numChar * Val) - Set number value based on bit sequence

To declare big number variable, there are two macros, internally, the number variable is an array of certain unsigned integer type
 numDef(variable) - big number variable
 numPtr(variable) - pointer to big number variable

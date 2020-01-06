// Size of one number element (maximum element value)
#define numElementSize ((numElementType)1 << numBits)

// Size of one number element decreased by 1
#define numElementSizeX (numElementSize - 1)

// Declaration of number variable
#define numDef(X) numElementType X[numElements]

// Declaration of number variable pointer
#define numPtr(X) numElementType X[numElements]

// Number of elements decreased by 1
#define numElementsX (numElements - 1)

// Number of all number bits
#define numBitsAll (numBits * numElements)

// Get the most significiant bit of number
#define _numGetMSB(X) (X[numElementsX] & ((numElementType)numElementSize >> 1))

// Get the least significiant bit of number
#define _numGetLSB(X) (X[0] & 1)


// Array of bit shifts
numElementType numBitVal[numBits + 1];


// The unit number
numDef(numUnit);

// The number moved by one digit
numDef(_numShift);

// The dummy number used in multiplication and division
numDef(_numDummy);


// Set 0 as number value
void numZero(numPtr(N))
{
 numIteratorType I;
 for (I = 0; I < numElements; I++)
 {
  N[I] = 0;
 }
}

// Copy number value from NI to NO
void numCopy(numPtr(NI), numPtr(NO))
{
 numIteratorType I;
 for (I = 0; I < numElements; I++)
 {
  NO[I] = NI[I];
 }
}

// Set the number value sign
// 0 - the positive value
// 1 - the negative value
void numSignSet(numPtr(N), numByte S)
{
 numIteratorType I;
 N[numElementsX] &= numElementSizeX;
 for (I = 0; I < numElements; I++)
 {
  if (N[I] != 0)
  {
   if (S)
   {
    N[numElementsX] |= numElementSize;
   }
   else
   {
    N[numElementsX] &= numElementSizeX;
   }
   return;
  }
 }
 N[numElementsX] &= numElementSizeX;
}

// Get the number value sign
// 0 - the positive value or 0
// 1 - the negative value
numByte numSignGet(numPtr(N))
{
 numIteratorType I;
 numByte S = (N[numElementsX] & numElementSize) ? 1 : 0;
 N[numElementsX] &= numElementSizeX;
 for (I = 0; I < numElements; I++)
 {
  if (N[I] != 0)
  {
   if (S)
   {
    N[numElementsX] |= numElementSize;
    return 1;
   }
   else
   {
    return 0;
   }
  }
 }
 return 0;
}

// Get the number value sign and remove sign (convert to absolute value)
// 0 - the positive value or 0
// 1 - the negative value
numByte numSignGetAbs(numPtr(N))
{
 numByte S = numSignGet(N);
 numSignSet(N, 0);
 return S;
}

// Get the number value sign and invert sign
// 0 - the positive value or 0
// 1 - the negative value
numByte numSignGetInv(numPtr(N))
{
 numByte S = numSignGet(N);
 numSignSet(N, !S);
 return S;
}

// Compare number values
// -1 - N1<N2
// 0 - N1=N2
// 1 - N1>N2
numByte _numCompare(numPtr(N1), numPtr(N2))
{
 numIteratorType I;
 for (I = numElementsX; I != 0; I--)
 {
  if (N1[I] > N2[I])
  {
   return 1;
  }
  else
  {
   if (N1[I] < N2[I])
   {
    return -1;
   }
  }
 }
 if (N1[0] > N2[0])
 {
  return 1;
 }
 if (N1[0] < N2[0])
 {
  return -1;
 }
 return 0;
}

// Add operation
void _numAdd(numPtr(N1), numPtr(N2), numPtr(N3))
{
 numIteratorType I;

 // Adding number elements
 for (I = 0; I < numElements; I++)
 {
  N3[I] = N1[I] + N2[I];
 }

 // Carry propagation between elements
 for (I = 0; I < numElementsX; I++)
 {
  if ((N3[I] & numElementSize) > 0)
  {
   N3[I + 1]++;
   N3[I] = N3[I] & numElementSizeX;
  }
 }
}

// Add operation on the extended number values (the one number is stored in two variables)
void _numAddExt(numPtr(N1), numPtr(N2), numPtr(N1X), numPtr(N2X))
{
 numIteratorType I;

 // Adding number elements
 for (I = 0; I < numElements; I++)
 {
  N1[I] = N1[I] + N2[I];
  N1X[I] = N1X[I] + N2X[I];
 }

 // Carry propagation between elements of base number
 for (I = 0; I < numElementsX; I++)
 {
  if ((N1[I] & numElementSize) > 0)
  {
   N1[I + 1]++;
   N1[I] = N1[I] & numElementSizeX;
  }
 }

 // Carry propagation between base number and extension number
 if ((N1[numElementsX] & numElementSize) > 0)
 {
  N1X[0]++;
  N1[numElementsX] = N1[numElementsX] & numElementSizeX;
 }

 // Carry propagation between elements of extension number
 for (I = 0; I < numElementsX; I++)
 {
  if ((N1X[I] & numElementSize) > 0)
  {
   N1X[I + 1]++;
   N1X[I] = N1X[I] & numElementSizeX;
  }
 }
}

// Subtract operation
void _numSub(numPtr(N1), numPtr(N2), numPtr(N3))
{
 numIteratorType I;

 #ifdef numSubtractByAdd
  // Adding number elements
  N3[0] = numElementSize - N2[0] + N1[0];
  for (I = 1; I < numElements; I++)
  {
   N3[I] = numElementSizeX - N2[I] + N1[I];
  }

  // Carry propagation between elements
  for (I = 0; I < numElementsX; I++)
  {
   if ((N3[I] & numElementSize) > 0)
   {
    N3[I + 1]++;
    N3[I] = N3[I] & numElementSizeX;
   }
  }
 #else
  numDef(N1X);
  numIteratorType T;

  // Copying minuend value
  for (I = 0; I < numElements; I++)
  {
   N1X[I] = N1[I];
  }

  // Borrowing the bits to prepare minuend element values to get not less than subtrahend element values
  for (I = 0; I < numElementsX; I++)
  {
   if (N2[I] > N1X[I])
   {
    N1X[I] = N1X[I] + numElementSize;
    T = I + 1;
    while (N1X[T] == 0)
    {
     N1X[T] = numElementSize;
     N1X[T]--;
     T++;
    }
    N1X[T]--;
   }
  }

  // Subtracting number elements
  for (I = 0; I < numElements; I++)
  {
   N3[I] = N1X[I] - N2[I];
  }
 #endif
}

// Shift the number value bits left (used in subtracting and dividing numbers)
void _numShiftLeft(numPtr(N))
{
 numIteratorType I;

 // Shifting number elements
 for (I = 0; I < numElements; I++)
 {
  N[I] = N[I] << 1;
 }

 // Carry propagation between elements
 for (I = 0; I < numElementsX; I++)
 {
  if (N[I] & numElementSize)
  {
   N[I] &= numElementSizeX;
   N[I + 1]++;
  }
 }
}

// Shift the number value bits right (used in multiplication and dividing numbers)
void _numShiftRight(numPtr(N))
{
 numIteratorType I;

 // Carry propagation between elements
 for (I = 1; I < numElements; I++)
 {
  if (N[I] & 1)
  {
   N[I]--;
   N[I - 1] |= numElementSize;
  }
 }

 // Shifting number elements
 for (I = 0; I < numElements; I++)
 {
  N[I] = N[I] >> 1;
 }
}

// Declaration of _numDiv method because _numDiv uses _numMul and _numMul uses _numDiv
numByte _numDiv(numPtr(N1), numPtr(N2), numPtr(N3), numByte Dec, numPtr(N1Ext), numByte Round);

// Multiplication with extended product value
numByte _numMul(numPtr(N1), numPtr(N2), numPtr(N3), numByte Dec, numPtr(N3Ext))
{
 numIteratorType I;
 numByte K;
 numBitIteratorType J;
 numDef(N1X);
 numDef(N2X);
 numDef(N1Ext);

 J = 0;
 K = 0;
 for (I = 0; I < numElements; I++)
 {
  N1X[I] = N1[I];
  N2X[I] = N2[I];
  N3[I] = 0;
  N1Ext[I] = 0;
  N3Ext[I] = 0;
  if (N1X[I] != 0)
  {
   K = 1;
  }
  if (N2X[I] != 0)
  {
   J = I + 1;
  }
 }

 // The multiplication is performed only if the both factors are not equal to 0, otherwise, the product value will equal to 0
 if ((K != 0) && (J != 0))
 {
  // The multiplication iteration number
  J = J * numBits;

  // Performing the multiplication by shifting the multiplier and summing the product in loop
  K = 0;
  I = 0;
  while (J != 0)
  {
   // If the current bit of multiplicand equals 1, the multiplier will be added to the product
   if ((N2X[I] & numBitVal[K]) != 0)
   {
    N1X[numElementsX] &= numElementSizeX;
    N1Ext[numElementsX] &= numElementSizeX;
    _numAddExt(N3, N1X, N3Ext, N1Ext);
   }

   // Shifting left the multiplier
   _numShiftLeft(N1Ext);
   if (_numGetMSB(N1X))
   {
    N1Ext[0]++;
   }
   _numShiftLeft(N1X);

   // Increating the current bit pointer, if the pointer gets value equals to element size,
   // the bit pointer will be set to zero and the element value will be increased
   K++;
   if (K >= numBits)
   {
    K = 0;
    I = I + 1;
   }
   J--;
  }

  if (Dec)
  {
   // Moving left the decimal point in product by number of fraction digits in factors
   _numDiv(N3, numUnit, N3, 0, N3Ext, 0);
  }
  return 1;
 }
 else
 {
  numZero(N3);
  return 0;
 }
}

// Division with extended dividend value
numByte _numDiv(numPtr(N1), numPtr(N2), numPtr(N3), numByte Dec, numPtr(N1Ext), numByte Round)
{
 numIteratorType I;
 numBitIteratorType J;
 numByte IsExt, Work;
 numDef(N1X);
 numDef(N2X);
 numDef(NX);
 numDef(N1XExt);
 numElementType * N1XPtr;
 numIteratorType DigN, DigXN;
 numByte DigB, DigXB;

 IsExt = 1;
 Work = 1;
 for (I = 0; I < numElements; I++)
 {
  N1XExt[I] = N1Ext[I];
  N1X[I] = N1[I];
  N2X[I] = N2[I];
  NX[I] = 0;
  if ((N2[I] & numElementSizeX) != 0)
  {
   IsExt = 0;
  }
  if ((N1[I] & numElementSizeX) != 0)
  {
   Work = 0;
  }
  N3[I] = 0;
 }

 // If the dividend equals to 0, the quotient will equals to 0
 if (Work == 1)
 {
  return 0;
 }

 // If the divisor equals to 0, the quotient will equal to the greatest possible value (the approximation of infinity)
 if (IsExt)
 {
  for (I = 0; I < numElements; I++)
  {
   N3[I] = numElementSizeX;
  }
  return 1;
 }

 if (Dec)
 {
  // Moving right the decimal point of dividend by number of fraction digits in divisor
  _numMul(N1X, numUnit, N1X, 0, N1XExt);
 }

 // Checking if the extension number is used, otherwise, the algorithm will use only the base number
 IsExt = 0;
 for (I = 0; I < numElements; I++)
 {
  if (N1XExt[I] != 0)
  {
   IsExt = 1;
  }
 }

 // Setting the bit pointer
 DigXN = numElementsX;
 DigXB = numBits - 1;

 // Setting the initial number of iterations, which will be corrected to get really needed iteration number
 J = 1 + numBitsAll;

 // Moving right the divisor to eliminate the tailing zeroes
 while (!(_numGetLSB(N2X)))
 {
  J--;
  _numShiftRight(N2X);
 }

 // Setting the ultimate number of iterations
 if (IsExt)
 {
  N1XPtr = N1XExt;
  J += numBitsAll;
 }
 else
 {
  N1XPtr = N1X;
 }

 // Searching for the first bit of dividend
 Work = 1;
 while (Work)
 {
  if ((N1XPtr[DigXN] & numBitVal[DigXB]))
  {
   Work = 0;
  }
  if (Work)
  {
   J--;
   DigXB--;
   if (DigXB < 0)
   {
    DigXB = numBits - 1;
    if (DigXN == 0)
    {
     N1XPtr = N1X;
     IsExt = 0;
     DigXN = numElementsX;
     DigXB = numBits - 1;
    }
    else
    {
     DigXN--;
    }
   }
  }
 }

 // Aquiring the first bits of dividend
 while ((J != 0) && (_numCompare(NX, N2X) < 0))
 {
  _numShiftLeft(NX);
  NX[numElementsX] &= numElementSizeX;
  if (N1XPtr[DigXN] & numBitVal[DigXB])
  {
   NX[0]++;
  }
  DigXB--;
  if (DigXB < 0)
  {
   DigXB = numBits - 1;
   if (DigXN == 0)
   {
    N1XPtr = N1X;
    IsExt = 0;
    DigXN = numElementsX;
    DigXB = numBits - 1;
   }
   else
   {
    DigXN--;
   }
  }
  J--;
 }


 // The division is performed only if the divisor after bit shifting is greater than dividend after bit shifting,
 // otherwise, the quotient will equal 0, because the real quotient is less than the minimum possible absolute value
 if (_numCompare(NX, N2X) >= 0)
 {
  DigN = ((J - 1) / numBits);
  DigB = ((J - 1) % numBits);
  while (J != 0)
  {
   if (_numCompare(NX, N2X) >= 0)
   {
    N3[DigN] |= numBitVal[DigB];
    _numSub(NX, N2X, NX);
   }
   DigB--;
   if (DigB < 0)
   {
    DigN--;
    DigB = numBits - 1;
   }

   _numShiftLeft(NX);
   NX[numElementsX] &= numElementSizeX;
   if (N1XPtr[DigXN] & numBitVal[DigXB])
   {
    NX[0]++;
   }
   DigXB--;
   if (DigXB < 0)
   {
    DigXB = numBits - 1;
    if (DigXN == 0)
    {
     N1XPtr = N1X;
     IsExt = 0;
     DigXN = numElementsX;
     DigXB = numBits - 1;
    }
    else
    {
     DigXN--;
    }
   }
   J--;
  }

  // Checking the bit followed after the least significiant bit and rounding according the bit value
  if (Round)
  {
   if (_numCompare(NX, N2X) >= 0)
   {
    N3[0]++;
    for (I = 0; I < numElementsX; I++)
    {
     if ((N3[I] & numElementSize) > 0)
     {
      N3[I + 1]++;
      N3[I] = N3[I] & numElementSizeX;
     }
     else
     {
      I = numElementsX;
     }
    }
   }
  }
 }
 return 1;
}

// Core initialization - it must be performed before any operation, once in whole program
void numInit()
{
 numBitIteratorType I;
 numElementType BitPointer = 1;
 for (I = 0; I <= numBits; I++)
 {
  numBitVal[I] = BitPointer;
  BitPointer = BitPointer << 1;
 }

 numZero(numUnit);
 numZero(_numShift);
 numZero(_numDummy);
 numUnit[0] = 1;
 _numShift[0] = 10;
 I = numDecimal;
 while (I != 0)
 {
  _numMul(numUnit, _numShift, numUnit, 0, _numDummy);
  I--;
 }
}

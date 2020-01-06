// Set number value based on text
void numSet(numPtr(N), numChar * Val)
{
 numChar Val0[numInputDigits + 1];
 numIOArrayIterator L = 0;
 numIOArrayIterator Sep = -1;
 numIOArrayIterator L0 = 0;
 numIOArrayIterator I;
 numByte IsNeg = 0;
 numDef(NV);
 numZero(_numDummy);
 numZero(N);
 numZero(NV);
 while (Val[L] != 0)
 {
  if ((Val[L] >= 48) && (Val[L] <= 57))
  {
   if (L0 < numInputDigits)
   {
    Val0[L0] = Val[L];
   }
   L0++;
  }
  else
  {
   if (Val[L] == '-')
   {
    IsNeg = 1;
   }
   #ifdef numDecimalSep
    else
    {
     if (Val[L] == numDecimalSep)
     {
      Sep = L0;
     }
    }
   #endif
  }
  L++;
 }
 if (Sep < 0)
 {
  Sep = L0;
 }
 if (L0 <= numInputDigits)
 {
  Val0[L0] = 0;
  I = L0;
 }
 else
 {
  Val0[numInputDigits] = 0;
 }
 I = 0;
 while (I < (Sep + numDecimal))
 {
  _numMul(N, _numShift, N, 0, _numDummy);
  if ((I < L0) && (I < numInputDigits))
  {
   NV[0] = (Val0[I] - 48);
   _numAdd(N, NV, N);
  }
  I++;
 }
 if ((I < numInputDigits) && (I <= L0))
 {
  if (Val0[I] >= (48 + 5))
  {
   NV[0] = 1;
   _numAdd(N, NV, N);
  }
 }
 if (IsNeg)
 {
  N[numElementsX] |= numElementSize;
 }
}


// Print number value as text
void numGet(numPtr(N), numChar * Val)
{
 numIOArrayIterator I;
 numIOArrayIterator J;
 numIOArrayIterator K;
 numByte S;
 numByte BCD[numDisplayDigits + 1];
 numElementType X;
 numElementType XX;

 // Clearing BCD value sequence to print
 for (I = numDisplayDigits; I >= 0; I--)
 {
  BCD[I] = 0;
 }

 // Converting number value into BCD value sequence to print
 for (K = numElementsX; K >= 0; K--)
 {
  X = N[K];
  for (I = 0; I < numBits; I++)
  {
   XX = X & ((numElementType)numElementSize >> 1);
   for (J = (numDisplayDigits - 1); J >= 0; J--)
   {
    if (BCD[J] >= 5)
    {
     BCD[J] += 3;
    }
    BCD[J] = BCD[J] << 1;
    if (BCD[J] > 15)
    {
     BCD[J] = BCD[J] & 15;
     BCD[J + 1] += 1;
    }
   }
   if (XX > 0)
   {
    BCD[0] += 1;
   }
   X = X << 1;
  }
 }

 // Determining the number sign
 Val[0] = ' ';
 S = 0;
 if (N[numElementsX] & numElementSizeX)
 {
  S = 1;
 }
 else
 {
  for (I = 0; I < numElementsX; I++)
  {
   if (N[I] != 0)
   {
    S = 1;
    I = numElements;
   }
  }
 }
 if (S)
 {
  if (N[numElementsX] & numElementSize)
  {
   Val[0] = '-';
  }
  else
  {
   Val[0] = '+';
  }
 }

 // Inputting digit sequence based on BCD
 J = 1;
 for (I = (numDisplayDigits - 1); I >= 0; I--)
 {
  Val[J] = BCD[I] + 48;
  J++;
  if ((numDecimal > 0) && (numDecimal == I))
  {
   #ifdef numDecimalSep
    Val[J] = numDecimalSep;
    J++;
   #else
    #ifdef numThousandSep
     Val[J] = numThousandSep;
     J++;
    #endif
   #endif
  }
  #ifdef numThousandSep
   else
   {
    if ((((numDecimal - I) % 3) == 0) && (I > 0))
    {
     Val[J] = numThousandSep;
     J++;
    }
   }
  #endif
 }
 Val[J] = 0;
}


// Set number value based on array
void numSetArray(numPtr(N), numElementType * NumA, numElementType Weight, numIOArrayIterator ElementCount)
{
 numDef(NWeight);
 numDef(NTemp);
 numIOArrayIterator I;

 numZero(NWeight);
 numZero(NTemp);
 numZero(N);
 NWeight[0] = Weight;
 for (I = ElementCount; I > 0; I--)
 {
  NTemp[0] = NumA[I];
  _numAdd(N, NTemp, N);
  _numMul(N, NWeight, N, 0, _numDummy);
 }
 NTemp[0] = NumA[0];
 _numAdd(N, NTemp, N);
}

// Read number sign and absolute value into array
numByte numGetArray(numPtr(N), numElementType * NumA, numElementType Weight, numIOArrayIterator Count)
{
 numDef(NX);
 numDef(NWeight);
 numDef(NTemp);
 numDef(NTempX);
 numIOArrayIterator I;
 numByte S;

 numZero(NWeight);
 numZero(NTemp);
 NWeight[0] = Weight;
 numCopy(N, NX);
 S = numSignGetAbs(NX);

 for (I = 0; I < Count; I++)
 {
  if (_numCompare(NX, NWeight) > 0)
  {
   numZero(_numDummy);
   _numDiv(NX, NWeight, NTempX, 0, _numDummy, 0);
   _numMul(NTempX, NWeight, NTempX, 0, _numDummy);
   _numSub(NX, NTempX, NTemp);
   NumA[I] = NTemp[0];
   _numDiv(NX, NWeight, NX, 0, _numDummy, 0);
  }
  else
  {
   NumA[I] = NX[0];
   I++;
   while (I < Count)
   {
    NumA[I] = 0;
    I++;
   }
  }
 }

 return S;
}

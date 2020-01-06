// Multiply number by 10^I (I > 0)
void _numMul10(numPtr(N), numIteratorSignedType I)
{
 while (I != 0)
 {
  _numMul(N, _numShift, N, 0, _numDummy);
  I--;
 }
}

// Multiply number by 10^I (I > 0)
void numMul10(numPtr(N), numIteratorSignedType I)
{
 numByte S;
 S = numSignGetAbs(N);
 _numMul10(N, I);
 numSignSet(N, S);
}

// Divide number by 10^I (I > 0)
void _numDiv10(numPtr(N), numIteratorSignedType I)
{
 #ifdef numDiv10ByMul
  numDef(X);
  numCopy(_numShift, X);
  I--;
  while (I != 0)
  {
   _numMul(X, _numShift, X, 0, _numDummy);
   I--;
  }
  _numDiv(N, X, N, 0, _numDummy, 0);
 #else
  while (I != 0)
  {
   numZero(_numDummy);
   _numDiv(N, _numShift, N, 0, _numDummy, 0);
   I--;
  }
 #endif
}

// Divide number by 10^I (I > 0)
void numDiv10(numPtr(N), numIteratorSignedType I)
{
 numByte S;
 S = numSignGetAbs(N);
 _numDiv10(N, I);
 numSignSet(N, S);
}

// Round number
void numRound(numPtr(N), numIteratorSignedType I, numByte Corr)
{
 I = numDecimal - I;
 if (Corr)
 {
  I--;
  Corr = numSignGetAbs(N);
  _numDiv10(N, I);
  numZero(_numDummy);
  _numDiv(N, _numShift, N, 0, _numDummy, 1);
  I++;
  _numMul10(N, I);
  numSignSet(N, Corr);
 }
 else
 {
  Corr = numSignGetAbs(N);
  _numDiv10(N, I);
  _numMul10(N, I);
  numSignSet(N, Corr);
 }
}

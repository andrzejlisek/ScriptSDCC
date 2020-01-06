// Number compare
// N1 = N2 ==> 0
// N1 > N2 ==> 1
// N1 < N2 ==> -1
numByte numCompare(numPtr(N1), numPtr(N2))
{
 numByte Sign1 = numSignGetAbs(N1);
 numByte Sign2 = numSignGetAbs(N2);
 numByte X = 0;

 if (Sign1 == Sign2)
 {
  X = _numCompare(N1, N2);
  if ((Sign1) && (X != 0))
  {
   if (X == 1)
   {
    X = -1;
   }
   else
   {
    X = 1;
   }
  }
 }
 else
 {
  if ((!Sign1) && (Sign2))
  {
   X = 1;
  }
  else
  {
   X = -1;
  }
 }

 numSignSet(N1, Sign1);
 numSignSet(N2, Sign2);
 return X;
}

// Number add
void numAdd(numPtr(N1), numPtr(N2), numPtr(N3))
{
 numByte Sign1 = numSignGetAbs(N1);
 numByte Sign2 = numSignGetAbs(N2);
 if (Sign1 == Sign2)
 {
  _numAdd(N1, N2, N3);
  numSignSet(N1, Sign1);
  numSignSet(N2, Sign2);
  numSignSet(N3, Sign1);
 }
 else
 {
  if (_numCompare(N1, N2) < 0)
  {
   _numSub(N2, N1, N3);
   numSignSet(N1, Sign1);
   numSignSet(N2, Sign2);
   numSignSet(N3, Sign2);
  }
  else
  {
   _numSub(N1, N2, N3);
   numSignSet(N1, Sign1);
   numSignSet(N2, Sign2);
   numSignSet(N3, Sign1);
  }
 }
}

// Number subtract
void numSub(numPtr(N1), numPtr(N2), numPtr(N3))
{
 numByte Sign1 = numSignGetAbs(N1);
 numByte Sign2 = numSignGetAbs(N2);
 if (Sign1 == Sign2)
 {
  if (_numCompare(N1, N2) < 0)
  {
   _numSub(N2, N1, N3);
   numSignSet(N1, Sign1);
   numSignSet(N2, Sign2);
   numSignSet(N3, 1 - Sign1);
  }
  else
  {
   _numSub(N1, N2, N3);
   numSignSet(N1, Sign1);
   numSignSet(N2, Sign2);
   numSignSet(N3, Sign1);
  }
 }
 else
 {
  _numAdd(N1, N2, N3);
  numSignSet(N1, Sign1);
  numSignSet(N2, Sign2);
  numSignSet(N3, Sign1);
 }
}

// Num multiply
void numMul(numPtr(N1), numPtr(N2), numPtr(N3))
{
 numByte Sign1 = numSignGetAbs(N1);
 numByte Sign2 = numSignGetAbs(N2);
 numZero(_numDummy);
 if (_numMul(N1, N2, N3, 1, _numDummy))
 {
  numSignSet(N1, Sign1);
  numSignSet(N2, Sign2);
  if (Sign1 == Sign2)
  {
   numSignSet(N3, 0);
  }
  else
  {
   numSignSet(N3, 1);
  }
 }
 else
 {
  numSignSet(N1, Sign1);
  numSignSet(N2, Sign2);
 }
}

// Num divide
void numDiv(numPtr(N1), numPtr(N2), numPtr(N3))
{
 numByte Sign1 = numSignGetAbs(N1);
 numByte Sign2 = numSignGetAbs(N2);
 numZero(_numDummy);
 if (_numDiv(N1, N2, N3, 1, _numDummy, 1))
 {
  numSignSet(N1, Sign1);
  numSignSet(N2, Sign2);
  if (Sign1 == Sign2)
  {
   numSignSet(N3, 0);
  }
  else
  {
   numSignSet(N3, 1);
  }
 }
 else
 {
  numSignSet(N1, Sign1);
  numSignSet(N2, Sign2);
 }
}

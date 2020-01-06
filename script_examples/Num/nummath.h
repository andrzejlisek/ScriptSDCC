// Power of natural exponent (uses fast power calculating algorithm)
void _numExp(numPtr(X), numPtr(N), numPtr(Val))
{
 numDef(X0);
 numDef(N0);
 numDef(NZero);
 numDef(Val0);
 int I = numBitsAll;

 // Calculating exponent into binary integer number
 numZero(N0);
 numZero(_numDummy);
 _numDiv(N, numUnit, N0, 0, _numDummy, 1);

 // Zero valued number used to compare
 numZero(NZero);

 // Returning 1 value in case of 0 exponent
 if (numCompare(N0, NZero) == 0)
 {
  numCopy(numUnit, Val);
  return;
 }

 // Fast computing algorithm using binary form of exponent
 numCopy(numUnit, Val);
 numCopy(X, X0);
 while (numCompare(N0, NZero) != 0)
 {
  if (_numGetLSB(N0))
  {
   // Val = X0 * Val
   // X0 = X0 * X0
   numMul(Val, X0, Val0);
   numCopy(Val0, Val);
   numMul(X0, X0, Val0);
   numCopy(Val0, X0);
  }
  else
  {
   // X0 = X0 * X0
   numMul(X0, X0, Val0);
   numCopy(Val0, X0);
  }
  _numShiftRight(N0);
 }

 // Invert value sign if the base is negative
 if (numSignGet(X))
 {
  if (numCompare(N, numUnit) != 0)
  {
   numSignGetInv(Val);
  }
 }
}

// Power of integer exponent
void numExp(numPtr(X), numPtr(N), numPtr(Val))
{
 numDef(Nx);
 if (numSignGet(N))
 {
  numCopy(N, Nx);
  numSignGetAbs(Nx);
  _numExp(X, Nx, Val);
  numDiv(numUnit, Val, Nx);
  numCopy(Nx, Val);
 }
 else
 {
  _numExp(X, N, Val);
 }
}

// Root of natural degree
// It begins of Val[0], which equals X/N (this value can be any of number between 0 and X)
// The next value of formula is calculates many times until Val[i+1] will be equal Val[i]
// Val[i+1] = (1 / N) * ((N - 1) * Val[i] + X / (Val[i] ^ (N - 1)))
void _numRoot(numPtr(X), numPtr(N), numPtr(Val))
{
 numDef(Nx);
 numDef(N1);
 numDef(Val0);
 numDef(T1);
 numDef(T2);
 numDiv(numUnit, N, Nx);
 numSub(N, numUnit, N1);
 numZero(Val0);
 numDiv(X, N, Val);
 while (numCompare(Val, Val0) != 0)
 {
  // Saving current value to compare
  numCopy(Val, Val0);

  // The first summand in brackets
  numMul(Val, N1, T1);

  // The power in fraction of the second sum element
  numExp(Val, N1, T2);

  // The second summand
  numDiv(X, T2, Val);

  // Calculating sum
  numAdd(T1, Val, T2);

  // Multiplying the sum by inversion of degree
  numMul(Nx, T2, Val);
 }
}

// Root of integer degree
void numRoot(numPtr(X), numPtr(N), numPtr(Val))
{
 numDef(Nx);
 if (numSignGet(N))
 {
  numCopy(N, Nx);
  numSignGetAbs(Nx);
  _numRoot(X, Nx, Val);
  numDiv(numUnit, Val, Nx);
  numCopy(Nx, Val);
 }
 else
 {
  _numRoot(X, N, Val);
 }
}
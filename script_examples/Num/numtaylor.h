// Array of factorial values
numDef(numTaylorFact[numTaylorSize * 2 + 1]);

// Array of (((n!))^2)*(4^n) values
numDef(numTaylorFact2[numTaylorSize + 3]);

// Sine from Taylor serie
void numSin(numPtr(Arg), numPtr(Val))
{
 numDef(S);
 numDef(P);
 numTaylorIteratorType I, J;
 J = 3;
 numCopy(Arg, Val);
 numCopy(numUnit, P);
 numMul(P, Arg, P);
 for (I = 1; I < numTaylorSize; I++)
 {
  numMul(P, Arg, P);
  numMul(P, Arg, P);
  numDiv(P, numTaylorFact[J], S);
  if (I & 1)
  {
   numSub(Val, S, Val);
  }
  else
  {
   numAdd(Val, S, Val);
  }
  J += 2;
 }
}

// Cosine from Taylor serie
void numCos(numPtr(Arg), numPtr(Val))
{
 numDef(S);
 numDef(P);
 numTaylorIteratorType I, J;
 J = 2;
 numCopy(numUnit, Val);
 numCopy(numUnit, P);
 for (I = 1; I < numTaylorSize; I++)
 {
  numMul(P, Arg, P);
  numMul(P, Arg, P);
  numDiv(P, numTaylorFact[J], S);
  if (I & 1)
  {
   numSub(Val, S, Val);
  }
  else
  {
   numAdd(Val, S, Val);
  }
  J += 2;
 }
}

// Tangent as sine divided by cosine
void numTan(numPtr(Arg), numPtr(Val))
{
 numDef(NSin);
 numDef(NCos);
 numSin(Arg, NSin);
 numCos(Arg, NCos);
 numDiv(NSin, NCos, Val);
}

// Cotangent as cosine divided by sine
void numCtg(numPtr(Arg), numPtr(Val))
{
 numDef(NSin);
 numDef(NCos);
 numSin(Arg, NSin);
 numCos(Arg, NCos);
 numDiv(NCos, NSin, Val);
}

// Secant as 1 divided by cosine
void numSec(numPtr(Arg), numPtr(Val))
{
 numDef(NCos);
 numCos(Arg, NCos);
 numDiv(numUnit, NCos, Val);
}

// Cosecant as 1 divided by sine
void numCsc(numPtr(Arg), numPtr(Val))
{
 numDef(NSin);
 numSin(Arg, NSin);
 numDiv(numUnit, NSin, Val);
}

// Filling in numTaylorFact i numTaylorFact2 arrays
void numInitTaylor()
{
 numTaylorIteratorType I;
 numDef(NFactor);
 numDef(NTemp);
 numDef(NTempX);
 numCopy(numUnit, NFactor);
 numCopy(numUnit, numTaylorFact[0]);
 numCopy(numUnit, numTaylorFact[1]);
 for (I = 2; I < (numTaylorSize * 2 + 1); I++)
 {
  numAdd(NFactor, numUnit, NFactor);
  numMul(numTaylorFact[I - 1], NFactor, numTaylorFact[I]);
 }
 numZero(NTemp);
 NTemp[0] = 4;
 _numMul(NTemp, numUnit, NFactor, 0, _numDummy);
 numCopy(numUnit, numTaylorFact2[0]);
 numCopy(NFactor, numTaylorFact2[1]);
 for (I = 2; I <= (numTaylorSize + 2); I++)
 {
  numMul(numTaylorFact2[I - 1], NFactor, numTaylorFact2[I]);
  numMul(numTaylorFact[I - 2], numTaylorFact[I - 2], NTempX);
  numMul(NTempX, numTaylorFact2[I - 2], NTemp);
  numCopy(NTemp, numTaylorFact2[I - 2]);
 }
}

// Print number value bits grouped by 4
void _numGetX(numPtr(N), numIteratorType I, numByte NoSplit, numChar * Val, int * CharIterator)
{
 numElementType BitPointer = (numElementType)numElementSize;
 int IX = numBits;
 if (NoSplit)
 {
  BitPointer = BitPointer >> 1;
  IX--;
 }
 while (IX >= 0)
 {
  Val[*CharIterator] = (N[I] & BitPointer ? '1' : '0');
  *CharIterator = *CharIterator + 1;
  if (!NoSplit)
  {
   if (((IX & 3) == 0) && (IX > 0))
   {
    Val[*CharIterator] = ' ';
    *CharIterator = *CharIterator + 1;
   }
  }
  BitPointer = BitPointer >> 1;
  IX--;
 }
}

// Print number raw value
void numGetRaw(numPtr(N), numByte NoSplit, numChar * Val)
{
 int CharIterator = 0;
 numIteratorType I = numElements - 1;
 while (I != 0)
 {
  _numGetX(N, I, NoSplit, Val, &CharIterator);
  if (!NoSplit)
  {
   Val[CharIterator] = '|';
   CharIterator++;
  }
  I--;
 }
 _numGetX(N, 0, NoSplit, Val, &CharIterator);
 Val[CharIterator] = 0;
}

// Set number value based on bit sequence
void numSetBits(numPtr(N), numChar * Val)
{
 numBitIteratorType L = 0;
 while (Val[L] != 0)
 {
  _numShiftLeft(N);
  if (Val[L] == '1')
  {
   N[0]++;
  }
  L++;
 }
}

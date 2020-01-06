#include "core.h"
#include "console.h"

#include "numdef.h"
#include "numcore.h"
#include "numbase.h"
#include "numio.h"
#include "numtaylor.h"
#include "numround.h"
#include "nummath.h"
#include "numdebug.h"

#define NumStackSize 50

#define CONST_PI "3.1415926535897932384626433832795"
#define CONST_E "2.7182818284590452353602874713527"

char Temp[1000];
numElementType Num1[10];
numElementType Num2[10];

numDef(NumReg[26]);
numDef(NumStack[NumStackSize + 1]);
uchar NumStackI;
char DefReg = 23;

uint NumStackPointer;

#define CmdBufSize 200
uchar CmdBuf[CmdBufSize + 1];
int I, II;

char PrintRaw = 0;

void numPrint(numPtr(N))
{
 if (PrintRaw)
 {
  numGetRaw(N, 0, Temp);
 }
 else
 {
  numGet(N, Temp);
 }
 console_print_string(0, Temp);
 console_print_line(0);
}

uchar CommandCompare(uchar * CmdWord)
{
 int I = 0;
 if (CmdBuf[0] == 0)
 {
  return 0;
 }
 while ((CmdBuf[I] != ' ') && (CmdBuf[I] != 0) && (CmdWord[I] != 0))
 {
  if (CmdBuf[I] != CmdWord[I])
  {
   return 0;
  }
  I++;
 }
 if ((CmdWord[I] == 0) && ((CmdBuf[I] == ' ') || (CmdBuf[I] == 0)))
 {
  return 1;
 }
 return 0;
}

void printHelp()
{
 console_print_string(0, "C formula - calculate formula and save into answer variable");
 console_print_line(0);
 console_print_string(0, "   0..9 - digits");
 console_print_line(0);
 console_print_string(0, "   . - decimal point, it can be input using \".\" or \",\"");
 console_print_line(0);
 console_print_string(0, "   \\ - negative number sign, it can be input using \"\\\" or \"`\"");
 console_print_line(0);
 console_print_string(0, "   +-*/ - binary operators");
 console_print_line(0);
 console_print_string(0, "   A..Z - one letter is variable, mote than one letter is function");
 console_print_line(0);
 console_print_string(0, "   ; - argument separator in function");
 console_print_line(0);
 console_print_string(0, "   ( - opening bracket, it can be input using \"(\" or \"[\" or \"{\"");
 console_print_line(0);
 console_print_string(0, "   ) - closing bracket, it can be input using \")\" or \"]\" or \"}\"");
 console_print_line(0);
 console_print_string(0, "   Spaces are ignored, letters are case insensitive");
 console_print_line(0);
 console_print_string(0, "   Unary functions: SIN, COS, TAN, CTG, SEC, CSC");
 console_print_line(0);
 console_print_string(0, "   Binary functions: EXP, ROOT");
 console_print_line(0);
 console_print_string(0, "   Function arguments must be in brackets, for example SIN(1), ROOT(2;2)");
 console_print_line(0);
 console_print_string(0, "   The calculation result is saved in answer variable, command examples:");
 console_print_line(0);
 console_print_string(0, "   C 10");
 console_print_line(0);
 console_print_string(0, "   C 6*5");
 console_print_line(0);
 console_print_string(0, "   C (3/2)*SIN(1)+COS(2)");
 console_print_line(0);
 console_print_string(0, "   C A*ROOT(B;2)/ROOT(3;2)");
 console_print_line(0);
 console_print_string(0, "T formula - works same as C, but additionally displays token list and RPN form,");
 console_print_line(0);
 console_print_string(0, "   intended to test formula parsing");
 console_print_line(0);
 console_print_string(0, "S - clear console screen");
 console_print_line(0);
 console_print_string(0, "A - get current answer variable");
 console_print_line(0);
 console_print_string(0, "A variable - set variable as answer");
 console_print_line(0);
 console_print_string(0, "D variable - display specified variable");
 console_print_line(0);
 console_print_string(0, "D - display all variables");
 console_print_line(0);
 console_print_string(0, "H - display help");
 console_print_line(0);
 console_print_string(0, "R - switch between number display and raw display");
 console_print_line(0);
 console_print_string(0, "Answer variable: ");
 console_print_uchar_t(0, DefReg + 65);
 console_print_string(0, " = ");
 numPrint(NumReg[DefReg]);
}

#define TokenBufSize 250
char TokenBuf[TokenBufSize];
uchar TokenBufI;
uchar TokenI;

#define TokenStackSize 20
char TokenStack[TokenBufSize];
char TokenStackS[TokenStackSize];
uchar TokenStackB;
uchar TokenStackI;


uchar TokenCompare(uchar * TokenWord)
{
 int I = 0;
 while ((TokenBuf[I] != 0) && (TokenWord[I] != 0))
 {
  if (TokenBuf[I] != TokenWord[I])
  {
   return 0;
  }
  I++;
 }
 if ((TokenBuf[I] == 0) && (TokenWord[I] == 0))
 {
  return 1;
 }
 else
 {
  return 0;
 }
}

void CalcWork(uchar Test, uchar Op)
{
 uchar StackErr = 0;
 if (Test)
 {
  console_print_string(0, "                    ");
  console_print_uchar_t(0, '[');
  console_print_uchar_v(0, Op);
  console_print_uchar_t(0, ']');
  console_print_uchar_t(0, '[');
  console_print_string(0, TokenBuf);
  console_print_uchar_t(0, ']');
  console_print_line(0);
 }

 // Operator or function
 if (Op == 0)
 {
  if (TokenBuf[1] == 0)
  {
   if (NumStackI >= 2)
   {
    switch (TokenBuf[0])
    {
     case '+':
      NumStackI--;
      numAdd(NumStack[NumStackI - 1], NumStack[NumStackI], NumStack[NumStackI + 1]);
      numCopy(NumStack[NumStackI + 1], NumStack[NumStackI - 1]);
      break;
     case '-':
      NumStackI--;
      numSub(NumStack[NumStackI - 1], NumStack[NumStackI], NumStack[NumStackI + 1]);
      numCopy(NumStack[NumStackI + 1], NumStack[NumStackI - 1]);
      break;
     case '*':
      NumStackI--;
      numMul(NumStack[NumStackI - 1], NumStack[NumStackI], NumStack[NumStackI + 1]);
      numCopy(NumStack[NumStackI + 1], NumStack[NumStackI - 1]);
      break;
     case '/':
      NumStackI--;
      numDiv(NumStack[NumStackI - 1], NumStack[NumStackI], NumStack[NumStackI + 1]);
      numCopy(NumStack[NumStackI + 1], NumStack[NumStackI - 1]);
      break;
    }
   }
   else
   {
    if ((TokenBuf[0] == '+') | (TokenBuf[0] == '-') | (TokenBuf[0] == '*') | (TokenBuf[0] == '/'))
    {
     console_print_string(0, "Stack underflow by operator: ");
     console_print_schar_t(0, TokenBuf[0]);
     console_print_line(0);
    }
   }
  }
  else
  {
   StackErr = 1;
   if (NumStackI >= 1)
   {
    if (TokenCompare("SIN"))
    {
     StackErr = 0;
     numSin(NumStack[NumStackI - 1], NumStack[NumStackI]);
     numCopy(NumStack[NumStackI], NumStack[NumStackI - 1]);
    }
    if (TokenCompare("COS"))
    {
     StackErr = 0;
     numCos(NumStack[NumStackI - 1], NumStack[NumStackI]);
     numCopy(NumStack[NumStackI], NumStack[NumStackI - 1]);
    }
    if (TokenCompare("TAN"))
    {
     StackErr = 0;
     numTan(NumStack[NumStackI - 1], NumStack[NumStackI]);
     numCopy(NumStack[NumStackI], NumStack[NumStackI - 1]);
    }
    if (TokenCompare("CTG"))
    {
     StackErr = 0;
     numCtg(NumStack[NumStackI - 1], NumStack[NumStackI]);
     numCopy(NumStack[NumStackI], NumStack[NumStackI - 1]);
    }
    if (TokenCompare("SEC"))
    {
     StackErr = 0;
     numSec(NumStack[NumStackI - 1], NumStack[NumStackI]);
     numCopy(NumStack[NumStackI], NumStack[NumStackI - 1]);
    }
    if (TokenCompare("CSC"))
    {
     StackErr = 0;
     numCsc(NumStack[NumStackI - 1], NumStack[NumStackI]);
     numCopy(NumStack[NumStackI], NumStack[NumStackI - 1]);
    }
   }
   if (NumStackI >= 2)
   {
    if (TokenCompare("EXP"))
    {
     StackErr = 0;
     NumStackI--;
     numExp(NumStack[NumStackI - 1], NumStack[NumStackI], NumStack[NumStackI + 1]);
     numCopy(NumStack[NumStackI + 1], NumStack[NumStackI - 1]);
    }
    if (TokenCompare("ROOT"))
    {
     StackErr = 0;
     NumStackI--;
     numRoot(NumStack[NumStackI - 1], NumStack[NumStackI], NumStack[NumStackI + 1]);
     numCopy(NumStack[NumStackI + 1], NumStack[NumStackI - 1]);
    }
   }
   if (StackErr)
   {
    console_print_string(0, "Stack underflow or unsupported function: ");
    console_print_string(0, TokenBuf);
    console_print_line(0);
   }
  }
 }

 // Number
 if (Op == 1)
 {
  if (NumStackI < NumStackSize)
  {
   numSet(NumStack[NumStackI], TokenBuf);
   NumStackI++;
  }
  else
  {
   console_print_string(0, "Stack overflow by number: ");
   console_print_string(0, TokenBuf);
   console_print_line(0);
  }
 }

 // Variable
 if (Op == 2)
 {
  if (NumStackI < NumStackSize)
  {
   numCopy(NumReg[TokenBuf[0] - 65], NumStack[NumStackI]);
   NumStackI++;
  }
  else
  {
   console_print_string(0, "Stack overflow by variable: ");
   console_print_string(0, TokenBuf);
   console_print_line(0);
  }
 }
}

void TokenStackPush()
{
 uchar I = 0;
 while (TokenBuf[I] != 0)
 {
  TokenStack[TokenStackB] = TokenBuf[I];
  I++;
  TokenStackB++;
 }
 TokenStackS[TokenStackI] = I;
 TokenStackI++;
}

void TokenStackPop()
{
 TokenStackI--;
 uchar L = TokenStackS[TokenStackI];
 uchar B = TokenStackB - L;
 uchar I = 0;
 TokenStackB -= L;
 while (L > 0)
 {
  TokenBuf[I] = TokenStack[B];
  I++;
  L--;
  B++;
 }
 TokenBuf[I] = 0;
}

uchar TokenStackTop()
{
 return TokenStack[TokenStackB - TokenStackS[TokenStackI - 1]];
}

void Calc(uchar Test)
{
 uchar II;
 
 uchar TokenType = 0;
 uchar TokenChar = 0;
 uchar TokenCharX = 0;
 uchar TokenWork = 1;

 NumStackI = 0;

 TokenI = 1;
 TokenBufI = 0;

 // Convert to reverse Polish notation and process
 TokenStackB = 0;
 TokenStackI = 0;
 while (TokenType != 10)
 {
  // Get token
  TokenBufI = 0;
  TokenType = 0;
  TokenWork = 1;
  while (TokenWork)
  {
   TokenChar = 0;
   TokenCharX = 0;
   while (TokenChar == 0)
   {
    TokenI++;
    if ((CmdBuf[TokenI] >= 'A') && (CmdBuf[TokenI] <= 'Z')) { TokenChar = 1; }
    if (((CmdBuf[TokenI] >= '0') && (CmdBuf[TokenI] <= '9')) || (CmdBuf[TokenI] == '.') || (CmdBuf[TokenI] == '\\')) { TokenChar = 2; }
    if ((CmdBuf[TokenI] == ';') || (CmdBuf[TokenI] == '(') || (CmdBuf[TokenI] == ')') || (CmdBuf[TokenI] == '+') || (CmdBuf[TokenI] == '-') || (CmdBuf[TokenI] == '*') || (CmdBuf[TokenI] == '/'))  { TokenChar = 3; }
    if ((CmdBuf[TokenI] == 0))  { TokenChar = 10; }
    if (TokenChar != 10)
    {
     II = 0;
     TokenCharX = 0;
     while (TokenCharX == 0)
     {
      TokenI++;
      II++;
      if ((CmdBuf[TokenI] >= 'A') && (CmdBuf[TokenI] <= 'Z')) { TokenCharX = 1; }
      if (((CmdBuf[TokenI] >= '0') && (CmdBuf[TokenI] <= '9')) || (CmdBuf[TokenI] == '.') || (CmdBuf[TokenI] == '\\')) { TokenCharX = 2; }
      if ((CmdBuf[TokenI] == ';') || (CmdBuf[TokenI] == '(') || (CmdBuf[TokenI] == ')') || (CmdBuf[TokenI] == '+') || (CmdBuf[TokenI] == '-') || (CmdBuf[TokenI] == '*') || (CmdBuf[TokenI] == '/'))  { TokenCharX = 3; }
      if ((CmdBuf[TokenI] == 0))  { TokenCharX = 10; }
     }
     TokenI -= II;
    }
   }
   TokenBuf[TokenBufI] = CmdBuf[TokenI];
   TokenBufI++;
   TokenBuf[TokenBufI] = 0;
   switch (TokenChar)
   {
    case 1:
     if ((TokenType == 0) || (TokenType == 1))
     {
      TokenType = 1;
     }
     if (TokenCharX != 1)
     {
      TokenWork = 0;
     }
     break;
    case 2:
     if (TokenBuf[TokenBufI - 1] == '\\')
     {
      TokenBuf[TokenBufI - 1] = '-';
     }
     if ((TokenType == 0) || (TokenType == 2))
     {
      TokenType = 2;
     }
     if (TokenCharX != 2)
     {
      TokenWork = 0;
     }
     break;
    case 3:
     {
      TokenType = 3;
      TokenWork = 0;
     }
     break;
    case 10:
     {
      TokenType = 10;
      TokenWork = 0;
     }
     break;
   }
  }

  // Print token
  if (Test)
  {
   if (TokenType != 10)
   {
    console_print_uchar_t(0, '[');
    console_print_uchar_v(0, TokenType);
    console_print_uchar_t(0, ']');
    console_print_uchar_t(0, '[');
    console_print_string(0, TokenBuf);
    console_print_uchar_t(0, ']');
    console_print_line(0);
   }
  }

  // ### Shunting yard algorithm ###
  
  // If token is a number then put to the output
  if (TokenType == 2)
  {
   CalcWork(Test, 1);
  }
  if ((TokenType == 1) && (TokenBufI == 1))
  {
   CalcWork(Test, 2);
  }

  // If token is a function then put onto the stack
  if ((TokenType == 1) && (TokenBufI > 1))
  {
   TokenStackPush();
  }

  // If token is a function argument separator ";", then:
  //  - While the top element on the stack is not "(", pop the element and put to the output
  //  - If the stack does not contain "(", raise syntax error
  if ((TokenType == 3) && (TokenBuf[0] == ';'))
  {
   while ((TokenStackI > 0) && (TokenStackTop() != '('))
   {
    TokenStackPop();
    CalcWork(Test, 0);
   }
  }
  
  // If token is a operator O:
  //  - While the top element on the stack is operator OX:
  //     - O priority is lower than OX priority, pop OX and put on the output
  //  - Put O on the stack
  if ((TokenType == 3) && (TokenBuf[0] != ';') && (TokenBuf[0] != '(') && (TokenBuf[0] != ')'))
  {
   II = TokenBuf[0];
   if ((II == '+') || (II == '-'))
   {
    while ((TokenStackI > 0) && ((TokenStackTop() == '*') || (TokenStackTop() == '/')))
    {
     TokenStackPop();
     CalcWork(Test, 0);
    }
   }

   TokenBuf[0] = II;
   TokenBuf[1] = 0;
   TokenStackPush();
  }

  // If token is "(", put on the stack
  if ((TokenType == 3) && (TokenBuf[0] == '('))
  {
   TokenStackPush();
  }

  // If token is ")":
  //  - Pop operator from stack and put on the output while there is not "("
  //  - If "(" does not exist on the stack, raise syntax error
  //  - Pop "(" from the stack
  //  - If the top element is function, put to the output
  if ((TokenType == 3) && (TokenBuf[0] == ')'))
  {
   while ((TokenStackI > 0) && (TokenStackTop() != '('))
   {
    TokenStackPop();
    CalcWork(Test, 0);
   }
   if (TokenStackI > 0)
   {
    TokenStackPop();
   }
   if (TokenStackI > 0)
   {
    if ((TokenStackTop() >= 'A') && (TokenStackTop() <= 'Z'))
    {
     TokenStackPop();
     CalcWork(Test, 0);
    }
   }
  }
 }

 // Pop all operators from the stack and put on the output, if there is "(" or ")" raise syntax error
 while (TokenStackI > 0)
 {
  TokenStackPop();
  if ((TokenBuf[0] != '(') && (TokenBuf[0] != ')'))
  {
   CalcWork(Test, 0);
  }
 }

 NumStackI--;
 numCopy(NumStack[NumStackI], NumReg[DefReg]);
}

void main()
{
 uchar Key;
 char GoodChar;
 numInit();
 numInitTaylor();

 console_clear(0);

 numSet(NumReg[4], CONST_E);
 numSet(NumReg[15], CONST_PI);

// numPrint(NumReg[0]);
// numPrint(NumReg[1]);
// numPrint(NumReg[2]);

 printHelp();
 while (1)
 {
  // Getting command from user
  Key = 0;
  console_print_string(0, ">");
  console_input_flush(0);
  I = 0;
  while ((Key != '\r') && (Key != '\n'))
  {
   Key = console_input_uchar(0);
   if ((Key >= 97) && (Key <= 122))
   {
    Key = Key - 32;
   }
   if (Key == ',') { Key = '.'; }
   if (Key == '`') { Key = '\\'; }
   if ((Key == '[') || (Key == '{')) { Key = '('; }
   if ((Key == ']') || (Key == '}')) { Key = ')'; }
   if (Key == 8)
   {
    if (I > 0)
    {
     console_print_line(0);
     console_print_uchar_t(0, '>');
     CmdBuf[I - 1] = 0;
     I = 0;
     while (CmdBuf[I] != 0)
     {
      console_print_uchar_t(0, CmdBuf[I]);
      I++;
     }
    }
   }
   else
   {
    if (I < CmdBufSize)
    {
     GoodChar = 0;
     if ((Key >= 'A') && (Key <= 'Z')) { GoodChar = 1; }
     if ((Key >= '0') && (Key <= '9')) { GoodChar = 1; }
     if ((Key == ';') || (Key == '\\') || (Key == ' ') || (Key == '.') || (Key == '+') || (Key == '-') || (Key == '*') || (Key == '/') || (Key == '(') || (Key == ')')) { GoodChar = 1; }
     if (GoodChar)
     {
      console_print_uchar_t(0, Key);
      CmdBuf[I] = Key;
      I++;
     }
    }
   }
  }
  CmdBuf[I] = 0;
  console_print_line(0);

  GoodChar = 0;

  if (CommandCompare("H"))
  {
   GoodChar = 1;
   printHelp();
  }
  if (CommandCompare("A"))
  {
   GoodChar = 1;
   I = 4;
   while (CmdBuf[I] != 0)
   {
    if ((CmdBuf[I] >= 'A') && (CmdBuf[I] <= 'Z'))
    {
     DefReg = CmdBuf[I] - 65;
    }
    I++;
   }
   console_print_string(0, "Answer variable: ");
   console_print_uchar_t(0, DefReg + 65);
   console_print_string(0, " = ");
   numPrint(NumReg[DefReg]);
  }
  if (CommandCompare("S"))
  {
   GoodChar = 1;
   console_clear(0);
  }
  if (CommandCompare("T"))
  {
   GoodChar = 1;
   console_print_string(0, "Token               RPN formula");
   console_print_line(0);
   Calc(1);
   console_print_uchar_t(0, DefReg + 65);
   console_print_string(0, " = ");
   numPrint(NumReg[DefReg]);
  }
  if (CommandCompare("C"))
  {
   GoodChar = 1;
   Calc(0);
   console_print_uchar_t(0, DefReg + 65);
   console_print_string(0, " = ");
   numPrint(NumReg[DefReg]);
  }
  if (CommandCompare("R"))
  {
   GoodChar = 1;
   PrintRaw = !PrintRaw;
   console_print_uchar_t(0, DefReg + 65);
   console_print_string(0, " = ");
   numPrint(NumReg[DefReg]);
  }
  if (CommandCompare("D"))
  {
   GoodChar = 1;
   II = -1;
   I = 1;
   while (CmdBuf[I] != 0)
   {
    if ((CmdBuf[I] >= 'A') && (CmdBuf[I] <= 'Z'))
    {
     II = CmdBuf[I] - 65;
    }
    I++;
   }
   if (II >= 0)
   {
    console_print_uchar_t(0, II + 65);
    console_print_string(0, " = ");
    numPrint(NumReg[II]);
   }
   else
   {
    I = 0;
    while (I < 26)
    {
     console_print_uchar_t(0, I + 65);
     console_print_string(0, " = ");
     numPrint(NumReg[I]);
     I++;
    }
   }
  }
  if (!GoodChar)
  {
   console_print_string(0, "Unsupported command");
   console_print_line(0);
  }
 }
}

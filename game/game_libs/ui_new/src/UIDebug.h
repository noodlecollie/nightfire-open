#pragma once

#ifdef _DEBUG
void UIDBG_AssertFunction(bool fExpr, const char* szExpr, const char* szFile, int szLine, const char* szMessage);
#define ASSERT(f) UIDBG_AssertFunction(f, #f, __FILE__, __LINE__, NULL)
#define ASSERTSZ(f, sz) UIDBG_AssertFunction(f, #f, __FILE__, __LINE__, sz)
#define ASSERTSZ_Q(f, sz) UIDBG_AssertFunction(f, #f, __FILE__, __LINE__, sz, false)
#else  // !DEBUG
#define ASSERT(f)
#define ASSERTSZ(f, sz)
#define ASSERTSZ_Q(f, sz)
#endif  // !DEBUG

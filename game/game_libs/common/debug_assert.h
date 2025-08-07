#pragma once

#ifdef _DEBUG
void DBG_AssertFunction(
	bool fExpr,
	const char* szExpr,
	const char* szFile,
	int szLine,
	const char* szMessage,
	bool showAlert = true);
#define ASSERT(f) DBG_AssertFunction(f, #f, __FILE__, __LINE__, NULL)
#define ASSERTSZ(f, sz) DBG_AssertFunction(f, #f, __FILE__, __LINE__, sz)
#define ASSERTSZ_Q(f, sz) DBG_AssertFunction(f, #f, __FILE__, __LINE__, sz, false)
#else  // !DEBUG
#define ASSERT(f)
#define ASSERTSZ(f, sz)
#define ASSERTSZ_Q(f, sz)
#endif  // !DEBUG

#pragma once

enum CompileToolsLogLevel
{
	D_INFO = 1,	// "-dev 1", shows various system messages
	D_ERROR,	// "-dev 2", shows critical warnings
	D_WARN,		// "-dev 3", shows not critical system warnings
	D_REPORT,	// "-dev 4", show system reports for advanced users and engine developers
	D_NOTE		// "-dev 5", show system notifications for engine developers
};

#pragma once

#define NUM_AMBIENTS 4  // automatic ambient sounds
#define MAX_INFO_STRING 256  // infostrings are transmitted across network
#define MAX_SERVERINFO_STRING 512  // server handles too many settings. expand to 1024?
#define MAX_LOCALINFO_STRING 32768  // localinfo used on server and not sended to the clients
#define MAX_SYSPATH 1024  // system filepath
#define MAX_VA_STRING 1024  // string length returned by va()
#define MAX_PRINT_MSG 8192  // how many symbols can handle single call of Con_Printf or Con_DPrintf
#define MAX_TOKEN 2048  // parse token length
#define MAX_MODS 512  // environment games that engine can keep visible
#define MAX_USERMSG_LENGTH 2048  // don't modify it's relies on a client-side definitions

// config strings are a general means of communication from
// the server to all connected clients.
// each config string can be at most CS_SIZE characters.

#define MAX_QPATH 64  // max length of a game pathname
#define MAX_OSPATH 260  // max length of a filesystem pathname
#define CS_SIZE 64  // size of one config string
#define CS_TIME 16  // size of time string

// This used to be defined based on the number of network bits used to represent
// the edicts, but is now decoupled from the netcode definitions.
// If this number is changed, protocol.h will need to be updated (there is a
// compile-time check there that should trigger anyway).
#define MAX_EDICTS 8192
#define MIN_EDICTS 64

#if MAX_EDICTS < MIN_EDICTS
#error MAX_EDICTS cannot be less than MIN_EDICTS!
#endif

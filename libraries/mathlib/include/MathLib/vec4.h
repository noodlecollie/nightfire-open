#pragma once

#define Vector4Copy(a, b) ((b)[0] = (a)[0], (b)[1] = (a)[1], (b)[2] = (a)[2], (b)[3] = (a)[3])
#define Vector4Set(v, a, b, c, d) ((v)[0] = (a), (v)[1] = (b), (v)[2] = (c), (v)[3] = (d))

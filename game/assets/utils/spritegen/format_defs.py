SPRITE_HEADER_IDENT = ((b'P'[0] << 24) + (b'S'[0] << 16) + (b'D'[0] << 8) + b'I'[0])

SPRITE_SYNCTYPE_SYNC = 0
SPRITE_SYNCTYPE_RAND = 1

SPRITE_FRAMETYPE_SINGLE = 0
SPRITE_FRAMETYPE_GROUP = 1
SPRITE_FRAMETYPE_ANGLED = 2

SPRITE_DRWTYPE_NORMAL = 0
SPRITE_DRWTYPE_ADDITIVE = 1
SPRITE_DRWTYPE_INDEXALPHA = 2
SPRITE_DRWTYPE_ALPHTEST = 3

SPRITE_ANGLETYPE_FWD_PARALLEL_UPRIGHT = 0
SPRITE_ANGLETYPE_FACING_UPRIGHT = 1
SPRITE_ANGLETYPE_FWD_PARALLEL = 2
SPRITE_ANGLETYPE_ORIENTED = 3
SPRITE_ANGLETYPE_FWD_PARALLEL_ORIENTED = 4

SPRITE_CULL_FRONT = 0
SPRITE_CULL_NONE = 1

SPRITE_VERSION_Q1 = 1
SPRITE_VERSION_HL = 2
SPRITE_VERSION_32 = 32

SPRITE_GENERIC_HEADER_FORMAT = "".join((
	"i",	# "ISPR" identifier
	"i",	# Version
))

SPRITE_QUAKE_HEADER_FORMAT = "".join((
	SPRITE_GENERIC_HEADER_FORMAT,
	"i",	# Type
	"f",	# Bounding radius
	"ii",	# Min/max bounds
	"i",	# Num frames
	"f",	# Beam length (usage unknown)
	"i",	# Sync type
))

SPRITE_HALFLIFE_HEADER_FORMAT = "".join((
	SPRITE_GENERIC_HEADER_FORMAT,
	"i",	# Type
	"i",	# Texture format
	"i",	# Bounding radius
	"ii",	# Min/max bounds
	"i",	# Num frames
	"i",	# Face type (for culling, Xash3d extension)
	"i",	# Sync type
))

SPRITE_FRAME_FORMAT = "".join((
	"ii",	# Origin
	"i",	# Width
	"i",	# Height
))

SPRITE_FRAME_GROUP_FORMAT = "".join((
	"i"		# Num frames
))

SPRITE_INTERVAL_FORMAT = "".join((
	"f"		# Interval
))

SPRITE_FRAMETYPE_FORMAT = "".join((
	"i",	# Frame type
))

import struct
import format_defs
import io
from PIL import Image

class SingleFrameSpriteData:
	def __init__(self):
		self.header = None
		self.frameType = None
		self.frame = None
		self.data = None

	def write(self, outFile):
		outFile.write(self.header)
		outFile.write(self.frameType)
		outFile.write(self.frame)
		outFile.write(self.data)

def imageToBytes(imageFile : Image):
	numChannels = 0

	if imageFile.mode == "RGBA":
		numChannels = 4
	elif imageFile.mode == "RGB":
		numChannels = 3
	else:
		raise ValueError(f"Unsupported image format {imageFile.mode}.")

	imageData = imageFile.getdata()
	rawData = bytearray(4 * len(imageData))

	for i in range(0, len(imageData)):
		outIndex = 4 * i
		pixel = imageData[i]

		rawData[outIndex + 0] = pixel[0]
		rawData[outIndex + 1] = pixel[1]
		rawData[outIndex + 2] = pixel[2]

		if numChannels == 4:
			rawData[outIndex + 3] = pixel[3]
		else:
			rawData[outIndex + 3] = 255

	return bytes(rawData)

def createSingleFrameSprite(imageFile : Image):
	# Required:
	# - Header ident = SPRITE_HEADER_IDENT
	# - Version = SPRITE_VERSION_32
	# - Angle type = SPRITE_ANGLETYPE_FWD_PARALLEL (will do for now)
	# - Bounding radius - appears to be specified as a float but interpreted as an int??? Leave as 0.
	# - Min/max (should be image dimensions)
	# - Num frames = 1
	# - Beam length = 0
	# - Sync type = SPRITE_SYNCTYPE_RAND
	# - Frame type = SPRITE_FRAMETYPE_SINGLE

	sprite = SingleFrameSpriteData()

	sprite.header = struct.pack(format_defs.SPRITE_QUAKE_HEADER_FORMAT,
								format_defs.SPRITE_HEADER_IDENT,
								format_defs.SPRITE_VERSION_32,
								format_defs.SPRITE_ANGLETYPE_FWD_PARALLEL,
								0,
								imageFile.width,
								imageFile.height,
								1,
								0,
								format_defs.SPRITE_SYNCTYPE_RAND)

	sprite.frameType = struct.pack(format_defs.SPRITE_FRAMETYPE_FORMAT, format_defs.SPRITE_FRAMETYPE_SINGLE)
	sprite.frame = struct.pack(format_defs.SPRITE_FRAME_FORMAT, 0, 0, imageFile.width, imageFile.height)
	sprite.data = imageToBytes(imageFile)
	return sprite

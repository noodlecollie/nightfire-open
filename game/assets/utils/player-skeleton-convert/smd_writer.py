def __indent(val:int):
	return "  " if val < 10 else " "

def __writeLine(outFile, line):
	outFile.write(line + "\n")

def __writeBones(outFile, bones):
	__writeLine(outFile, "nodes")

	for bone in bones.list():
		__writeLine(outFile, f"{__indent(bone.index())}{bone.index()} \"{bone.name()}\"  {bone.parent().index() if bone.parent() is not None else -1}")

	__writeLine(outFile, "end")

def __writeSkeletonFrameBones(outFile, frame):
	for index in range(0, len(frame)):
		bone = frame[index]
		outString = __indent(index)

		for index in range(0, len(bone)):
			if index == 0:
				outString += str(bone[index])
			else:
				outString += " {0:.6f}".format(bone[index])

		__writeLine(outFile, outString)

def __writeFullSkeleton(outFile, skeleton):
	__writeLine(outFile, "skeleton")

	for frame in skeleton:
		__writeLine(outFile, f"time {frame['number']}")
		__writeSkeletonFrameBones(outFile, frame["bones"])

	__writeLine(outFile, "end")

def __writeSimpleSkeleton(outFile, skeleton):
	__writeFullSkeleton(outFile, [{"number": 0, "bones": skeleton}])

def __writeTriangles(outFile, triangles):
	__writeLine(outFile, "triangles")

	for triangle in triangles:
		__writeLine(outFile, triangle[0])

		for index in range(1, len(triangle)):
			__writeLine(outFile, f"{__indent(index)}" + (" ".join([str(item) for item in triangle[index]])))

	__writeLine(outFile, "end")

def write(fileName, bones, skeleton, triangles=None):
	with open(fileName, "w") as outFile:
		__writeLine(outFile, "version 1")
		__writeBones(outFile, bones)

		if len(skeleton) > 0 and isinstance(skeleton[0], dict):
			__writeFullSkeleton(outFile, skeleton)
		else:
			__writeSimpleSkeleton(outFile, skeleton)

		if triangles is not None:
			__writeTriangles(outFile, triangles)

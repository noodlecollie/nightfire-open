import shlex
from smd_bones import SMDBone, SMDBoneList

class SMDReader():
	def __init__(self, fileName):
		self.__fileName = fileName
		self.__file = None
		self.__currentLine = 0

	def __enter__(self):
		self.__file = open(self.__fileName, "r")
		self.__currentLine = 1
		return self

	def __exit__(self, *args):
		if self.__file is not None:
			self.__file.close()

		self.__currentLine = 0

	def expect(self, expectedLine):
		line = self.__readLine()
		lineNo = line[0]
		text = self.__textNoNewlines(line)

		if text != expectedLine:
			raise ValueError(f"File {self.__fileName}, line {lineNo}: expected '{expectedLine}' but got '{text}'.")

	def readBones(self):
		self.expect("nodes")

		bones = []

		while True:
			line = self.__readLine()
			text = self.__textNoNewlines(line)

			if text == "end":
				break

			try:
				tokens = shlex.split(text)

				if len(tokens) != 3:
					raise ValueError(f"Expected 3 tokens but got {len(tokens)}.")

				parentIndex = int(tokens[2])
				bones.append(SMDBone(int(tokens[0]), tokens[1], bones[parentIndex] if parentIndex >= 0 else None))
			except ValueError as ex:
				raise ValueError(f"File {self.__fileName}, line: {line[0]}: {str(ex)}")
			except TypeError as ex:
				raise TypeError(f"File {self.__fileName}, line: {line[0]}: {str(ex)}")
			except IndexError as ex:
				raise IndexError(f"File {self.__fileName}, line: {line[0]}: {str(ex)}")

		return SMDBoneList(bones)

	def readSkeleton(self):
		return self.__readSkeleton(0)[0]["bones"]

	def readAllSkeletonFrames(self):
		return self.__readSkeleton()

	def readTriangles(self):
		self.expect("triangles")

		triangles = []
		currentTriangle = []

		while True:
			line = self.__readLine()
			text = self.__textNoNewlines(line)

			try:
				if text == "end":
					if len(currentTriangle) > 0:
						raise ValueError(f"Incomplete triangle created ({len(currentTriangle)} components) before end of triangles section.")

					break

				self.__addComponentToTriangle(currentTriangle, text)

				if len(currentTriangle) == 4:
					triangles.append(tuple(currentTriangle))
					currentTriangle = []
			except ValueError as ex:
				raise ValueError(f"File {self.__fileName}, line: {line[0]}: {str(ex)}")

		return triangles

	def __readSkeleton(self, maxFrame=-1):
		self.expect("skeleton")

		skeletonFrames = []

		while True:
			line = self.__readLine()
			text = self.__textNoNewlines(line)

			if text == "end":
				break

			tokens = shlex.split(text)

			try:
				if len(tokens) == 2 and tokens[0] == "time":
					frameNumber = int(tokens[1])

					if maxFrame >= 0 and frameNumber > maxFrame:
						raise ValueError(f"Expected frames up to a max of {maxFrame}, but got frame with number {frameNumber}.")

					item = \
					{
						"number": frameNumber,
						"bones": []
					}

					skeletonFrames.append(item)
				else:
					if len(tokens) != 7:
						raise ValueError(f"Expected 7 tokens for bone position, but got {len(tokens)}.")

					for index in range(0, len(tokens)):
						if index == 0:
							tokens[index] = int(tokens[index])
						else:
							tokens[index] = float(tokens[index])

					item = skeletonFrames[len(skeletonFrames) - 1]
					item["bones"].append(tuple(tokens))
			except ValueError as ex:
				raise ValueError(f"File {self.__fileName}, line: {line[0]}: {str(ex)}")

		return skeletonFrames

	def __addComponentToTriangle(self, triangle, input):
		if len(triangle) >= 4:
			raise ValueError("Triangle defined with more than 4 components (texture + 3 vertices expected).")

		if len(input) < 1:
			raise ValueError("Unexpected empty line.")

		# Maybe not the most intelligent, but it'll do.
		# If first character is not a space, assume it's a texture name.
		if input[0] != " ":
			if len(triangle) != 0:
				raise ValueError("Texture encountered which was not the first component of the triangle.")

			triangle.append(input)
			return

		# Treat as a vertex.
		if len(triangle) == 0:
			raise ValueError("Got a vertex definition for the first component of a triangle, when a texture was expected.")

		tokens = shlex.split(input)

		if len(tokens) != 9:
			raise ValueError(f"Triangle vertex definition contained {len(tokens)} components when 9 were expected.")

		for index in range(0, len(tokens)):
			tokens[index] = int(tokens[index]) if index == 0 else float(tokens[index])

		triangle.append(tuple(tokens))

	def __readLine(self):
		ret = (self.__currentLine, self.__file.readline())

		if ret[1] == "":
			raise OSError(f"Reached end of file {self.__fileName}")

		self.__currentLine += 1
		return ret

	def __textNoNewlines(self, line):
		return line[1].rstrip("\r\n")

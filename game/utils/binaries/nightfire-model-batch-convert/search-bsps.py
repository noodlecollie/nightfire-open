import struct
import argparse
import sys
import os

SCRIPT_DIR = os.path.dirname(os.path.realpath(sys.argv[0]))

def parseArgs():
	parser = argparse.ArgumentParser(description="Searches BSP files in a directory for a specified search string")

	parser.add_argument("-s", "--string",
						help="String to search for",
						type=str,
						required=True)

	parser.add_argument("-d", "--dir",
						help="Directory where BSPs are located",
						type=str,
						required=True)

	return parser.parse_args()

def searchBSP(bspPath:str, searchString:str):
	data = bytearray()

	with open(bspPath, "rb") as inFile:
		data = inFile.read()

	(_, entsOffset, entsLength) = struct.unpack_from("iii", data)
	entsData = str(data[entsOffset : entsOffset + entsLength], "utf-8")
	entsDataLines = entsData.split("\n")

	for index in range(0, len(entsDataLines)):
		line = entsDataLines[index]

		if line.find(searchString) >= 0:
			print(f'{bspPath} contains search string "{searchString}"')
			return True

	return False

def main():
	args = parseArgs()

	if not os.path.isdir(args.dir):
		print("BSP directory", args.dir, "does not exist.", file=sys.stderr)
		sys.exit(1)

	containedInAnyFiles = False

	for fileName in os.listdir(args.dir):
		fullPath = os.path.abspath(os.path.join(args.dir, fileName))

		if os.path.isdir(fullPath):
			continue

		(_, ext) = os.path.splitext(fileName)
		if ext != ".bsp":
			continue

		try:
			if searchBSP(fullPath, args.string):
				containedInAnyFiles = True
		except Exception as ex:
			print(f"Error processing {fullPath}: {str(ex)}")

	if not containedInAnyFiles:
		print(f'Search string "{args.string}" was not contained in any BSP files.')

if __name__ == "__main__":
	main()

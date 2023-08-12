import argparse
import sys
import os
import re

encounteredProps = {}

def parseArguments():
	parser = argparse.ArgumentParser(description="Creates texture property files in bulk.")

	parser.add_argument("-d", "--directory",
						required=True,
						help="Directory within which to search for files.")

	parser.add_argument("-v", "--verify",
						required=False,
						action="store_true",
						help="Verifies that each PNG in each directory has a corresponding .props file.")

	parser.add_argument("-l", "--list",
						required=False,
						action="store_true",
						help="Lists different surfaceprop types that are encountered in files.")

	return parser.parse_args()

def createPropertiesFile(path, surfacePropName):
	print("Creating", path, "for surface prop", surfacePropName)

	with open(path, "w") as outFile:
		outFile.write(f"surfaceprop {surfacePropName}")

def createPropertiesInDirectory(directory, surfacePropName):
	print("Creating", surfacePropName, "property files for contents in", directory)

	for item in os.listdir(directory):
		fullPath = os.path.join(directory, item)

		if os.path.isdir(fullPath):
			createPropertiesInDirectory(fullPath, surfacePropName)
		elif os.path.isfile(fullPath) and os.path.splitext(item)[1].lower() == ".png":
			createPropertiesFile(os.path.join(directory, os.path.splitext(item)[0] + ".props"), surfacePropName)

def recordEncounteredSurfaceProp(path):
	global encounteredProps

	lines = []

	with open(path, "r") as inFile:
		lines = inFile.readlines()

	for line in lines:
		tokens = re.split(r'\s+', line)

		if len(tokens) < 2 or tokens[0] != "surfaceprop":
			continue

		if tokens[1] not in encounteredProps:
			encounteredProps[tokens[1]] = 1
		else:
			encounteredProps[tokens[1]] = encounteredProps[tokens[1]] + 1

def inspect(directory, shouldVerify, shouldList):
	for item in os.listdir(directory):
		fullPath = os.path.join(directory, item)

		if os.path.isdir(fullPath):
			inspect(fullPath, shouldVerify, shouldList)
		elif os.path.isfile(fullPath):
			if shouldVerify and os.path.splitext(item)[1].lower() == ".png":
				targetPath = os.path.join(directory, os.path.splitext(item)[0] + ".props")

				if not os.path.isfile(targetPath):
					print("File", fullPath, "has no corresponding properties file.")
			elif shouldList and os.path.splitext(item)[1].lower() == ".props":
				recordEncounteredSurfaceProp(fullPath)

def main():
	args = parseArguments()

	if not os.path.isdir(args.directory):
		print("The directory", args.directory, "was not valid.", file=sys.stderr)
		sys.exit(1)

	if args.verify or args.list:
		inspect(args.directory, args.verify, args.list)
	else:
		print("Creating property files for all items recursively in", args.directory)
		for item in os.listdir(args.directory):
			fullPath = os.path.join(args.directory, item)

			if os.path.isdir(fullPath):
				# Each folder under the root corresponds to a surfaceprop.
				# Files within each of the subtrees will have surfaceprops generated
				# for them according to the name of the top-level folder.
				createPropertiesInDirectory(fullPath, item)

	if args.verify:
		print("Verification complete.")

	if args.list:
		print("Encountered surfaceprops:")

		encountered = [key for key in encounteredProps]
		encountered.sort()

		for key in encountered:
			print(f"  {key}: {encounteredProps[key]}")

if __name__ == "__main__":
	main()

import argparse
import os
import math
import re

def parseArgs():
	parser = argparse.ArgumentParser("filetobytearray",
									 description="Converts a binary file to a C/C++ header and source file representing the data.")

	parser.add_argument("-i", "--input",
						required=True,
						help="Input file.")

	parser.add_argument("-o", "--output",
						required=True,
						help="Output file name. Will produce a header and source file with this name.")

	parser.add_argument("-n", "--name",
						required=True,
						help="C name prefix, or C++ namespace, applied to the variable names.")

	parser.add_argument("-c", "--c-style",
						action="store_true",
						help="If set, output is C-style instead of C++-style.")

	parser.add_argument("--include-path-prefix",
						required=False,
						help="Prefix to add to #include statement in source file.")

	return parser.parse_args()

def createCppHeaderFile(inData, name):
	lines = \
	[
		"#pragma once\n",
		"\n",
		"#include <cstdint>\n",
		"#include <cstddef>\n",
		"\n",
		f"namespace {name}\n",
		"{\n",
		f"\tstatic constexpr size_t LENGTH = {len(inData)};\n",
		f"\textern const uint8_t DATA[::{name}::LENGTH];\n",
		"}\n"
	]

	return "".join(lines)

def createCppSourcefile(inData, name, headerName):
	preLines = \
	[
		f"#include \"{headerName}.h\"\n",
		"\n",
		f"namespace {name}\n",
		"{\n",
		f"\tconst uint8_t DATA[::{name}::LENGTH] =\n",
		"\t{\n"
	]

	dataLines = []

	for i in range(0, len(inData)):
		line = ""

		if i == 0:
			# First line, so provide indent.
			line += "\t\t"
		elif (i % 8) == 0:
			# Need comma, new line + indent.
			line += ",\n\t\t"
		else:
			# Need comma only.
			line += ", "

		value = inData[i]
		line += f"{value:#04x}"

		if i == len(inData) - 1:
			line += "\n"

		dataLines.append(line)

	postLines = \
	[
		"\t};\n",
		"}\n"
	]

	return "".join(preLines + dataLines + postLines)

def createCHeaderFile(inData, name):
	upperName = name.upper()

	lines = \
	[
		"#pragma once\n",
		"\n",
		"#include <stdint.h>\n",
		"#include <stddef.h>\n",
		"\n",
		f"#define {upperName}_LENGTH ((size_t){len(inData)})\n"
		f"extern const uint8_t {name}_Data[{upperName}_LENGTH];\n",
	]

	return "".join(lines)

def createCSourcefile(inData, name, headerName):
	upperName = name.upper()

	preLines = \
	[
		f"#include \"{headerName}.h\"\n",
		"\n",
		f"const uint8_t {name}_Data[{upperName}_LENGTH] =\n",
		"{\n",
	]

	dataLines = []

	for i in range(0, len(inData)):
		line = ""

		if i == 0:
			# First line, so provide indent.
			line += "\t"
		elif (i % 8) == 0:
			# Need comma, new line + indent.
			line += ",\n\t"
		else:
			# Need comma only.
			line += ", "

		value = inData[i]
		line += f"{value:#04x}"

		if i == len(inData) - 1:
			line += "\n"

		dataLines.append(line)

	postLines = \
	[
		"};\n"
	]

	return "".join(preLines + dataLines + postLines)

def main():
	args = parseArgs()
	fileContent = []

	with open(args.input, "rb") as inFile:
		fileContent = inFile.read()

	print("Read file:", args.input, f"({len(fileContent)} bytes)")

	includePrefix = args.include_path_prefix

	if includePrefix is None:
		includePrefix = ""

	if includePrefix and not includePrefix.endswith("/"):
		includePrefix += "/"

	headerName = includePrefix + os.path.basename(args.output)

	if args.c_style:
		headerData = createCHeaderFile(fileContent, args.name)
		sourceData = createCSourcefile(fileContent, args.name, headerName)
	else:
		headerData = createCppHeaderFile(fileContent, args.name)
		sourceData = createCppSourcefile(fileContent, args.name, headerName)

	headerName = args.output + ".h"

	print("Writing header:", headerName)
	with open(headerName, "w") as outFile:
		outFile.write(headerData)

	sourceName = args.output + (".c" if args.c_style else ".cpp")

	print("Writing source file:", sourceName)
	with open(sourceName, "w") as outFile:
		outFile.write(sourceData)

	print("Done.")

if __name__ == "__main__":
	main()

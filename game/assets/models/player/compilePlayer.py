import os
import sys
import argparse
import subprocess
import shutil

SCRIPT_DIR = os.path.abspath(os.path.dirname(__file__))
STUDIOMDL_EXE = "studiomdl.exe" if sys.platform == "win32" else "studiomdl"
STUDIOMDL_PATH = os.path.abspath(os.path.join(SCRIPT_DIR, "..", "..", "..", "..", "build", "install", "nightfire-open", "tools", STUDIOMDL_EXE))
MODEL_OUTPUT_ROOT_PATH = os.path.abspath(os.path.join(SCRIPT_DIR, "..", "..", "..", "content", "nfopen", "models", "player"))

QC_COMMAND_MODELNAME = "$modelname"

def parseArguments():
	parser = argparse.ArgumentParser(description="Compiles the given player models and copies tuem to the game content folder.")

	parser.add_argument("dirs",
						nargs="+",
						help="Subdirectories within which to look for model QC files. Use * to compile all subdirectories.")

	return parser.parse_args()

def ensureExists(dirPath : str):
	if not os.path.isdir(dirPath):
		raise OSError(f"Directory {dirPath} does not exist.")

def fetchModelNameFromQC(qcFilePath : str):
	with open(qcFilePath, "r") as inFile:
		lines = inFile.readlines()

		for line in lines:
			tokens = line.strip().split()
			tokens = [token.strip('"') for token in tokens]

			if len(tokens) < 2 or tokens[0] != QC_COMMAND_MODELNAME:
				continue

			return tokens[1]

	raise ValueError(f"{qcFilePath}: Could not find a {QC_COMMAND_MODELNAME} line within this file.")

def runStudioMDL(qcFilePath : str):
	print("Running StudioMDL for", qcFilePath)

	print()
	ret = subprocess.run([STUDIOMDL_PATH, "-e", qcFilePath], cwd=os.path.dirname(qcFilePath))
	print()

	if ret.returncode != 0:
		raise OSError(f"StudioMDL execution for {qcFilePath} returned error code {ret.returncode}.")
	else:
		print("StudioMDL completed successfully.")

def compileAndCopyToOutput(qcFilePath : list):
	print("Found QC file:", qcFilePath)

	localDir = os.path.dirname(qcFilePath)
	modelName = fetchModelNameFromQC(qcFilePath)

	print("  Player model name:", modelName)
	print("  Output directory:", MODEL_OUTPUT_ROOT_PATH)

	runStudioMDL(qcFilePath)

	# We expect the compiled model to be output to:
	localModelPath = os.path.join(localDir, modelName)

	if not os.path.isfile(localModelPath):
		raise OSError(f"Could not find compiled model {localModelPath}")

	targetModelPath = os.path.join(MODEL_OUTPUT_ROOT_PATH, modelName)

	print("Copying compiled player model to", targetModelPath)

	if not os.path.isdir(MODEL_OUTPUT_ROOT_PATH):
		os.makedirs(MODEL_OUTPUT_ROOT_PATH)

	shutil.copyfile(localModelPath, targetModelPath)

	print("Deleting local compiled model", localModelPath)
	os.remove(localModelPath)

	print("Complete.")
	print()

def compileRecursive(path : str):
	ensureExists(path)

	for item in os.listdir(path):
		newPath = os.path.join(path, item)

		if os.path.isdir(newPath):
			compileRecursive(newPath)
		elif os.path.splitext(item)[1] == ".qc":
			compileAndCopyToOutput(newPath)

def main():
	args = parseArguments()

	if not os.path.isfile(STUDIOMDL_PATH):
		print("Could not find", STUDIOMDL_PATH, "- make sure you have installed the game to build/install")
		sys.exit(1)

	dirsToCompile = list(args.dirs)

	if "*" in dirsToCompile:
		dirsToCompile = os.listdir(SCRIPT_DIR)

	totalDirs = len(dirsToCompile)
	successfulDirs = 0

	for subdir in dirsToCompile:
		try:
			print("Compiling player models in subdirectory:", subdir)
			compileRecursive(os.path.join(SCRIPT_DIR, subdir))
			successfulDirs += 1
		except Exception as ex:
			print(f"Failed to compile model '{subdir}': {str(ex)}")

	print("Successfully compiled", successfulDirs, "of", totalDirs, "models.")

if __name__ == "__main__":
	main()

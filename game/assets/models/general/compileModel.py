import os
import sys
import argparse
import subprocess
import shutil
import signal

SCRIPT_DIR = os.path.abspath(os.path.dirname(__file__))
STUDIOMDL_EXE = "studiomdl.exe" if sys.platform == "win32" else "studiomdl"
STUDIOMDL_PATH = os.path.abspath(os.path.join(SCRIPT_DIR, "..", "..", "..", "..", "build", "install", "nightfire-open", "tools", STUDIOMDL_EXE))
MODEL_OUTPUT_ROOT_PATH = os.path.abspath(os.path.join(SCRIPT_DIR, "..", "..", "..", "content", "nfopen", "models"))

QC_COMMAND_MODELNAME = "$modelname"

Aborted = False

def sigintHandler(signal, frame):
	global Aborted
	Aborted = True

def parseArguments():
	parser = argparse.ArgumentParser(description="Compiles the given models and copies them to the game content folder.")

	parser.add_argument("dirs",
						nargs="+",
						help="Subdirectories within which to look for model QC files. Use * to compile all subdirectories.")

	return parser.parse_args()

def ensureExists(dirPath : str):
	if not os.path.isdir(dirPath):
		raise OSError(f"Directory {dirPath} does not exist.")

def shouldCompileSubdir(subdir:str):
	if subdir == "." or subdir == "..":
		return False

	fullPath = os.path.join(SCRIPT_DIR, subdir)

	if not os.path.isdir(fullPath):
		return False

	# Don't compile weapon models from here, since they'll
	# be moved over to the actual weapons directory when
	# it's time to make a weapon out of them.
	if subdir.startswith("p_") or subdir.startswith("v_") or subdir.startswith("w_"):
		return False

	return True

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
	ret = subprocess.run([STUDIOMDL_PATH, qcFilePath], cwd=os.path.dirname(qcFilePath))
	print()

	if ret.returncode != 0:
		raise OSError(f"StudioMDL execution for {qcFilePath} returned error code {ret.returncode}.")
	else:
		print("StudioMDL completed successfully.")

def compileAndCopyToOutput(qcFilePath : list):
	print("Found QC file:", qcFilePath)

	localDir = os.path.dirname(qcFilePath)
	modelName = fetchModelNameFromQC(qcFilePath)

	print("  Model name:", modelName)
	print("  Output directory:", MODEL_OUTPUT_ROOT_PATH)

	runStudioMDL(qcFilePath)

	# We expect the compiled model to be output to:
	localWeaponModelPath = os.path.join(localDir, modelName)

	if not os.path.isfile(localWeaponModelPath):
		raise OSError(f"Could not find compiled model {localWeaponModelPath}")

	targetPath = os.path.join(MODEL_OUTPUT_ROOT_PATH, modelName)

	print("Copying compiled weapon model to", targetPath)

	shutil.copyfile(localWeaponModelPath, targetPath)

	print("Deleting local compiled model", localWeaponModelPath)
	os.remove(localWeaponModelPath)

	print("Complete.")
	print()

def compileModel(path : str):
	ensureExists(path)

	for item in os.listdir(path):
		newPath = os.path.join(path, item)

		if os.path.splitext(item)[1] == ".qc":
			compileAndCopyToOutput(newPath)

def main():
	signal.signal(signal.SIGINT, sigintHandler)

	if not os.path.isfile(STUDIOMDL_PATH):
		print("Could not find", STUDIOMDL_PATH, "- make sure you have installed the game to build/install")
		sys.exit(1)

	args = parseArguments()
	dirsToCompile = list(args.dirs)

	if "*" in dirsToCompile:
		dirsToCompile = os.listdir(SCRIPT_DIR)

	dirsToCompile = [path for path in dirsToCompile if shouldCompileSubdir(path)]

	totalDirs = len(dirsToCompile)
	successfulDirs = 0
	failedPaths = []

	for subdir in dirsToCompile:
		if Aborted:
			break

		try:
			print("Compiling model:", subdir)
			compileModel(os.path.join(SCRIPT_DIR, subdir))
			successfulDirs += 1
		except Exception as ex:
			print(f"Failed to compile model '{subdir}': {str(ex)}")
			failedPaths.append(subdir)

	print("Successfully compiled", successfulDirs, "of", totalDirs, "models.")

	if failedPaths:
		print("Failed:", ", ".join(failedPaths))

if __name__ == "__main__":
	main()

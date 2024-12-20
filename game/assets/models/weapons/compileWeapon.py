import os
import sys
import argparse
import subprocess
import shutil
import re

SCRIPT_DIR = os.path.abspath(os.path.dirname(__file__))
STUDIOMDL_EXE = "studiomdl.exe" if sys.platform == "win32" else "studiomdl"
STUDIOMDL_PATH = os.path.abspath(os.path.join(SCRIPT_DIR, "..", "..", "..", "..", "build", "install", "nightfire-open", "tools", STUDIOMDL_EXE))
MODEL_OUTPUT_ROOT_PATH = os.path.abspath(os.path.join(SCRIPT_DIR, "..", "..", "..", "content", "nfopen", "models"))

QC_COMMAND_MODELNAME = "$modelname"
EXPECTED_MODELNAME_PREFIXES = [ "p_", "v_", "w_", "w_ammo_" ]

REGEX_FOLDER_PREFIX = re.compile(r"^[A-Za-z0-9]+_")

def parseArguments():
	parser = argparse.ArgumentParser(description="Compiles the given weapons and copies the models to the game content folder.")

	parser.add_argument("dirs",
						nargs="+",
						help="Subdirectories within which to look for model QC files. Use * to compile all subdirectories.")

	return parser.parse_args()

def ensureExists(dirPath : str):
	if not os.path.isdir(dirPath):
		raise OSError(f"Directory {dirPath} does not exist.")

def modelNameWithoutPrefix(modelName : str):
	modelName = os.path.splitext(modelName)[0]
	longestPrefix = None

	for prefix in EXPECTED_MODELNAME_PREFIXES:
		if modelName.startswith(prefix):
			longestPrefix = prefix

	if longestPrefix is not None:
		return modelName[len(longestPrefix):]
	else:
		raise ValueError(f"Model name '{modelName}' was not in expected format. " +
						 f"Expected name prefix to be one of: {', '.join(EXPECTED_MODELNAME_PREFIXES)}.")

def fetchWeaponModelNameFromQC(qcFilePath : str):
	with open(qcFilePath, "r") as inFile:
		lines = inFile.readlines()

		for line in lines:
			tokens = line.strip().split()
			tokens = [token.strip('"') for token in tokens]

			if len(tokens) < 2 or tokens[0] != QC_COMMAND_MODELNAME:
				continue

			return tokens[1]

	raise ValueError(f"{qcFilePath}: Could not find a {QC_COMMAND_MODELNAME} line within this file.")

def outputDirForWeapon(weaponName : str):
	return os.path.join(MODEL_OUTPUT_ROOT_PATH, f"weapon_{weaponName}")

def runStudioMDL(qcFilePath : str):
	print("Running StudioMDL for", qcFilePath)

	print()
	ret = subprocess.run([STUDIOMDL_PATH, qcFilePath], cwd=os.path.dirname(qcFilePath))
	print()

	if ret.returncode != 0:
		raise OSError(f"StudioMDL execution for {qcFilePath} returned error code {ret.returncode}.")
	else:
		print("StudioMDL completed successfully.")

def compileAndCopyToOutput(qcFilePath: str, weaponName: str):
	print("Found QC file:", qcFilePath)

	localDir = os.path.dirname(qcFilePath)
	weaponModelName = fetchWeaponModelNameFromQC(qcFilePath)

	outputDir = outputDirForWeapon(weaponName)

	print("  Weapon model name:", weaponModelName)
	print("  Weapon name:", weaponName)
	print("  Output directory:", outputDir)

	runStudioMDL(qcFilePath)

	# We expect the compiled model to be output to:
	localWeaponModelPath = os.path.join(localDir, weaponModelName)

	if not os.path.isfile(localWeaponModelPath):
		raise OSError(f"Could not find compiled model {localWeaponModelPath}")

	targetWeaponModelPath = os.path.join(outputDir, weaponModelName)

	print("Copying compiled weapon model to", targetWeaponModelPath)

	if not os.path.isdir(outputDir):
		os.makedirs(outputDir)

	shutil.copyfile(localWeaponModelPath, targetWeaponModelPath)

	print("Deleting local compiled model", localWeaponModelPath)
	os.remove(localWeaponModelPath)

	print("Complete.")
	print()

def compileRecursive(path: str, weaponName: str):
	ensureExists(path)

	for item in os.listdir(path):
		newPath = os.path.join(path, item)

		if os.path.isdir(newPath) and not item.startswith("_"):
			compileRecursive(newPath, weaponName)
		elif os.path.splitext(item)[1] == ".qc":
			compileAndCopyToOutput(newPath, weaponName)

def shouldCompileSubdir(subdir:str):
	if subdir == "." or subdir == "..":
		return False

	fullPath = os.path.join(SCRIPT_DIR, subdir)

	if not os.path.isdir(fullPath):
		return False

	return True

def main():
	args = parseArguments()

	if not os.path.isfile(STUDIOMDL_PATH):
		print("Could not find", STUDIOMDL_PATH, "- make sure you have installed the game to build/install")
		sys.exit(1)

	dirsToCompile = list(args.dirs)

	if "*" in dirsToCompile:
		dirsToCompile = os.listdir(SCRIPT_DIR)

	dirsToCompile = [path for path in dirsToCompile if shouldCompileSubdir(path)]

	totalDirs = len(dirsToCompile)
	successfulDirs = 0
	failedPaths = []

	for subdir in dirsToCompile:
		try:
			print("Compiling weapon models in subdirectory:", subdir)
			compileRecursive(os.path.join(SCRIPT_DIR, subdir), subdir)
			successfulDirs += 1
		except Exception as ex:
			print(f"Failed to compile model '{subdir}': {str(ex)}")
			failedPaths.append(subdir)

	print("Successfully compiled", successfulDirs, "of", totalDirs, "weapons.")

	if failedPaths:
		print("Failed:", ", ".join(failedPaths))

if __name__ == "__main__":
	main()

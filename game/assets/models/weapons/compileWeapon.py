import os
import sys
import argparse
import subprocess
import shutil

SCRIPT_DIR = os.path.abspath(os.path.dirname(__file__))
STUDIOMDL_PATH = os.path.abspath(os.path.join(SCRIPT_DIR, "..", "..", "..", "..", "utils", "binaries", "afterburner-compile-tools", "abstudiomdl.exe"))
MODEL_OUTPUT_ROOT_PATH = os.path.abspath(os.path.join("..", "..", "..", "afterburner", "models"))

QC_COMMAND_MODELNAME = "$modelname"
EXPECTED_MODELNAME_PREFIXES = [ "p_", "v_", "w_", "w_ammo_" ]

def parseArguments():
	parser = argparse.ArgumentParser(description="Compiles the given weapons and copies the models to the game content folder.")

	parser.add_argument("dirs",
						nargs="+",
						help="Subdirectories within which to look for model QC files.")

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
		return OSError(f"StudioMDL execution for {qcFilePath} returned error code {ret.returncode}.")
	else:
		print("StudioMDL completed successfully.")

def compileAndCopyToOutput(qcFilePath : list):
	print("Found QC file:", qcFilePath)

	localDir = os.path.dirname(qcFilePath)
	weaponModelName = fetchWeaponModelNameFromQC(qcFilePath)
	weaponName = modelNameWithoutPrefix(weaponModelName)
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

def compileRecursive(path : str):
	ensureExists(path)

	for item in os.listdir(path):
		newPath = os.path.join(path, item)

		if os.path.isdir(newPath):
			compileRecursive(newPath)
		elif os.path.splitext(item)[1] == ".qc":
			compileAndCopyToOutput(newPath)

def main():
	if sys.platform != "win32":
		print("This script is only supported on Windows.", file=sys.stderr)
		sys.exit(1)

	args = parseArguments()
	totalDirs = len(args.dirs)
	successfulDirs = 0

	for subdir in args.dirs:
		try:
			print("Compiling weapon models in subdirectory:", subdir)
			compileRecursive(os.path.join(SCRIPT_DIR, subdir))
			successfulDirs += 1
		except Exception as ex:
			print(f"Failed to compile model '{subdir}': {str(ex)}")

	print("Successfully compiled", successfulDirs, "of", totalDirs, "weapons.")

if __name__ == "__main__":
	main()

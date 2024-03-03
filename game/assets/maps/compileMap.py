import os
import sys
import argparse
import subprocess
import shutil

def exe_name(name:str):
	return f"{name}.exe" if sys.platform == "win32" else name

SCRIPT_DIR = os.path.abspath(os.path.dirname(__file__))
TOOLS_DIR = os.path.abspath(os.path.join(SCRIPT_DIR, "..", "..", "..", "build", "install", "nightfire-open", "tools"))
CSG_PATH = os.path.join(TOOLS_DIR, exe_name("nfcsg"))
BSP_PATH = os.path.join(TOOLS_DIR, exe_name("nfbsp"))
VIS_PATH = os.path.join(TOOLS_DIR, exe_name("nfvis"))
RAD_PATH = os.path.join(TOOLS_DIR, exe_name("nfrad"))
CONTENT_DIR = os.path.abspath(os.path.join(SCRIPT_DIR, "..", "..", "content", "nfopen"))
MAPS_DIR = os.path.join(CONTENT_DIR, "maps")
TEXTURES_DIR = os.path.join(CONTENT_DIR, "textures")

def parseArguments():
	parser = argparse.ArgumentParser(description="Compiles the given maps and copies them to the game content folder.")

	parser.add_argument("dirs",
						nargs="+",
						help="Subdirectories within which to look for maps. Use * to compile all subdirectories.")

	return parser.parse_args()

def runCompileCommand(args:list):
	print("Running:", " ".join(args))
	subprocess.run(args, check=True)

def runAllCompileStages(mapPath:str):
	bspPath = os.path.splitext(mapPath)[0] + ".bsp"

	runCompileCommand([
		CSG_PATH,
		"-texturedir", TEXTURES_DIR,
		mapPath
	])

	runCompileCommand([
		BSP_PATH,
		bspPath
	])

	runCompileCommand([
		VIS_PATH,
		bspPath
	])

	runCompileCommand([
		RAD_PATH,
		"-texturedir", TEXTURES_DIR,
		bspPath
	])

def findMapFile(path:str):
	items = [item for item in os.listdir(path) if os.path.splitext(item)[1] == ".map"]

	if not items:
		raise RuntimeError(f"Could not find any .map files in {path}")

	if len(items) > 1:
		raise RuntimeError(f"Found more than one map file in {path} - don't know which map to compile")

	return items[0]

def compileMap(path:str):
	mapFile = findMapFile(path)

	compileDir = os.path.join(path, "compile")
	print("Making directory:", compileDir)
	os.makedirs(compileDir, exist_ok=True)

	targetFile = os.path.join(compileDir, mapFile)
	print("Copying map to:", targetFile)
	shutil.copy2(os.path.join(path, mapFile), targetFile)

	oldCwd = os.getcwd()
	os.chdir(compileDir)

	print("Compiling:", targetFile)
	runAllCompileStages(targetFile)

	os.chdir(oldCwd)

def sortSubdirs(path:str):
	# For the keys returned here, Python treats
	# numbers as coming before capital letters,
	# and capital letters as coming before
	# lower case letters. We return keys such
	# that more preferential maps are sorted
	# into later positions than less preferential
	# maps.

	if path.startswith("a"):
		# Alpha maps
		return "A" + path[1:]
	elif path.startswith("b"):
		# Beta maps
		return "B" + path[1:]
	elif path.startswith("rc"):
		# Release candidate maps
		return "C" + path[2:]
	elif path == "final":
		# Final maps should always go last
		return "Z" + path
	else:
		# Not a recognised format,
		# so go before everything.
		return "0" + path

def compileMapSubdir(path:str):
	items = sorted([item for item in os.listdir(path) if os.path.isdir(os.path.join(path, item))], key=sortSubdirs)

	if not items:
		raise RuntimeError(f"Path {path} did not contain any map subdirectories")

	item = items[-1]
	print(f"Compiling latest version of map {os.path.basename(path)}: {item}")
	compileMap(os.path.join(path, item))

def shouldCompileSubdir(subdir:str):
	if subdir == "." or subdir == "..":
		return False

	fullPath = os.path.join(SCRIPT_DIR, subdir)

	if not os.path.isdir(fullPath):
		return False

	return True

def main():
	args = parseArguments()

	for path in [CSG_PATH, BSP_PATH, VIS_PATH, RAD_PATH]:
		if not os.path.isfile(path):
			print("Could not find", path, "- make sure you have installed the game to build/install")
			sys.exit(1)

	for path in [MAPS_DIR, TEXTURES_DIR]:
		if not os.path.isdir(MAPS_DIR):
			print("Required directory", path, "does not exist")
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
			print("Compiling map subdirectory:", subdir)
			compileMapSubdir(os.path.join(SCRIPT_DIR, subdir))
			successfulDirs += 1
		except Exception as ex:
			print(f"Failed to compile map '{subdir}': {str(ex)}")
			failedPaths.append(subdir)

	print("Successfully compiled", successfulDirs, "of", totalDirs, "maps.")

	if failedPaths:
		print("Failed:", ", ".join(failedPaths))

if __name__ == "__main__":
	main()

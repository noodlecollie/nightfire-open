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
INSTALLED_GAME_DIR = os.path.abspath(os.path.join(SCRIPT_DIR, "..", "..", "..", "build", "install", "nightfire-open", "nfopen"))
INSTALLED_MAPS_DIR = os.path.join(INSTALLED_GAME_DIR, "maps")

def parseArguments():
	parser = argparse.ArgumentParser(
		description="Compiles the given maps and copies them to the game content folder.",
		epilog="BEWARE that this process will simply compile the .map files present without running "
		"a TrenchBroom export on them. This means that things like hidden layers will not be excluded "
		"from the compiled result. Use with caution."
	)

	parser.add_argument("--no-compile",
		action="store_true",
		help="Don't compile, only copy compiled files to the game contents directory.")

	parser.add_argument("--no-copy",
		action="store_true",
		help="Only compile the map, do not copy it to the game contents directory.")

	parser.add_argument("--no-vis",
		action="store_true",
		help="Don't run VIS step. Implies --no-rad.")

	parser.add_argument("--no-rad",
		action="store_true",
		help="Don't run RAD step.")

	parser.add_argument("--files",
		action="store_true",
		help="If set, paths provided will be treated as specific .map files to compile, instead of as subdirectories.")

	parser.add_argument("dirs",
		nargs="+",
		help="Subdirectories within which to look for maps. Use * to compile all subdirectories. If --files is set, "
		"these paths will instead be treated as specific .map files to compile.")

	return parser.parse_args()

def runCompileCommand(args:list):
	print("Running:", " ".join(args))
	subprocess.run(args, check=True)

def runAllCompileStages(mapPath:str, args):
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

	if not args.no_vis:
		runCompileCommand([
			VIS_PATH,
			bspPath
		])

	if not args.no_rad:
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

def compileMap(mapDir:str, args, mapFileOverride:str=None):
	mapFile = mapFileOverride if mapFileOverride else findMapFile(mapDir)
	compileDir = os.path.join(mapDir, "compile")

	if not args.no_compile:
		print("Making directory:", compileDir)
		os.makedirs(compileDir, exist_ok=True)

		targetFile = os.path.join(compileDir, mapFile)
		print("Copying map to:", targetFile)
		shutil.copy2(os.path.join(mapDir, mapFile), targetFile)

		oldCwd = os.getcwd()
		os.chdir(compileDir)

		print("Compiling:", targetFile)
		runAllCompileStages(targetFile, args)

		os.chdir(oldCwd)

	if not args.no_copy:
		mapBspName = os.path.splitext(mapFile)[0] + ".bsp"
		bspSrcPath = os.path.join(compileDir, mapBspName)

		for destDir in [MAPS_DIR, INSTALLED_MAPS_DIR]:
			bspDestPath = os.path.join(destDir, mapBspName)
			print("Copying map to", bspDestPath)
			shutil.copy2(bspSrcPath, bspDestPath)

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

def compileMapSubdir(path:str, args):
	items = sorted([item for item in os.listdir(path) if os.path.isdir(os.path.join(path, item))], key=sortSubdirs)

	if not items:
		raise RuntimeError(f"Path {path} did not contain any map subdirectories")

	item = items[-1]
	print(f"Compiling latest version of map {os.path.basename(path)}: {item}")
	compileMap(os.path.join(path, item), args)

def shouldCompileSubdir(subdir:str):
	if subdir == "." or subdir == "..":
		return False

	fullPath = os.path.join(SCRIPT_DIR, subdir)

	if not os.path.isdir(fullPath):
		return False

	return True

def main():
	args = parseArguments()

	if args.no_vis:
		args.no_rad = True

	for path in [CSG_PATH, BSP_PATH, VIS_PATH, RAD_PATH]:
		if not os.path.isfile(path):
			print("Could not find", path, "- make sure you have installed the game to build/install")
			sys.exit(1)

	for path in [MAPS_DIR, TEXTURES_DIR, INSTALLED_MAPS_DIR, INSTALLED_MAPS_DIR]:
		if not os.path.isdir(MAPS_DIR):
			print("Required directory", path, "does not exist")
			sys.exit(1)

	itemsToCompile = list(args.dirs)

	if not args.files:
		if "*" in itemsToCompile:
			itemsToCompile = os.listdir(SCRIPT_DIR)

		itemsToCompile = [path for path in itemsToCompile if shouldCompileSubdir(path)]

	totalDirs = len(itemsToCompile)
	successfulDirs = 0
	failedPaths = []

	if args.files:
		for filePath in itemsToCompile:
			mapFilePath = os.path.join(SCRIPT_DIR, filePath) if not os.path.isabs(filePath) else filePath
			mapFileDir = os.path.dirname(mapFilePath)

			try:
				print("Compiling map:", filePath)
				compileMap(mapFileDir, args, os.path.basename(mapFilePath))
				successfulDirs += 1
			except Exception as ex:
				print(f"Failed to compile map '{mapFilePath}': {str(ex)}")
				failedPaths.append(mapFilePath)
	else:
		for subdir in itemsToCompile:
			try:
				print("Compiling map subdirectory:", subdir)
				compileMapSubdir(os.path.join(SCRIPT_DIR, subdir), args)
				successfulDirs += 1
			except Exception as ex:
				print(f"Failed to compile map '{subdir}': {str(ex)}")
				failedPaths.append(subdir)

	print("Successfully compiled", successfulDirs, "of", totalDirs, "maps.")

	if failedPaths:
		print("Failed:", ", ".join(failedPaths))

if __name__ == "__main__":
	main()

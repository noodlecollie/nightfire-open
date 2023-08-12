import sys
import os
import subprocess
import shutil
import struct
import threading
import inspect
import math
import signal
import argparse

from concurrent.futures.thread import ThreadPoolExecutor

SCRIPT_DIR = os.path.dirname(os.path.abspath(os.path.realpath(sys.argv[0])))
GAME_CONTENT_DIR = os.path.abspath(os.path.join(SCRIPT_DIR, "..", "..", "..", "afterburner"))
MDL_OUTPUT_DIR = os.path.join(GAME_CONTENT_DIR, "models")
TEXTURE_DIR = os.path.join(GAME_CONTENT_DIR, "textures")
TEXTURE_SUBDIR_NAME = "mdl"

STUDIOMDL_PATH = os.path.abspath(os.path.join(SCRIPT_DIR, "..", "..", "..", "..", "utils", "binaries", "afterburner-compile-tools", "abstudiomdl.exe"))
ERROR_SUMMARY_PATH = os.path.join(SCRIPT_DIR, "errors.log")

Aborted = False

def sigintHandler(signal, frame):
	global Aborted
	Aborted = True

def relPath(path:str):
	return os.path.relpath(path, SCRIPT_DIR)

class FileProcessor():
	def __init__(self, args, inputQc):
		self.args = args
		self.inputQc = inputQc
		self.inputQcDir = os.path.dirname(self.inputQc)
		self.inputModelName = os.path.splitext(os.path.basename(self.inputQc))[0] + ".mdl"

		# For eventual copying:
		self.srcMdlPath = os.path.join(self.inputQcDir, self.inputModelName)
		srcQcDirRelPath = os.path.relpath(self.inputQcDir, SCRIPT_DIR)
		destRelDir = os.path.dirname(srcQcDirRelPath)
		self.destMdlPath = os.path.join(MDL_OUTPUT_DIR, destRelDir, self.inputModelName)

		self.currentLog = ""

	def processInputFile(self):
		if not self.args.skip_patch:
			self.patch()

		if not self.args.skip_compile:
			self.compile()

	def patch(self):
		referenceSmds = self.patchQc()

		self.prepareLog("PatchSMD")
		for fileName in referenceSmds:
			self.patchSmd(os.path.join(self.inputQcDir, fileName))

	def compile(self):
		self.compileQc()
		self.copyMdlToOutput()

	def patchQc(self):
		self.prepareLog("PatchQc")

		referenceSmds = []
		lines = []

		with open(self.inputQc, "r") as inFile:
			lines = [line.rstrip() for line in inFile.readlines()]

		inBodyGroup = False
		modelNameReplaced = False

		texturePath = os.path.relpath(TEXTURE_DIR, self.inputQcDir).replace("\\", "/")
		modifiedLines = \
		[
			f'$cdtexture "{texturePath}"',
			'$cd "."'
		]

		for line in lines:
			segments = line.strip().split(maxsplit=1)

			if len(segments) > 0 and segments[0] == "$bodygroup" and not inBodyGroup:
				inBodyGroup = True
			elif inBodyGroup:
				if line == "}":
					inBodyGroup = False
				elif len(segments) == 2 and segments[0] == "studio":
					smdName = segments[1].strip('"') + ".smd"
					self.logMsg("Found reference SMD:", smdName)
					referenceSmds.append(smdName)
			elif len(segments) == 2 and segments[0] == "$flags":
				# Ensure that flags always prohibit embedded textures
				flagsInt = int(segments[1]) | 2048
				line = f"$flags {flagsInt}"
			elif len(segments) == 2 and segments[0] == "$modelname":
					# Make sure the QC model name matches the model we decompiled.
					# Sometimes the name embedded in the model is different, but we
					# want to make sure the output MDL is named the same as the
					# input MDL. If this is not the case, maps which reference the
					# MDL by name may not be able to load it.

					# Only log if we change the name
					internalName = segments[1].strip('"')
					if internalName != self.inputModelName:
						self.logMsg(f"Model's internal name was {internalName}, changing to {self.inputModelName}")

					line = f'$modelname "{self.inputModelName}"'
					modelNameReplaced = True
			elif len(segments) > 0 and (segments[0] == "$cd" or segments[0] == "$cdtexture"):
					# We set these manually, so kill off any others.
					line = None

			if line is not None:
				modifiedLines.append(line)

		if not modelNameReplaced:
			raise RuntimeError(f"Could not find $modelname in {relPath(self.inputQc)}")

		with open(self.inputQc, "w") as outFile:
			# The final \n is added for safety, as StudioMDL requires it
			outFile.write("\n".join(modifiedLines) + "\n")

		self.logMsg(f"Found a total of {len(referenceSmds)} reference SMDs")
		return referenceSmds

	def patchSmd(self, smdPath:str):
		self.logMsg("Patching", relPath(smdPath))

		lines = []

		with open(smdPath, "r") as inFile:
			lines = [line.rstrip() for line in inFile.readlines()]

		referencedTextures = {}

		for index in range(0, len(lines)):
			line = lines[index]

			if line.endswith(".bmp"):
				stripped = line.strip()

				if stripped not in referencedTextures:
					self.logMsg("Found referenced texture:", stripped)
					referencedTextures[stripped] = True

				withoutExt = os.path.splitext(line)[0]
				lines[index] = f"{TEXTURE_SUBDIR_NAME}/{withoutExt}.png"

		with open(smdPath, "w") as outFile:
			# The final \n is added for safety, as StudioMDL requires it
			outFile.write("\n".join(lines) + "\n")

		# Log newline:
		self.logMsg()

	def compileQc(self):
		self.prepareLog("CompileQC")

		args = \
		[
			STUDIOMDL_PATH,
			os.path.basename(self.inputQc)
		]

		self.runCommand(args, self.inputQcDir)

	def copyMdlToOutput(self):
		self.prepareLog("CopyMDLToOutput")

		self.logMsg("Copying", self.srcMdlPath, "to", self.destMdlPath)
		shutil.copy2(self.srcMdlPath, self.destMdlPath)

	def runCommand(self, args, cwd):
		self.logMsg("*** Running command:", *args)

		with open(self.currentLog, "a+") as stdOut:
				result = subprocess.run(args, shell=True, stdout=stdOut, cwd=cwd)

		if result.returncode != 0:
			raise RuntimeError(f"Command {' '.join(args)} returned error code {result.returncode}")

		self.logMsg("*** Command complete.")

	def prepareLog(self, name:str):
		self.currentLog = os.path.join(self.inputQcDir, name + ".log")

		# Overwrite.
		with open(self.currentLog, "w"):
			pass

	def logMsg(self, *args):
		if self.currentLog:
			with open(self.currentLog, "a+") as outFile:
				print(*args, file=outFile)

def parseArgs():
	parser = argparse.ArgumentParser(description="Batch convert Nightfire models.")

	parser.add_argument("-t", "--threads",
						help="Number of threads to use",
						type=int,
						default=4)

	parser.add_argument("--run-once",
						help="Limits processing to one single input file, for debugging.",
						action="store_true")

	parser.add_argument("--skip-compile",
						help="Only modifies SMD and QC files, does not compile any models",
						action="store_true")

	parser.add_argument("--skip-patch",
						help="Only compiled models, does not patch SMD or QC files.",
						action="store_true")

	return parser.parse_args()

def validateArgs(args):
	if args.threads < 1:
		print("Invalid thread count of", args.threads, "specified.", file=sys.stderr)
		sys.exit(1)

def getInputFilesRecursive(path:str):
	entries = []

	for entry in os.listdir(path):
		entryPath = os.path.join(path, entry)

		if os.path.isdir(entryPath):
			# Ignore player models, since they're tweaked and compiled manually.
			if entry != "player":
				entries += getInputFilesRecursive(entryPath)

		(fileName, fileExt) = os.path.splitext(entry)

		if fileExt == ".qc":

			# Ignore these special files, which are just kept here until
			# we go through and tweak/compile them manually.
			if  not fileName.lower().startswith("p_") and \
				not fileName.lower().startswith("v_") and \
				not fileName.lower().startswith("w_") and \
				fileName != "player":
				entries.append(entryPath)

	return entries

def getInputFiles():
	print("Finding input files")
	return getInputFilesRecursive(SCRIPT_DIR)

def threadTask(args, files:dict, filePath:str):
	global Aborted

	if Aborted:
		return

	try:
		print("Processing file:", relPath(filePath))
		FileProcessor(args, filePath).processInputFile()
		files[filePath] = None
	except Exception as ex:
		details = inspect.trace()[-1]
		errorString = f"Exception from {details.filename}:{details.lineno}: {str(ex)}"
		files[filePath] = errorString
		print("***", errorString, file=sys.stderr)
		print("*** An error occurred, skipping file", relPath(filePath), file=sys.stderr)

def main():
	print("Running from:", SCRIPT_DIR)

	args = parseArgs()
	validateArgs(args)

	filesToProcess = getInputFiles()

	if len(filesToProcess) < 1:
		print("Could not find any files to process.")
		sys.exit(0)

	if args.run_once:
		filesToProcess = [filesToProcess[0]]

	print("Found", len(filesToProcess), "input files")

	filesMap = {filePath: f"{filePath} has not yet been processed." for filePath in filesToProcess}
	threadWork = [(args, filesMap, filePath) for filePath in filesMap.keys()]

	print("Starting thread pool with", args.threads, "threads")

	signal.signal(signal.SIGINT, sigintHandler)

	with ThreadPoolExecutor(max_workers=args.threads) as executor:
		executor.map(lambda args: threadTask(*args), threadWork)

	filesProcessed = 0

	print("Writing error summary...")

	with open(ERROR_SUMMARY_PATH, "w") as errorLogFile:
		for filePath in filesMap:
			if filesMap[filePath] is None:
				filesProcessed += 1
			else:
				errorLogFile.write(filesMap[filePath] + "\n")

	print("Processed", filesProcessed, "of", len(filesToProcess), "files.")
	print("Any processing errors have been logged to", ERROR_SUMMARY_PATH)

if __name__ == "__main__":
	main()

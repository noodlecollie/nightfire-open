import os
import subprocess

SCRIPT_DIR = os.path.realpath(os.path.dirname(__file__))
ROOT_DIR = os.path.abspath(os.path.join(SCRIPT_DIR, "..", "..", "..", ".."))

BATCH_SIZE = 20

def runGitCommand(args):
	subprocess.run(["git"] + args, check=True, cwd=ROOT_DIR)

def gitRemoveFiles(files):
	runGitCommand(["rm", "--cached"] + [os.path.relpath(file, ROOT_DIR) for file in files])

def renameFiles(files):
	newFiles = []

	for file in files:
		baseName = os.path.basename(file)
		newPath = os.path.join(os.path.dirname(file), baseName.lower())

		os.rename(file, newPath)
		newFiles.append(newPath)

	return newFiles

def gitAddFiles(files):
	runGitCommand(["add"] + [os.path.relpath(file, ROOT_DIR) for file in files])

def iterateInBatches(input, batchSize, callback, *args):
	output = None

	for start in range(0, len(input), batchSize):
		end = start + BATCH_SIZE

		if end > len(input):
			end = len(input)

		result = callback(input[start:end], *args)

		if result is not None:
			if output is None:
				output = []

			output += result

	return output

def main():
	texturesDir = os.path.abspath(os.path.join(SCRIPT_DIR, "..", "..", "..", "content", "nfopen", "textures"))
	print("Checking textures in", texturesDir)

	pathsToFix = []

	for root, dirs, files in os.walk(texturesDir):
		for file in files:
			lowercaseExt = os.path.splitext(file)[1].lower()

			if lowercaseExt != ".png":
				continue

			fullPath = os.path.join(root, file)

			# Check the file name only, directories are more than we care about right now
			if file.lower() != file:
				print("Found inconsistent case:", fullPath)
				pathsToFix.append(fullPath)

	print()
	print("Fixing", len(pathsToFix), "paths")

	iterateInBatches(pathsToFix, BATCH_SIZE, gitRemoveFiles)
	newFilePaths = iterateInBatches(pathsToFix, BATCH_SIZE, renameFiles)
	iterateInBatches(newFilePaths, BATCH_SIZE, gitAddFiles)

if __name__ == "__main__":
	main()

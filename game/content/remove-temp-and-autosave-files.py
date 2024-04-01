import os
import argparse

SCRIPT_DIR = os.path.abspath(os.path.dirname(__file__))
DISALLOWED_SUFFIXES = [
	"~",
	"-autosave.kra"
]

def parseArguments():
	parser = argparse.ArgumentParser(description="Removes unneeded temporary and autosave files, "
		"so they don't get copied to the game install directory.")

	parser.add_argument("--confirm",
						action="store_true",
						help="Pass this flag to perform deletions. If not provided, a dry run will be performed.s")

	return parser.parse_args()

def main():
	args = parseArguments()
	rootDir = os.path.join(SCRIPT_DIR, "nfopen")
	filesToDelete = []

	for root, _, files in os.walk(rootDir):
		for file in files:
			for suffix in DISALLOWED_SUFFIXES:
				if file.endswith(suffix):
					filesToDelete.append(os.path.join(root, file))
					break

	print(f"Found {len(filesToDelete)} files to delete:")

	if filesToDelete:
		print("\n".join(filesToDelete))

	if args.confirm:
		for file in filesToDelete:
			os.unlink(file)

		print("Files deleted.")

if __name__ == "__main__":
	main()

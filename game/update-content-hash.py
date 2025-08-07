import os
import subprocess
import sys
import re

SCRIPT_DIR = os.path.dirname(os.path.realpath(__file__))

def content_cmd(content_path, cmd, shell=False):
	result = subprocess.run(
		cmd,
		cwd=content_path,
		capture_output=True,
		text=True,
		shell=shell)

	if result.returncode != 0:
		cmd_str = " ".join(cmd) if type(cmd) is list else cmd
		raise RuntimeError(f"Command {cmd_str} returned error code {result.returncode}. Stderr:\n{result.stderr}")

	return result.stdout

def content_has_staged_files(content_path):
	has_staged_files = subprocess.run(
		["git", "diff-index", "--quiet", "--cached", "HEAD", "--"],
		cwd=content_path)

	return has_staged_files.returncode != 0

def content_has_unstaged_files(content_path):
	has_unstaged_files = subprocess.run(
		["git", "diff-files", "--quiet"],
		cwd=content_path)

	return has_unstaged_files.returncode != 0

def main():
	if len(sys.argv) != 2:
		raise RuntimeError("Expected a single argument pointing to the nightfire-open-content repo")

	content_path = sys.argv[1]

	if not content_path:
		raise RuntimeError("Provided content path was empty")

	if content_has_staged_files(content_path):
		raise RuntimeError("Content repo has staged files - please commit these first")

	if content_has_unstaged_files(content_path):
		raise RuntimeError("Content repo has unstaged files - please commit these first")

	content_hash_output = content_cmd(content_path, ["git", "rev-parse", "origin/master"])
	content_hash_match = re.match(r"^([0-9a-f]{40})$", content_hash_output)

	if not content_hash_match:
		raise RuntimeError(f"Could not parse content hash from git rev-parse. Output:\n{content_hash_output}")

	content_hash = content_hash_match.group(1)

	with open(os.path.join(SCRIPT_DIR, "content-hash.txt"), "w") as out_file:
		out_file.write(content_hash + "\n")

	print("Updated local content hash to:", content_hash)

if __name__ == "__main__":
	main()

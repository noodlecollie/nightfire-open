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

	local_branch = content_cmd(content_path, ["git", "branch", "--show-current"]).strip()

	if local_branch != "master":
		raise RuntimeError(f"Local branch is {local_branch} not master, refusing to update content hash")

	has_staged_files = content_has_staged_files(content_path)
	has_unstaged_files = content_has_unstaged_files(content_path)

	local_master_rev = content_cmd(content_path, ["git", "rev-parse", "master"]).strip()
	origin_master_rev = content_cmd(content_path, ["git", "rev-parse", "origin/master"]).strip()

	if local_master_rev != origin_master_rev:
		raise RuntimeError(f"Local revision {local_master_rev} did not match origin revision {origin_master_rev} - make sure your local branch is up to date")

	content_hash_match = re.match(r"^([0-9a-f]{40})$", origin_master_rev)

	if not content_hash_match:
		raise RuntimeError(f"Could not parse content hash from git rev-parse. Output:\n{origin_master_rev}")

	content_hash = content_hash_match.group(1)

	# If we detected uncommitted files, add this to the hash when it's written.
	# This should at least show up in the content hash file and make it obvious
	# that we've forgotten to commit.
	if has_staged_files or has_unstaged_files:
		content_hash += "-dirty"

	with open(os.path.join(SCRIPT_DIR, "content-hash.txt"), "w") as out_file:
		out_file.write(content_hash + "\n")

	if has_staged_files or has_unstaged_files:
		raise RuntimeError(f"Content repo has {'staged' if has_staged_files else 'unstaged'} files - please commit these first")

	print("Updated local content hash to:", content_hash)

if __name__ == "__main__":
	main()

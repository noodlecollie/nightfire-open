"""
We want a way to ensure that content that's been added to the content repo has actually been committed and pushed by
the time a PR is merged into master. A good way to make this obvious is to have a script run whenever the game is built,
which updates content-hash.txt with the SHA of the current commit from the content repo. This achieves two things:

1. It means that, if the state of the content repo has changed, content-hash.txt shows as modified when committing to
the main repo. This is obvious from a developer point of view, and also tracks the state of the content repo as part of
the commit.
2. When CI is run on a PR, the state of the content repo can be checked against content-hash.txt, and the CI run will
fail if the repo doesn't match the committed state.

content-hash.txt can follow the format:

<branch>-<commit SHA>(-dirty)?

This allows us to easily check which branch was used, which commit was used, and whether there were uncommitted files
present at the time the content hash was updated (eg. if the developer forgot to finalise the commit).
"""

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

def content_has_uncommitted_files(content_path):
	result = subprocess.run(
		["git", "status", "--porcelain=v1"],
		cwd=content_path,
		text=True,
		capture_output=True,
		check=True)

	return result.stdout.strip() or result.stderr.strip()

def main():
	if len(sys.argv) != 2:
		raise RuntimeError("Expected a single argument pointing to the nightfire-open-content repo")

	content_path = sys.argv[1]

	if not content_path:
		raise RuntimeError("Provided content path was empty")

	local_branch = content_cmd(content_path, ["git", "branch", "--show-current"]).strip()
	local_rev = content_cmd(content_path, ["git", "rev-parse", local_branch]).strip()
	has_uncommitted_files = content_has_uncommitted_files(content_path)
	content_hash_match = re.match(r"^([0-9a-f]{40})$", local_rev)

	if not content_hash_match:
		raise RuntimeError(f"Could not parse content hash from git rev-parse. Output:\n{local_rev}")

	content_hash = f"{local_branch}-{content_hash_match.group(1)}"

	# If we detected uncommitted files, add this to the hash when it's written.
	# This should at least show up in the content hash file and make it obvious
	# that we've forgotten to commit.
	if has_uncommitted_files:
		content_hash += "-dirty"
		print(f"Content repo has uncommitted files!", file=sys.stderr)

	with open(os.path.join(SCRIPT_DIR, "content-hash.txt"), "w") as out_file:
		out_file.write(content_hash + "\n")

	print("Updated local content hash to:", content_hash)

if __name__ == "__main__":
	main()

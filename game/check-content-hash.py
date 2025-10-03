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

import sys
import subprocess
import re
import os

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

def main():
	if len(sys.argv) != 2:
		raise RuntimeError("Expected a single argument pointing to the nightfire-open-content repo")

	content_path = sys.argv[1]

	origin_master_rev = content_cmd(content_path, ["git", "rev-parse", "origin/master"]).strip()
	origin_master_rev_match = re.match(r"^[0-9a-f]{40}$", origin_master_rev)

	if not origin_master_rev_match:
		raise RuntimeError(f"Could not parse SHA from git rev-parse. Output:\n{origin_master_rev}")

	# Make 100% sure we know what we're working with
	origin_master_rev = origin_master_rev_match.group(0)

	with open(os.path.join(SCRIPT_DIR, "content-hash.txt"), "r") as in_file:
		content_hash = in_file.read().strip()

	content_hash_match = re.match(r"^([^\s-]+)-([0-9a-f]{40})(-dirty)?$", content_hash)

	if not content_hash_match:
		raise RuntimeError(f"Could not parse hash from content-hash.txt. Output:\n{content_hash}")

	content_branch = content_hash_match.group(1)
	content_sha = content_hash_match.group(2)
	content_was_dirty = bool(content_hash_match.group(3))

	is_valid = content_branch == "master" and content_sha == origin_master_rev and not content_was_dirty

	if not is_valid:
		content_commit_output = \
			content_cmd(content_path, ["git", "log", "-1", "--format=%an (%cd): %s"], shell=False).strip()

		raise RuntimeError(
			f"Content hash mismatch: {content_hash}.\n"
			f"  Latest content repo origin/master SHA: {origin_master_rev}\n"
			f"  Latest content repo commit: {content_commit_output}\n"
			f"  Branch matches: {content_branch == 'master'}\n"
			f"  SHA matches: {content_sha == origin_master_rev}\n"
			f"  Was clean: {not content_was_dirty}"
		)

	print(f"Content hash {content_hash} matches content repo master branch")

if __name__ == "__main__":
	main()

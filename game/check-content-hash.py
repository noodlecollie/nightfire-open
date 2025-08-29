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

	content_hash_output = content_cmd(content_path, ["git", "rev-parse", "origin/master"])
	content_hash_match = re.match(r"^([0-9a-f]{40})(-dirty)?$", content_hash_output)

	if not content_hash_match:
		raise RuntimeError(f"Could not parse content hash from git rev-parse. Output:\n{content_hash_output}")

	content_hash = content_hash_match.group(1)

	if content_hash_match.group(2):
		raise RuntimeError(f"Content hash {content_hash} was last updated with uncommitted files present!")

	with open(os.path.join(SCRIPT_DIR, "content-hash.txt"), "r") as in_file:
		local_content_hash = in_file.read().strip()

	if content_hash != local_content_hash:
		content_commit_output = content_cmd(content_path, ["git", "log", "-1", "--format=%an (%cd): %s"], shell=False)

		error_message = \
		f"Content hash mismatch.\n"
		f"  This repo: {local_content_hash}\n"
		f"  Content repo master branch: {content_hash}\n"
		f"  Content repo last commit: {content_commit_output}"

		# Make it obvious if we'd forgotten to commit when the hash was updated.
		if local_content_hash.endswith("-dirty"):
			error_message += "\n  Hash was last updated with uncommitted files present!"

		raise RuntimeError(error_message)

	print(f"Content hash {local_content_hash} matches content repo master branch")

if __name__ == "__main__":
	main()

import argparse
import signal
import os
import hashlib
import time
import shutil

SPIN_INTERVAL_MS = 100
CHECK_INTERVAL_SECS = 10

WHITELIST_PATHS = [
	"waypoints"
]

should_run = True

def signal_handler(sig, frame):
	global should_run

	print("Received Ctrl-C, stopping")
	should_run = False

def parse_args():
	parser = argparse.ArgumentParser("content-copyback-monitor.py",
		description="Monitors the game directory and copies new content "
		"(eg. waypoint files) back to the content repo")

	parser.add_argument("--game-dir",
		help="Directory where gameinfo.txt is",
		required=True)

	parser.add_argument("--repo-dir",
		help="Root directory of the nightfire-open-content repo",
		required=True)

	return parser.parse_args()

def md5hash(fileName):
	with open(fileName, 'rb') as fh:
		m = hashlib.md5()
		while True:
			chunk = fh.read(65536)
			if not chunk:
				break
			m.update(chunk)
		return m.hexdigest()

def check_files(game_dir, repo_dir):
	for whitelist_path in WHITELIST_PATHS:
		sub_root = os.path.join(game_dir, whitelist_path)

		for root, dirs, files in os.walk(sub_root):
			for file in files:
				if not should_run:
					raise RuntimeError("Sigint received")

				full_path = os.path.join(root, file)
				rel_file_path = os.path.relpath(full_path, game_dir)
				repo_file_path = os.path.join(repo_dir, rel_file_path)

				should_copy = False

				if not os.path.isfile(repo_file_path):
					print(f"{rel_file_path}: New file")
					should_copy = True
				else:
					if os.stat(full_path).st_mtime > os.stat(repo_file_path).st_mtime:
						print(f"{rel_file_path}: Update time is newer")
						should_copy = True
					elif md5hash(repo_file_path) != md5hash(full_path):
						print(f"{rel_file_path}: MD5 is different")
						should_copy = True

				if not should_copy:
					continue

				os.makedirs(os.path.dirname(repo_file_path), exist_ok=True)
				shutil.copyfile(full_path, repo_file_path)
				print(f"  Copied to {repo_file_path}")

def main():
	signal.signal(signal.SIGINT, signal_handler)

	args = parse_args()

	gameinfo_txt_path = os.path.join(args.game_dir, "gameinfo.txt")

	if not os.path.isfile(gameinfo_txt_path):
		raise RuntimeError(f"Could not find {gameinfo_txt_path} for game - is the game directory correct?")

	source_dir = os.path.join(args.repo_dir, "content", "nfopen")

	if not os.path.isdir(source_dir):
		raise RuntimeError(f"Could not find {source_dir} for repo - is the repo directory correct?")

	print(f"Monitoringfor changed files in {args.game_dir} under:")

	for whitelist_path in WHITELIST_PATHS:
		print(f"  * {whitelist_path}/")

	print(f"Copying any changed files to {source_dir}")
	print(f"Press Ctrl-C to exit")

	while should_run:
		try:
			check_files(args.game_dir, source_dir)
		except Exception as ex:
			if str(ex) == "Sigint received":
				break
			else:
				raise ex

		for _ in range(int((CHECK_INTERVAL_SECS * 1000) / SPIN_INTERVAL_MS)):
			if not should_run:
				break
			time.sleep(float(SPIN_INTERVAL_MS) / 1000.0)

if __name__ == "__main__":
	main()

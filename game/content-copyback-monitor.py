import argparse
import signal
import os
import hashlib
import time
import shutil

SPIN_INTERVAL_MS = 100
CHECK_INTERVAL_SECS = 10

SKIP_EXTS = [
	".bak",
	".dll"
]

SKIP_RELPATHS = [
	".xash_id",
	"opengl.cfg",
	"console_history.txt",
	".fontcache" + os.sep,
	os.path.join("media", "cdaudio.txt")
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

def find_all_files(root_dir, log_every:int=25):
	found_files = {}

	for root, dirs, files in os.walk(root_dir):
		for file in files:
			full_path = os.path.join(root, file)
			found_files[full_path] = (os.stat(full_path).st_mtime, md5hash(full_path))

			found_so_far = len(found_files)
			if found_so_far % log_every == 0:
				print(f"Discovered {found_so_far} files...")

	print(f"Discovered a total of {len(found_files)} files")
	return found_files

def check_files(game_dir, repo_dir, existing_files):
	for file_path in existing_files.keys():
		if not should_run:
			raise RuntimeError("Sigint received")

		if not os.path.isfile(file_path):
			print(f"File {file_path} no longer exists on disk")
			del existing_files[file_path]

		rel_file_path = os.path.relpath(file_path, game_dir)
		should_skip = False

		if os.path.splitext(rel_file_path)[1] in SKIP_EXTS:
			should_skip = True
		else:
			for prefix in SKIP_RELPATHS:
				if rel_file_path.startswith(prefix):
					should_skip = True
					break

		if should_skip:
			continue

		repo_file_path = os.path.join(repo_dir, rel_file_path)

		should_copy = False

		if not os.path.isfile(repo_file_path):
			print(f"{file_path}: New file")
			should_copy = True
		else:
			update_time, md5 = existing_files[file_path]

			if update_time > os.stat(repo_file_path).st_mtime:
				print(f"{file_path}: Update time is newer")
				should_copy = True
			elif md5hash(repo_file_path) != md5:
				print(f"{file_path}: MD5 is different")
				should_copy = True

		if not should_copy:
			continue

		os.makedirs(os.path.dirname(repo_file_path), exist_ok=True)
		shutil.copyfile(file_path, repo_file_path)
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

	print(f"Discovering existing files in {args.game_dir}")
	existing_files = find_all_files(args.game_dir)

	print(f"Monitoring {args.game_dir} for changed files and copying to {source_dir}")

	while should_run:
		try:
			check_files(args.game_dir, source_dir, existing_files)
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

import os
import sys
import argparse
import spritecreation
from PIL import Image

def parseArguments():
	parser = argparse.ArgumentParser(description="Converts images to engine-compatible sprite files.")

	parser.add_argument("-i", "--input",
						required=True,
						help="Input image file.")

	parser.add_argument("-o", "--output",
						required=True,
						help="Output sprite file.")

	return parser.parse_args()

def convertFile(input : str, output : str):
	print("Reading:", input)

	if not os.path.isfile(input):
		raise OSError(f"File {input} was not found.")

	image = Image.open(input)
	imageData = image.getdata()

	if len(imageData) < 1:
		raise ValueError("Image file was empty.")

	numBytes = len(imageData) * len(imageData[0])

	print("Image data:", float(numBytes) / 1024.0, "KB uncompressed.")

	sprite = spritecreation.createSingleFrameSprite(image)
	print("Writing:", output)

	with open(output, "wb") as outFile:
		sprite.write(outFile)

	print("Written successfully.")

def main():
	args = parseArguments()
	convertFile(args.input, args.output)
	print("Done.")

if __name__ == "__main__":
	main()

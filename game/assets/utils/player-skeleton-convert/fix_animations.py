import argparse
import smd_writer
import os
from smd_reader import SMDReader

def __parseArgs():
	parser = argparse.ArgumentParser(description="Script for performing various compatibility fixes to existing HLDM animations.")
	parser.add_argument("--ref",
						help="Reference SMD containing converted model/skeleton of a Nightfire player model.")
	parser.add_argument("smd",
						action="append",
						help="SMD files, and/or directories containing files, to modify.")

	return parser.parse_args()

def __parseReference(filePath):
	print("Reading model reference from:", filePath)

	with SMDReader(filePath) as reader:
		reader.expect("version 1")
		return (reader.readBones(), reader.readSkeleton())

def __parseSmdBonesAndSkeleton(filePath):
	with SMDReader(filePath) as reader:
		reader.expect("version 1")
		return (reader.readBones(), reader.readAllSkeletonFrames())

def __computeBonesToRemove(bones, allowedBoneNames):
	disallowedBoneNames = {bone.name(): True for bone in bones.list()}

	for name in allowedBoneNames:
		if name in disallowedBoneNames:
			del disallowedBoneNames[name]

	return disallowedBoneNames.keys()

def __removeBones(bones, boneNamesToRemove):
	removalMap = {name: True for name in boneNamesToRemove}
	boneList = bones.list()

	# Iterate backwards to avoid issues with shifting indices.
	for index in range(len(boneList) - 1, -1, -1):
		bone = boneList[index]
		if bone.name() in removalMap:
			del boneList[index]

def __fixThumbBone(entryAsList, isLeft):
	# Rotate the thumb on this frame by a certain amount, so that
	# it isn't positioned weirdly. The HLDM animations don't quite work
	# right with the way the thumbs are positioned on NF models.
	# Right now these rotations are just manually calculated from the
	# position in the Nightfire reference skeleton, because I CBA to
	# do anything more complicated.

	if isLeft:
		entryAsList[4] += -1.54727
	else:
		entryAsList[4] += 1.626523

def __modifySkeletonFrame(frame, boneRenumberMap, thumbIndices):
	bones = frame["bones"]

	# Reverse iteration to avoid index shifting issues.
	for index in range(len(bones) - 1, -1, -1):
		entry = bones[index]
		oldBoneIndex = entry[0]

		if oldBoneIndex in boneRenumberMap:
			tempList = list(entry)

			try:
				__fixThumbBone(tempList, thumbIndices.index(oldBoneIndex) == 0)
			except:
				pass

			tempList[0] = boneRenumberMap[oldBoneIndex]
			bones[index] = tuple(tempList)
		else:
			del bones[index]

def __modifySkeleton(skeleton, boneRenumberMap, thumbIndices):
	for frame in skeleton:
		__modifySkeletonFrame(frame, boneRenumberMap, thumbIndices)

def __outputFilePath(filePath):
	directory = os.path.normpath(os.path.dirname(filePath))
	fileName = os.path.basename(filePath)

	outputDirectory = os.path.join(directory, "output")
	if not os.path.isdir(outputDirectory):
		os.mkdir(outputDirectory)

	return os.path.join(outputDirectory, fileName)

def __modifySmdFile(allowedBoneNames, refSkeleton, filePath):
	print("Modifying SMD file:", filePath)

	(bones, skeleton) = __parseSmdBonesAndSkeleton(filePath)
	thumbIndices = [bones.getByName("Bip01 L Finger0").index(), bones.getByName("Bip01 R Finger0").index()]

	bonesToRemove = __computeBonesToRemove(bones, allowedBoneNames)
	__removeBones(bones, bonesToRemove)
	boneRenumberMap = bones.renumber()

	__modifySkeleton(skeleton, boneRenumberMap, thumbIndices)

	outputPath = __outputFilePath(filePath)
	smd_writer.write(outputPath, bones, skeleton)

def __modifySmdDirectory(allowedBoneNames, skeleton, dirPath):
	print("Finding SMD files in directory:", dirPath)

	for item in os.listdir(dirPath):
		fullPath = os.path.join(dirPath, item)
		(_, fileExt) = os.path.splitext(os.path.basename(item))
		if fileExt == ".smd" and os.path.isfile(fullPath):
			__modifySmdFile(allowedBoneNames, skeleton, fullPath)

def __modifySmdByPath(allowedBoneNames, skeleton, path):
	if os.path.isdir(path):
		__modifySmdDirectory(allowedBoneNames, skeleton, os.path.abspath(path))
	else:
		__modifySmdFile(allowedBoneNames, skeleton, os.path.abspath(path))

def main():
	args = __parseArgs()
	(boneList, skeleton) = __parseReference(args.ref)

	allowedBoneNames = [bone.name() for bone in boneList.list()]
	print("Allowed bones:\n", "\n".join(["  " + item for item in allowedBoneNames]))

	for smdPath in args.smd:
		__modifySmdByPath(allowedBoneNames, skeleton, smdPath)

if __name__ == "__main__":
	main()
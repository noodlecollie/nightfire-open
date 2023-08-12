# This script is for converting the skeleton of a Nightfire player model
# to use the same bone configuration as a HLDM player model.
# The HLDM animations can then be used instead.

import argparse
import os
import sys
import smd_writer
from smd_reader import SMDReader
from smd_bones import SMDBone

# Making an actual index so that we can tweak which finger is best to keep.
# It turns out that using the index finger makes the hands look messed up,
# so we need to choose a more central finger.
FINGER_INDEX_FOR_HAND_BONE = 2

# This is taken from a HLDM model, once the unneeded bones are stripped out.
IDEAL_BONE_ORDER = \
[
	"Bip01",
	"Bip01 Footsteps",
	"Bip01 Pelvis",
	"Bip01 L Leg",
	"Bip01 L Leg1",
	"Bip01 L Foot",
	"Bip01 L Toe0",
	"Bip01 R Leg",
	"Bip01 R Leg1",
	"Bip01 R Foot",
	"Bip01 R Toe0",
	"Bip01 Spine",
	"Bip01 Spine1",
	"Bip01 Spine2",
	"Bip01 Spine3",
	"Bip01 Neck",
	"Bip01 Head",
	"Bip01 L Arm",
	"Bip01 L Arm1",
	"Bip01 L Arm2",
	"Bip01 L Hand",
	"Bip01 L Finger0",
	"Bip01 L Finger01",
	"Bip01 L Finger02",
	f"Bip01 L Finger{FINGER_INDEX_FOR_HAND_BONE}",
	f"Bip01 L Finger{FINGER_INDEX_FOR_HAND_BONE}1",
	f"Bip01 L Finger{FINGER_INDEX_FOR_HAND_BONE}2",
	"Bip01 R Arm",
	"Bip01 R Arm1",
	"Bip01 R Arm2",
	"Bip01 R Hand",
	"Bip01 R Finger0",
	"Bip01 R Finger01",
	"Bip01 R Finger02",
	f"Bip01 R Finger{FINGER_INDEX_FOR_HAND_BONE}",
	f"Bip01 R Finger{FINGER_INDEX_FOR_HAND_BONE}1",
	f"Bip01 R Finger{FINGER_INDEX_FOR_HAND_BONE}2",
	"Player_Mesh"
]

BONE_RENAME = \
{
	"Bip01 L Thigh": "Bip01 L Leg",
	"Bip01 L Calf": "Bip01 L Leg1",
	"Bip01 L Clavicle": "Bip01 L Arm",
	"Bip01 L UpperArm": "Bip01 L Arm1",
	"Bip01 L Forearm": "Bip01 L Arm2",

	"Bip01 R Thigh": "Bip01 R Leg",
	"Bip01 R Calf": "Bip01 R Leg1",
	"Bip01 R Clavicle": "Bip01 R Arm",
	"Bip01 R UpperArm": "Bip01 R Arm1",
	"Bip01 R Forearm": "Bip01 R Arm2",
}

BONE_FINAL_RENAME = \
{
	f"Bip01 L Finger{FINGER_INDEX_FOR_HAND_BONE}": "Bip01 L Finger1",
	f"Bip01 L Finger{FINGER_INDEX_FOR_HAND_BONE}1": "Bip01 L Finger11",
	f"Bip01 L Finger{FINGER_INDEX_FOR_HAND_BONE}2": "Bip01 L Finger12",
	f"Bip01 R Finger{FINGER_INDEX_FOR_HAND_BONE}": "Bip01 R Finger1",
	f"Bip01 R Finger{FINGER_INDEX_FOR_HAND_BONE}1": "Bip01 R Finger11",
	f"Bip01 R Finger{FINGER_INDEX_FOR_HAND_BONE}2": "Bip01 R Finger12"
}

def __parseArgs():
	parser = argparse.ArgumentParser(description="Script for converting Nightfire player model skeleton to HLDM.")
	parser.add_argument("ref", help="Reference SMD file to edit.")
	parser.add_argument("-o", "--output", default=None, help="Output file to write to. Default is input name with '_new' suffix.")

	return parser.parse_args()

# Re-parents leg bones to from spine to pelvis,
# which is the correct parent in Half Life meshes.
def __reparentLegs(bones):
	print("Re-parenting leg bones")

	pelvisBone = bones.getByName("Bip01 Pelvis")

	if pelvisBone is None:
		raise ValueError(f"Could not find bone: 'Bip01 Pelvis'.")

	for index in range(0, 2):
		side = "L" if index == 0 else "R"
		legBoneName = f"Bip01 {side} Thigh"

		bone = bones.getByName(legBoneName)

		if bone is None:
			raise ValueError(f"Could not find bone '{legBoneName}'.")

		bone.setParent(pelvisBone)

# Remaps fingers 2, 3, and 4 to just be 1, as is the case in Half Life.
def __calculateFingerRemap(bones, boneMap):
	print("Remapping finger bones")

	for index in range(0, 2):
		side = "L" if index == 0 else "R"

		handBones = []

		for boneIndex in range(0, 3):
			boneName = f"Bip01 {side} Finger{FINGER_INDEX_FOR_HAND_BONE}{boneIndex if boneIndex > 0 else ''}"
			bone = bones.getByName(boneName)

			if bone is None:
				raise ValueError(f"Could not find bone '{boneName}'.")

			handBones.append(bone)

		# Thumb and index finger are fine to remain as-is.
		# The rest must be remapped. The index finger bones
		# will be repurposed as general hand bones.

		for fingerIndex in range(1, 5):
			if fingerIndex == FINGER_INDEX_FOR_HAND_BONE:
				continue

			for subIndex in range(0, 3):
				boneName = f"Bip01 {side} Finger{fingerIndex}{subIndex if subIndex > 0 else ''}"
				bone = bones.getByName(boneName)

				if bone is None:
					raise ValueError(f"Could not find bone '{boneName}'.")

				# Map the name of this bone to the corresponding name of the general hand bones.
				boneMap[bone.name()] = handBones[subIndex].name()

# Creates a "PlayerMesh" bone, which may or may not be used by HLDM
# (but I'm playing it safe).
def __createPlayerMeshBone(bones):
	print("Creating Player_Mesh bone")

	pelvis = bones.getByName("Bip01 Pelvis")
	if pelvis is None:
		raise ValueError(f"Could not find bone 'Bip01 Pelvis'.")

	bones.list().append(SMDBone(len(bones.list()), "Player_Mesh", pelvis))

def __createFootstepsBone(bones):
	print("Creating Footsteps bone")

	root = bones.getByName("Bip01")
	if root is None:
		raise ValueError(f"Could not find bone 'Bip01'.")

	# Just append - re-ordering will occur later.
	bones.list().append(SMDBone(len(bones.list()), "Bip01 Footsteps", root))

# Creates the corresponding skeleton entry for the new Player_Mesh bone.
def __createPlayerMeshSkeletonEntry(bones, skeleton):
	print("Creating Player_Mesh skeleton entry")

	bone = bones.getByName("Player_Mesh")
	if bone is None:
		raise ValueError(f"Could not find bone 'Player_Mesh'.")

	# Gonna assume that the HLDM position is fine for Nightfire models too.
	# The model proportions seem to be the same anyway.
	skeleton.append((bone.index(), 3.279230, 2.542519, -1.277027, -1.571592, -1.570451, 0.000000))

def __createFootstepsSkeletonEntry(bones, skeleton):
	print("Creating Footsteps skeleton entry")

	bone = bones.getByName("Bip01 Footsteps")
	if bone is None:
		raise ValueError(f"Could not find bone 'Bip01 Footsteps'.")

	# Gonna assume that the HLDM position is fine for Nightfire models too.
	# The model proportions seem to be the same anyway.
	skeleton.append((bone.index(), -0.000000, 0.000000, -39.526119, 0.000000, 0.000000, 1.570795))

def __finaliseBoneList(bones, aliases):
	print("Finalising bone list")

	newBoneList = []
	boneOrderInverseMap = {}	# Name to index
	oldBoneToNewBone = {}		# Index to index
	newBoneToOldBone = {}		# Index to index

	# Create blank bones, ready to be filled in.
	for index in range(0, len(IDEAL_BONE_ORDER)):
		newBoneList.append(SMDBone(index, ""))
		boneOrderInverseMap[IDEAL_BONE_ORDER[index]] = index

	# First pass: rename appropriately.
	for bone in bones.list():
		name = bone.name()

		if name in BONE_RENAME:
			name = BONE_RENAME[name]

		if name not in boneOrderInverseMap:
			oldBoneToNewBone[bone.index()] = None
			continue

		newBone = newBoneList[boneOrderInverseMap[name]]
		newBone.setName(name)
		oldBoneToNewBone[bone.index()] = newBone.index()
		newBoneToOldBone[newBone.index()] = bone.index()

	# Second pass: link up parents.
	for bone in bones.list():
		parent = bone.parent()

		if parent is None:
			continue

		if oldBoneToNewBone[bone.index()] is None or oldBoneToNewBone[parent.index()] is None:
			continue

		newBone = newBoneList[oldBoneToNewBone[bone.index()]]
		newParent = newBoneList[oldBoneToNewBone[parent.index()]]
		newBone.setParent(newParent)

	# Record aliases.
	for name in aliases:
		bone = bones.getByName(name)
		oldIndex = bone.index()

		mappedName = aliases[name]
		if mappedName in BONE_RENAME:
			mappedName = BONE_RENAME[mappedName]

		oldBoneToNewBone[oldIndex] = boneOrderInverseMap[mappedName]

	# Do a final name fixup for fingers, as these always need to be named Finger1.
	for bone in newBoneList:
		name = bone.name()

		if name in BONE_FINAL_RENAME:
			bone.setName(BONE_FINAL_RENAME[name])

	# Sanity:
	for bone in newBoneList:
		if bone.index() not in newBoneToOldBone:
			raise AssertionError(f"New bone {bone.index()} did not have a mapping to an old bone!")

	for bone in bones.list():
		if bone.index() not in oldBoneToNewBone:
			raise AssertionError(f"Old bone '{bone.name()}' ({bone.index()}) did not have a mapping to a new bone!")

	info = [oldBoneToNewBone[index] for index in range(0, len(bones.list()))]
	for index in range(0, len(info)):
		oldBone = bones.list()[index]
		newBone = newBoneList[oldBoneToNewBone[oldBone.index()]]

		print(f"  Bone {oldBone.index()} '{oldBone.name()}': -> {newBone.index()} '{newBone.name()}'")

	bones.setList(newBoneList)
	return (oldBoneToNewBone, newBoneToOldBone)

# Renumbers skeleton items due to bone re-ordering.
def __renumberSkeletonEntries(skeleton, boneList, newBoneToOldBone):
	print("Renumbering skeleton entries")

	newSkeleton = []

	for bone in boneList.list():
		# Get the original skeleton entry index that this bone referenced
		# before it was re-ordered.
		skeletonIndex = newBoneToOldBone[bone.index()]

		# Update the index of this entry to point to the new bone.
		item = list(skeleton[skeletonIndex])
		item[0] = bone.index()

		# Add to new skeleton list.
		newSkeleton.append(tuple(item))

	return sorted(newSkeleton, key=lambda item: item[0])

# Changes bones that triangle vertices map to, due to bone re-ordering.
def __remapTriangleBones(boneMap, triangles):
	print("Remapping triangle bones")

	for index in range(0, len(triangles)):
		triangle = list(triangles[index])

		for subIndex in range(1, len(triangle)):
			vertexDef = list(triangle[subIndex])
			vertexDef[0] = boneMap[vertexDef[0]]
			triangle[subIndex] = tuple(vertexDef)

		triangles[index] = tuple(triangle)

def __readSmd(refSmd):
	with SMDReader(refSmd) as reader:
		reader.expect("version 1")

		# Load bones and create those that don't exist yet.
		bones = reader.readBones()
		__createFootstepsBone(bones)
		__createPlayerMeshBone(bones)

		# Load bone orientations.
		skeleton = reader.readSkeleton()
		__createFootstepsSkeletonEntry(bones, skeleton)
		__createPlayerMeshSkeletonEntry(bones, skeleton)

		# Re-parent legs to pelvis, as per HLDM structure.
		__reparentLegs(bones)

		# Calculate mapping from unused finger bones to index finger
		# bone structure, which will function as the hand bones.
		fingerRemap = {}
		__calculateFingerRemap(bones, fingerRemap)

		# Regenerate bone list according to desired order and bone names.
		# The old -> new (and inverse) mappings are returned.
		(oldBoneToNewBone, newBoneToOldBone) = __finaliseBoneList(bones, fingerRemap)

		# Renumber skeleton entries according to new bone indices.
		skeleton = __renumberSkeletonEntries(skeleton, bones, newBoneToOldBone)

		# Read triangles and modify which bones the vertices use, as per the new bone list.
		triangles = reader.readTriangles()
		__remapTriangleBones(oldBoneToNewBone, triangles)

		return (bones, skeleton, triangles)

def main():
	args = __parseArgs()

	refSmd = args.ref

	if not os.path.isfile(refSmd):
		print("Reference SMD", refSmd, "was not found on disk.", file=sys.stderr)
		sys.exit(1)

	if args.output is None:
		outDir = os.path.dirname(refSmd)
		fileWithExt = os.path.splitext(os.path.basename(refSmd))
		args.output = os.path.join(outDir, fileWithExt[0] + "_new" + fileWithExt[1])

	print("Converting", refSmd, "to", args.output)
	(bones, skeleton, triangles) = __readSmd(refSmd)

	print("Writing", args.output)
	smd_writer.write(args.output, bones, skeleton, triangles)

if __name__ == "__main__":
	main()

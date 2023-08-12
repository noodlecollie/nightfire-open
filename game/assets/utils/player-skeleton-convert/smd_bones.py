class SMDBone():
	def __init__(self, index=0, name="", parent=None):
		self.__parent = None
		self.__index = 0
		self.__name = ""
		self.__children = []

		self.setIndex(index)
		self.setName(name)
		self.setParent(parent)

		if self.__parent is not None:
			self.__parent.addChild(self)

	def __repr__(self):
		return f"SMDBone({self.__index}, \"{self.__name}\", {self.__parent.index() if self.__parent is not None else -1}, [{len(self.__children)}])"

	def name(self):
		return self.__name

	def setName(self, newName):
		if not isinstance(newName, str):
			raise TypeError("Name must be a string.")

		self.__name = newName

	def index(self):
		return self.__index

	def setIndex(self, newIndex):
		if not isinstance(newIndex, int):
			raise TypeError("Index must be an integer.")

		self.__index = newIndex

	def parent(self):
		return self.__parent

	def setParent(self, newParent):
		if newParent is not None and not isinstance(newParent, SMDBone):
			raise TypeError("Parent must be an SMDBone, or None.")

		if self.__parent is not None:
			self.__parent.removeChild(self)

		self.__parent = newParent

		if self.__parent is not None:
			self.__parent.addChild(self)

	def addChild(self, child):
		if not isinstance(child, SMDBone):
			raise TypeError("Child must be an SMDBone.")

		self.__children.append(child)

	def removeChild(self, child):
		if not isinstance(child, SMDBone):
			raise TypeError("Child must be an SMDBone.")

		for index in range(0, len(self.__children)):
			if self.__children[index] is child:
				del self.__children[index]
				return

		raise ValueError("Child to remove was not a child of this bone.")

	def children(self):
		return [child for child in self.__children]

class SMDBoneList():
	def __init__(self, boneList):
		if not isinstance(boneList, list):
			raise TypeError("Bones must be provided as a list.")

		self.__list = boneList

	def list(self):
		return self.__list

	def setList(self, newList):
		if not isinstance(newList, list):
			raise TypeError("Bones must be provided as a list.")

		self.__list = newList

	def getByName(self, name):
		for bone in self.__list:
			if bone.name() == name:
				return bone

		return None

	def renumber(self):
		renumberMap = {}

		for index in range(0, len(self.__list)):
			bone = self.__list[index]
			renumberMap[bone.index()] = index
			bone.setIndex(index)

		return renumberMap
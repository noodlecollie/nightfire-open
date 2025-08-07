#ifndef TEXTURECOLLECTION_H
#define TEXTURECOLLECTION_H

#include <vector>
#include <memory>
#include <functional>
#include <string>
#include "cppmemory.h"

class MiptexWrapper;
class PNGTexturePath;

class TextureCollection
{
public:
	class Item;
	enum class ItemType
	{
		Undefined = -1,
		Miptex = 0,
		PngOnDisk
	};

	TextureCollection();
	~TextureCollection();

	ItemType itemType(uint32_t index) const;
	std::string itemName(uint32_t index) const;
	uint32_t count(ItemType itemType = ItemType::Undefined) const;
	void clear();

	// Regardless of whether textures are in an exportable state.
	size_t totalBytesInUse(ItemType itemType = ItemType::Undefined) const;

	// Only counts textures that can currently be exported.
	size_t exportBytesRequired(ItemType itemType = ItemType::Undefined) const;
	size_t exportableTextureCount(ItemType itemType = ItemType::Undefined) const;

	// If index is out of range, or type does not match, returns NULL.
	MiptexWrapper* miptexAt(uint32_t index);
	const MiptexWrapper* miptexAt(uint32_t index) const;
	PNGTexturePath* pngTextureAt(uint32_t index);
	const PNGTexturePath* pngTextureAt(uint32_t index) const;

	// The callback should take a uint32_t index and an item type.
	// It should return true if the item is to be kept, and false otherwise.
	// Output vector will hold a list of mappings from original index to new index.
	// A mapping to -1 means that the item has been removed.
	void filter(const std::function<bool(uint32_t, ItemType)>& callback, std::vector<int32_t>& map);

	bool allocateAndAppend(size_t count, ItemType type);

	// If newCount is >= current count, nothing happens.
	void truncate(size_t newCount);

private:
	typedef std::shared_ptr<Item> ItemPtr;
	typedef std::vector<ItemPtr> ItemList;

	static ItemPtr createItem(ItemType type);

	void mapItems(const std::vector<int32_t>& map, uint32_t newCount);
	uint32_t countItemsOfType(ItemType itemType) const;

	ItemList m_Items;
};

#endif // TEXTURECOLLECTION_H

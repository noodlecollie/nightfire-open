#include "qrad.h"
#ifdef HLRAD_TEXTURE

#ifdef WORDS_BIGENDIAN
#error "HLRAD_TEXTURE doesn't support WORDS_BIGENDIAN, because I have no big endian machine to test it"
#endif

#include <vector>
#include <memory>
#include "miptexwrapper.h"
#include "pngtexturepath.h"
#include "stb_image.h"
#include "radtexture.h"
#include "texturedirectorylisting.h"

std::vector<RadTexture> g_RadTextures;

const std::vector<RadTexture>& RadTextures()
{
	return g_RadTextures;
}

static void LoadPngFromFileData(const char* fullPath, RadTexture& texture, const char* fileData, int dataSize)
{
	int width = 0;
	int height = 0;
	int channels = 0;

	// We always specify 4 channels to load, as RadTexture always uses RGBA for non-miptex textures.
	stbi_uc* pngRawData = stbi_load_from_memory(reinterpret_cast<const stbi_uc*>(fileData), dataSize, &width, &height, &channels, 4);

	if ( !pngRawData )
	{
		Error("Failed to load PNG data from texture file %s.\n", fullPath);
	}

	if ( width < 1 || height < 1 )
	{
		Error("Texture file %s has invalid dimensions %dx%d.\n", fullPath, width, height);
	}

	if ( !texture.loadFromRGBAData(static_cast<uint32_t>(width),
								   static_cast<uint32_t>(height),
								   reinterpret_cast<const RadTexture::RGBA*>(pngRawData),
								   static_cast<uint32_t>(width * height)) )
	{
		Error("Failed to construct texture from %s.\n", fullPath);
	}

	stbi_image_free(pngRawData);
}

static void LoadPngFromFile(const char* fullPath, RadTexture& texture)
{
	char* buffer = NULL;
	const int size = LoadFile(fullPath, &buffer);

	if ( size < 1 || !buffer )
	{
		Error("Could not open texture file %s\n", fullPath);
	}

	LoadPngFromFileData(fullPath, texture, buffer, size);
	Free(buffer);
}

static void LoadPngIntoRadTexture(const std::string path, RadTexture& texture)
{
	const std::string fullPath = g_TexDirListing.makeFullTexturePath(path);
	LoadPngFromFile(fullPath.c_str(), texture);

	texture.setName(path, true);

	Developer(DEVELOPER_LEVEL_MESSAGE, "Texture '%s': loaded from '%s'.\n",
			  texture.name().c_str(),
			  fullPath.c_str());

	Developer(DEVELOPER_LEVEL_MESSAGE, "Texture '%s': name '%s', width %u, height %u, attributes %u.\n",
		texture.name().c_str(),
		texture.name().c_str(),
		texture.width(),
		texture.height(),
		texture.attributeFlags());
}

void LoadTextures()
{
	if (!g_notextures)
	{
		Log("Load Textures:\n");
	}

	const uint32_t textureCount = g_TextureCollection.count();

	if ( textureCount > 0 )
	{
		g_RadTextures.resize(textureCount);
	}
	else
	{
		g_RadTextures.clear();
	}

	for (uint32_t textureIndex = 0; textureIndex < textureCount; ++textureIndex)
	{
		RadTexture& texture = g_RadTextures[textureIndex];

		if (g_notextures)
		{
			Developer(DEVELOPER_LEVEL_SPAM, "RAD textures not enabled, using default for texture %u.\n", textureIndex);
			texture.setToDefaultTextureImage("DEFAULT");
		}
		else
		{
			switch ( g_TextureCollection.itemType(textureIndex) )
			{
				case TextureCollection::ItemType::Miptex:
				{
					// Should already be loaded, so just validate.
					MiptexWrapper* miptexWrapper = g_TextureCollection.miptexAt(textureIndex);

					if ( miptexWrapper->hasMipmap(0) && miptexWrapper->hasPalette() )
					{
						texture.loadFromMiptex(*miptexWrapper);

						Developer(DEVELOPER_LEVEL_MESSAGE, "Texture '%s': found in '%s'.\n",
							miptexWrapper->name(),
							g_source);

						Developer(DEVELOPER_LEVEL_MESSAGE, "Texture '%s': name '%s', width %u, height %u.\n",
							miptexWrapper->name(),
							miptexWrapper->name(),
							miptexWrapper->width(),
							miptexWrapper->height());
					}
					else
					{
						// We used to load textures from WADs, but this is not supported any more.
						Error("Texture %u not present in BSP.\n", textureIndex);
					}

					break;
				}

				case TextureCollection::ItemType::PngOnDisk:
				{
					// This will need to be loaded.
					PNGTexturePath* pngTex = g_TextureCollection.pngTextureAt(textureIndex);
					LoadPngIntoRadTexture(pngTex->path(), texture);

					break;
				}

				default:
				{
					hlassert(false);
					break;
				}
			}
		}

#ifdef HLRAD_REFLECTIVITY
		{
			vec3_t totalReflectivity;
			VectorClear(totalReflectivity);

			const bool textureIsAlphaMasked = texture.attributeFlags() & RadTexture::IsAlphaMasked;

			for ( uint32_t index = 0; index < texture.totalPixels(); ++index )
			{
				vec3_t reflectivity;
				const RadTexture::RGB* pixel = texture.canvasColour(index);
				const uint8_t opacity = (texture.attributeFlags() & RadTexture::IsSpecial) ? 0 : texture.opacity(index);

				hlassert(pixel);

				if ( opacity == 0 )
				{
					VectorFill(reflectivity, 0);
				}
				else
				{
					vec3_t pixelVec;

					pixelVec[0] = (*pixel)[0];
					pixelVec[1] = (*pixel)[1];
					pixelVec[2] = (*pixel)[2];

					VectorScale(pixelVec, static_cast<double>(opacity) / 255.0, pixelVec);
					VectorScale(pixelVec, 1.0/255.0, reflectivity);

					reflectivity[0] = pow(reflectivity[0], g_texreflectgamma);
					reflectivity[1] = pow(reflectivity[1], g_texreflectgamma);
					reflectivity[2] = pow(reflectivity[2], g_texreflectgamma);

					VectorScale(reflectivity, g_texreflectscale, reflectivity);
				}

				VectorAdd (totalReflectivity, reflectivity, totalReflectivity);
			}

			VectorScale(totalReflectivity, 1.0 / static_cast<double>(texture.totalPixels()), totalReflectivity);
			texture.setReflectivity(totalReflectivity);

			Developer(DEVELOPER_LEVEL_MESSAGE, "Texture '%s': reflectivity is (%f,%f,%f).\n",
					 texture.name().c_str(),
					 totalReflectivity[0],
					 totalReflectivity[1],
					 totalReflectivity[2]);

			if (VectorMaximum(totalReflectivity) > 1.0 + NORMAL_EPSILON)
			{
				Warning("Texture '%s': reflectivity (%f,%f,%f) greater than 1.0.\n",
						texture.name().c_str(),
						totalReflectivity[0],
						totalReflectivity[1],
						totalReflectivity[2]);
			}
		}
#endif

	}

	if (!g_notextures)
	{
		Log("%u textures referenced\n", static_cast<uint32_t>(g_RadTextures.size()));
	}
}

#ifdef ZHLT_EMBEDLIGHTMAP

// color quantization algorithm

#define CQ_DIM 3

template<class T, class T2, class T3> inline void CQ_VectorSubtract (const T a[CQ_DIM], const T2 b[CQ_DIM], T3 c[CQ_DIM])
{
	for (int x = 0; x < CQ_DIM; x++)
	{
		c[x] = a[x] - b[x];
	}
}

template<class T, class T2, class T3> inline void CQ_VectorAdd (const T a[CQ_DIM], const T2 b[CQ_DIM], T3 c[CQ_DIM])
{
	for (int x = 0; x < CQ_DIM; x++)
	{
		c[x] = a[x] + b[x];
	}
}

template<class T, class T2> inline void CQ_VectorScale (const T a[CQ_DIM], const T2 b, T c[CQ_DIM])
{
	for (int x = 0; x < CQ_DIM; x++)
	{
		c[x] = a[x] * b;
	}
}

template<class T, class T2> inline void CQ_VectorCopy (const T a[CQ_DIM], T2 b[CQ_DIM])
{
	for (int x = 0; x < CQ_DIM; x++)
	{
		b[x] = a[x];
	}
}

template<class T> inline void CQ_VectorClear (T a[CQ_DIM])
{
	for (int x = 0; x < CQ_DIM; x++)
	{
		a[x] = (T)0;
	}
}

template<class T> inline T CQ_DotProduct (const T a[CQ_DIM], const T b[CQ_DIM])
{
	T dot = (T)0;
	for (int x = 0; x < CQ_DIM; x++)
	{
		dot += a[x] * b[x];
	}
	return dot;
}

typedef struct
{
	int axis;
	int dist;
	double numpoints[2];
}
cq_splitter_t; // partition the space into { point: point[axis] < dist } and { point: point[axis] >= dist }

typedef struct cq_node_s
{
	bool isleafnode;
	cq_node_s *parentnode;
	cq_node_s *childrennode[2];

	int numpoints; // numpoints > 0
	unsigned char (*refpoints)[CQ_DIM];
	double centerofpoints[CQ_DIM];

	bool needsplit;
	cq_splitter_t bestsplitter;
	double splitpriority;
}
cq_node_t; // a cuboid region; the root node is the entire cube whose size is 255

typedef struct cq_searchnode_s
{
	bool isleafnode;
	cq_searchnode_s *childrennode[2];

	int planeaxis;
	int planedist;

	int result;
}
cq_searchnode_t;

static void CQ_SelectPartition (cq_node_t *node)
{
	CQ_VectorClear (node->centerofpoints);
	for (int i = 0; i < node->numpoints; i++)
	{
		CQ_VectorAdd (node->centerofpoints, node->refpoints[i], node->centerofpoints);
	}
	CQ_VectorScale (node->centerofpoints, 1 / (double)node->numpoints, node->centerofpoints);

	node->needsplit = false;
	for (int k = 0; k < CQ_DIM; k++)
	{
		double count;
		double counts[256];
		double sum[CQ_DIM];
		double sums[256][CQ_DIM];

		double bucketsums[256][CQ_DIM];
		int bucketsizes[256];

		const unsigned char (*nodepoints)[CQ_DIM] = node->refpoints;
		const int nodenumpoints = node->numpoints;

		memset (bucketsums, 0, 256 * sizeof (double [CQ_DIM]));
		memset (bucketsizes, 0, 256 * sizeof (int));
		for (int i = 0; i < nodenumpoints; i++)
		{
			int j = nodepoints[i][k];
			bucketsizes[j]++;
			CQ_VectorAdd (bucketsums[j], nodepoints[i], bucketsums[j]);
		}

		int min = 256;
		int max = -1;
		count = 0;
		CQ_VectorClear (sum);
		int j;
		for (j = 0; j < 256; j++)
		{
			counts[j] = count;
			CQ_VectorCopy (sum, sums[j]);
			count += bucketsizes[j];
			CQ_VectorAdd (sum, bucketsums[j], sum);
			if (bucketsizes[j] > 0)
			{
				if (j < min)
				{
					min = j;
				}
				if (j > max)
				{
					max = j;
				}
			}
		}
		if (max < min)
		{
			Error ("CQ_SelectPartition: internal error");
		}
		// sweep along the axis and find the plane that maximize square error reduction
		for (j = min + 1; j < max + 1; j++)
		{
			double priority = 0; // the decrease in total square deviation
			priority -= CQ_DotProduct (sum, sum) / count;
			priority += CQ_DotProduct (sums[j], sums[j]) / counts[j];
			double remain[CQ_DIM];
			CQ_VectorSubtract (sum, sums[j], remain); // sums and counts are precise (have no round-off error)
			priority += CQ_DotProduct (remain, remain) / (count - counts[j]);
			if (node->needsplit == false ||
				priority > node->splitpriority + 0.1 ||
				priority >= node->splitpriority - 0.1
				&& fabs (counts[j] - count / 2) < fabs (node->bestsplitter.numpoints[0] - count / 2))
			{
				node->needsplit = true;
				node->splitpriority = priority;
				node->bestsplitter.axis = k;
				node->bestsplitter.dist = j;
				node->bestsplitter.numpoints[0] = counts[j];
				node->bestsplitter.numpoints[1] = count - counts[j];
			}
		}
	}
}

static cq_searchnode_t *CQ_AllocSearchTree (int maxcolors)
{
	cq_searchnode_t *searchtree;
	searchtree = (cq_searchnode_t *)malloc ((2 * maxcolors - 1) * sizeof (cq_searchnode_t));
	hlassume (searchtree != NULL, assume_NoMemory);
	return searchtree;
}

static void CQ_FreeSearchTree (cq_searchnode_t *searchtree)
{
	free (searchtree);
}

static void CQ_CreatePalette (int numpoints, const unsigned char (*points)[CQ_DIM],
					   int maxcolors, unsigned char (*colors_out)[CQ_DIM], int &numcolors_out,
					   cq_searchnode_t *searchtree_out) //[2 * maxcolors - 1]
{
	if (numpoints <= 0 || maxcolors <= 0)
	{
		numcolors_out = 0;
		return;
	}

	unsigned char (*pointarray)[CQ_DIM];
	pointarray = (unsigned char (*)[CQ_DIM])malloc (numpoints * sizeof (unsigned char [CQ_DIM]));
	hlassume (pointarray != NULL, assume_NoMemory);
	memcpy (pointarray, points, numpoints * sizeof (unsigned char [CQ_DIM]));

	cq_node_t *n;
	cq_searchnode_t *s;
	int numnodes = 0;
	int maxnodes = 2 * maxcolors - 1;
	cq_node_t *nodes = (cq_node_t *)malloc (maxnodes * sizeof (cq_node_t));
	hlassume (nodes != NULL, assume_NoMemory);

	n = &nodes[0];
	numnodes++;

	n->isleafnode = true;
	n->parentnode = NULL;
	n->numpoints = numpoints;
	n->refpoints = pointarray;
	CQ_SelectPartition (n);
	int i;

	for (i = 1; i < maxcolors; i++)
	{
		bool needsplit;
		double bestpriority;
		cq_node_t *bestnode;

		needsplit = false;
		for (int j = 0; j < numnodes; j++)
		{
			n = &nodes[j];
			if (!n->isleafnode || !n->needsplit)
			{
				continue;
			}
			if (needsplit == false || n->splitpriority > bestpriority + 0.1)
			{
				needsplit = true;
				bestpriority = n->splitpriority;
				bestnode = n;
			}
		}
		if (!needsplit)
		{
			break;
		}

		bestnode->isleafnode = false;
		for (int k = 0; k < 2; k++)
		{
			n = &nodes[numnodes];
			numnodes++;
			if (numnodes > maxnodes)
			{
				Error ("CQ_CreatePalette: internal error");
			}

			bestnode->childrennode[k] = n;
			n->isleafnode = true;
			n->parentnode = bestnode;
			n->numpoints = 0;
			n->refpoints = NULL;
		}

		// partition the points using the best splitter
		{
			const int splitaxis = bestnode->bestsplitter.axis;
			const int splitdist = bestnode->bestsplitter.dist;
			const int numpoints = bestnode->numpoints;
			unsigned char (*points)[CQ_DIM] = bestnode->refpoints;

			unsigned char (*left)[CQ_DIM];
			unsigned char (*right)[CQ_DIM];
			left = &bestnode->refpoints[0];
			right = &bestnode->refpoints[bestnode->numpoints - 1];
			while (1)
			{
				while ((*left)[splitaxis] < splitdist)
				{
					left++;
				}
				while ((*right)[splitaxis] >= splitdist)
				{
					right--;
				}
				if (left >= right)
				{
					break;
				}
				unsigned char tmp[CQ_DIM];
				CQ_VectorCopy (*left, tmp);
				CQ_VectorCopy (*right, *left);
				CQ_VectorCopy (tmp, *right);
			}
			if (right != left - 1)
			{
				Error ("CQ_CreatePalette: internal error");
			}

			bestnode->childrennode[0]->numpoints = left - bestnode->refpoints;
			bestnode->childrennode[0]->refpoints = bestnode->refpoints;
			bestnode->childrennode[1]->numpoints = &bestnode->refpoints[bestnode->numpoints] - left;
			bestnode->childrennode[1]->refpoints = left;
			if (bestnode->childrennode[0]->numpoints <= 0 ||
				bestnode->childrennode[0]->numpoints != bestnode->bestsplitter.numpoints[0])
			{
				Error ("CQ_CreatePalette: internal error");
			}
			if (bestnode->childrennode[1]->numpoints <= 0 ||
				bestnode->childrennode[1]->numpoints != bestnode->bestsplitter.numpoints[1])
			{
				Error ("CQ_CreatePalette: internal error");
			}
		}

		CQ_SelectPartition (bestnode->childrennode[0]);
		CQ_SelectPartition (bestnode->childrennode[1]);
	}

	for (i = 0; i < numnodes; i++)
	{
		n = &nodes[i];
		s = &searchtree_out[i];
		s->isleafnode = n->isleafnode;
		if (!n->isleafnode)
		{
			s->planeaxis = n->bestsplitter.axis;
			s->planedist = n->bestsplitter.dist;
			s->childrennode[0] = &searchtree_out[n->childrennode[0] - nodes];
			s->childrennode[1] = &searchtree_out[n->childrennode[1] - nodes];
		}
	}

	numcolors_out = 0;
	n = &nodes[0];
	while (1)
	{
		while (!n->isleafnode)
		{
			n = n->childrennode[0];
		}
		s = &searchtree_out[n - nodes];
		s->result = numcolors_out;
		for (int k = 0; k < CQ_DIM; k++)
		{
			int val = (int)floor (n->centerofpoints[k] + 0.5 + 0.00001);
			val = qmax (0, qmin (val, 255));
			colors_out[numcolors_out][k] = val;
		}
		numcolors_out++;
		while (n->parentnode)
		{
			if (n == n->parentnode->childrennode[0])
			{
				break;
			}
			n = n->parentnode;
		}
		if (!n->parentnode)
		{
			break;
		}
		n = n->parentnode->childrennode[1];
	}

	if (2 * numcolors_out - 1 != numnodes)
	{
		Error ("CQ_CreatePalette: internal error");
	}

	free (pointarray);
	free (nodes);
}

static void CQ_MapPoint_r (int *bestdist, int *best,
					cq_searchnode_t *node, const unsigned char (*colors)[CQ_DIM],
					const unsigned char point[CQ_DIM], int searchradius)
{
	while (!node->isleafnode)
	{
		int dist = point[node->planeaxis] - node->planedist;
		if (dist <= -searchradius)
		{
			node = node->childrennode[0];
		}
		else if (dist >= searchradius - 1)
		{
			node = node->childrennode[1];
		}
		else
		{
			CQ_MapPoint_r (bestdist, best, node->childrennode[0], colors, point, searchradius);
			CQ_MapPoint_r (bestdist, best, node->childrennode[1], colors, point, searchradius);
			return;
		}
	}
	int dist = 0;
	for (int k = 0; k < CQ_DIM; k++)
	{
		dist += (colors[node->result][k] - point[k]) * (colors[node->result][k] - point[k]);
	}
	if (dist <= *bestdist)
	{
		if (dist < *bestdist || node->result < *best)
		{
			*bestdist = dist;
			*best = node->result;
		}
	}
}

static int CQ_MapPoint (const unsigned char point[CQ_DIM], const unsigned char (*colors)[CQ_DIM], int numcolors, cq_searchnode_t *searchtree)
{
	if (numcolors <= 0)
	{
		Error ("CQ_MapPoint: internal error");
	}

	cq_searchnode_t *node;
	int bestdist;
	int best;
	int searchradius;

	for (node = searchtree; !node->isleafnode; )
	{
		node = node->childrennode[point[node->planeaxis] >= node->planedist];
	}
	best = node->result;
	bestdist = 0;
	for (int k = 0; k < CQ_DIM; k++)
	{
		bestdist += (colors[best][k] - point[k]) * (colors[best][k] - point[k]);
	}

	searchradius = (int)ceil(sqrt ((double)bestdist) + 0.1);
	CQ_MapPoint_r (&bestdist, &best, searchtree, colors, point, searchradius);
	return best;
}

// =====================================================================================
//  EmbedLightmapInTextures
//      check for "zhlt_embedlightmap" and update g_dfaces, g_texinfo, g_dtexdata and g_dlightdata
// =====================================================================================

#define RADTEXTURES_MAX 2048 // should be smaller than 62 * 62 and smaller than MAX_MAP_TEXTURES
static int g_newtextures_num = 0;
static byte *g_newtextures_data[RADTEXTURES_MAX];
static int g_newtextures_size[RADTEXTURES_MAX];

int NewTextures_GetCurrentMiptexIndex()
{
	return g_TextureCollection.count() + g_newtextures_num;
}

void NewTextures_PushTexture (int size, void *data)
{
	if (g_newtextures_num >= RADTEXTURES_MAX)
	{
		Error("the number of textures created by hlrad has exceeded its internal limit (%d).", (int)RADTEXTURES_MAX);
	}
	g_newtextures_data[g_newtextures_num] = (byte *)malloc (size);
	hlassume (g_newtextures_data[g_newtextures_num] != NULL, assume_NoMemory);
	memcpy (g_newtextures_data[g_newtextures_num], data, size);
	g_newtextures_size[g_newtextures_num] = size;
	g_newtextures_num++;
}

void NewTextures_Write()
{
	if (!g_newtextures_num)
	{
		return;
	}

	const uint32_t oldCount = g_TextureCollection.count();
	g_TextureCollection.allocateAndAppend(g_newtextures_num, TextureCollection::ItemType::Miptex);

	for ( uint32_t index = oldCount; index < g_TextureCollection.count(); ++index )
	{
		MiptexWrapper* wrapper = g_TextureCollection.miptexAt(index);
		hlassert(wrapper);

		const miptex_t* miptex = (const miptex_t*)g_newtextures_data[index];
		hlassert(g_newtextures_size[index] == MiptexWrapper::totalIdealBytesRequired(miptex->width, miptex->height));

		wrapper->setFromMiptex(miptex);
		free(g_newtextures_data[index]);
	}

	hlassume(g_TextureCollection.count() < MAX_MAP_TEXTURES, assume_MAX_MAP_TEXTURES);

	g_newtextures_num = 0;
}

static unsigned int Hash (int size, void *data)
{
	unsigned int hash = 0;
	for (int i = 0; i < size; i++)
	{
		hash = 31 * hash + ((unsigned char *)data)[i];
	}
	return hash;
}

static void GetLightInt (dface_t *face, const int texsize[2], int ix, int iy, vec3_t &light)
{
	ix = qmax (0, qmin (ix, texsize[0]));
	iy = qmax (0, qmin (iy, texsize[1]));
	VectorClear (light);
	if (face->lightofs < 0)
	{
		return;
	}
	for (int k = 0; k < MAXLIGHTMAPS && face->styles[k] != 255; k++)
	{
		byte *samples = &g_dlightdata[face->lightofs + k * (texsize[0] + 1) * (texsize[1] + 1) * 3];
		if (face->styles[k] == 0)
		{
			VectorAdd (light, &samples[(iy * (texsize[0] + 1) + ix) * 3], light);
		}
	}
}

static void GetLight (dface_t *face, const int texsize[2], double x, double y, vec3_t &light)
{
	int ix, iy;
	double dx, dy;
	ix = (int)floor (x);
	iy = (int)floor (y);
	dx = x - ix;
	dx = qmax (0, qmin (dx, 1));
	dy = y - iy;
	dy = qmax (0, qmin (dy, 1));

	// do bilinear interpolation
	vec3_t light00, light10, light01, light11;
	GetLightInt (face, texsize, ix, iy, light00);
	GetLightInt (face, texsize, ix + 1, iy, light10);
	GetLightInt (face, texsize, ix, iy + 1, light01);
	GetLightInt (face, texsize, ix + 1, iy + 1, light11);
	vec3_t light0, light1;
	VectorScale (light00, 1 - dy, light0);
	VectorMA (light0, dy, light01, light0);
	VectorScale (light10, 1 - dy, light1);
	VectorMA (light1, dy, light11, light1);
	VectorScale (light0, 1 - dx, light);
	VectorMA (light, dx, light1, light);
}

static bool GetValidTextureName (int miptex, char* name, unsigned int length)
{
	static const char* RAD_PREFIX = "_rad";

	const MiptexWrapper* wrapper = g_TextureCollection.miptexAt(miptex);
	if ( !wrapper )
	{
		return false;
	}

	safe_strncpy (name, wrapper->name(), length);

	if (strcmp(name, wrapper->name()) != 0)
	{
		return false;
	}

	if (strlen(name) >= sizeof(RAD_PREFIX) && strncasecmp(&name[1], RAD_PREFIX, sizeof(RAD_PREFIX) - 1) == 0)
	{
		return false;
	}

	return true;
}

void EmbedLightmapInTextures ()
{
	if (!g_lightdatasize)
	{
		// hlrad hasn't run
		return;
	}
	if (g_TextureCollection.count() < 1)
	{
		// texdata hasn't been initialized
		return;
	}
	if (g_notextures)
	{
		// hlrad didn't load the wad files
		return;
	}

	int i, j, k;
	int miplevel;
	int count = 0;
	int count_bytes = 0;
	bool logged = false;

	for (i = 0; i < g_numfaces; i++)
	{
		dface_t *f = &g_dfaces[i];

		int max_surface_extent = GetSurfaceExtent( f );
		int texture_step = GetTextureStep( f );

		if (f->lightofs == -1) // some faces don't have lightmap
		{
			continue;
		}
		if (f->texinfo < 0 || f->texinfo >= g_numtexinfo)
		{
			continue;
		}

		entity_t *ent = g_face_entity[i];
		int originaltexinfonum = f->texinfo;
		texinfo_t *originaltexinfo = &g_texinfo[originaltexinfonum];
		char texname[MIPTEX_NAME_LENGTH];
		if (!GetValidTextureName (originaltexinfo->miptex, texname, sizeof(texname)))
		{
			continue;
		}

		if ( originaltexinfo->miptex == -1 )
		{
			// This is fine - hint brushes trigger this.
			continue;
		}

		if ( originaltexinfo->miptex < 0 || originaltexinfo->miptex >= g_RadTextures.size() )
		{
			Warning("EmbedLightmapTextures(): Texinfo %d referenced invalid texture %d.\n", originaltexinfonum, originaltexinfo->miptex);
			continue;
		}

		RadTexture& radTex = g_RadTextures[originaltexinfo->miptex];

		if (ent == &g_entities[0]) // world
		{
			continue;
		}
		if (!strncmp (texname, SPECIALTEX_SKY, sizeof(SPECIALTEX_SKY) - 1)
			|| originaltexinfo->flags & TEX_SPECIAL) // skip special surfaces
		{
			continue;
		}
		if (!IntForKey (ent, "zhlt_embedlightmap"))
		{
			continue;
		}

		if (!logged)
		{
			Log ("\n");
			Log ("Embed Lightmap : ");
			Developer (DEVELOPER_LEVEL_MESSAGE, "\n");
			logged = true;
		}

		bool poweroftwo = DEFAULT_EMBEDLIGHTMAP_POWEROFTWO;
		vec_t denominator = DEFAULT_EMBEDLIGHTMAP_DENOMINATOR;
		vec_t gamma = DEFAULT_EMBEDLIGHTMAP_GAMMA;
		int resolution = DEFAULT_EMBEDLIGHTMAP_RESOLUTION;
		if (IntForKey (ent, "zhlt_embedlightmapresolution"))
		{
			resolution = IntForKey (ent, "zhlt_embedlightmapresolution");
			if (resolution <= 0 || resolution > texture_step || ((resolution - 1) & resolution) != 0)
			{
				Error ("resolution cannot be %d; valid values are 1, 2, 4 ... %d.", resolution, (int)texture_step);
			}
		}

		// calculate texture size and allocate memory for all miplevels

		int texturesize[2];
		float (*texture)[5]; // red, green, blue and alpha channel; the last one is number of samples
		byte (*texturemips[MIPLEVELS])[4]; // red, green, blue and alpha channel
		int s, t;
		int texmins[2];
		int texmaxs[2];
		int texsize[2]; // texturesize = (texsize + 1) * texture_step
		int side[2];

		GetFaceExtents (i, texmins, texmaxs);
		texsize[0] = texmaxs[0] - texmins[0];
		texsize[1] = texmaxs[1] - texmins[1];
		if (texsize[0] < 0 || texsize[1] < 0 || texsize[0] > max_surface_extent || texsize[1] > max_surface_extent)
		{
			Warning ("skipped a face with bad surface extents @ (%4.3f %4.3f %4.3f)", g_face_centroids[i][0], g_face_centroids[i][1], g_face_centroids[i][2]);
			continue;
		}

		for (k = 0; k < 2; k++)
		{
			texturesize[k] = (texsize[k] + 1) * texture_step;
			if (texturesize[k] < texsize[k] * texture_step + resolution * 4)
			{
				texturesize[k] = texsize[k] * texture_step + resolution * 4; // prevent edge bleeding
			}
			texturesize[k] = (texturesize[k] + resolution - 1) / resolution;
			texturesize[k] += 15 - (texturesize[k] + 15) % 16; // must be multiples of 16
			if (poweroftwo)
			{
				for (j = 0; j <= 30; j++)
				{
					if ((1 << j) >= texturesize[k])
					{
						texturesize[k] = (1 << j);
						break;
					}
				}
			}
			side[k] = (texturesize[k] * resolution - texsize[k] * texture_step) / 2;
		}
		texture = (float (*)[5])malloc (texturesize[0] * texturesize[1] * sizeof (float [5]));
		hlassume (texture != NULL, assume_NoMemory);
		for (miplevel = 0; miplevel < MIPLEVELS; miplevel++)
		{
			texturemips[miplevel] = (byte (*)[4])malloc ((texturesize[0] >> miplevel) * (texturesize[1] >> miplevel) * sizeof (byte [4]));
			hlassume (texturemips[miplevel] != NULL, assume_NoMemory);
		}

		// calculate the texture

		for (t = 0; t < texturesize[1]; t++)
		{
			for (s = 0; s < texturesize[0]; s++)
			{
				float (*dest)[5] = &texture[t * texturesize[0] + s];
				VectorFill (*dest, 0);
				(*dest)[3] = 0;
				(*dest)[4] = 0;
			}
		}
		for (t = -side[1]; t < texsize[1] * texture_step + side[1]; t++)
		{
			for (s = -side[0]; s < texsize[0] * texture_step + side[0]; s++)
			{
				double s_vec, t_vec;
				double src_s, src_t;
				int src_is, src_it;
				byte src_color[3] = {0,0,0};
				double dest_s, dest_t;
				int dest_is, dest_it;
				float (*dest)[5];
				double light_s, light_t;
				vec3_t light;

				s_vec = s + texmins[0] * texture_step + 0.5;
				t_vec = t + texmins[1] * texture_step + 0.5;

				if (resolution == 1)
				{
					dest_s = s_vec;
					dest_t = t_vec;
				}
				else // the final blurred texture is shifted by a half pixel so that lightmap samples align with the center of pixels
				{
					dest_s = s_vec / resolution + 0.5;
					dest_t = t_vec / resolution + 0.5;
				}
				dest_s = dest_s - texturesize[0] * floor (dest_s / texturesize[0]);
				dest_t = dest_t - texturesize[1] * floor (dest_t / texturesize[1]);
				dest_is = (int)floor (dest_s); // dest_is = dest_s % texturesize[0]
				dest_it = (int)floor (dest_t); // dest_it = dest_t % texturesize[1]
				dest_is = qmax (0, qmin (dest_is, texturesize[0] - 1));
				dest_it = qmax (0, qmin (dest_it, texturesize[1] - 1));
				dest = &texture[dest_it * texturesize[0] + dest_is];

				src_s = s_vec;
				src_t = t_vec;
				src_s = src_s - radTex.width() * floor (src_s / radTex.width());
				src_t = src_t - radTex.height() * floor (src_t / radTex.height());
				src_is = (int)floor (src_s); // src_is = src_s % radTex.width()
				src_it = (int)floor (src_t); // src_it = src_t % radTex.height()
				src_is = qmax (0, qmin (src_is, radTex.width() - 1));
				src_it = qmax (0, qmin (src_it, radTex.height() - 1));

				const RadTexture::RGB* colour = radTex.canvasColour(src_is, src_it);
				if ( colour )
				{
					src_color[0] = (*colour)[0];
					src_color[1] = (*colour)[1];
					src_color[2] = (*colour)[2];
				}

				const uint8_t opacity = radTex.opacity(src_is, src_it);

				// get light from the center of the destination pixel
				light_s = (s_vec + resolution * (dest_is + 0.5 - dest_s)) / texture_step - texmins[0];
				light_t = (t_vec + resolution * (dest_it + 0.5 - dest_t)) / texture_step - texmins[1];
				GetLight (f, texsize, light_s, light_t, light);

				(*dest)[4] += 1;

				if (opacity > 0x00)
				{
					float flOpacity = (float)opacity / 255.0f;

					for (k = 0; k < 3; k++)
					{
						float v = (float)src_color[k] * flOpacity * pow(light[k] / denominator, gamma);
						(*dest)[k] += 255 * qmax (0, qmin (v, 255));
					}

					(*dest)[3] += 255;
				}
			}
		}
		for (t = 0; t < texturesize[1]; t++)
		{
			for (s = 0; s < texturesize[0]; s++)
			{
				float (*src)[5] = &texture[t * texturesize[0] + s];
				byte (*dest)[4] = &texturemips[0][t * texturesize[0] + s];

				if ((*src)[4] == 0) // no samples (outside face range?)
				{
					VectorFill (*dest, 0);
					(*dest)[3] = 255;
				}
				else
				{
					if ((*src)[3] / (*src)[4] <= 0.4 * 255) // transparent
					{
						VectorFill (*dest, 0);
						(*dest)[3] = 0;
					}
					else // normal
					{
						for (j = 0; j < 3; j++)
						{
							int val = (int)floor ((*src)[j] / (*src)[3] + 0.5);
							(*dest)[j] = qmax (0, qmin (val, 255));
						}
						(*dest)[3] = 255;
					}
				}
			}
		}

		for (miplevel = 1; miplevel < MIPLEVELS; miplevel++)
		{
			for (t = 0; t < (texturesize[1] >> miplevel); t++)
			{
				for (s = 0; s < (texturesize[0] >> miplevel); s++)
				{
					byte (*src[4])[4];
					byte (*dest)[4];
					double average[4];

					dest = &texturemips[miplevel][t * (texturesize[0] >> miplevel) + s];
					src[0] = &texturemips[miplevel - 1][(2 * t) * (texturesize[0] >> (miplevel - 1)) + (2 * s)];
					src[1] = &texturemips[miplevel - 1][(2 * t) * (texturesize[0] >> (miplevel - 1)) + (2 * s + 1)];
					src[2] = &texturemips[miplevel - 1][(2 * t + 1) * (texturesize[0] >> (miplevel - 1)) + (2 * s)];
					src[3] = &texturemips[miplevel - 1][(2 * t + 1) * (texturesize[0] >> (miplevel - 1)) + (2 * s + 1)];

					VectorClear (average);
					average[3] = 0;
					for (k = 0; k < 4; k++)
					{
						for (j = 0; j < 3; j++)
						{
							average[j] += (*src[k])[3] * (*src[k])[j];
						}
						average[3] += (*src[k])[3];
					}

					if (average[3] / 4 <= 0.4 * 255)
					{
						VectorClear (*dest);
						(*dest)[3] = 0;
					}
					else
					{
						for (j = 0; j < 3; j++)
						{
							int val = (int)floor (average[j] / average[3] + 0.5);
							(*dest)[j] = qmax (0, qmin (val, 255));
						}
						(*dest)[3] = 255;
					}
				}
			}
		}

		// create its palette

		byte palette[256][3];
		cq_searchnode_t *palettetree = CQ_AllocSearchTree (256);
		int paletteoffset;
		int palettenumcolors;

		{
			int palettemaxcolors;
			int numsamplepoints;
			unsigned char (*samplepoints)[3];

			if (radTex.attributeFlags() & RadTexture::IsAlphaMasked)
			{
				paletteoffset = 0;
				palettemaxcolors = 255;

				const RadTexture::RGB* transparentColour = radTex.paletteColour(255);

				if ( transparentColour )
				{
					palette[255][0] = (*transparentColour)[0];
					palette[255][1] = (*transparentColour)[1];
					palette[255][2] = (*transparentColour)[2];
				}
				else
				{
					// Should never happen.
					Error("Internal error: could not get RadTexture transparency colour.\n");
				}
			}
			else
			{
				paletteoffset = 0;
				palettemaxcolors = 256;
			}

			samplepoints = (unsigned char (*)[3])malloc (texturesize[0] * texturesize[1] * sizeof (unsigned char [3]));
			hlassume (samplepoints != NULL, assume_NoMemory);
			numsamplepoints = 0;
			for (t = 0; t < texturesize[1]; t++)
			{
				for (s = 0; s < texturesize[0]; s++)
				{
					byte (*src)[4] = &texturemips[0][t * texturesize[0] + s];
					if ((*src)[3] > 0)
					{
						VectorCopy (*src, samplepoints[numsamplepoints]);
						numsamplepoints++;
					}
				}
			}

			CQ_CreatePalette (numsamplepoints, samplepoints, palettemaxcolors, &palette[paletteoffset], palettenumcolors, palettetree);
			for (j = palettenumcolors; j < palettemaxcolors; j++)
			{
				VectorClear (palette[paletteoffset + j]);
			}

			free (samplepoints);
		}

		// emit a texinfo

		hlassume (g_numtexinfo < MAX_MAP_TEXINFO, assume_MAX_MAP_TEXINFO);
		f->texinfo = g_numtexinfo;
		texinfo_t *info = &g_texinfo[g_numtexinfo];
		g_numtexinfo++;

		*info = g_texinfo[originaltexinfonum];
		if (resolution != 1)
		{
			// apply a scale and a shift over the original vectors
			for (k = 0; k < 2; k++)
			{
				VectorScale (info->vecs[k], 1.0 / resolution, info->vecs[k]);
				info->vecs[k][3] = info->vecs[k][3] / resolution + 0.5;
			}
		}
		info->miptex = NewTextures_GetCurrentMiptexIndex ();

		// emit a texture

		int miptexsize;

		miptexsize = (int)sizeof (miptex_t);
		for (miplevel = 0; miplevel < MIPLEVELS; miplevel++)
		{
			miptexsize += (texturesize[0] >> miplevel) * (texturesize[1] >> miplevel);
		}
		miptexsize += 2 + 256 * 3 + 2;
		miptex_t *miptex = (miptex_t *)malloc (miptexsize);
		hlassume (miptex != NULL, assume_NoMemory);

		memset (miptex, 0, sizeof (miptex_t));
		miptex->width = texturesize[0];
		miptex->height = texturesize[1];
		byte *p = (byte *)miptex + sizeof (miptex_t);
		for (miplevel = 0; miplevel < MIPLEVELS; miplevel++)
		{
			miptex->offsets[miplevel] = p - (byte *)miptex;
			for (int t = 0; t < (texturesize[1] >> miplevel); t++)
			{
				for (int s = 0; s < (texturesize[0] >> miplevel); s++)
				{
					byte (*src)[4] = &texturemips[miplevel][t * (texturesize[0] >> miplevel) + s];
					if ((*src)[3] > 0)
					{
						if (palettenumcolors)
						{
							unsigned char point[3];
							VectorCopy (*src, point);
							*p = paletteoffset + CQ_MapPoint (point, &palette[paletteoffset], palettenumcolors, palettetree);
						}
						else // this should never happen
						{
							*p = paletteoffset + 0;
						}
					}
					else
					{
						*p = 255;
					}
					p++;
				}
			}
		}
		*(short *)p = 256;
		p += 2;
		memcpy (p, palette, 256 * 3);
		p += 256 * 3;
		*(short *)p = 0;
		p += 2;
		if (p != (byte *)miptex + miptexsize)
		{
			Error ("EmbedLightmapInTextures: internal error");
		}

		if (radTex.attributeFlags() & RadTexture::IsAlphaMasked)
		{
			safe_strncpy(miptex->name, "{_rad", sizeof(miptex->name));
		}
		else
		{
			safe_strncpy(miptex->name, "__rad", sizeof(miptex->name));
		}
		if (originaltexinfonum < 0 || originaltexinfonum > 99999)
		{
			Error ("EmbedLightmapInTextures: internal error: texinfo out of range");
		}

		if ( sizeof(miptex->name) < 16 )
		{
			Error("EmbedLightmapInTextures: internal error: texture name buffer size is too small");
		}

		miptex->name[5] = '0' + (originaltexinfonum / 10000) % 10; // store the original texinfo
		miptex->name[6] = '0' + (originaltexinfonum / 1000) % 10;
		miptex->name[7] = '0' + (originaltexinfonum / 100) % 10;
		miptex->name[8] = '0' + (originaltexinfonum / 10) % 10;
		miptex->name[9] = '0' + (originaltexinfonum) % 10;
		char table[62];
		for (int k = 0; k < 62; k++)
		{
			table[k] = k >= 36? 'a' + (k - 36): k >= 10? 'A' + (k - 10): '0' + k; // same order as the ASCII table
		}
		miptex->name[10] = '\0';
		miptex->name[11] = '\0';
		miptex->name[12] = '\0';
		miptex->name[13] = '\0';
		miptex->name[14] = '\0';
		miptex->name[15] = '\0';
		unsigned int hash = Hash (miptexsize, miptex);
		miptex->name[10] = table[(hash / 62 / 62) % 52 + 10];
		miptex->name[11] = table[(hash / 62) % 62];
		miptex->name[12] = table[(hash) % 62];
		miptex->name[13] = table[(count / 62) % 62];
		miptex->name[14] = table[(count) % 62];
		miptex->name[15] = '\0';
		NewTextures_PushTexture (miptexsize, miptex);
		count++;
		count_bytes += miptexsize;
		Developer (DEVELOPER_LEVEL_MESSAGE, "Created texture '%s' for face (texture %s) at (%4.3f %4.3f %4.3f)\n", miptex->name, texname, g_face_centroids[i][0], g_face_centroids[i][1], g_face_centroids[i][2]);

		free (miptex);

		CQ_FreeSearchTree (palettetree);

		free (texture);
		for (miplevel = 0; miplevel < MIPLEVELS; miplevel++)
		{
			free (texturemips[miplevel]);
		}
	}
	NewTextures_Write (); // update texdata now

	if (logged)
	{
		Log ("added %d texinfos and textures (%d bytes)\n", count, count_bytes);
	}
}

#endif
#endif

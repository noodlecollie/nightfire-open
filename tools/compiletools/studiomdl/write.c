#pragma warning(disable : 4244)
#pragma warning(disable : 4237)
#pragma warning(disable : 4305)

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "cmdlib.h"
#include "scriplib.h"
#include "mathlib.h"
#include "studio.h"
#include "studiomdl.h"
#include "bonetags.h"

int totalframes = 0;
float totalseconds = 0;
extern int numcommandnodes;

byte* pData;
byte* pStart;
studiohdr_t* phdr;
studioseqhdr_t* pseqhdr;

static nfmdltexturedim_t textureDimensions[MAXSTUDIOSKINS];

#define PLACEHOLDER_TEXTURE_WIDTH 4
#define PLACEHOLDER_TEXTURE_HEIGHT 1

// Create a placeholder so that we don't crash
// asset loaders that expect textures to be present.
// This is the minimum we can get away with - a 4x1 bitmap.
// The width is 4 because being a multiple of 4 is
// required by some hardware, according to code in
// ResizeTexture(), and we want to be backwards-compatible.
// When external textures are loaded by the engine,
// the width and height should be determined by the file
// that is loaded, so the dimensions of the dummy
// texture embedded in the model should be ignored.
static void SetPlaceholderBMP(s_texture_t* texture)
{
	static const size_t PALETTE_SIZE_BYTES = 256 * 3;

	if ( !texture )
	{
		return;
	}

	if ( texture->ppal )
	{
		free(texture->ppal);
	}

	if ( texture->ppicture )
	{
		free(texture->ppicture);
	}

	texture->srcwidth = PLACEHOLDER_TEXTURE_WIDTH;
	texture->srcheight = PLACEHOLDER_TEXTURE_HEIGHT;
	texture->skinwidth = texture->srcwidth;
	texture->skinheight = texture->srcheight;
	texture->skintop = 0;
	texture->skinleft = 0;
	texture->size = PALETTE_SIZE_BYTES + (texture->skinwidth * texture->skinheight);

	// Zero everything - every colour in the palette is black,
	// and every pixel references the first palette colour.
	texture->pdata = (byte*)calloc(texture->size, sizeof(byte));
}

#define ALIGN(a) a = (byte*)((uint64_t)((byte*)a + 3) & ~3)
void WriteBoneInfo()
{
	int i, j;
	mstudiobone_t* pbone;
	mstudiobonecontroller_t* pbonecontroller;
	mstudioattachment_t* pattachment;
	mstudiobbox_t* pbbox;

	// save bone info
	pbone = (mstudiobone_t*)pData;
	phdr->numbones = numbones;
	phdr->boneindex = (pData - pStart);

	for ( i = 0; i < numbones; i++ )
	{
		strcpy(pbone[i].name, bonetable[i].name);
		pbone[i].parent = bonetable[i].parent;
		pbone[i].flags = bonetable[i].flags;
		pbone[i].value[0] = bonetable[i].pos[0];
		pbone[i].value[1] = bonetable[i].pos[1];
		pbone[i].value[2] = bonetable[i].pos[2];
		pbone[i].value[3] = bonetable[i].rot[0];
		pbone[i].value[4] = bonetable[i].rot[1];
		pbone[i].value[5] = bonetable[i].rot[2];
		pbone[i].scale[0] = bonetable[i].posscale[0];
		pbone[i].scale[1] = bonetable[i].posscale[1];
		pbone[i].scale[2] = bonetable[i].posscale[2];
		pbone[i].scale[3] = bonetable[i].rotscale[0];
		pbone[i].scale[4] = bonetable[i].rotscale[1];
		pbone[i].scale[5] = bonetable[i].rotscale[2];
	}
	pData += numbones * sizeof(mstudiobone_t);
	ALIGN(pData);

	// map bonecontroller to bones
	for ( i = 0; i < numbones; i++ )
	{
		for ( j = 0; j < 6; j++ )
		{
			pbone[i].bonecontroller[j] = -1;
		}
	}

	for ( i = 0; i < numbonecontrollers; i++ )
	{
		j = bonecontroller[i].bone;
		switch ( bonecontroller[i].type & STUDIO_TYPES )
		{
			case STUDIO_X:
				pbone[j].bonecontroller[0] = i;
				break;
			case STUDIO_Y:
				pbone[j].bonecontroller[1] = i;
				break;
			case STUDIO_Z:
				pbone[j].bonecontroller[2] = i;
				break;
			case STUDIO_XR:
				pbone[j].bonecontroller[3] = i;
				break;
			case STUDIO_YR:
				pbone[j].bonecontroller[4] = i;
				break;
			case STUDIO_ZR:
				pbone[j].bonecontroller[5] = i;
				break;
			default:
				printf("unknown bonecontroller type\n");
				exit(1);
		}
	}

	// save bonecontroller info
	pbonecontroller = (mstudiobonecontroller_t*)pData;
	phdr->numbonecontrollers = numbonecontrollers;
	phdr->bonecontrollerindex = (pData - pStart);

	for ( i = 0; i < numbonecontrollers; i++ )
	{
		pbonecontroller[i].bone = bonecontroller[i].bone;
		pbonecontroller[i].index = bonecontroller[i].index;
		pbonecontroller[i].type = bonecontroller[i].type;
		pbonecontroller[i].start = bonecontroller[i].start;
		pbonecontroller[i].end = bonecontroller[i].end;
	}
	pData += numbonecontrollers * sizeof(mstudiobonecontroller_t);
	ALIGN(pData);

	// save attachment info
	pattachment = (mstudioattachment_t*)pData;
	phdr->numattachments = numattachments;
	phdr->attachmentindex = (pData - pStart);

	for ( i = 0; i < numattachments; i++ )
	{
		safe_strncpy(pattachment[i].name, attachment[i].name, sizeof(pattachment[i].name));
		pattachment[i].bone = attachment[i].bone;
		VectorCopy(attachment[i].org, pattachment[i].org);
	}
	pData += numattachments * sizeof(mstudioattachment_t);
	ALIGN(pData);

	// save bbox info
	pbbox = (mstudiobbox_t*)pData;
	phdr->numhitboxes = numhitboxes;
	phdr->hitboxindex = (pData - pStart);

	for ( i = 0; i < numhitboxes; i++ )
	{
		pbbox[i].bone = hitbox[i].bone;
		pbbox[i].group = hitbox[i].group;
		VectorCopy(hitbox[i].bmin, pbbox[i].bbmin);
		VectorCopy(hitbox[i].bmax, pbbox[i].bbmax);
	}
	pData += numhitboxes * sizeof(mstudiobbox_t);
	ALIGN(pData);
}

void WriteSequenceInfo()
{
	int i, j;

	mstudioseqgroup_t* pseqgroup;
	mstudioseqdesc_t* pseqdesc;
	mstudioseqdesc_t* pbaseseqdesc;
	mstudioevent_t* pevent;
	mstudiopivot_t* ppivot;
	byte* ptransition;

	// save sequence info
	pseqdesc = (mstudioseqdesc_t*)pData;
	pbaseseqdesc = pseqdesc;
	phdr->numseq = numseq;
	phdr->seqindex = (pData - pStart);
	pData += numseq * sizeof(mstudioseqdesc_t);

	for ( i = 0; i < numseq; i++, pseqdesc++ )
	{
		strcpy(pseqdesc->label, sequence[i].name);
		pseqdesc->numframes = sequence[i].numframes;
		pseqdesc->fps = sequence[i].fps;
		pseqdesc->flags = sequence[i].flags;

		pseqdesc->numblends = sequence[i].numblends;

		pseqdesc->blendtype[0] = sequence[i].blendtype[0];
		pseqdesc->blendtype[1] = sequence[i].blendtype[1];
		pseqdesc->blendstart[0] = sequence[i].blendstart[0];
		pseqdesc->blendend[0] = sequence[i].blendend[0];
		pseqdesc->blendstart[1] = sequence[i].blendstart[1];
		pseqdesc->blendend[1] = sequence[i].blendend[1];

		pseqdesc->motiontype = sequence[i].motiontype;
		pseqdesc->motionbone = 0;  // sequence[i].motionbone;
		VectorCopy(sequence[i].linearmovement, pseqdesc->linearmovement);

		pseqdesc->seqgroup = sequence[i].seqgroup;

		pseqdesc->animindex = sequence[i].animindex;

		pseqdesc->activity = sequence[i].activity;
		pseqdesc->actweight = sequence[i].actweight;

		VectorCopy(sequence[i].bmin, pseqdesc->bbmin);
		VectorCopy(sequence[i].bmax, pseqdesc->bbmax);

		pseqdesc->entrynode = sequence[i].entrynode;
		pseqdesc->exitnode = sequence[i].exitnode;
		pseqdesc->nodeflags = sequence[i].nodeflags;

		totalframes += sequence[i].numframes;
		totalseconds += sequence[i].numframes / sequence[i].fps;

		// save events
		pevent = (mstudioevent_t*)pData;
		pseqdesc->numevents = sequence[i].numevents;
		pseqdesc->eventindex = (pData - pStart);
		pData += pseqdesc->numevents * sizeof(mstudioevent_t);
		for ( j = 0; j < sequence[i].numevents; j++ )
		{
			pevent[j].frame = sequence[i].event[j].frame - sequence[i].frameoffset;
			pevent[j].event = sequence[i].event[j].event;
			memcpy(pevent[j].options, sequence[i].event[j].options, sizeof(pevent[j].options));
		}
		ALIGN(pData);

		// save pivots
		ppivot = (mstudiopivot_t*)pData;
		pseqdesc->numpivots = sequence[i].numpivots;
		pseqdesc->pivotindex = (pData - pStart);
		pData += pseqdesc->numpivots * sizeof(mstudiopivot_t);
		for ( j = 0; j < sequence[i].numpivots; j++ )
		{
			VectorCopy(sequence[i].pivot[j].org, ppivot[j].org);
			ppivot[j].start = sequence[i].pivot[j].start - sequence[i].frameoffset;
			ppivot[j].end = sequence[i].pivot[j].end - sequence[i].frameoffset;
		}
		ALIGN(pData);
	}

	// save sequence group info
	pseqgroup = (mstudioseqgroup_t*)pData;
	phdr->numseqgroups = numseqgroups;
	phdr->seqgroupindex = (pData - pStart);
	pData += phdr->numseqgroups * sizeof(mstudioseqgroup_t);
	ALIGN(pData);

	for ( i = 0; i < numseqgroups; i++ )
	{
		strcpy(pseqgroup[i].label, sequencegroup[i].label);
		strcpy(pseqgroup[i].name, sequencegroup[i].name);
	}

	// save transition graph
	ptransition = (byte*)pData;
	phdr->numtransitions = numxnodes;
	phdr->transitionindex = (pData - pStart);
	pData += numxnodes * numxnodes * sizeof(byte);
	ALIGN(pData);
	for ( i = 0; i < numxnodes; i++ )
	{
		for ( j = 0; j < numxnodes; j++ )
		{
			*ptransition++ = xnode[i][j];
		}
	}
}

byte* WriteAnimations(byte* pData, byte* pStart, int group)
{
	int i, j, k;
	int q, n;

	mstudioanim_t* panim;
	mstudioanimvalue_t* panimvalue;

	// hack for seqgroup 0
	// pseqgroup->data = (pData - pStart);

	for ( i = 0; i < numseq; i++ )
	{
		if ( sequence[i].seqgroup == group )
		{
			// save animations
			panim = (mstudioanim_t*)pData;
			sequence[i].animindex = (pData - pStart);
			pData += sequence[i].numblends * numbones * sizeof(mstudioanim_t);
			ALIGN(pData);

			panimvalue = (mstudioanimvalue_t*)pData;
			for ( q = 0; q < sequence[i].numblends; q++ )
			{
				// save animation value info
				for ( j = 0; j < numbones; j++ )
				{
					for ( k = 0; k < 6; k++ )
					{
						if ( sequence[i].panim[q]->numanim[j][k] == 0 )
						{
							panim->offset[k] = 0;
						}
						else
						{
							panim->offset[k] = ((byte*)panimvalue - (byte*)panim);
							for ( n = 0; n < sequence[i].panim[q]->numanim[j][k]; n++ )
							{
								panimvalue->value = sequence[i].panim[q]->anim[j][k][n].value;
								panimvalue++;
							}
						}
					}
					if ( ((byte*)panimvalue - (byte*)panim) > 65535 )
						Error("sequence \"%s\" is greate than 64K\n", sequence[i].name);
					panim++;
				}
			}

			// printf("raw bone data %d : %s\n", (byte *)panimvalue - pData, sequence[i].name);
			pData = (byte*)panimvalue;
			ALIGN(pData);
		}
	}
	return pData;
}

void WriteTextures()
{
	int i, j;
	mstudiotexture_t* ptexture;
	short* pref;

	// save bone info
	ptexture = (mstudiotexture_t*)pData;
	phdr->numtextures = numtextures;
	phdr->textureindex = (pData - pStart);
	pData += numtextures * sizeof(mstudiotexture_t);
	ALIGN(pData);

	phdr->skinindex = (pData - pStart);
	phdr->numskinref = numskinref;
	phdr->numskinfamilies = numskinfamilies;
	pref = (short*)pData;

	for ( i = 0; i < phdr->numskinfamilies; i++ )
	{
		for ( j = 0; j < phdr->numskinref; j++ )
		{
			*pref = skinref[i][j];
			pref++;
		}
	}

	pData = (byte*)pref;
	ALIGN(pData);

	phdr->texturedataindex = (pData - pStart);  // must be the end of the file!

	for ( i = 0; i < numtextures; i++ )
	{
		if ( noEmbeddedTextures )
		{
			// Reset the texture to be a placeholder,
			// so as not to crash asset readers that
			// expect valid texture data.
			SetPlaceholderBMP(&texture[i]);
		}

		strcpy(ptexture[i].name, texture[i].name);
		ptexture[i].flags = texture[i].flags;
		ptexture[i].width = texture[i].skinwidth;
		ptexture[i].height = texture[i].skinheight;

		ptexture[i].index = (pData - pStart);
		memcpy(pData, texture[i].pdata, texture[i].size);
		pData += texture[i].size;
	}
	ALIGN(pData);
}

void WriteModel()
{
	int i, j, k;

	mstudiobodyparts_t* pbodypart;
	mstudiomodel_t* pmodel;
	// vec3_t			*bbox;
	byte* pbone;
	vec3_t* pvert;
	vec3_t* pnorm;
	mstudiomesh_t* pmesh;
	s_trianglevert_t* psrctri;
	byte* cur;
	int total_tris = 0;
	int total_strips = 0;

	pbodypart = (mstudiobodyparts_t*)pData;
	phdr->numbodyparts = numbodyparts;
	phdr->bodypartindex = (pData - pStart);
	pData += numbodyparts * sizeof(mstudiobodyparts_t);

	pmodel = (mstudiomodel_t*)pData;
	pData += nummodels * sizeof(mstudiomodel_t);

	for ( i = 0, j = 0; i < numbodyparts; i++ )
	{
		strcpy(pbodypart[i].name, bodypart[i].name);
		pbodypart[i].nummodels = bodypart[i].nummodels;
		pbodypart[i].base = bodypart[i].base;
		pbodypart[i].modelindex = ((byte*)&pmodel[j]) - pStart;
		j += bodypart[i].nummodels;
	}
	ALIGN(pData);

	cur = pData;
	for ( i = 0; i < nummodels; i++ )
	{
		int normmap[MAXSTUDIOVERTS];
		int normimap[MAXSTUDIOVERTS];
		int n = 0;

		strcpy(pmodel[i].name, model[i]->name);

		// save bbox info

		// remap normals to be sorted by skin reference
		for ( j = 0; j < model[i]->nummesh; j++ )
		{
			for ( k = 0; k < model[i]->numnorms; k++ )
			{
				if ( model[i]->normal[k].skinref == model[i]->pmesh[j]->skinref )
				{
					normmap[k] = n;
					normimap[n] = k;
					n++;
					model[i]->pmesh[j]->numnorms++;
				}
			}
		}

		// save vertice bones
		pbone = pData;
		pmodel[i].numverts = model[i]->numverts;
		pmodel[i].vertinfoindex = (pData - pStart);
		for ( j = 0; j < pmodel[i].numverts; j++ )
		{
			*pbone++ = model[i]->vert[j].bone;
		}
		ALIGN(pbone);

		// save normal bones
		pmodel[i].numnorms = model[i]->numnorms;
		pmodel[i].norminfoindex = ((byte*)pbone - pStart);
		for ( j = 0; j < pmodel[i].numnorms; j++ )
		{
			*pbone++ = model[i]->normal[normimap[j]].bone;
		}
		ALIGN(pbone);

		pData = pbone;

		// save group info
		pvert = (vec3_t*)pData;
		pData += model[i]->numverts * sizeof(vec3_t);
		pmodel[i].vertindex = ((byte*)pvert - pStart);
		ALIGN(pData);

		pnorm = (vec3_t*)pData;
		pData += model[i]->numnorms * sizeof(vec3_t);
		pmodel[i].normindex = ((byte*)pnorm - pStart);
		ALIGN(pData);

		for ( j = 0; j < model[i]->numverts; j++ )
		{
			VectorCopy(model[i]->vert[j].org, pvert[j]);
		}

		for ( j = 0; j < model[i]->numnorms; j++ )
		{
			VectorCopy(model[i]->normal[normimap[j]].org, pnorm[j]);
		}
		printf("vertices  %6ld bytes (%d vertices, %d normals)\n", pData - cur, model[i]->numverts, model[i]->numnorms);
		cur = pData;

		// save mesh info
		pmesh = (mstudiomesh_t*)pData;
		pmodel[i].nummesh = model[i]->nummesh;
		pmodel[i].meshindex = (pData - pStart);
		pData += pmodel[i].nummesh * sizeof(mstudiomesh_t);
		ALIGN(pData);

		total_tris = 0;
		total_strips = 0;
		for ( j = 0; j < model[i]->nummesh; j++ )
		{
			int numCmdBytes;
			byte* pCmdSrc;

			pmesh[j].numtris = model[i]->pmesh[j]->numtris;
			pmesh[j].skinref = model[i]->pmesh[j]->skinref;
			pmesh[j].numnorms = model[i]->pmesh[j]->numnorms;

			psrctri = (s_trianglevert_t*)(model[i]->pmesh[j]->triangle);
			for ( k = 0; k < pmesh[j].numtris * 3; k++ )
			{
				psrctri->normindex = normmap[psrctri->normindex];
				psrctri++;
			}

			numCmdBytes = BuildTris(model[i]->pmesh[j]->triangle, model[i]->pmesh[j], &pCmdSrc);

			pmesh[j].triindex = (pData - pStart);
			memcpy(pData, pCmdSrc, numCmdBytes);
			pData += numCmdBytes;
			ALIGN(pData);
			total_tris += pmesh[j].numtris;
			total_strips += numcommandnodes;
		}
		printf("mesh      %6ld bytes (%d tris, %d strips)\n", pData - cur, total_tris, total_strips);
		cur = pData;
	}
}

static const int32_t* ComputeBoneIndicesForGait(int32_t* numBones)
{
	static int32_t boneIndices[MAXSTUDIOSRCBONES];

	int32_t count = 0;
	memset(boneIndices, 0, sizeof(boneIndices));

	for ( int32_t boneIndex = 0; boneIndex < MAXSTUDIOSRCBONES; ++boneIndex )
	{
		const s_bonetable_t* srcBone = &bonetable[boneIndex];

		if ( srcBone->name[0] == '\0' || !HasBoneTag(srcBone->name, BONE_TAG_APPLY_GAIT) )
		{
			continue;
		}

		boneIndices[count++] = boneIndex;
	}

	if ( numBones )
	{
		*numBones = count;
	}

	return boneIndices;
}

static void WriteGaitBones(nfmdlheader_t* nfHeader)
{
	nfmdlheader_v1_t* v1Header = (nfmdlheader_v1_t*)((byte*)nfHeader + NFMDLHEADER_LOCAL_OFFSET_V1);

	v1Header->gaitBonesIndex = (int32_t)(pData - pStart);

	int32_t numIndices = 0;
	const int32_t* boneIndices = ComputeBoneIndicesForGait(&numIndices);
	v1Header->gaitBonesCount = numIndices;

	for ( int32_t index = 0; index < numIndices; ++index )
	{
		*((int32_t*)pData) = boneIndices[index];
		pData += sizeof(int32_t);
	}
}

static void WriteTextureDimensions(nfmdlheader_t* nfHeader)
{
	nfmdlheader_v1_t* v1Header = (nfmdlheader_v1_t*)((byte*)nfHeader + NFMDLHEADER_LOCAL_OFFSET_V1);

	v1Header->textureDimsIndex = (int32_t)(pData - pStart);

	for ( int textureIndex = 0; textureIndex < numtextures; ++textureIndex )
	{
		nfmdltexturedim_t* texDim = (nfmdltexturedim_t*)pData;

		texDim->width = texture[textureIndex].srcwidth;
		texDim->height = texture[textureIndex].srcheight;

		pData += sizeof(nfmdltexturedim_t);
	}
}

static void WriteTextureMaskParams(nfmdlheader_t* nfHeader)
{
	nfmdlheader_v2_t* v2Header = (nfmdlheader_v2_t*)((byte*)nfHeader + NFMDLHEADER_LOCAL_OFFSET_V2);

	v2Header->textureMaskParamsIndex = (int32_t)(pData - pStart);

	for ( int textureIndex = 0; textureIndex < numtextures; ++textureIndex )
	{
		nfmdltexturemaskparam_t* maskParam = (nfmdltexturemaskparam_t*)pData;
		const s_texture_t* tex = &texture[textureIndex];

		float threshold = (tex->flags & STUDIO_NF_MASKED) ? tex->maskThreshold : 0.5f;

		if ( threshold < 0.0f )
		{
			threshold = 0.0f;
		}
		else if ( threshold > 1.0f )
		{
			threshold = 1.0f;
		}

		maskParam->threshold = (uint8_t)(threshold * 255.0f);

		pData += sizeof(nfmdltexturemaskparam_t);
	}

	ALIGN(pData);
}

static void WriteMDLTags(nfmdlheader_t* nfHeader)
{
	nfmdlheader_v2_t* v2Header = (nfmdlheader_v2_t*)((byte*)nfHeader + NFMDLHEADER_LOCAL_OFFSET_V2);

	v2Header->mdlTagsIndex = (int32_t)(pData - pStart);
	v2Header->mdlTagsCount = Tag_Count(mdlTags);

	const tagitem_t* src = mdlTags;

	for ( int index = 0; index < v2Header->mdlTagsCount; ++index )
	{
		nfmdltag_t* dest = (nfmdltag_t*)pData;
		strncpy(dest->name, Tag_GetName(src), sizeof(dest->name));
		dest->name[sizeof(dest->name) - 1] = '\0';

		pData += sizeof(nfmdltag_t);
		src = Tag_ConstGetNext(src);
	}
}

#define FILEBUFFER (16 * 1024 * 1024)

void WriteFile(void)
{
	FILE* modelouthandle;
	int total = 0;
	int i;
	const qboolean shouldSplitTextures = split_textures && !noEmbeddedTextures;

	pStart = (byte*)kalloc(1, FILEBUFFER);

	StripExtension(outname);

	for ( i = 1; i < numseqgroups; i++ )
	{
		// write the non-default sequence group data to separate files
		char groupname[128], localname[128];

		safe_snprintf(groupname, sizeof(groupname), "%s%02d.mdl", outname, i);

		printf("writing %s:\n", groupname);
		modelouthandle = SafeOpenWrite(groupname);

		pseqhdr = (studioseqhdr_t*)pStart;
		pseqhdr->id = IDSTUDIOSEQHEADER;
		pseqhdr->version = STUDIO_VERSION;

		pData = pStart + sizeof(studioseqhdr_t);

		pData = WriteAnimations(pData, pStart, i);

		ExtractFileBase(groupname, localname);
		sprintf(sequencegroup[i].name, "models\\%s.mdl", localname);
		strcpy(pseqhdr->name, sequencegroup[i].name);
		pseqhdr->length = pData - pStart;

		printf("total     %6d\n", pseqhdr->length);

		SafeWrite(modelouthandle, pStart, pseqhdr->length);

		fclose(modelouthandle);
		memset(pStart, 0, pseqhdr->length);
	}

	if ( shouldSplitTextures )
	{
		// write textures out to a separate file
		char texname[128];

		safe_snprintf(texname, sizeof(texname), "%sT.mdl", outname);

		printf("writing %s:\n", texname);
		modelouthandle = SafeOpenWrite(texname);

		phdr = (studiohdr_t*)pStart;
		phdr->ident = IDSTUDIOHEADER;
		phdr->version = STUDIO_VERSION;

		pData = (byte*)phdr + sizeof(studiohdr_t);

		WriteTextures();

		phdr->length = pData - pStart;
		printf("textures  %6d bytes\n", phdr->length);

		SafeWrite(modelouthandle, pStart, phdr->length);

		fclose(modelouthandle);
		memset(pStart, 0, phdr->length);
		pData = pStart;
	}

	//
	// write the model output file
	//
	strcat(outname, ".mdl");

	printf("---------------------\n");
	printf("writing %s:\n", outname);
	modelouthandle = SafeOpenWrite(outname);

	phdr = (studiohdr_t*)pStart;

	phdr->ident = IDSTUDIOHEADER;
	phdr->version = STUDIO_VERSION;
	strcpy(phdr->name, outname);
	VectorCopy(eyeposition, phdr->eyeposition);
	VectorCopy(bbox[0], phdr->min);
	VectorCopy(bbox[1], phdr->max);
	VectorCopy(cbox[0], phdr->bbmin);
	VectorCopy(cbox[1], phdr->bbmax);

	phdr->flags = gflags;

	pData = (byte*)phdr + sizeof(studiohdr_t);

	nfmdlheader_t* nfHeader = (nfmdlheader_t*)pData;
	nfHeader->id = IDNFMDLHEADER;
	nfHeader->version = NFMDLHEADER_VERSION_LATEST;

	// Data is zeroed, so we can just skip on.
	pData += NFMDLHEADER_LOCAL_OFFSET_LATEST + NFMDLHEADER_SIZE_LATEST;

	WriteBoneInfo();
	printf("bones     %6ld bytes (%d)\n", pData - pStart - total, numbones);
	total = pData - pStart;

	pData = WriteAnimations(pData, pStart, 0);

	WriteSequenceInfo();
	printf(
		"sequences %6ld bytes (%d frames) [%d:%02d]\n",
		pData - pStart - total,
		totalframes,
		(int)totalseconds / 60,
		(int)totalseconds % 60);
	total = pData - pStart;

	WriteModel();
	printf("models    %6ld bytes\n", pData - pStart - total);
	total = pData - pStart;

	// Any custom NF stuff MUST go before the textures, since
	// the game may truncate the model data once imported so
	// as not to waste memory on embedded textures.
	WriteGaitBones(nfHeader);
	printf("gaitbones %6ld bytes\n", pData - pStart - total);
	total = pData - pStart;

	WriteMDLTags(nfHeader);
	printf("tags      %6ld bytes\n", pData - pStart - total);
	total = pData - pStart;

	if ( !shouldSplitTextures )
	{
		WriteTextureDimensions(nfHeader);
		printf("texdims   %6ld bytes\n", pData - pStart - total);
		total = pData - pStart;

		WriteTextureMaskParams(nfHeader);
		printf("texmask   %6ld bytes\n", pData - pStart - total);
		total = pData - pStart;

		WriteTextures();
		printf("textures  %6ld bytes\n", pData - pStart - total);
	}

	phdr->length = pData - pStart;

	printf("total     %6d\n", phdr->length);

	SafeWrite(modelouthandle, pStart, phdr->length);

	fclose(modelouthandle);
}

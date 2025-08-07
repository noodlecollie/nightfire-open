#include <stdexcept>
#include "SMDv10/SMDReferencePopulator.h"
#include "MDLv14/ComponentReflection.h"

namespace SMDv10
{
	SMDReferencePopulator::SMDReferencePopulator(
		const std::shared_ptr<MDLv14::MDLFile>& mdlFile,
		const std::string& modelName) :
		m_MDLFile(mdlFile),
		m_ModelName(modelName)
	{
		if ( !m_MDLFile )
		{
			throw std::invalid_argument("Expected a valid MDL file");
		}
	}

	std::shared_ptr<SMDFile> SMDReferencePopulator::Populate()
	{
		const Container<MDLv14::TriangleMap>& triangleMap = m_MDLFile->GetTriangleMap();
		const Container<MDLv14::Vertex>& vertices = m_MDLFile->GetVertices();
		const Container<MDLv14::Normal>& normals = m_MDLFile->GetNormals();
		const Container<MDLv14::TextureCoOrdinate> texCoOrds = m_MDLFile->GetTextureCoOrdinates();
		const Container<MDLv14::Bone>& bones = m_MDLFile->GetBones();
		const Container<MDLv14::Texture>& textures = m_MDLFile->GetTextures();

		std::shared_ptr<SMDv10::SMDFile> smdFile = std::make_shared<SMDv10::SMDFile>(SMDv10::SMDFile::Type::Reference);

		const MDLv14::Model* submodel = m_MDLFile->FindModelByName(m_ModelName);

		if ( !submodel )
		{
			// Should never happen
			throw std::runtime_error("Lookup failed for submodel \"" + m_ModelName + "\"");
		}

		for ( const MDLv14::ModelInfo& modelInfo : submodel->modelInfos )
		{
			for ( const MDLv14::Mesh& mesh : modelInfo.meshList )
			{
				for ( size_t base = 0; base < mesh.triangleCount; base += 3 )
				{
					// NOTE: If we want to retain multi-blends in the decompiled models (because
					// the Xash model compiler should support that), this is the place to do it.
					// In the original decompiler code, only blend 0 was used and assigned to
					// the vertices generated here, and this behaviour has been retained for now.

					const auto createVertex = [&](size_t triangleMapIndex) -> SMDv10::Vertex
					{
						SMDv10::Vertex vertex;

						uint16_t vIndex = triangleMap.GetElementChecked(triangleMapIndex).vertexIndex;
						vertex.position = vertices.GetElementChecked(vIndex).position;
						vertex.normal = normals.GetElementChecked(vIndex).position;
						vertex.bone = m_MDLFile->GetBoneIndicesUsedByMeshVertex(mesh, vIndex)[0];
						vertex.texU = texCoOrds.GetElementChecked(vIndex).u;
						vertex.texV = texCoOrds.GetElementChecked(vIndex).v * -1.0f;

						return vertex;
					};

					SMDv10::Vertex v0 = createVertex(mesh.triangleIndex + base + 1);
					SMDv10::Vertex v1 = createVertex(mesh.triangleIndex + base + 0);
					SMDv10::Vertex v2 = createVertex(mesh.triangleIndex + base + 2);

					smdFile->AddTriangle(SMDv10::Triangle(
						textures.GetElementChecked(modelInfo.skinReference).textureName,
						std::move(v0),
						std::move(v1),
						std::move(v2)));
				}
			}
		}

		Container<SMDv10::NodeFrame> boneFrameValues;

		for ( size_t boneIndex = 0; boneIndex < bones.size(); ++boneIndex )
		{
			const MDLv14::Bone& bone = bones.GetElementChecked(boneIndex);

			smdFile->AddNode(SMDv10::Node(static_cast<int8_t>(boneIndex), bone.name, static_cast<int8_t>(bone.parent)));
			boneFrameValues.emplace_back(
				SMDv10::NodeFrame(static_cast<int8_t>(boneIndex), bone.position, bone.rotation));
		}

		smdFile->AddFrame(SMDv10::Frame(0, std::move(boneFrameValues)));

		return smdFile;
	}
}  // namespace SMDv10

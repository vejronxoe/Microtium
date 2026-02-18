#include"structures.h"

#include"math/matrix.h"

void DrawCraftStations(std::vector<CraftStation>& structures
	, Shader& sh
	, float* scale
	, float* transform
	, unsigned int* structureDDs
	, unsigned int* structureTexs)
{
	sh.Bind();
	for(int i = 0; i < structures.size(); i++)
	{
		ChangeScale(structures.at(i).m_LookAt, 1, scale);
		ChangeTransform(structures.at(i).m_Transform[0], structures.at(i).m_Transform[1], transform);
		sh.SetUniformMat4(structureTransform, transform);
		sh.SetUniformMat4(structureScale, scale);
		ErrorGL(glBindVertexArray(structureDDs[structures.at(i).m_CraftStationtype]));
		ErrorGL(glBindTexture(GL_TEXTURE_2D, structureTexs[structures.at(i).m_CraftStationtype]));
		ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
	}
}
#include "notefield.h"

#include "../util/shaders.h"

const float wvertices[] = {
	-1.0, 1.0f,	0.0f, 1.0f,
	1.0f, -1.0f,	1.0f, 0.0f,
	-1.0f, -1.0f,	0.0f, 0.0f,

	-1.0, 1.0f,		0.0f, 1.0f,
	1.0, 1.0f,		1.0f, 1.0f,
	1.0f, -1.0f,	1.0f, 0.0f
};

shader wmShader;
namespace washing
{
	template <class T>
	void note<T>::draw(GLuint FBO)
	{
		static_cast<T*>(this)->draw(FBO);
	}

	template <class T>
	double note<T>::getTime()
	{
		return time;
	}

	template <class T>
	noteSlot note<T>::getSlot()
	{
		return slot;
	}

	void tap::draw(GLuint FBO)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);


		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	notefield::notefield(glm::vec2 widthHeight)
	{
		if (wmShader.ID == 0) wmShader = shader("src/shaders/washingMachine.vs", "src/shaders/washingMachine.fs");

		glGenBuffers(1, &VBO);
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(wvertices), wvertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
		glEnableVertexAttribArray(1);


		glGenFramebuffers(1, &FBO);
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);

		glGenTextures(1, &FBOtexture);
		glBindTexture(GL_TEXTURE_2D, FBOtexture);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthHeight.x, widthHeight.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, FBOtexture, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void notefield::draw()
	{
		wmShader.use();
		glActiveTexture(FBOtexture);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	GLuint notefield::getFBO()
	{
		return FBO;
	}
}
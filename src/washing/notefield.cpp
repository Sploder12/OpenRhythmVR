#include "notefield.h"

#include <glm/gtc/matrix_transform.hpp>

#include "../util/shaders.h"

#include "washglobals.h"

const float wvertices[] = {
	-1.0, 1.0f,	0.0f, 1.0f,
	1.0f, -1.0f,	1.0f, 0.0f,
	-1.0f, -1.0f,	0.0f, 0.0f,

	-1.0, 1.0f,		0.0f, 1.0f,
	1.0, 1.0f,		1.0f, 1.0f,
	1.0f, -1.0f,	1.0f, 0.0f
};

const float nvertices[] = {
	-1.0, 1.0f,
	1.0f, -1.0f,
	-1.0f, -1.0f,

	-1.0, 1.0f,
	1.0, 1.0f,
	1.0f, -1.0f
};

#define SLOTS 60
#define INV_SLOTS (1.0 / (SLOTS - 1))

#define NOTE_HEIGHT 0.1f

namespace washing
{
	template <class T>
	void note<T>::draw()
	{
		static_cast<T*>(this)->draw();
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

	constexpr float slotToXPos(const char slot)
	{
		//0 - 60 -> 0 - 1 -> 0 - 2 -> -1 - 1
		return float((slot * INV_SLOTS) * 2 - 1);
	}

	constexpr float timeToYPos(const double time, const double noteTime, const float scrollSpeed)
	{
		// align with bottom of screen then scale by scroll speed
		const double dt = noteTime - time;
		return float((dt - 1.0 + NOTE_HEIGHT) * scrollSpeed);
	}

	//temporaries for shader
	GLuint VAO = 0, VBO;
	shader tshader;
	void tap::draw()
	{
		double yPos = timeToYPos(wglobals::gameTime, time, wglobals::scrollSpeed);
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(slotToXPos(slot.first), yPos, 0.0f));

		//@TODO the actual shader and setting the uniforms for the shader

		//temporary shader
		if (VAO == 0)
		{
			tshader = shader("src/shaders/box.vs", "src/shaders/box.fs");

			glGenBuffers(1, &VBO);
			glGenVertexArrays(1, &VAO);
			glBindVertexArray(VAO);

			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(nvertices), nvertices, GL_STATIC_DRAW);

			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
		}

		tshader.use();
		tshader.setVec4("color", glm::vec4(0.5f, 0.0f, 1.0f, 1.0f));
		glBindVertexArray(VAO);
		if (slot.first > slot.second)
		{
			model = glm::scale(model, glm::vec3(slotToXPos(60) - slotToXPos(slot.first - 60), NOTE_HEIGHT * yPos, 1.0f));
			tshader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(-1.0, yPos, 0.0f));
			model = glm::scale(model, glm::vec3(slotToXPos(slot.second), NOTE_HEIGHT * yPos, 1.0f));
		}
		else
		{
			model = glm::scale(model, glm::vec3(slotToXPos(slot.second) - slotToXPos(slot.first), NOTE_HEIGHT * (yPos - 1.0) * 0.5, 1.0f));
		}
		tshader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	shader wmShader;
	GLuint wVBO, wVAO, wFBO, wFBOtexture;
	notefield::notefield(glm::vec2 widthHeight)
	{
		if (wmShader.ID == 0)
		{
			wmShader = shader("src/shaders/washingMachine.vs", "src/shaders/washingMachine.fs");

			glGenBuffers(1, &wVBO);
			glGenVertexArrays(1, &wVAO);
			glBindVertexArray(wVAO);

			glBindBuffer(GL_ARRAY_BUFFER, wVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(wvertices), wvertices, GL_STATIC_DRAW);

			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);

			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
			glEnableVertexAttribArray(1);


			glGenFramebuffers(1, &wFBO);
			glBindFramebuffer(GL_FRAMEBUFFER, wFBO);

			glGenTextures(1, &wFBOtexture);
			glBindTexture(GL_TEXTURE_2D, wFBOtexture);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, int(widthHeight.x), int(widthHeight.y), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, wFBOtexture, 0);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
	}

	void notefield::update(GLuint prevFBO)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, wFBO);
		glClearColor(0.0, 0.0, 0.4, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);
		for (note<tap>& note : taps)
		{
			note.draw();
		}
		glBindFramebuffer(GL_FRAMEBUFFER, prevFBO);
	}

	shader tmp;

	void notefield::draw()
	{
		/*
		if (tmp.ID == 0)
		{
			tmp = shader("src/shaders/texture2D.vs", "src/shaders/texture2D.fs");
			
		}
		tmp.use();
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.5, 0.5, 1.0));
		tmp.setMat4("model", model);
		*/
		wmShader.use();
		glBindTexture(GL_TEXTURE_2D, wFBOtexture);
		glBindVertexArray(wVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	GLuint notefield::getFBO()
	{
		return wFBO;
	}
}
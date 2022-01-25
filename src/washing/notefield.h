#ifndef WASH_NOTEFIELD_H
#define WASH_NOTEFIELD_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <vector>

namespace washing
{
	//notes are defined by a start and end slot
	//using a start or end beyond 0 and 59 will wrap around
	typedef std::pair<char, char> noteSlot;

	template <class T>
	class note
	{
	private:
		double time;
		noteSlot slot;
	public:
		
		void draw(GLuint FBO);

		double getTime();
		noteSlot getSlot();
	};

	class tap : public note<tap>
	{
		void draw(GLuint FBO);
	};

	class notefield
	{
	private:
		GLuint VAO, VBO, FBO, FBOtexture;
		std::vector<note<tap>> taps;
	public:
		//default is 64 pixels for all 60 note slots, height is just divided by 3
		notefield(glm::vec2 widthHeight = glm::vec2(3840.0f, 1280.0f));

		void draw();

		GLuint getFBO();
	};
}

#endif
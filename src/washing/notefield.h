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
	typedef std::pair<unsigned char, unsigned char> noteSlot;

	template <class T>
	class note
	{
	protected:
		double time;
		noteSlot slot;
	public:
		note(double time, noteSlot slot) :
			time(time), slot(slot) {}
		
		void draw();

		double getTime();
		noteSlot getSlot();
	};

	class tap : public note<tap>
	{
	public:
		tap(double time, noteSlot slot) :
			note<tap>(time, slot) {}

		void draw();
	};

	struct notefield
	{
		std::vector<note<tap>> taps;
		unsigned int curTap = 0;

		//default is 64 pixels for all 60 note slots, height is just divided by 3
		notefield(glm::vec2 widthHeight = glm::vec2(3840.0f, 1280.0f));

		void update(GLuint prevFBO = 0);

		void draw();

		GLuint getFBO();
	};
}

#endif
#ifndef WASH_SONG_H
#define WASH_SONG_H

#include "notefield.h"

#include <string>

namespace washing
{
	enum class difficulties : unsigned char
	{
		beginner,
		easy,
		normal,
		hard,
		expert,
		challenge,

		difCount
	};

	struct song
	{
		std::string title;
		std::string artist;
		double offset;
		//@TODO the song file itself

		//default as nullptr
		notefield* charts[(unsigned char)(difficulties::difCount)];

		song() :
			title("E"), artist("E"), offset(0.0)
		{
			for (unsigned char i = 0; i < (unsigned char)(difficulties::difCount); i++)
			{
				charts[i] = nullptr;
			}
		}

		~song();
	};

	//this uses the "wsm" format
	//also make sure you delete (aka. unload) the song when you're done
	song* loadSong(const char* filename);
}

/*"wsm" format specifications

Begins with several meta data tags similar to the sm format
However there are far less meta data tags used.
The supported tags include:
#TITLE
#ARTIST
#STEPARTIST
#MUSIC
#OFFSET

an example of tag looks like this "#MUSIC:coolSong.mp3"
spaces are not ignored, be warned.
The tags must be all uppercase
note the lack of ; unlike sm format

bad meta data can result in partial or no loading of a file

Song difficulties are started with their respective tag (eg. "#EASY:")
they are then closed with ; once all the note data has been put in
similarities to the sm format end there as the sm format would be incredibly bloated.

note data itself is stored in a "relative to" format.
each note has data containing type, receptor location, and beats from previous note (as decimal)
additionally a #BPM: tag exists that also has relative to positioning and acts as a note
Everything is in 4/4 and 8th notes can be represented as 0.5, 16ths as 0.25 and so on.
Each note is line seperated.

The types of notes are as follows
0 - tap
1 - hold
2 - slide

holds have end locations and slides have direction (UDLR)

Here is a small example of just taps
0 3-6 0.0
0 8-9 0.0
0 3-6 0.5
0 8-9 0.0
0 0-59 1.5
This will produce a immediate jump, then another jump an 8th later then a big tap on the 2nd beat

*/
#endif
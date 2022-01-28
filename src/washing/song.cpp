#include "song.h"

#include <fstream>

namespace washing
{

	song::~song()
	{
		//note we don't use delete[] here since charts is a static array of pointers
		for (unsigned char i = 0; i < (unsigned char)(difficulties::difCount); i++)
		{
			delete charts[i];
		}
	}

	std::vector<std::string> splitStr(std::string& str, char delim = ' ')
	{
		std::vector<std::string> opt;
		size_t curPos = 0;
		size_t nPos = str.find_first_of(delim);
		while (nPos != std::string::npos)
		{
			opt.push_back(str.substr(curPos, nPos - curPos));
			curPos = nPos + 1;
			nPos = str.find_first_of(delim, curPos);
		}

		opt.push_back(str.substr(curPos));
		return opt;
	}

	noteSlot toNoteslot(std::string& str)
	{
		noteSlot opt;
		auto dat = splitStr(str, '-');
		opt.first = std::stoi(dat[0]);
		opt.second = std::stoi(dat[1]);
		return opt;
	}

	std::pair<std::string, std::string> parseTag(std::string& str)
	{
		std::pair<std::string, std::string> opt;
		auto dat = splitStr(str, ':');
		opt.first = dat[0];
		opt.second = dat[1];
		return opt;
	}

	//@TODO add error reporting so the bad line can be found easily
	notefield* parseNoteData(std::ifstream& file, const double startBPM, const double offset)
	{
		std::string str;
		notefield* opt = new notefield(glm::vec2(1280, 720));
		double curBPM = startBPM;
		double curTime = offset;
		while (std::getline(file, str))
		{
			if (str.find(';') != std::string::npos) break;

			if (str[0] == '#')
			{
				//@TODO BPM tag
			}
			else
			{
				std::vector<std::string> dat = splitStr(str);
				unsigned char type = dat[0][0] - '0';

				switch (type)
				{
				case 0:
				{
					if (dat.size() != 3)
					{
						delete opt;
						return nullptr;
					}
					noteSlot slot = toNoteslot(dat[1]);
					double beats = std::stod(dat[2]);

					tap n = tap(curTime = curTime + (beats / curBPM) * 60.0, slot);
					opt->taps.push_back(n);
					
					break;
				}
				case 1:
				{
					if (dat.size() != 5)
					{
						delete opt;
						return nullptr;
					}
					//@TODO hold
					break;
				}
				case 2:
				{
					if (dat.size() != 4)
					{
						delete opt;
						return nullptr;
					}
					//@TODO slide
					break;
				}
				default:
					delete opt;
					return nullptr;
				}
			}
		}

		return opt;
	}

	song* loadSong(const char* filename)
	{
		std::ifstream file(filename);
		if (file.is_open())
		{
			song* opt = new song();

			opt->title = "TEST";
			opt->artist = "me";
			opt->offset = 5.0;

			opt->charts[0] = parseNoteData(file, 120, opt->offset);

			//check to make sure a valid chart exists
			for (unsigned char i = 0; i < (unsigned char)(difficulties::difCount); i++)
			{
				if (opt->charts[i] != nullptr)
				{
					return opt;
				}
			}

			delete opt;
			return nullptr;
		}

		return nullptr;
	}
}
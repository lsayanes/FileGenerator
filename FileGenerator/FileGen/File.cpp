

#include <stdio.h>
#include <string>

#include "utils.h"

#include "File.h"


File::File(const char* path, unsigned int id) :
	m_szPath{ new char[std::strlen(path) + 1] },
	m_File{ nullptr },
	m_bWorking{ false },
	m_ulTotalWords{ 0 },
	m_szLastLine{ "" },
	m_bStop{ true }
{
	std::strcpy(m_szPath, path);
	m_File = fopen(m_szPath, "w");

	m_srand = static_cast<unsigned int>((10.4 + static_cast<double>(id)) * 3456.9867);
}

void File::doit()
{
	FILE* pRead = fopen("read.txt", "r");
	if (pRead)
	{
		m_bWorking = true;
		m_bStop = false;

		fseek(pRead, 0, SEEK_END);
		size_t stFileLen = ftell(pRead);
		rewind(pRead);


		if (stFileLen)
		{
			std::string  word{""};
			std::string  line{""};
			int ch;
			int pos;
			int nWordCnt = 0;

			stFileLen -= 1;

			srand(m_srand);

			while (m_bWorking)
			{
				pos = static_cast<int>(File::random(1, static_cast<double>(stFileLen)));

				fseek(pRead, pos, SEEK_SET);
	
				//find word`s start
				while (!feof(pRead))
				{
					ch = fgetc(pRead);
					if (' ' == ch)
						break;
				}

				while (!feof(pRead))
				{
					ch = fgetc(pRead);
					if (' ' == ch)
					{
						if (word.length())
						{
							line += word;
							line += " ";
							word.clear();
							nWordCnt++;
							m_ulTotalWords++;
							break;
						}
					}
					else
					{
						if(isalpha(ch))
							word += ch;
					}
				}

				if (nWordCnt > 20)
				{
					fprintf(m_File, "%s\r\n", line.c_str());
					fflush(m_File);
					nWordCnt = 0;
					m_szLastLine = line;
					line.clear();
				}
				
				utils::sleep(100);
				rewind(pRead);
			}

		}
		fclose(pRead);
	}

	m_bStop = true;
}


File::~File()
{
	m_bWorking = false;

	uint16_t unWd = 0;

	utils::sleep(500);
	while (!m_bStop && unWd < 500)
	{
		utils::sleep(1);
		unWd++;
	}


	if (m_szPath)
		delete m_szPath;

	m_szPath = nullptr;

	if (m_File)
		fclose(m_File);

	m_File = nullptr;

}

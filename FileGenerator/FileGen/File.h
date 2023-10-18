#pragma once
class File
{
private:	
	char			* m_szPath{nullptr};
	FILE			*m_File;
	bool			m_bWorking;
	bool			m_bStop;

	uint32_t		m_ulTotalWords;
	std::string		m_szLastLine;

	unsigned int	m_srand;

private:

	inline double random(double min, double max)
	{
		//return (double(rand()) / (double(RAND_MAX)) * (max - min)) + min;

		double ret = rand();
		while (ret < min || ret > max)
			ret = rand();

		return ret;
	}
public:
	File(const char *path, unsigned int id);

	File(const File&) = delete;
	File& operator=(const File&) = delete;


	void doit();

	inline const char* path() const { return m_szPath;  }
	inline const char* lastLine() const { return m_szLastLine.c_str();  }

	virtual ~File();

};


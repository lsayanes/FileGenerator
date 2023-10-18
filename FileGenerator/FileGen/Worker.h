#pragma once
class Worker
{
private:

	const char	*szFolder;
	const char	*szName;
	const char	*szExtension;
	bool		bWorking;
	bool		bStop;

	std::vector<File*> files{};
public:
	static constexpr size_t minute{ 60000 };
	static constexpr size_t totalFilesView{ 50 };
	
public:
	Worker(const char *szFolderPath, const char *szNameFiles, const char *szExtensionFiles, size_t totalFiles);
	virtual ~Worker();


	Worker(const Worker&) = delete;
	Worker& operator=(const Worker&) = delete;
	
	void createVector(size_t from, size_t to);
	void runVector();
	void deleteVector(bool bDeleteFiles = true);
	void printVector();

	void start();

	inline size_t maxAllowed() { return (files.capacity() < totalFilesView) ? files.capacity() : totalFilesView; }
	inline bool isWorking() { return bWorking; }
	inline bool isStop() { return bStop; }
	inline void stop() { bWorking = false; }

	virtual void print(uint16_t line, const char*) = 0;
};


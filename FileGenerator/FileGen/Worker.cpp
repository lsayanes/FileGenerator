
#if !defined(MAX_PATH)
#define MAX_PATH (260)
#endif

#include <vector>
#include <thread>
#include <cstdio>

#include <stdint.h>
#include <string>
#include "utils.h"
#include "File.h"
#include "Worker.h"

Worker::Worker(const char* szFolderPath, const char* szNameFiles, const char* szExtensionFiles, size_t totalFiles):
    szFolder{ szFolderPath },
    szName{ szNameFiles },
    szExtension{ szExtensionFiles },
    bWorking{ false },
    bStop{ true }
{
    files.reserve(totalFiles);
}

Worker::~Worker()
{
    deleteVector();
}

void Worker::createVector(size_t from, size_t to)
{
    char strPath[MAX_PATH];

    for (size_t i = from; i < to; i++)
    {
        sprintf(strPath, "%s\\%s%llu.%s", szFolder, szName, i + 1, szExtension);
        File* p = new File(strPath, static_cast<unsigned int>(i * 1000));

        std::strcat(strPath, " <-Created");     
        print(static_cast<uint16_t>(files.size()), strPath);

        files.push_back(p);
    }
}

void Worker::runVector()
{
    std::vector<File*>::iterator it{ std::begin(files) };

    while (it != std::end(files))
    {
        std::thread t(&File::doit, *it);
        t.detach();
        it++;
    }
}

void Worker::deleteVector(bool bDeleteFiles)
{
    int bRet;
    char str[MAX_PATH * 2];
    char strPath[MAX_PATH];
    std::vector<File*>::iterator it{ std::begin(files) };

    uint16_t i = 0;
    while (it != std::end(files))
    {
        strcpy(strPath, (*it)->path());
        delete* it;
        it++;
        if (bDeleteFiles)
        {
            bRet = std::remove(strPath);
            sprintf(str, "DeleteFile %s = %s", strPath, bRet ? "FALSE" : "TRUE");
            print(i++, str);
        }
    }

    files.clear();
}

void Worker::printVector()
{
    size_t i;
    File* p;

    size_t total = maxAllowed();
 
    for (i = 0;  i < total; i++)
    {
        char str[MAX_PATH * 2];

        p = files.at(i);
        
        sprintf(str, "File: %003llu - %s - last line: \"%s\"", i + 1, p->path(), p->lastLine());
        print(static_cast<uint16_t>(i), str);
        
    }
}

void Worker::start()
{

    uint32_t totalIt = 0;
    uint32_t dwWait = 250;

    size_t from = 0, to = maxAllowed();
    size_t totalViews = 0;

    bWorking = true;
    bStop = false;
    
    createVector(from, to);
    runVector();
    printVector();

    while (bWorking)
    {
        printVector();
        utils::sleep(dwWait);
        totalIt++;

        if (totalIt > (minute / dwWait))
        {
            utils::sleep(10);
            deleteVector();
 
            totalViews = to;
            from = to; 
            to = files.capacity() - totalViews > totalFilesView ? from + totalFilesView : files.capacity();

            if(from >= to)
            {
                from = 0; 
                to = maxAllowed();
            }

            createVector(from, to);
            runVector();
            totalIt = 0;
        }
    }

    bStop = true;
}

#include "platform/windows/FileUtilsWindows.h"
#include <unistd.h>

namespace xGame2D
{
    static std::string currentDir = "";

    FileUtilsWindows::FileUtilsWindows()
    {
    }

    FileUtilsWindows::~FileUtilsWindows()
    {
    }

    const std::string &FileUtilsWindows::getCurrentDir()
    {
        if (currentDir.empty())
        {
            char cwd[256];
            _getcwd(cwd, sizeof(cwd));
            currentDir = cwd;
            currentDir.append("\\");
        }
        return currentDir;
    }

    std::string &FileUtilsWindows::getFullPath(std::string &path)
    {
        if (!(path.length() > 2
              && ((path[0] >= 'a' && path[0] <= 'z') || (path[0] >= 'A' && path[0] <= 'Z'))
              && path[1] == ':'))
        {
            path.insert(0, getCurrentDir());
        }
        return path;
    }

    FileUtils* FileUtils::getInstance()
    {
        if (instance == nullptr)
        {
            instance = new FileUtilsWindows();
        }
        return instance;
    }
}
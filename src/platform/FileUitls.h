#ifndef __X_FILE_UTILS_H__
#define __X_FILE_UTILS_H__

#include <string>

namespace xGame2D
{
    class FileUtils
    {
    public:
        static FileUtils *getInstance();
        virtual ~FileUtils();

        virtual const std::string &getCurrentDir() = 0;
        virtual std::string &getFullPath(std::string &path) = 0;

    protected:
        FileUtils();
        static FileUtils *instance;
    };
}

#endif
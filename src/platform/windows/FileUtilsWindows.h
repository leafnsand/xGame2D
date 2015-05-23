#ifndef __X_FILEUTILSWINDOWS_H__
#define __X_FILEUTILSWINDOWS_H__

#include "platform/FileUitls.h"

namespace xGame2D
{
    class FileUtilsWindows : public FileUtils
    {
    public:
        FileUtilsWindows();
        virtual ~FileUtilsWindows();
        virtual const std::string &getCurrentDir() override;
        virtual std::string &getFullPath(std::string &path) override;
    };
}

#endif
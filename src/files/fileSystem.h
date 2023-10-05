#pragma once
#include <string>
#include <cstdlib>
#include <helpers/RootDir.h>

class FileSystem
{
private:
    typedef std::string(*Builder) (const std::string& path);

public:
    static std::string GetPath(const std::string& path)
    {
        static std::string(*pathBuilder)(std::string const&) = GetPathBuilder();
        return (*pathBuilder)(path);
    }

private:
    static Builder GetPathBuilder()
    {
        if (ROOT_DIR != "")
            return &FileSystem::GetPathRelativeRoot;
        else
            return &FileSystem::GetPathRelativeBinary;
    }

    static std::string GetPathRelativeRoot(const std::string& path)
    {
        return ROOT_DIR + std::string("/") + path;
    }

    static std::string GetPathRelativeBinary(const std::string& path)
    {
        return "../../../" + path;
    }
};

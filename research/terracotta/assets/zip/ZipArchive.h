#ifndef TERRACOTTA_UTIL_ZIPARCHIVE_H_
#define TERRACOTTA_UTIL_ZIPARCHIVE_H_

#include <string>
#include <vector>

#include "miniz.h"

namespace terra {
namespace assets {

class ZipArchive {
public:
    ZipArchive();

    bool Open(const char *archivePath);
    void Close();

    // Reads a file from the archive into the heap.
    char* ReadFile(const char* filename, size_t* size);
    void FreeFileData(char* data);

    // Lists all of the files in the archive that contain search in their name.
    std::vector<std::string> ListFiles(const char* search);
    
private:
    mz_zip_archive m_Archive;
};

} // ns assets
} // ns terra

#endif

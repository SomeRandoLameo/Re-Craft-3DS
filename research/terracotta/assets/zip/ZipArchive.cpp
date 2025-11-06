#include "ZipArchive.h"

namespace terra {
namespace assets {

ZipArchive::ZipArchive() {

}

bool ZipArchive::Open(const char *archivePath) {
    mz_zip_zero_struct(&m_Archive);

    if (!mz_zip_reader_init_file_v2(&m_Archive, archivePath, MZ_ZIP_FLAG_DO_NOT_SORT_CENTRAL_DIRECTORY, 0, 0)) {
        return false;
    }

    return true;
}

void ZipArchive::Close() {
    mz_zip_reader_end(&m_Archive);

    mz_zip_zero_struct(&m_Archive);
}

// Reads a file from the archive into the heap.
char* ZipArchive::ReadFile(const char* filename, size_t* size) {
    unsigned int index;
    void* p = nullptr;

    if (mz_zip_reader_locate_file_v2(&m_Archive, filename, nullptr, 0, &index)) {
        p = mz_zip_reader_extract_to_heap(&m_Archive, index, size, 0);
    }

    return static_cast<char*>(p);
}

void ZipArchive::FreeFileData(char* data) {
    mz_free(data);
}

// Lists all of the files in the archive that contain search in their name.
std::vector<std::string> ZipArchive::ListFiles(const char* search) {
    std::vector<std::string> results;
    unsigned int count = mz_zip_reader_get_num_files(&m_Archive);

    mz_zip_archive_file_stat stat;

    if (!mz_zip_reader_file_stat(&m_Archive, 0, &stat)) {
        return results;
    }

    for (unsigned int i = 0; i < count; ++i) {
        if (!mz_zip_reader_file_stat(&m_Archive, i, &stat)) continue;
        if (mz_zip_reader_is_file_a_directory(&m_Archive, i)) continue;

        const char* current = stat.m_filename;

        if (search == nullptr || strstr(current, search) != nullptr) {
            results.emplace_back(current);
        }
    }

    return results;
}

} // ns assets
} // ns terra

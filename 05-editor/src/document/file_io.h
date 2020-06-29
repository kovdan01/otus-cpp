#ifndef FILE_IO_H
#define FILE_IO_H

#include "document.h"

#include <string>

namespace my
{

namespace file_io
{

/**
 * @brief Imports document from a file
 * @return Pointer to the imported document
 */
inline Document::Ptr import_document(const std::string& /*filename*/)
{
    return std::make_unique<Document>();
}

/**
 * @brief Exports document to a file
 */
inline void export_document(const Document::Ptr& /*document*/, const std::string& /*filename*/)
{
}

} // namespace file_op

} // namespace my

#endif // FILE_IO_H

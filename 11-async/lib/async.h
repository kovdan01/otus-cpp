#ifndef ASYNC_H
#define ASYNC_H

#include <cstddef>

namespace async
{

using handle_t = void*;

/**
 * @brief connect Create a new command bulks handler
 * @param bulk Bulk size
 * @return Handle to pass to other functions
 */
handle_t connect(std::size_t bulk);

/**
 * @brief receive Pass input to command bulks handler
 * @param handle Handle received from connect
 * @param data String
 * @param size String size
 */
void receive(handle_t handle, const char* data, std::size_t size);

/**
 * @brief Delete command bulks handler
 * @param handle Handle received from connect
 */
void disconnect(handle_t handle);

} // namespace async

#endif // ASYNC_H

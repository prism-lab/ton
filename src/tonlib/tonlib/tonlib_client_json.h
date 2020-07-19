/*
    This file is part of TON Blockchain Library.

    TON Blockchain Library is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    TON Blockchain Library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with TON Blockchain Library.  If not, see <http://www.gnu.org/licenses/>.

    Copyright 2017-2020 Telegram Systems LLP
*/
#pragma once

/**
 * \file
 * C interface for interaction with TonLib via JSON-serialized objects.
 * Can be used to easily integrate TonLib with any programming language which supports calling C functions
 * and is able to work with JSON.
 *
 * The JSON serialization of TonLib API objects is straightforward: all API objects are represented as JSON objects with
 * the same keys as the API object field names. The object type name is stored in the special field '@type' which is
 * optional in places where type is uniquely determined by the context.
 * Fields of Bool type are stored as Boolean, fields of int32, int53, and double types are stored as Number, fields of
 * int64 and string types are stored as String, fields of bytes type are base64 encoded and then stored as String,
 * fields of vector type are stored as Array.
 * The main TonLib interface is asynchronous. To match requests with a corresponding response a field "@extra" can
 * be added to the request object. The corresponding response will have an "@extra" field with exactly the same value.
 *
 * A TonLib client instance should be created through tonlib_client_json_create.
 * Requests then can be sent using tonlib_client_json_send from any thread.
 * New updates and request responses can be received through tonlib_client_json_receive from any thread. This function
 * shouldn't be called simultaneously from two different threads. Also note that all updates and request responses
 * should be applied in the order they were received to ensure consistency.
 * Given this information, it's advisable to call this function from a dedicated thread.
 * Some service TonLib requests can be executed synchronously from any thread by using tonlib_client_json_execute.
 * The TonLib client instance can be destroyed via tonlib_client_json_destroy.
 *
 * General pattern of usage:
 * \code
 * void *client = tonlib_client_json_create();
 * // somehow share the client with other threads, which will be able to send requests via tonlib_client_json_send
 *
 * const double WAIT_TIMEOUT = 10.0; // seconds
 * int is_closed = 0;  // should be set to 1, when updateAuthorizationState with authorizationStateClosed is received
 * while (!is_closed) {
 *   const char *result = tonlib_client_json_receive(client, WAIT_TIMEOUT);
 *   if (result) {
 *     // parse the result as JSON object and process it as an incoming update or an answer to a previously sent request
 *   }
 * }
 * tonlib_client_json_destroy(client);
 * \endcode
 */

#include "tonlib/tonlibjson_export.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Creates a new instance of TonLib.
 * \return Pointer to the created instance of TonLib.
 */
TONLIBJSON_EXPORT void *tonlib_client_json_create();

/**
 * Sends request to the TonLib client. May be called from any thread.
 * \param[in] client The client.
 * \param[in] request JSON-serialized null-terminated request to TonLib.
 */
TONLIBJSON_EXPORT void tonlib_client_json_send(void *client, const char *request);

/**
 * Receives incoming updates and request responses from the TonLib client. May be called from any thread, but
 * shouldn't be called simultaneously from two different threads.
 * Returned pointer will be deallocated by TonLib during next call to tonlib_client_json_receive or tonlib_client_json_execute
 * in the same thread, so it can't be used after that.
 * \param[in] client The client.
 * \param[in] timeout The maximum number of seconds allowed for this function to wait for new data.
 * \return JSON-serialized null-terminated incoming update or request response. May be NULL if the timeout expires.
 */
TONLIBJSON_EXPORT const char *tonlib_client_json_receive(void *client, double timeout);

/**
 * Synchronously executes TonLib request. May be called from any thread.
 * Only a few requests can be executed synchronously.
 * Returned pointer will be deallocated by TonLib during next call to tonlib_client_json_receive or tonlib_client_json_execute
 * in the same thread, so it can't be used after that.
 * \param[in] client The client. Currently ignored for all requests, so NULL can be passed.
 * \param[in] request JSON-serialized null-terminated request to TonLib.
 * \return JSON-serialized null-terminated request response. May be NULL if the request can't be parsed.
 */
TONLIBJSON_EXPORT const char *tonlib_client_json_execute(void *client, const char *request);

/**
 * Destroys the TonLib client instance. After this is called the client instance shouldn't be used anymore.
 * \param[in] client The client.
 */
TONLIBJSON_EXPORT void tonlib_client_json_destroy(void *client);

#ifdef __cplusplus
}  // extern "C"
#endif

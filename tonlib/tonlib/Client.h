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
#include "auto/tl/tonlib_api.h"

namespace tonlib_api = ton::tonlib_api;

namespace tonlib {

/**
 * Native C++ interface for interaction with TonLib.
 *
 * The TonLib instance is created for the lifetime of the Client object.
 * Requests to TonLib can be sent using the Client::send method from any thread.
 * New updates and responses to requests can be received using the Client::receive method from any thread,
 * this function shouldn't be called simultaneously from two different threads. Also note that all updates and
 * responses to requests should be applied in the same order as they were received, to ensure consistency.
 * Given this information, it's advisable to call this function from a dedicated thread.
 * Some service TonLib requests can be executed synchronously from any thread by using the Client::execute method.
 *
 * General pattern of usage:
 * \code
 * std::shared_ptr<tonlib::Client> client = std::make_shared<tonlib::Client>();
 * // somehow share the client with other threads, which will be able to send requests via client->send
 *
 * const double WAIT_TIMEOUT = 10.0;  // seconds
 * bool is_closed = false;            // should be set to true, when updateAuthorizationState with
 *                                    // authorizationStateClosed is received
 * while (!is_closed) {
 *   auto response = client->receive(WAIT_TIMEOUT);
 *   if (response.object == nullptr) {
 *     continue;
 *   }
 *
 *   if (response.id == 0) {
 *     // process response.object as an incoming update of type tonlib_api::Update
 *   } else {
 *     // process response.object as an answer to a sent request with id response.id
 *   }
 * }
 * \endcode
 */
class Client final {
 public:

  /**
   * Creates a new TonLib client.
   */
  Client();

  /**
   * A request to the TonLib.
   */
  struct Request {
    /**
     * Request identifier.
     * Responses to TonLib requests will have the same id as the corresponding request.
     * Updates from TonLib will have id == 0, incoming requests are thus disallowed to have id == 0.
     */
    std::uint64_t id;

    /**
     * TonLib API function representing a request to TonLib.
     */
    tonlib_api::object_ptr<tonlib_api::Function> function;
  };

  /**
   * Sends request to TonLib. May be called from any thread.
   * \param[in] request Request to TonLib.
   */
  void send(Request&& request);

  /**
   * A response to a request, or an incoming update from TonLib.
   */
  struct Response {
    /**
     * TonLib request identifier, which corresponds to the response or 0 for incoming updates from TonLib.
     */
    std::uint64_t id;

    /**
     * TonLib API object representing a response to a TonLib request or an incoming update.
     */
    tonlib_api::object_ptr<tonlib_api::Object> object;
  };

  /**
   * Receives incoming updates and request responses from TonLib. May be called from any thread, but shouldn't be
   * called simultaneously from two different threads.
   * \param[in] timeout The maximum number of seconds allowed for this function to wait for new data.
   * \return An incoming update or request response. The object returned in the response may be a nullptr
   *         if the timeout expires.
   */
  Response receive(double timeout);

  /**
   * Synchronously executes TonLib requests. Only a few requests can be executed synchronously.
   * May be called from any thread.
   * \param[in] request Request to the TonLib.
   * \return The request response.
   */
  static Response execute(Request&& request);

  /**
   * Destroys the client and TonLib instance.
   */
  ~Client();

  /**
   * Move constructor.
   */
  Client(Client&& other);

  /**
   * Move assignment operator.
   */
  Client& operator=(Client&& other);

 private:
  class Impl;
  std::unique_ptr<Impl> impl_;
};
}  // namespace tonlib

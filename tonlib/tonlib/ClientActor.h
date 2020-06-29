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

#include "tonlib/TonlibCallback.h"
#include "tonlib/TonlibClient.h"
#include "auto/tl/tonlib_api.h"

namespace tonlinb {

/**
 * This is a low-level Actor interface for interaction with TonLib. The interface is a lot more flexible than
 * the Client interface, however, for most usages the Client interface should be sufficient.
 */
class ClientActor : public td::actor::Actor {
 public:

  /**
   * Creates a ClientActor using the specified callback.
   * \param[in] callback Callback for outgoing notifications from TonLib.
   */
  explicit ClientActor(td::unique_ptr<TonlibCallback> callback);

  /**
   * Sends one request to TonLib. The answer will be received via callback.
   * \param[in] id Request identifier, must be positive.
   * \param[in] request The request.
   */
  void request(td::uint64 id, tonlib_api::object_ptr<tonlib_api::Function> request);

  /**
 * Synchronously executes a TonLib request. Only a few requests can be executed synchronously.
 * May be called from any thread.
 * \param[in] request Request to the TonLib.
 * \return The request response.
 */
  static tonlib_api::object_ptr<tonlib_api::Object> execute(tonlib_api::object_ptr<tonlib_api::Function> request);

  /**
   * Destroys the ClientActor and the TonLib instance.
   */
  ~ClientActor();

  /**
   * Move constructor.
   */
  ClientActor(ClientActor&& other);

  /**
   * Move assignment operator.
   */
  ClientActor& operator=(ClientActor&& other);

  ClientActor(const ClientActor& other) = delete;
  ClientActor& operator=(const ClientActor& other) = delete;

 private:
  td::actor::ActorOwn<TonlibClient> tonlib_;
};
}  // namespace tonlinb

//
// Created by prism-lab on 29-Aug-20.
//
#include "base64.h"

#include "td/utils/crypto.h"
#include "td/utils/base64.h"


namespace td {

td::Result<std::string> adnl_id_encode(td::Slice id, bool upper_case) {
  if (id.size() != 32) {
    return td::Status::Error("Wrong andl id size");
  }
  td::uint8 buf[35];
  td::MutableSlice buf_slice(buf, 35);
  buf_slice[0] = 0x2d;
  buf_slice.substr(1).copy_from(id);
  auto hash = td::crc16(buf_slice.substr(0, 33));
  buf[33] = static_cast<td::uint8>((hash >> 8) & 255);
  buf[34] = static_cast<td::uint8>(hash & 255);
  return td::base32_encode(buf_slice, upper_case).substr(1);
}

std::string adnl_id_encode(td::Bits256 adnl_addr, bool upper_case) {
  return adnl_id_encode(adnl_addr.as_slice(), upper_case).move_as_ok();
}

td::Result<Bits256> adnl_id_decode(td::Slice id) {
  if (id.size() != 55) {
    return td::Status::Error("Wrong length of adnl id");
  }
  td::uint8 buf[56];
  buf[0] = 'f';
  td::MutableSlice buf_slice(buf, 56);
  buf_slice.substr(1).copy_from(id);
  TRY_RESULT(decoded_str, td::base32_decode(buf_slice));
  auto decoded = td::Slice(decoded_str);
  if (decoded[0] != 0x2d) {
    return td::Status::Error("Invalid first byte");
  }
  auto got_hash = (decoded.ubegin()[33] << 8) | decoded.ubegin()[34];
  auto hash = td::crc16(decoded.substr(0, 33));
  if (hash != got_hash) {
    return td::Status::Error("Hash mismatch");
  }
  Bits256 res;
  res.as_slice().copy_from(decoded.substr(1, 32));
  return res;
}

}
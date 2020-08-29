//
// Created by prism-lab on 29-Aug-20.
//
#pragma once

#include <string>

#include "td/utils/Slice.h"
#include "td/utils/Status.h"
//#include "td/utils/buffer.h"

#include "bitstring.h"

namespace td {

td::Result<std::string> adnl_id_encode(td::Slice id, bool upper_case = false);
std::string adnl_id_encode(Bits256 adnl_addr, bool upper_case = false);
td::Result<Bits256> adnl_id_decode(td::Slice id);

}
/*
 * Copyright (c) 2021, RPTU Kaiserslautern-Landau
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Authors:
 *    Derek Christ
 */

#ifndef DRAMSYSCONFIGURATION_ADDRESSMAPPING_H
#define DRAMSYSCONFIGURATION_ADDRESSMAPPING_H

#include "DRAMSys/util/json.h"

#include <optional>

namespace DRAMSys::Config {

/// @brief Address mapping configuration, which defines the mapping of the address entries
/// to the physical address space.
/// @details HBM2: BYTE_BIT, COLUMN_BIT, ROW_BIT, BANK_BIT, BANKGROUP_BIT, STACK_BIT, PSEUDOCHANNEL_BIT.
///          DDR3: BYTE_BIT, COLUMN_BIT, ROW_BIT, BANK_BIT.
///          DDR4: BYTE_BIT, COLUMN_BIT, ROW_BIT, BANK_BIT, BANKGROUP_BIT.
///          LPDDR4: BYTE_BIT, COLUMN_BIT, ROW_BIT, BANK_BIT.
struct AddressMapping {
  static constexpr std::string_view KEY = "addressmapping";
  static constexpr std::string_view SUB_DIR = "addressmapping";

  using BitEntry = std::variant<unsigned int, std::vector<unsigned int>>;

  std::optional<std::vector<BitEntry>> BYTE_BIT;
  std::optional<std::vector<BitEntry>> COLUMN_BIT;
  std::optional<std::vector<BitEntry>> ROW_BIT;
  std::optional<std::vector<BitEntry>> BANK_BIT;
  std::optional<std::vector<BitEntry>> BANKGROUP_BIT;
  std::optional<std::vector<BitEntry>> RANK_BIT;
  std::optional<std::vector<BitEntry>> STACK_BIT;
  std::optional<std::vector<BitEntry>> PSEUDOCHANNEL_BIT;
  std::optional<std::vector<BitEntry>> CHANNEL_BIT;
};

NLOHMANN_JSONIFY_ALL_THINGS(AddressMapping, BYTE_BIT, COLUMN_BIT, ROW_BIT,
                            BANK_BIT, BANKGROUP_BIT, RANK_BIT, STACK_BIT,
                            PSEUDOCHANNEL_BIT, CHANNEL_BIT)

}  // namespace DRAMSys::Config

#endif  // DRAMSYSCONFIGURATION_ADDRESSMAPPING_H

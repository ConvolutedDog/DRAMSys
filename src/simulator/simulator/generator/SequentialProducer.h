/*
 * Copyright (c) 2023, RPTU Kaiserslautern-Landau
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

#pragma once

#include "simulator/request/RequestProducer.h"

#include <optional>
#include <random>

class SequentialProducer : public RequestProducer {
public:
  SequentialProducer(uint64_t numRequests, std::optional<uint64_t> seed,
                     double rwRatio, std::optional<uint64_t> addressIncrement,
                     std::optional<uint64_t> minAddress,
                     std::optional<uint64_t> maxAddress, uint64_t memorySize,
                     unsigned int dataLength);

  Request nextRequest() override;

  uint64_t totalRequests() override { return numberOfRequests; }
  void reset() override { generatedRequests = 0; }

  const uint64_t numberOfRequests;
  const uint64_t addressIncrement;
  const uint64_t minAddress;
  const uint64_t maxAddress;
  const uint64_t seed;
  const double rwRatio;
  const unsigned int dataLength;

  std::default_random_engine randomGenerator;
  std::uniform_real_distribution<double> readWriteDistribution{0.0, 1.0};

  uint64_t generatedRequests = 0;
};

/************************************************************
 *
 *                 OPEN TRANSACTIONS
 *
 *       Financial Cryptography and Digital Cash
 *       Library, Protocol, API, Server, CLI, GUI
 *
 *       -- Anonymous Numbered Accounts.
 *       -- Untraceable Digital Cash.
 *       -- Triple-Signed Receipts.
 *       -- Cheques, Vouchers, Transfers, Inboxes.
 *       -- Basket Currencies, Markets, Payment Plans.
 *       -- Signed, XML, Ricardian-style Contracts.
 *       -- Scripted smart contracts.
 *
 *  EMAIL:
 *  fellowtraveler@opentransactions.org
 *
 *  WEBSITE:
 *  http://www.opentransactions.org/
 *
 *  -----------------------------------------------------
 *
 *   LICENSE:
 *   This Source Code Form is subject to the terms of the
 *   Mozilla Public License, v. 2.0. If a copy of the MPL
 *   was not distributed with this file, You can obtain one
 *   at http://mozilla.org/MPL/2.0/.
 *
 *   DISCLAIMER:
 *   This program is distributed in the hope that it will
 *   be useful, but WITHOUT ANY WARRANTY; without even the
 *   implied warranty of MERCHANTABILITY or FITNESS FOR A
 *   PARTICULAR PURPOSE.  See the Mozilla Public License
 *   for more details.
 *
 ************************************************************/

#include <iostream>

#include "opentxs-proto/verify/VerificationGroup.hpp"

#include "opentxs-proto/verify/VerificationIdentity.hpp"

namespace opentxs { namespace proto
{

bool CheckProto_1(
    const VerificationGroup& verificationGroup,
    const VerificationType indexed)
{
    VerificationNymMap nymMap;

    for (auto& it : verificationGroup.identity()) {
        bool validIdentity = Check(
            it,
            VerificationGroupAllowedIdentity.at(verificationGroup.version()).first,
            VerificationGroupAllowedIdentity.at(verificationGroup.version()).second,
            nymMap,
            indexed);

        if (!validIdentity) {
            std::cerr << "Verify serialized verification group failed:"
                      << " invalid identity: (" << it.nym() << ")."
                      << std::endl;

            return false;
        }
    }

    for (auto& nym : nymMap) {
        if (nym.second > 1) {
            std::cerr << "Verify serialized verification group failed:"
                      << " duplicate identity: (" << nym.first << ")."
                      << std::endl;

            return false;
        }
    }

    return true;
}

} // namespace proto
} // namespace opentxs

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

#ifndef OPENTXS_PROTO_VERIFY
#define OPENTXS_PROTO_VERIFY

#include <iostream>
#include <map>

namespace opentxs { namespace proto
{
    // This defined a map between the version of the parent object
    // and the (minimum, maximum) acceptable versions of a child object.
    typedef std::map<uint32_t, std::pair<uint32_t, uint32_t> > VersionMap;

    static const uint32_t MIN_PLAUSIBLE_IDENTIFIER = 20;
    static const uint32_t MIN_PLAUSIBLE_KEYSIZE = 16;
    static const uint32_t MIN_PLAUSIBLE_SIGNATURE = 32;

    template<typename T, typename ...Args>
    bool Check(
        const T& serialized,
        const uint32_t minVersion,
        const uint32_t maxVersion,
        Args&&... params)
    {
        if (!serialized.has_version()) {
            std::cerr << "Verify protobuf failed: missing version." << std::endl;

            return false;
        }

        uint32_t version = serialized.version();

        if ((version < minVersion) || (version > maxVersion)) {
            std::cerr << "Verify protobuf failed: incorrect version ("
            << serialized.version() << ")." << std::endl;

            return false;
        }

        switch (version) {
            case 1 :

                return CheckProto_1(serialized, params...);
            default :
                std::cerr << "Verify protobuf failed: unknown version ("
                << serialized.version() << ")." << std::endl;

                return false;
        }

        return true;
    }
} // namespace proto
} // namespace opentxs
#endif // OPENTXS_PROTO_VERIFY

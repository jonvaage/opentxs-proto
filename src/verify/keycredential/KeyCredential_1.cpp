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

#include "../../../include/verify/KeyCredential.hpp"

#include <iostream>

namespace opentxs { namespace proto
{

bool KeyCredential_1(
    const KeyCredential serializedKeyCred,
    const proto::CredentialRole role,
    const proto::CredentialType credType,
    const proto::KeyMode mode,
    const proto::SourceType sourceType)
{
    AsymmetricKey authKey;
    AsymmetricKey encryptKey;
    AsymmetricKey signKey;
    bool validChildData = false;
    bool validMasterData = false;
    bool validAuthKey = false;
    bool validEncryptKey = false;
    bool validSignKey = false;

    if (!serializedKeyCred.has_mode()) {
        std::cerr << "Verify serialized key credential failed: missing mode." << std::endl;
        return false;
    }

    if (serializedKeyCred.mode() != mode) {
        std::cerr << "Verify serialized key credential failed: incorrect mode ("
                << serializedKeyCred.mode() << ")." << std::endl;
        return false;
    }

    if ((KEYMODE_PUBLIC == mode) && (CREDROLE_MASTERKEY == role)) {
        if (!serializedKeyCred.has_masterdata()) {
            std::cerr << "Verify serialized key credential failed: missing master data." << std::endl;
            return false;
        }

        if (serializedKeyCred.has_childdata()) {
            std::cerr << "Verify serialized key credential failed: master credential contains child data." << std::endl;
            return false;
        }

        validMasterData = Verify(
            serializedKeyCred.masterdata(),
            KeyCredentialAllowedMasterParams.at(serializedKeyCred.version()).first,
            KeyCredentialAllowedChildParams.at(serializedKeyCred.version()).second,
            sourceType);

        if (!validMasterData) {
            std::cerr << "Verify serialized key credential failed: invalid master data." << std::endl;
            return false;
        }
    }

    if ((KEYMODE_PUBLIC == mode) && (CREDROLE_CHILDKEY == role)) {
        if (!serializedKeyCred.has_childdata()) {
            std::cerr << "Verify serialized key credential failed: missing child data." << std::endl;
            return false;
        }

        if (serializedKeyCred.has_masterdata()) {
            std::cerr << "Verify serialized key credential failed: child credential contains master data." << std::endl;
            return false;
        }

        validChildData = Verify(
            serializedKeyCred.childdata(),
            KeyCredentialAllowedChildParams.at(serializedKeyCred.version()).first,
            KeyCredentialAllowedChildParams.at(serializedKeyCred.version()).second);

        if (!validChildData) {
            std::cerr << "Verify serialized key credential failed: invalid child data." << std::endl;
            return false;
        }
    }

    if (3 != serializedKeyCred.key_size()) {
        std::cerr << "Verify serialized key credential failed: missing keys ("
        << serializedKeyCred.key_size() << " of 3 found)." << std::endl;
        return false;
    }

    authKey = serializedKeyCred.key(KEYROLE_AUTH - 1);
    encryptKey = serializedKeyCred.key(KEYROLE_ENCRYPT - 1);
    signKey = serializedKeyCred.key(KEYROLE_SIGN - 1);

    validAuthKey = Verify(
        authKey,
        KeyCredentialAllowedAsymmetricKey.at(serializedKeyCred.version()).first,
        KeyCredentialAllowedAsymmetricKey.at(serializedKeyCred.version()).second,
        credType,
        mode,
        KEYROLE_AUTH);
    validEncryptKey = Verify(
        encryptKey,
        KeyCredentialAllowedAsymmetricKey.at(serializedKeyCred.version()).first,
        KeyCredentialAllowedAsymmetricKey.at(serializedKeyCred.version()).second,
        credType,
        mode,
        KEYROLE_ENCRYPT);
    validSignKey = Verify(
        signKey,
        KeyCredentialAllowedAsymmetricKey.at(serializedKeyCred.version()).first,
        KeyCredentialAllowedAsymmetricKey.at(serializedKeyCred.version()).second,
        credType,
        mode,
        KEYROLE_SIGN);

    if (!validAuthKey) {
        std::cerr << "Verify serialized key credential failed: invalid auth key." << std::endl;
        return false;
    }

    if (!validEncryptKey) {
        std::cerr << "Verify serialized key credential failed: invalid encrypt key." << std::endl;
        return false;
    }

    if (!validSignKey) {
        std::cerr << "Verify serialized key credential failed: invalid sign key." << std::endl;
        return false;
    }

    return true;
}

} // namespace proto
} // namespace opentxs

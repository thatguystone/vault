/**
 * @copyright 2015 Andrew Stone <a@stoney.io>
 * @license MIT
 */

#include "test_test.h"
#include "vault_test.h"

namespace vault
{

VLTEST(vaultCreate)
{
	Vault v(test::file("vault.vlt"));

	v.create(test::file("vault"), test::bytes("1m"),
			 std::make_unique<crypt::NewCreds>());
}
}

# Vault [![Build Status](https://travis-ci.org/thatguystone/vault.svg)](https://travis-ci.org/thatguystone/vault)

Vault is like TrueCrypt: it creates a file-backed encrypted dev where you can
store all your files. This is just a thin wrapper around cryptsetup with some
niceness to make managing the vault easier.

## Installation

1. Add `deb http://deb.stoney.io/ testing/` to your `sources.list`.
1. Install the archive keys: `sudo apt-get install stoney.io-archive-keyring`
1. `sudo aptitude install vault`

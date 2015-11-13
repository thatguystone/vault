/**
 * @copyright 2015 Andrew Stone <a@stoney.io>
 * @license MIT
 */

#include <boost/range/adaptor/reversed.hpp>
#include "transaction.h"

namespace vault
{

Transaction::~Transaction()
{
	if (this->ok_) {
		return;
	}

	for (auto &op : boost::adaptors::reverse(this->ops_)) {
		op->undo();
	}
}

void Transaction::commit()
{
	this->ok_ = true;
}
}

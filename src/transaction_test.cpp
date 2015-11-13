/**
 * @copyright 2015 Andrew Stone <a@stoney.io>
 * @license MIT
 */

#include "test_test.h"
#include "error.h"
#include "transaction.h"

namespace vault
{

const int totalRuns = 10;

struct opState {
	int runs_ = 0;
	int undos_ = 0;
};

struct op : public Transaction::Op {
	sp<opState> state_;
	int i_ = 0;

	void run(sp<opState> state)
	{
		this->state_ = std::move(state);
		this->i_ = ++this->state_->runs_;
	}

	void undo() noexcept
	{
		int me = totalRuns - this->i_;
		pt_eq(this->state_->undos_, me);

		this->state_->undos_++;
	}
};

VLTEST(transactionBasic)
{
	Transaction t;
	auto state = std::make_shared<opState>();

	for (int i = 0; i < totalRuns; i++) {
		t.run<op>(state);
	}

	t.commit();

	pt_eq(state->runs_, totalRuns);
	pt_eq(state->undos_, 0);
}

VLTEST(transactionFailure)
{
	auto state = std::make_shared<opState>();

	{
		Transaction t;
		for (int i = 0; i < totalRuns; i++) {
			t.run<op>(state);
		}
	}

	pt_eq(state->runs_, totalRuns);
	pt_eq(state->undos_, totalRuns);
}
}

#include "APIHandler.hpp"
#include "ExpertAdvisor/ExpertAdvisor.mqh"

ExpertAdvisor get_expert_advisor() {
	STATES current_state = NONE_NONE;
	double discrete_level = 0.002;
	double init_balance = 2000;
	int percentage_of_balance_to_use = 1;
	bool use_max_balance = false;
	ExpertAdvisor expert_advisor(current_state, discrete_level, init_balance, percentage_of_balance_to_use, use_max_balance);

	return expert_advisor;
}
#ifndef _DF_PROBLEM_H_
#define _DF_PROBLEM_H_

#include <ai_search.h>
#include <vector>
#include "df_State.h"
#include "df_Action.h"
#include "WorldAndAgent.h"

namespace df
{
	namespace Scavenger
	{
		class Problem : public ai::Search::Problem
		{
		public:
			Problem(df::Scavenger::State *initial_state_in);
			Problem(df::Scavenger::State *initial_state_in, secretAgentMan *model_in);
			~Problem();
			void setModel(df::Scavenger::secretAgentMan *model_in);
			void setGoal(df::Scavenger::State *goal_in);
			void Display() const;
			std::vector<ai::Search::Action *> Actions(const ai::Search::State * const state_in);
			ai::Search::State *Result(const ai::Search::State * const state_in, const ai::Search::Action * const action_in);

			bool GoalTest(const ai::Search::State * const state_in) const;
			double StepCost(const ai::Search::State  * const state1_in, const ai::Search::Action * const action_in,	const ai::Search::State  * const state2_in) const;
			double Heuristic(const ai::Search::State  * const state_in) const;
			void SetFindAll();


		private:
		protected:
			//df::Scavenger::State *agent_state;
			df::Scavenger::State *goal_state;
			df::Scavenger::secretAgentMan *model;
			bool find_all;
			bool pVisited;
		};
	}
}

#endif


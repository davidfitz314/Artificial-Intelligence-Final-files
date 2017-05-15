#ifndef _DF_SEARCH_H_
#define _DF_SEARCH_H_
#include "df_State.h"
#include "df_Action.h"
#include "df_Problem.h"
#include <queue>

namespace df
{
	namespace Scavenger
	{
		struct slocation {
			int x;
			int y;
		};

		class Search
		{
		public:
			Search(df::Scavenger::secretAgentMan *model_in);
			~Search();
			void goSearch();
			bool isEmpty();
			int qSize();
			df::Scavenger::Action *getQueAction();
			std::queue<df::Scavenger::Action *> getFifo();
			void setPathCost(double path_in);
			double getPathCost();
			void setLast(slocation last_in);
			slocation getLast();
			void setUnvisited(bool isitVisited);
			bool isUnvisited();

		private:
		protected:
			df::Scavenger::secretAgentMan* world_model;
			std::queue<df::Scavenger::Action * > fifo;
			double current_path_cost;
			slocation lastxy;
			bool mUnvisited;
		};
	}
}

#endif

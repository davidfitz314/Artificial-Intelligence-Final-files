#ifndef _DF_ACTION_H_
#define _DF_ACTION_H_
#include <ai_search.h>
#include <iostream>


namespace df
{
	namespace Scavenger
	{
		class Action : public ai::Search::Action
		{
		public:
			Action();
			Action(const Action &rhs);
			~Action();
			int getAction() const;
			std::string getActionWord(int act_in);
			bool setAction(int action);
			void Display() const;
			Action &operator=(const Action &rhs);
	        bool operator==(const Action &rhs) const;
		enum ActionEnum
        {
          /* Movement actions */
          GO_NORTH = 0,
          GO_SOUTH,
          GO_EAST,
          GO_WEST,
		  QUIT
		};
		private:
		protected:
			int mAction;
		};	
	}
}

#endif

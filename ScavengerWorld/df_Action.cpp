#include "df_Action.h"

namespace df
{
	namespace Scavenger
	{
	////////////Begin Action Class///////////////////

		Action::Action()
			: mAction(GO_NORTH)
		{
			//empty
		}

		Action::Action(const Action &rhs)
		{
			*this = rhs;
		}

		Action::~Action()
		{
			//empty
		}

		int Action::getAction() const
		{
			return this->mAction;
		}
	
		std::string Action::getActionWord(int act_in)
		{
			switch (act_in)
			{			
				case 0:
					return "GO_NORTH";
				case 1:
					return "GO_SOUTH";
				case 2:
					return "GO_EAST";
				case 3:
					return "GO_WEST";
				default:
					return "QUIT";
			}
		}

		bool Action::setAction(int action)
		{
			if (action >= GO_NORTH && action <= QUIT)
			{
				this->mAction = action;
				return true;
			}
			return false;
		}
	
		void Action::Display() const
		{
			std::cout << "+------+" << std::endl;
			int r = this->mAction;
			switch(r)
			{
				case 0  : std::cout << "North\n";   break;
				case 1 : std::cout << "South\n"; break;
				case 2 : std::cout << "East\n";  break;
				case 3 : std::cout << "West\n";  break;
				case 4 : std::cout << "Quit\n"; break;
			}
			
			std::cout << "+------+" << std::endl;
		}

		Action &Action::operator=(const Action &rhs)
		{
		  this->mAction = rhs.mAction;
		  return *this;
		}
		  
		bool Action::operator==(const Action &rhs) const
		{
		  return (this->mAction == rhs.mAction);
		}

	////////////end action class/////////////////////	
	}
}

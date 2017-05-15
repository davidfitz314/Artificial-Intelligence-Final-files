#ifndef _DF_STATE_H_
#define _DF_STATE_H_
#include <ai_search.h>
#include <cstdio>
#include <cstring>
#include <math.h>
#include <iostream>

namespace df
{
  	namespace Scavenger
  	{
		class State : public ai::Search::State
		{
		public:
			State();
			State(int x, int y, double z, double charge);
			~State();
			int getX() const;
			int getY() const;
			double getZ() const;
			double getCharge() const;
			void setX(const int &xIn);
			void setY(const int &yIn);
			void setZ(const double &zIn);
			void setCharge(const double &chargeIn);
			void Display() const;

			/* Used for equality testing. Must use dynamic_cast
		  	* to convert state_in to the correct pointer type.
		  	* True if the current object and state_in are the same value.
		  	*/
			bool IsEqual(const ai::Search::State * const state_in) const;
		  	/* Used for ordered storing. Must use dynamic_cast
		   	* to convert state_in to the correct pointer type.
		   	* True if current object is less than state_in.
		   	*/
		  	bool IsLessThan(const ai::Search::State * const state_in) const;

			bool operator==(const df::Scavenger::State &rhs) const;
			bool operator!=(const df::Scavenger::State &rhs) const;
			bool operator<(const df::Scavenger::State &rhs) const;

		protected:
			int mX, mY;
			double mZ;
			double mCharge;	

		};
	}
}

#endif

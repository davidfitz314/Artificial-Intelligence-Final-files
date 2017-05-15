#include "df_State.h"
namespace df
{
  namespace Scavenger
  {
/////////////////state class section////////////////////////////////
	State::State()
	{
		this->setX(0);
		this->setY(0);
		this->setZ(0.0);
		this->setCharge(100);
	}

	State::State(int x, int y, double z, double charge)
	{
		this->setX(x);
		this->setY(y);
		this->setZ(z);
		this->setCharge(charge);
	}

	State::~State()
	{
		//State();	
	}

	int State::getX() const
	{
		return this->mX;
	}

	int State::getY() const
	{
		return this->mY;
	}

	double State::getZ() const
	{
		return this->mZ;
	}

	double State::getCharge() const
	{
		return this->mCharge;
	}

	void State::setX(const int &xIn)
	{
		this->mX = xIn;
	}

	void State::setY(const int &yIn)
	{
		this->mY = yIn;
	}

	void State::setZ(const double &zIn)
	{
		this->mZ = zIn;
	}

	void State::setCharge(const double &chargeIn)
	{
		this->mCharge = chargeIn;
	}

	void State::Display() const
	{
		std::cout << std::endl;
		std::cout << "+------+" << std::endl;
		std::cout << "charge is " << this->mCharge << std::endl;
		std::cout << "X is " << this->mX << std::endl;
		std::cout << "Y is " << this->mY << std::endl;
		std::cout << "Z is " << this->mZ << std::endl;
		std::cout << "+------+" << std::endl;	
	}

	bool State::IsEqual(const ai::Search::State * const state_in) const
	{
		const State * const rhs = dynamic_cast<const State * const>(state_in);
		return (*this == *rhs);
	}

	bool State::IsLessThan(const ai::Search::State * const state_in) const
	{
		const State * const rhs = dynamic_cast<const State * const>(state_in);
		return (*this < *rhs);
	}

	bool State::operator==(const df::Scavenger::State &rhs) const
	{
		if (this->mX == rhs.mX && this->mY == rhs.mY)
			return true;
		return false;				
	}
	bool State::operator!=(const df::Scavenger::State &rhs) const
	{
		return !(*this == rhs);
	}
	bool State::operator<(const df::Scavenger::State &rhs) const
	{
		if (this->mX < rhs.mX)
			return true;
		if (this->mX > rhs.mX)
			return false;
		if (this->mY < rhs.mY)
			return true;
		if (this->mY > rhs.mY)
			return false;	
		return false;
	}
////////////end state class section//////////////
   }
}

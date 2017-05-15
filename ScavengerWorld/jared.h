#ifndef _JARED_H_
#define _JARED_H_
#include <queue>
#include <ai_scavenger.h>
#include "WorldAndAgent.h"
#include "df_Action.h"
#include "df_Search.h"


namespace df
{
  namespace Scavenger
  {
    class Jared : public ai::Agent::AgentProgram
    {
    public:
      Jared(ai::Agent::Options *opts);
      ~Jared();
      virtual ai::Agent::Action * Program(const ai::Agent::Percept * percept);
	  void addlObject(df::Scavenger::object object_in);
	  void updatelObject(df::Scavenger::object object_in);
	  void removelObject(df::Scavenger::object object_in);
    protected:
	  df::Scavenger::secretAgentMan mModel;
	  //df::Scavenger::cell mCell;
	  bool looking;
	  bool goHome;
	  bool examining;
	  std::queue<df::Scavenger::Action *> myFifo;
    private:
	  bool first_time;
	  char direction;
	  bool finalCheck;
	  //bool otest;
	  int objectCount;
	  bool preceptCheck;
	  std::string lastExamine;
	  std::string lastpicked;
	  int hopperCount;
	  std::map<std::string, df::Scavenger::object> hopperMap;
    };
	
  }
}

#endif /* _JARED_H_ */

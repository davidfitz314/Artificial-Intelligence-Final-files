#ifndef _DF_H_
#define _DF_H_
#include <queue>
#include "df_Action.h"
#include <ai_scavenger.h>
#include "WorldAndAgent.h"



namespace df
{
  namespace Scavenger
  {
    class Dagent : public ai::Agent::AgentProgram
    {
    public:
      Dagent(ai::Agent::Options *opts);
      ~Dagent();
      virtual ai::Agent::Action * Program(const ai::Agent::Percept * percept);
    protected:
	  df::Scavenger::secretAgentMan mModel;
	  std::queue<df::Scavenger::Action *> afifo;
	  bool finished;
	  bool first;
	  int *count;
    private:
    };

	
	
  }
}

#endif /* _DF_H_ */

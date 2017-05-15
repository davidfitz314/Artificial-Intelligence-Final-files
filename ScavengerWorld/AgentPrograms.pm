#
#
#
our $aps =
[
 {
   name => 'Snorlax',
   key  => 's',
   header => 'Snorlax.h',
   cpp    => ['Snorlax.cpp'],
   constructor => 'ai::Sample::Snorlax(opts)',
 },
 {
   name => 'Manual',
   key  => 'm',
   header => 'Manual.h',
   cpp    => ['Manual.cpp'],
   constructor => 'ai::Sample::Manual()',
 },
#{
#   name => 'Dagent',
#   key  => 'd',
#   header => 'df.h',
#   cpp    => ['df.cpp', 'WorldAndAgent.cpp', 'df_State.cpp', 'df_Action.cpp', 'df_Problem.cpp', 'df_Search.cpp'],
#   constructor => 'df::Scavenger::Dagent(opts)',
 #},
 {
   name => 'Jared',
   key  => 'j',
   header => 'jared.h',
   cpp    => ['jared.cpp', 'WorldAndAgent.cpp', 'df_State.cpp', 'df_Action.cpp', 'df_Problem.cpp', 'df_Search.cpp'],
   constructor => 'df::Scavenger::Jared(opts)',
 },
#{
#  name => '',
#  key  => '',
#  header => '',
#  constructor => '',
#},
 ];

1;

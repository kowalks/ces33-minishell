# Minishell - CES33

Implementation for a simple minishell on Linux. 

Features:
- Directory commands (try `cd ..`)
- Exit command (try `exit` on terminal)
- Output redirect (try `echo hello > out.txt`)
- Pipe construction (try `echo hello | grep h`)
- Env variable `PATH` for bin search (i.e. you can invoke
  a command with `<name>` instead of `/bin/<name>`)
- Signals (e.g. ctrl C or ctrl Z) redirected to child processes
- PGID set for commands inside same pipe

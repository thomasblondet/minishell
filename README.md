# minishell

Interpréteur de commande UNIX en C inspiré de Bash.

- Exécution de commandes avec `execve`.
- Gestion des pipes et redirections.
- Expansion des variables d'environnement.
- Commandes intégrées (`cd`, `echo`, `pwd`, `export`, `unset`, `env`, `exit`).
- Gestion des signaux.

```bash
make
./minishell
```
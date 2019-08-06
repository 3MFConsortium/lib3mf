# AStyle
http://astyle.sourceforge.net/

AStyle is the code formatter used for the development of lib3mf.

A pre-commit hook is in place that runs AStyle on all staged files to ensure consistent formatting of all C++-code in lib3mf.

## NOTE
This is WIP. A proper distribution mechanism of the client-side "pre-commit" is still needed.
Resources/ideas:
- https://www.viget.com/articles/two-ways-to-share-git-hooks-with-your-team/
- https://git-scm.com/book/en/v2/Customizing-Git-An-Example-Git-Enforced-Policy
- combination with server-side hook

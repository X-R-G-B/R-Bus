[fork]: https://github.com/X-R-G-B/R-Bus/fork
[pr]: https://github.com/X-R-G-B/R-Bus/compare/
[doc-codeQL]: https://docs.github.com/en/code-security/code-scanning/using-codeql-code-scanning-with-your-existing-ci-system/about-codeql-code-scanning-in-your-ci-system

# CONTRIBUTING

Here is the instructions if you want to contribute to the project

## Submitting a pull request

1. [Fork][fork] and clone the repository
2. Create a new branch: See '[creating a branch](#creating-a-branch)'
3. Make your change, add tests, and make sure the tests still pass
4. Make sure that the code is codeQL compliant
5. Make sure that you had documentation to the new added code (obviously compliant with our doc)
6. Push to your fork and [submit a pull request][pr] with correct labels added if necessary
7. Wait for your pull request to be reviewed and merged.  
  
Here are a few things you can do that will increase the likelihood of your pull request being accepted:

- The security of your code is analysed by [codeQL][doc-codeQL] the CI might fail if it's not secure.  
- The style of your code must respect the clang-format style.  
  Normally we have the .clang-format if you want for your IDE.
  If you don't want to use the .clang-format intergration in your IDE, you can still   
  launch the script format.sh (Linux) or format.ps1 (Windows), this will format automaticlly your code.  
**/!\ Warning ! If your code is not clang format compliant your CI will fail and your pr will be refused.**
- Write tests.
- Keep your change as focused as possible.  
  If there are multiple changes you would like to make that are not dependent upon each other, consider submitting them as separate pull requests.
- Write a commit message that respect the commit norm that we have, see '[Commit message norm](#commit-message-norm)'.


## Commit message norm
1. Define your context.
- CICD
- CLIENT-GAME
- DOCUMENTATION
- POC
- NETWORK
- SERVER
2. Followed by ":" + "action verb"
- : Add
- : Fix
- : Update
- etc ...
3. Explain what your commit introduce
- new feature for client-game
- bug inside network
- documentation for CLIENT-GAME
- etc...
4. Just after your commit message put a newline to indicate semver
- PATCH (If your commit introduce a bug fix)
- MINOR (If your commit introduce a new feature)
- MAJOR (If your commit introduce a new feature and the older version doesn't work anymore)

__/!\ Warning ! If your commit message doesn't respect those rules your pr might not be accepted__


## Creating a branch
If you need to contribute to our project, you will need to create a branch to submit a pull request.  
Basicly, you must indicate what is the context of your branch followed by a '/'.  
For exemple :
- feature/BRANCH-NAME
- doc/BRANCH-NAME
- refactor/BRANCH-NAME
- etc...

That's all you need to do to create a valid branch.
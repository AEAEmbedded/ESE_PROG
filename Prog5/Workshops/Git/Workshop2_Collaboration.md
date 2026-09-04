# Git Workshop 2 - Working together

**Prog 5 - date follows (Prog 5, period 1) - hands-on, about 95 minutes**

Builds on Workshop 1. After this workshop you develop on branches, open and review pull requests, resolve a merge conflict and recover from mistakes - the skills the rubric criterion "Manage: Code Management" rewards at the professional level. Work in pairs: most steps need a partner.

Steps:
1. Branches and pull requests (25 min)
2. Work together: collaborator and code review (25 min)
3. Resolve a merge conflict (25 min)
4. Undo without panic (20 min)

The interactive version of this workshop (progress, hints, self-checks, a clickable model of the four places) is [`index.html`](index.html) - open it in a browser.

## Step 1. Branches and pull requests  (25 min)

**Goal:** You develop on a branch, open a pull request on GitHub, merge it and update `main` locally.

Never develop directly on `main`. Create a branch for a piece of work, push it, and merge it through a pull request (PR) - the PR is where review happens.

```bash
git switch -c feature/button-input
# add button.cpp with a simple digitalRead loop
git add button.cpp
git commit -m "Read push button on D2"
git push -u origin feature/button-input
```

GitHub prints a link to open a pull request; open it, write a short description (what, why, how to test) and create the PR. Look at the *Files changed* tab. Merge it with the green button. Then bring `main` up to date locally and clean up:

```bash
git switch main
git pull
git branch -d feature/button-input
git log --oneline --graph
```

**Check:**
- [ ] The PR shows as *Merged* on GitHub
- [ ] `git log` on `main` contains the button commit
- [ ] The feature branch is deleted locally (and on GitHub, via the PR page)

> **Hint:** Branch names: `feature/...`, `fix/...`, `docs/...`. Short-lived: a branch should live hours or days, not weeks.

**Self-check:** What does `git push -u origin feature/x` do that `git push` does not?
1. It forces the push
2. It creates the branch on the remote and remembers it as the upstream, so later `git push`/`git pull` need no arguments
3. It merges into main

<details><summary>Answer</summary>

2. It creates the branch on the remote and remembers it as the upstream, so later `git push`/`git pull` need no arguments - `-u` (`--set-upstream`) links your local branch to the remote branch once. You saw this message when pushing a new branch.

</details>

## Step 2. Work together: collaborator and code review  (25 min)

**Goal:** Your partner contributes through a pull request and you review it like a professional.

Add your partner as a collaborator (GitHub > Settings > Collaborators). Partner: accept, clone, create a branch `feature/led-pattern`, add a file, commit, push, open a PR - exactly the flow from the previous step.

Owner: review the PR on GitHub. Leave at least one inline comment on a specific line (*Files changed* > hover a line > +), ask for one change (*Request changes*), and after the partner pushes a fix, approve and merge. Then swap roles on the partner's repository.

```bash
# partner, after 'Request changes':
git add led_pattern.cpp
git commit -m "Use constexpr for pattern length (review)"
git push                  # the PR updates automatically
```

**Check:**
- [ ] Each of you has opened one PR and reviewed one PR
- [ ] A review comment and a follow-up commit are visible in a PR
- [ ] `git log` on `main` shows commits by two different authors

> **Hint:** Review the code, not the person. Say what and why: 'Magic number 8 - name it so the next reader knows it is the LED count.'

**Self-check:** A reviewer clicked *Request changes*. What should the author do?
1. Open a new PR
2. Push extra commits to the same branch; the PR updates and the reviewer re-reviews
3. Force-push over main

<details><summary>Answer</summary>

2. Push extra commits to the same branch; the PR updates and the reviewer re-reviews - PRs track a branch. New commits on that branch appear in the PR automatically.

</details>

## Step 3. Resolve a merge conflict  (25 min)

**Goal:** You have caused, understood and resolved a merge conflict on purpose - so the real one next month does not scare you.

Both of you edit the **same line** of `blink.cpp` (the delay value) on two different branches from the same `main`. Merge the first PR on GitHub. The second PR now says *This branch has conflicts*. Resolve it locally:

```bash
git switch feature/blink-fast      # the second branch
git fetch origin
git rebase origin/main               # or: git merge origin/main
# CONFLICT (content): Merge conflict in blink.cpp
git status                           # 'both modified: blink.cpp'
```

```text
<<<<<<< HEAD
  delay(1000);
=======
  delay(100);
>>>>>>> feature/blink-fast
```

Open the file, decide what the code should be (one delay value, or something smarter), delete the `<<<<<<<`, `=======`, `>>>>>>>` markers, then:

```bash
git add blink.cpp
git rebase --continue                # (after a merge instead: git commit)
git push --force-with-lease          # only after a rebase, only on YOUR feature branch
# the PR is now mergeable
```

**Check:**
- [ ] No conflict markers are left in the file (`grep -n '<<<<' blink.cpp` finds nothing)
- [ ] The code compiles / makes sense - a conflict is resolved by thinking, not by picking a side blindly
- [ ] The second PR is merged

> **Hint:** `--force-with-lease` is the safe force: it refuses if someone else pushed in the meantime. Never force-push `main`.

**Self-check:** When does a merge conflict occur?
1. Whenever two people commit to the same repository
2. When two branches changed the same lines (or one deleted a file the other edited) and Git cannot decide
3. When you forget to pull

<details><summary>Answer</summary>

2. When two branches changed the same lines (or one deleted a file the other edited) and Git cannot decide - Git merges non-overlapping changes automatically. Only overlapping edits need a human decision.

</details>

## Step 4. Undo without panic  (20 min)

**Goal:** You know the right undo for each situation - and that almost nothing committed is ever lost.

Try each of these on your workshop repo. Read `git status` after every command - it tells you what it did.

```bash
# 1. discard an uncommitted change in the working tree
git restore blink.cpp
# 2. un-stage a file (keep the change)
git restore --staged blink.cpp
# 3. fix the last commit message (not yet pushed)
git commit --amend -m "Better message"
# 4. undo a pushed commit safely: make a new commit that reverses it
git revert HEAD
# 5. park half-done work to switch branches
git stash
git stash pop
# 6. find anything you 'lost'
git reflog
```

**Check:**
- [ ] You can say which of the six you would use for: wrong file staged / typo in last message / broken commit already on main
- [ ] You have seen `git reflog` list commits that no branch points to

> **Hint:** Rule of thumb: not pushed yet -> `amend`/`reset`; already pushed -> `revert`. Rewriting history that others have pulled breaks their repositories.

**Self-check:** A commit on `main` (already pushed, teammates have pulled it) broke the build. Safest fix?
1. git reset --hard HEAD~1 and force-push
2. git revert <commit> and push
3. Delete the repository and clone again

<details><summary>Answer</summary>

2. git revert <commit> and push - `revert` adds a new commit that undoes the change; history stays linear and nobody's clone breaks.

</details>

## Command reference

| Command | What it does |
| --- | --- |
| `git status` | where am I, what changed |
| `git add <file>` | stage a change |
| `git commit -m "msg"` | record staged changes |
| `git push` | send commits to remote |
| `git pull` | fetch + rebase (with pull.rebase=true) |
| `git switch -c <branch>` | create and switch to a branch |
| `git log --oneline --graph` | history |
| `git diff / --staged` | what changed |
| `git restore <file>` | discard working-tree change |
| `git revert <commit>` | undo a pushed commit safely |
| `git stash / stash pop` | park and restore work |
| `git reflog` | find lost commits |

## Resources

- [HAN SE Git manual v1.1 (Prog5/Demos)](../../Demos/Git manual SE HAN v1.1 jakorten.pdf)
- [Prog5_Git.pdf - basics slides](../../Demos/Prog5_Git.pdf)
- [Pro Git book (free)](https://git-scm.com/book/en/v2)
- [Merging vs rebasing - why not plain git pull (video)](https://youtu.be/xN1-2p06Urc)
- [Git stash in 5 minutes (video)](https://youtu.be/lH3ZkwbVp5E)
- [Oh Shit, Git!?! - undo recipes](https://ohshitgit.com/)
- [GitHub Docs: about pull requests](https://docs.github.com/pull-requests)

# Git Workshop 1 - The basics

**Prog 5 - Friday 4 September 2026 - hands-on, about 90 minutes**

Everything in Prog 5 and 6 is handed in through Git. After this workshop you can clone a repository, work in small commits, keep junk out of it and push your work to GitHub before a deadline. Work through the steps in order; every step ends with a check you can verify yourself.

Steps:
1. Set up Git and GitHub (15 min)
2. The mental model: four places (15 min)
3. Your first repository: clone, commit, push (20 min)
4. See what changed: status, diff, log (15 min)
5. Keep junk out: .gitignore (10 min)
6. Hand in the Prog 5 way (15 min)

The interactive version of this workshop (progress, hints, self-checks, a clickable model of the four places) is [`index.html`](index.html) - open it in a browser.

## Step 1. Set up Git and GitHub  (15 min)

**Goal:** Git works on your laptop, it knows who you are, and GitHub knows your SSH key or token.

Install Git (macOS: `xcode-select --install` or Homebrew; Windows: git-scm.com, choose Git Bash; Linux: your package manager). Create a GitHub account if you do not have one - use a name you are happy to show an employer.

```bash
git --version
git config --global user.name "Your Name"
git config --global user.email "you@student.han.nl"
git config --global init.defaultBranch main
git config --global pull.rebase true
git config --list --show-origin | grep -E 'user|pull|init'
```

Authenticate to GitHub once: either create an SSH key (`ssh-keygen -t ed25519`, add the public key under GitHub > Settings > SSH keys, test with `ssh -T git@github.com`) or install the GitHub CLI and run `gh auth login`.

**Check:**
- [ ] `git --version` prints 2.40 or newer
- [ ] `git config user.email` shows your HAN address
- [ ] `ssh -T git@github.com` greets you by username (or `gh auth status` says logged in)
- [ ] You have sent your GitHub username and student number to your lecturer

> **Hint:** `pull.rebase true` makes `git pull` do a rebase instead of a merge. Prog 5 rule: never use a plain merging pull - it litters your history with 'Merge branch' commits (see the video linked in Prog5_Git.pdf).

**Self-check:** What does `git config --global user.email` change?
1. The e-mail address GitHub sends notifications to
2. The author e-mail written into every commit you make on this computer
3. Your GitHub login

<details><summary>Answer</summary>

2. The author e-mail written into every commit you make on this computer - Git stamps author name and e-mail into each commit. GitHub only links the commit to your profile if that e-mail is registered on your account.

</details>

## Step 2. The mental model: four places  (15 min)

**Goal:** You can explain where a change is - working tree, staging area, local repository or remote - and which command moves it.

A Git project has four places. The **working tree** is the folder with your files. `git add` copies a snapshot of a file into the **staging area** (the index). `git commit` stores what is staged as a permanent commit in the **local repository** (the hidden `.git` folder). `git push` copies your commits to the **remote** (GitHub). `git clone` and `git pull` go the other way.

Use the interactive model (in the web version of this workshop) or draw the four boxes on paper. For every command in this workshop, ask: which arrow is this?

**Check:**
- [ ] You can draw the four places and put `add`, `commit`, `push`, `pull`, `clone` on the right arrows
- [ ] You can explain why a commit is not yet on GitHub

**Self-check:** You edited `main.cpp` and ran `git commit -m "fix"` - but `git status` still shows `main.cpp` as modified. Why?
1. The commit failed
2. You never ran `git add`, so nothing was staged and the commit contained no changes
3. GitHub rejected the commit

<details><summary>Answer</summary>

2. You never ran `git add`, so nothing was staged and the commit contained no changes - `git commit` only records what is in the staging area. Without `git add` the change stays in the working tree. (`git commit -a` stages tracked files for you - but be deliberate about what goes in a commit.)

</details>

## Step 3. Your first repository: clone, commit, push  (20 min)

**Goal:** A private repository on GitHub with one commit made by you, from the command line.

On GitHub create a new **private** repository called `prog5-git-workshop`, initialised with a README. Then follow the clone-first workflow from the HAN Git manual: create the remote first, clone it, work inside the clone.

```bash
cd ~/HAN/Prog5            # or any folder outside OneDrive/iCloud
git clone git@github.com:<you>/prog5-git-workshop.git
cd prog5-git-workshop
git status
```

Add a file `blink.cpp` with a minimal Arduino sketch (setup/loop that toggles the LED). Then stage, commit and push:

```bash
git add blink.cpp
git status                # blink.cpp is now 'staged' (green)
git commit -m "Add blink sketch"
git log --oneline
git push
```

```text
$ git log --oneline
3f2a9c1 (HEAD -> main, origin/main) Add blink sketch
b7e0d44 Initial commit
```

**Check:**
- [ ] `blink.cpp` is visible on GitHub
- [ ] `git log --oneline` shows your commit with `origin/main` next to it after the push
- [ ] Your commit on GitHub shows your avatar (author e-mail matches your account)

> **Hint:** Keep repositories out of cloud-synced folders (iCloud Drive, OneDrive, Google Drive). Sync clients corrupt `.git` folders. Git *is* your backup - that is what `push` is for.

**Self-check:** Which command sends your commits to GitHub?
1. git commit
2. git add
3. git push

<details><summary>Answer</summary>

3. git push - `commit` records locally, `push` transfers commits to the remote. Nothing is on GitHub until you push.

</details>

## Step 4. See what changed: status, diff, log  (15 min)

**Goal:** You read `git status` and `git diff` before every commit and write commit messages that explain *why*.

Change the blink delay in `blink.cpp` and add a comment. Before committing, look:

```bash
git status
git diff                  # working tree vs staging area
git add blink.cpp
git diff --staged         # staging area vs last commit
git commit -m "Slow down blink to 500 ms for readability"
git log --oneline --graph --decorate
```

A good commit message: imperative mood, under 50 characters on the first line, and it says what changed and why - not 'update' or 'fix stuff'. In the course rubric a readable history is part of the grade.

**Check:**
- [ ] You can tell the difference between `git diff` and `git diff --staged`
- [ ] Your last two commit messages would make sense to a colleague without opening the diff

> **Hint:** `git log --oneline --graph --all` is worth an alias: `git config --global alias.lg "log --oneline --graph --all --decorate"`. Then `git lg`.

**Self-check:** Which is the best commit message?
1. fixed
2. Update blink.cpp
3. Debounce button input to stop double counts

<details><summary>Answer</summary>

3. Debounce button input to stop double counts - It says what and why in the imperative. The other two force the reader to open the diff.

</details>

## Step 5. Keep junk out: .gitignore  (10 min)

**Goal:** Build output, IDE folders and OS files never enter the repository.

Simulate a build: create a folder `build/` with a file `blink.o` in it and a `.DS_Store` or `Thumbs.db` in the root. Run `git status` - Git offers to add them. Now add a `.gitignore`:

```bash
printf 'build/\ncmake-build-*/\n*.o\n*.a\n.DS_Store\nThumbs.db\n.vscode/\n.idea/\n' > .gitignore
git status                # build/ and *.o are gone from the list
git add .gitignore
git commit -m "Ignore build output and IDE files"
git push
```

If a file was already committed before you ignored it, `.gitignore` does not remove it: use `git rm --cached <file>` once, then commit.

**Check:**
- [ ] `git status` no longer lists `build/`
- [ ] `.gitignore` is committed and pushed
- [ ] You know why the course repository forbids committing `build/` (hint: the 2025 repo carried 2,000 build files and 240 MB)

**Self-check:** You committed `build/` last week and add `build/` to `.gitignore` today. What happens?
1. Git deletes the folder from history
2. Nothing changes for the already-tracked files - you must `git rm --cached -r build` first
3. GitHub blocks the push

<details><summary>Answer</summary>

2. Nothing changes for the already-tracked files - you must `git rm --cached -r build` first - `.gitignore` only affects untracked files. Tracked files stay tracked until you remove them from the index.

</details>

## Step 6. Hand in the Prog 5 way  (15 min)

**Goal:** You can set up a private repository for an assignment, work in it professionally and hand it in before the deadline.

Assignment 1 (stepper motor library, deadline Thursday 10 September) is handed in through your own Git repository. Create a new **private** repository on GitHub (e.g. `prog5-stepper`), add **jakorten** as collaborator (Settings > Collaborators) and clone it. Send your student number and GitHub username to your lecturer so the repository can be linked to you.

```bash
git clone git@github.com:<you>/prog5-stepper.git
cd prog5-stepper
# work in small steps:
git switch -c feature/stepper-class
... edit, git add, git commit (often) ...
git push -u origin feature/stepper-class
# open a PR, merge to main before the deadline
# the state of 'main' at the deadline is what is graded
```

What a professional repository looks like (this is what the rubric row *Manage: Code Management* rewards): a README that says what it is and how to build/run it; small commits with meaningful messages; branches and pull requests for each piece of work; no build output, binaries or IDE folders; a `.gitignore`; and, in team work, code reviews in the PRs.

**Check:**
- [ ] You created a private repository for assignment 1, added **jakorten** as collaborator and cloned it
- [ ] Your repository has a README and a `.gitignore` before the first real commit
- [ ] You know the deadline is Thu 10 Sep and that `main` on GitHub is what counts

**Self-check:** It is 23:50 on deadline day. Your work is committed but `git status` says 'Your branch is ahead of origin/main by 3 commits'. What is graded?
1. Your 3 local commits - Git is distributed
2. Only what is on GitHub - push now
3. The PR draft

<details><summary>Answer</summary>

2. Only what is on GitHub - push now - 'Ahead of origin' means the commits exist only on your laptop. Push, then check GitHub in the browser.

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

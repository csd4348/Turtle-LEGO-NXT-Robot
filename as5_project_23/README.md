## Assignment 5: Build Your Own Project
The goal of this assignment is to use the infrastructure you have built in previous assignments. 

### 1. Build a project

Choose a project of your liking. Look around for ideas. Talk to fellow students about ideas and their feasibility. Cite any sources (url, books, discussion with others) you used to come up with your project. The project does not need to move on wheels. It can just be a stationary construct with moving parts and does something interesting (and hopefully, useful).

Think through, upfront and before you get started, what will be the easy and what will be the challenging parts. Remember you have about the same time as an assignment for completing the project, so the challenging parts cannot be too many or too difficult. Plan to spend some time on the mechanical part of the project.

Design the code that will control the project carefully. Write and debug small pieces independently. Use the code from your previous assignment and write new pieces that you need. Do not use any code from third parties. If you do so, mention in your readme file why you had to do this and specify which parts are not yours.

Test the project and make sure you can demonstrate it in class! Be prepared to: (1) show the most insteresting aspects of the project, (2) explain what was challenging about it, and (3) say what you would have liked to do, but did not have enough time. 

### Use Git for your project
As you have done so far with your assignments fork the project repo.
1. Fork the repo (Follow the instructions in  [Assignments](https://www.csd.uoc.gr/~hy428/assignments.html)) 
2. Clone repo (git clone git@gitlab-csd.datacenter.uoc.gr:csdp1048/as5_project.git)
3. **Change** the permissions to **PRIVATE**, using the script "private.py" or by hand.

## Change the permissions
### By hand:

1. Go to Project Settings -> General
2. Visibility
3. Change Project Visibility to Private

Also add the TA in your project

1. Go to Project Settings -> Members
2. Add each TA as a Developer

### Using the script:
- The script requires gitlab lib
```
python3 -m pip install python-gitlab
```

- Run the script 
```
python3 private.py -t <yourToken> -p <ForkedProjectID>
```
## Submission 
- You should submit the **source files** of each assignment. **Do not** submit the toolchain and object or executable file
- Commit all files (not executables or object files!) 
```
git add file1.c file2.c file3.c  
git commit -m "Commit message"  
git push  
```

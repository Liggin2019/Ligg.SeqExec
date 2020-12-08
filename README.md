# About Ligg.SeqExec
- SeqExec is abbreviation of 'Sequential Executor'. 
- Current version: 1.2.1

## Introduction
This project is a sequential execution tool by C++, can be used as a starter for .net program. Tasks include: detect .net framework version, install .net framework, verify password at start, run exe file(or as admin), execute a command(or as admin), install MSI format file by options. 


## Usage Guide.

1. Each .exe file is linked to .ini file of same file name prefix. 

2. In .ini file, [RunAsAdmin]/[Id]="Administrator"  [RunAsAdmin]/[Password]="P@s$w0rd". To ensure run as admin when setting [RunByAdmin]=true in Step-x, please change  password of your local accout 'Administrator' to  "P@s$w0rd". Or you can change your [RunAsAdmin]/[Id][Password] according to actual by the way of Case 5. 

3. [IsDefaultMode]=false will pop up a list of all steps for selecting to execute.

d. [IsQuietMode]=false will pop up a step description window at the left-top corner of screen during running of each step.

e. [IsCompulsory]=true means you have to select this step to execute. 


f. For [RunType]=InstallNetFx, [Args]=v4\Full^Install^1 means if your .netFX version is above 4 and below 5, Detecting NetFx will return OK, will not install. 
   If the  [Args]=v4.6\Full^Install^1, means if your .netFX version is above 4.6 and below 4.7, Detecting NetFx will return OK.

g. [StartPolicy], decides if poping up a start password. The rule is: '.exe file name prefix' + '^0' or 'prefix' + '^n'(n>0). '^0'means no start password, '^n'means has.
   i.e., if your .exe file name is LgSeqExec1.exe, an encrypted text of 'LgSeqExec1^0' or 'LgSeqExec1^1' or 'LgSeqExec1^2'... is your [StartPolicy]. 'LgSeqExec1^0' means has no start password; 'LgSeqExec1^1' to 'LgSeqExec1^n' means has start password.
   Run case 5, to get an encrypted text. 

h. Following is the function description of each case:

## Cases
- Case0: .exe file: LgSeqExec.exe; .ini file: .\LgSeqExec.ini---Directly run an outside .exe file based on .netFx 4, but  your .net Framework on your machine is below 4, the outside application will pop up a error message.

- Case1: .exe file: LgSeqExec1.exe; .ini file: .\LgSeqExec1.ini---Detect and install .net Framework 4 by assigned [RunAsAdmin]/[Id]&[Password] , then run an outside .exe file based on .net Framework 4. 


- Case2: .exe file: LgSeqExec2.exe; .ini file: .\LgSeqExec2.ini---Detect and install .net Framework 4 by assigned [RunAsAdmin]/[Id]&[Password] ,Verify password(password is '123'), then run outside exe file based on .net Framework 4. 

- Case3: .exe file: LgSeqExec3.exe; .ini file: .\LgSeqExec3.ini---Detect and install .net Framework 4 by assigned [RunAsAdmin]/[Id]&[Password] , then run outside some exe files(based on .net Framework 4) by setting [IsQuietMode]=false, [IsDefaultMode]=false.


- Case4: .exe file: LgSeqExec4.exe; .ini file: .\LgSeqExec4.ini---Run outside LgXcopyA.exe(a native program) to copy a file to c:\ and c:\temp by assigned [RunAsAdmin]/[Id]&[Password]. it will pop up a UAC dialog, but no need to input password. 


- Case5: .exe file: LgSeqExec5.exe; .ini file: .\LgSeqExec5.ini---Encrypt text for [RunAsAdmin]/[Id]&[Password]ºÍ[StartPolicy] in .ini file. 

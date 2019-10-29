# Ligg.SeqExec

//*********************************************** Please see following guide before use it ***************************************************************//


1. Each .exe file is linked to .ini file of same file name prefix. 

2. In ini file, [RunAsAdmin]/[Id]="Administrator"  [RunAsAdmin]/[Password]="P@s$w0rd". 
To ensure run as admin when setting [IfRunByAdmin]=true in Step-x, please change  password of your local accout 'Administrator' to  "P@s$w0rd".

3. [IsDefaultMode]=false will pop up a list of all steps for selecting to execute.

4. [IsQuietMode]=false will pop up a description window during running of each step.

5. [IsCompulsory]=true means you have to select this step to execute. 

6. For [RunType]=InstallNetFx, [Args]=v4\Full^Install^1 means if your .netFX version is above 4 and below 5, Detecting NetFx will return OK, will not install. 
   If the  [Args]=v4.6\Full^Install^1, means if your .netFX version is above 4.6 and below 4.7, Detecting NetFx will return OK.

7. [StartPolicy], decides if poping up a start password. The rule is: '.exe file name prefix' + '^0' or 'prefix' + '^n'(n>0). '^0'means no start password, '^n'means has.
   i.e., if your .exe file name is LgSeqExec1.exe, an encrypted text of 'LgSeqExec1^0' or 'LgSeqExec1^1' or 'LgSeqExec1^2'... is your [StartPolicy]. 'LgSeqExec1^0' means has no start password; 'LgSeqExec1^1' means has start password.
   Run case 4, to get an encrypted text.
 
8. Following is the function description of each case:

Case0: .exe file: LgSeqExec.exe; .ini file: .\LgSeqExec.ini---Directly run outside .exe file based on .net Framework 4. If your .net Framework on your machine is below 4, the outside application will pop up a error message.

Case1: .exe file: LgSeqExec1.exe; .ini file: .\LgSeqExec1.ini---Detect and install .net Framework 4 by assigned [RunAsAdmin]/Id&Password , then run outside .exe file based on .net Framework 4. 

Case2: .exe file: LgSeqExec2.exe; .ini file: .\LgSeqExec2.ini---Detect and install .net Framework 4 by assigned [RunAsAdmin]/Id&Password ,Verify password(password is '123'), then run outside exe file based on .net Framework 4. 

Case3: .exe file: LgSeqExec3.exe; .ini file: .\LgSeqExec3.ini---Detect and install .net Framework 4 by assigned [RunAsAdmin]/Id&Password , then run outside some exe files(based on .net Framework 4) by setting [IsDefaultMode]=false, [IsDefaultMode]=false.
	
Case4: .exe file: LgSeqExec4.exe; .ini file: .\LgSeqExec4.ini---Run outside LgXcopyA.exe(a native program) to copy a file to c:\ and c:\temp by assigned [RunAsAdmin]/Id&Password.  

Case5: .exe file: LgSeqExec5.exe; .ini file: .\LgSeqExec5.ini---Encrypt text.

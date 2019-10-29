# Ligg.SeqExec

本项目是一个顺序执行工具，能被用作.net程序的启动器。任务包括：探测.net framework版本、安装.net framework、校验密码、运行.exe文件(以管理员帐号)、执行Windows脚本(以管理员帐号)以及以不同选项安装MSI文件。
This project is a sequential execution tool by C++, can be used as a starter for .net program. Tasks include: detect .net framework version, install .net framework, verify password at start, run exe file(or as admin), execute a command(or as admin), install MSI format file by options. 

以下是使用指南。
Following is the usage guide.

1. 每个.exe文件关联一个同名的配置文件。
1. Each .exe file is linked to .ini file of same file name prefix. 

2. 在.ini文件里, [RunAsAdmin]/[Id]="Administrator"  [RunAsAdmin]/[Password]="P@s$w0rd". 为确保在有[RunByAdmin]=true的设置步骤里，以管理员身份运行成功，请设置'Administrator'本地帐号的密码为"P@s$w0rd"。当然你可以用用例5的方式修改你的[RunAsAdmin]/[Id][Password]与实际匹配。
2. In .ini file, [RunAsAdmin]/[Id]="Administrator"  [RunAsAdmin]/[Password]="P@s$w0rd". To ensure run as admin when setting [RunByAdmin]=true in Step-x, please change  password of your local accout 'Administrator' to  "P@s$w0rd". Or you can change your [RunAsAdmin]/[Id][Password] according to actual by the way of Case 5. 

3.设置[IsDefaultMode]=false会弹出一个所有步骤的列表供选择是否执行。
3. [IsDefaultMode]=false will pop up a list of all steps for selecting to execute.

4. 设置[IsQuietMode]=false,会在当步骤运行时在屏幕左上角弹出一个步骤描述窗口。
4. [IsQuietMode]=false will pop up a step description window at the left-top corner of screen during running of each step.

5. 设置[IsCompulsory]=true,表示该步骤是强制执行的。
5. [IsCompulsory]=true means you have to select this step to execute. 

6. 对于[RunType]=InstallNetFx, [Args]=v4\Full^Install^1，假如.net Framework的版本大于4小于5，探测程序默认正确，不会安装.net Framework；同理对于[Args]=v4.6\Full^Install^1，假如.net Framework的版本大于4.6小于4.7，探测程序默认正确.
6. For [RunType]=InstallNetFx, [Args]=v4\Full^Install^1 means if your .netFX version is above 4 and below 5, Detecting NetFx will return OK, will not install. 
   If the  [Args]=v4.6\Full^Install^1, means if your .netFX version is above 4.6 and below 4.7, Detecting NetFx will return OK.

6. 配置项[StartPolicy]决定了是否弹出密码验证窗口,规则是'运行文件名前缀+prefix' + '^0' or '运行文件名前缀' + '^n'(n>0)。也就是说，假如你的执行文件是LgSeqExec1.exe，'LgSeqExec1^0' or 'LgSeqExec1^1' or 'LgSeqExec1^2'...的加密文本就是你的[StartPolicy]的值。'LgSeqExec1^0'无启动密码，'LgSeqExec1^1' to 'LgSeqExec1^n'有启动密码。
7. [StartPolicy], decides if poping up a start password. The rule is: '.exe file name prefix' + '^0' or 'prefix' + '^n'(n>0). '^0'means no start password, '^n'means has.
   i.e., if your .exe file name is LgSeqExec1.exe, an encrypted text of 'LgSeqExec1^0' or 'LgSeqExec1^1' or 'LgSeqExec1^2'... is your [StartPolicy]. 'LgSeqExec1^0' means has no start password; 'LgSeqExec1^1' to 'LgSeqExec1^n' means has start password.
   Run case 5, to get an encrypted text. 

8.下面是对各个用例的解释：
8. Following is the function description of each case:

用例0: .exe 文件: LgSeqExec.exe; .ini 文件: .\LgSeqExec.ini---直接运行一个基于.netFx4的外部执行文件，假如机器的.net Framework版本低于4.0，该执行文件会弹出错误提示。
Case0: .exe file: LgSeqExec.exe; .ini file: .\LgSeqExec.ini---Directly run an outside .exe file based on .netFx 4, but  your .net Framework on your machine is below 4, the outside application will pop up a error message.


用例1: .exe 文件: LgSeqExec1.exe; .ini 文件: .\LgSeqExec1.ini---通过配置项[RunAsAdmin]/[Id]&[Password]的用户来探测和安装.net Framework 4,然后运行一个基于.netFx4的外部执行文件。
Case1: .exe file: LgSeqExec1.exe; .ini file: .\LgSeqExec1.ini---Detect and install .net Framework 4 by assigned [RunAsAdmin]/[Id]&[Password] , then run an outside .exe file based on .net Framework 4. 

用例2: .exe 文件: LgSeqExec2.exe; .ini 文件: .\LgSeqExec2.ini---通过配置项[RunAsAdmin]/[Id]&[Password]的用户来探测和安装.net Framework 4,然后校验密码(密码:123)，再运行一个基于.netFx4的外部执行文件。
Case2: .exe file: LgSeqExec2.exe; .ini file: .\LgSeqExec2.ini---Detect and install .net Framework 4 by assigned [RunAsAdmin]/[Id]&[Password] ,Verify password(password is '123'), then run outside exe file based on .net Framework 4. 

用例3: .exe 文件: LgSeqExec3.exe; .ini 文件: .\LgSeqExec3.ini---通过配置项[RunAsAdmin]/[Id]&[Password]的用户来探测和安装.net Framework 4,然后运行一个基于.netFx4的外部执行文件。因为设置了[IsQuietMode]=false, [IsDefaultMode]=false，所以会弹出有步骤列表和步骤描述窗口。
Case3: .exe file: LgSeqExec3.exe; .ini file: .\LgSeqExec3.ini---Detect and install .net Framework 4 by assigned [RunAsAdmin]/[Id]&[Password] , then run outside some exe files(based on .net Framework 4) by setting [IsQuietMode]=false, [IsDefaultMode]=false.

用例4: .exe 文件: LgSeqExec4.exe; .ini 文件: .\LgSeqExec4.ini---通过配置项[RunAsAdmin]/[Id]&[Password]的用户来运行一个外部的LgXcopyA.exe(native程序)将文件拷贝到c:\和c:\temp。会有UAC窗口弹出，但不需输密码。
Case4: .exe file: LgSeqExec4.exe; .ini file: .\LgSeqExec4.ini---Run outside LgXcopyA.exe(a native program) to copy a file to c:\ and c:\temp by assigned [RunAsAdmin]/[Id]&[Password]. it will pop up a UAC dialog, but no need to input password. 

用例5: .exe 文件: LgSeqExec5.exe; .ini 文件: .\LgSeqExec5.ini---加密文本。供加密配置项[RunAsAdmin]/[Id]&[Password]和[StartPolicy]。
Case5: .exe file: LgSeqExec5.exe; .ini file: .\LgSeqExec5.ini---Encrypt text for [RunAsAdmin]/[Id]&[Password]和[StartPolicy] in .ini file. 

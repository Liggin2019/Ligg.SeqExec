# Ligg.SeqExec

����Ŀ��һ��˳��ִ�й��ߣ��ܱ�����.net����������������������̽��.net framework�汾����װ.net framework��У�����롢����.exe�ļ�(�Թ���Ա�ʺ�)��ִ��Windows�ű�(�Թ���Ա�ʺ�)�Լ��Բ�ͬѡ�װMSI�ļ���
This project is a sequential execution tool by C++, can be used as a starter for .net program. Tasks include: detect .net framework version, install .net framework, verify password at start, run exe file(or as admin), execute a command(or as admin), install MSI format file by options. 

������ʹ��ָ�ϡ�
Following is the usage guide.

1. ÿ��.exe�ļ�����һ��ͬ���������ļ���
1. Each .exe file is linked to .ini file of same file name prefix. 

2. ��.ini�ļ���, [RunAsAdmin]/[Id]="Administrator"  [RunAsAdmin]/[Password]="P@s$w0rd". Ϊȷ������[RunByAdmin]=true�����ò�����Թ���Ա������гɹ���������'Administrator'�����ʺŵ�����Ϊ"P@s$w0rd"����Ȼ�����������5�ķ�ʽ�޸����[RunAsAdmin]/[Id][Password]��ʵ��ƥ�䡣
2. In .ini file, [RunAsAdmin]/[Id]="Administrator"  [RunAsAdmin]/[Password]="P@s$w0rd". To ensure run as admin when setting [RunByAdmin]=true in Step-x, please change  password of your local accout 'Administrator' to  "P@s$w0rd". Or you can change your [RunAsAdmin]/[Id][Password] according to actual by the way of Case 5. 

3.����[IsDefaultMode]=false�ᵯ��һ�����в�����б�ѡ���Ƿ�ִ�С�
3. [IsDefaultMode]=false will pop up a list of all steps for selecting to execute.

4. ����[IsQuietMode]=false,���ڵ���������ʱ����Ļ���Ͻǵ���һ�������������ڡ�
4. [IsQuietMode]=false will pop up a step description window at the left-top corner of screen during running of each step.

5. ����[IsCompulsory]=true,��ʾ�ò�����ǿ��ִ�еġ�
5. [IsCompulsory]=true means you have to select this step to execute. 

6. ����[RunType]=InstallNetFx, [Args]=v4\Full^Install^1������.net Framework�İ汾����4С��5��̽�����Ĭ����ȷ�����ᰲװ.net Framework��ͬ�����[Args]=v4.6\Full^Install^1������.net Framework�İ汾����4.6С��4.7��̽�����Ĭ����ȷ.
6. For [RunType]=InstallNetFx, [Args]=v4\Full^Install^1 means if your .netFX version is above 4 and below 5, Detecting NetFx will return OK, will not install. 
   If the  [Args]=v4.6\Full^Install^1, means if your .netFX version is above 4.6 and below 4.7, Detecting NetFx will return OK.

6. ������[StartPolicy]�������Ƿ񵯳�������֤����,������'�����ļ���ǰ׺+prefix' + '^0' or '�����ļ���ǰ׺' + '^n'(n>0)��Ҳ����˵���������ִ���ļ���LgSeqExec1.exe��'LgSeqExec1^0' or 'LgSeqExec1^1' or 'LgSeqExec1^2'...�ļ����ı��������[StartPolicy]��ֵ��'LgSeqExec1^0'���������룬'LgSeqExec1^1' to 'LgSeqExec1^n'���������롣
7. [StartPolicy], decides if poping up a start password. The rule is: '.exe file name prefix' + '^0' or 'prefix' + '^n'(n>0). '^0'means no start password, '^n'means has.
   i.e., if your .exe file name is LgSeqExec1.exe, an encrypted text of 'LgSeqExec1^0' or 'LgSeqExec1^1' or 'LgSeqExec1^2'... is your [StartPolicy]. 'LgSeqExec1^0' means has no start password; 'LgSeqExec1^1' to 'LgSeqExec1^n' means has start password.
   Run case 5, to get an encrypted text. 

8.�����ǶԸ��������Ľ��ͣ�
8. Following is the function description of each case:

����0: .exe �ļ�: LgSeqExec.exe; .ini �ļ�: .\LgSeqExec.ini---ֱ������һ������.netFx4���ⲿִ���ļ������������.net Framework�汾����4.0����ִ���ļ��ᵯ��������ʾ��
Case0: .exe file: LgSeqExec.exe; .ini file: .\LgSeqExec.ini---Directly run an outside .exe file based on .netFx 4, but  your .net Framework on your machine is below 4, the outside application will pop up a error message.


����1: .exe �ļ�: LgSeqExec1.exe; .ini �ļ�: .\LgSeqExec1.ini---ͨ��������[RunAsAdmin]/[Id]&[Password]���û���̽��Ͱ�װ.net Framework 4,Ȼ������һ������.netFx4���ⲿִ���ļ���
Case1: .exe file: LgSeqExec1.exe; .ini file: .\LgSeqExec1.ini---Detect and install .net Framework 4 by assigned [RunAsAdmin]/[Id]&[Password] , then run an outside .exe file based on .net Framework 4. 

����2: .exe �ļ�: LgSeqExec2.exe; .ini �ļ�: .\LgSeqExec2.ini---ͨ��������[RunAsAdmin]/[Id]&[Password]���û���̽��Ͱ�װ.net Framework 4,Ȼ��У������(����:123)��������һ������.netFx4���ⲿִ���ļ���
Case2: .exe file: LgSeqExec2.exe; .ini file: .\LgSeqExec2.ini---Detect and install .net Framework 4 by assigned [RunAsAdmin]/[Id]&[Password] ,Verify password(password is '123'), then run outside exe file based on .net Framework 4. 

����3: .exe �ļ�: LgSeqExec3.exe; .ini �ļ�: .\LgSeqExec3.ini---ͨ��������[RunAsAdmin]/[Id]&[Password]���û���̽��Ͱ�װ.net Framework 4,Ȼ������һ������.netFx4���ⲿִ���ļ�����Ϊ������[IsQuietMode]=false, [IsDefaultMode]=false�����Իᵯ���в����б�Ͳ����������ڡ�
Case3: .exe file: LgSeqExec3.exe; .ini file: .\LgSeqExec3.ini---Detect and install .net Framework 4 by assigned [RunAsAdmin]/[Id]&[Password] , then run outside some exe files(based on .net Framework 4) by setting [IsQuietMode]=false, [IsDefaultMode]=false.

����4: .exe �ļ�: LgSeqExec4.exe; .ini �ļ�: .\LgSeqExec4.ini---ͨ��������[RunAsAdmin]/[Id]&[Password]���û�������һ���ⲿ��LgXcopyA.exe(native����)���ļ�������c:\��c:\temp������UAC���ڵ����������������롣
Case4: .exe file: LgSeqExec4.exe; .ini file: .\LgSeqExec4.ini---Run outside LgXcopyA.exe(a native program) to copy a file to c:\ and c:\temp by assigned [RunAsAdmin]/[Id]&[Password]. it will pop up a UAC dialog, but no need to input password. 

����5: .exe �ļ�: LgSeqExec5.exe; .ini �ļ�: .\LgSeqExec5.ini---�����ı���������������[RunAsAdmin]/[Id]&[Password]��[StartPolicy]��
Case5: .exe file: LgSeqExec5.exe; .ini file: .\LgSeqExec5.ini---Encrypt text for [RunAsAdmin]/[Id]&[Password]��[StartPolicy] in .ini file. 

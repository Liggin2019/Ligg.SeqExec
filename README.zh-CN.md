# ����Ligg.SeqExec
�������� | [English](./README.md)
- SeqExec��'Sequential Executor'����д��
- ��ǰ�汾: 1.2.1

## ����
����Ŀ��һ��˳��ִ�й��ߣ��ܱ�����.net����������������������̽��.net framework�汾����װ.net framework��У���������롢����.exe�ļ�(���Թ���Ա�ʺ�)��ִ��Windows�ű�(���Թ���Ա�ʺ�)�Լ��Բ�ͬѡ�װMSI�ļ�(���Թ���Ա�ʺ�)��


## ʹ��ָ��

1. ÿ��.exe�ļ�����һ��ͬ���������ļ���
2. ��.ini�ļ���, [RunAsAdmin]/[Id]="Administrator"  [RunAsAdmin]/[Password]="P@s$w0rd". Ϊȷ������[RunByAdmin]=true�����ò�����Թ���Ա������гɹ���������'Administrator'�����ʺŵ�����Ϊ"P@s$w0rd"����Ȼ�����������5�ķ�ʽ�޸����[RunAsAdmin]/[Id][Password]��ʵ��ƥ�䡣

3. ����[IsDefaultMode]=false�ᵯ��һ�����в�����б�ѡ���Ƿ�ִ�С�

4. ����[IsQuietMode]=false,���ڵ���������ʱ����Ļ���Ͻǵ���һ�������������ڡ�

5. ����[IsCompulsory]=true,��ʾ�ò�����ǿ��ִ�еġ�

6. ����[RunType]=InstallNetFx, [Args]=v4\Full^Install^1������.net Framework�İ汾����4С��5��̽�����Ĭ����ȷ�����ᰲװ.net Framework��ͬ�����[Args]=v4.6\Full^Install^1������.net Framework�İ汾����4.6С��4.7��̽�����Ĭ����ȷ��

7. ������[StartPolicy]�������Ƿ񵯳�������֤����,������'�����ļ���ǰ׺+prefix' + '^0' or '�����ļ���ǰ׺' + '^n'(n>0)��Ҳ����˵���������ִ���ļ���LgSeqExec1.exe��'LgSeqExec1^0' or 'LgSeqExec1^1' or 'LgSeqExec1^2'...�ļ����ı��������[StartPolicy]��ֵ��'LgSeqExec1^0'���������룬'LgSeqExec1^1' to 'LgSeqExec1^n'���������롣

## ����
### �����ء�demo.rar������
0. ����0: .exe �ļ�: LgSeqExec.exe; .ini �ļ�: .\LgSeqExec.ini---ֱ������һ������.netFx4���ⲿִ���ļ������������.net Framework�汾����4.0����ִ���ļ��ᵯ��������ʾ��

1. ����1: .exe �ļ�: LgSeqExec1.exe; .ini �ļ�: .\LgSeqExec1.ini---ͨ��������[RunAsAdmin]/[Id]&[Password]���û���̽��Ͱ�װ.net Framework 4,Ȼ������һ������.netFx4���ⲿִ���ļ���

2. ����2: .exe �ļ�: LgSeqExec2.exe; .ini �ļ�: .\LgSeqExec2.ini---ͨ��������[RunAsAdmin]/[Id]&[Password]���û���̽��Ͱ�װ.net Framework 4,Ȼ��У������(����:123)��������һ������.netFx4���ⲿִ���ļ���

3. ����3: .exe �ļ�: LgSeqExec3.exe; .ini �ļ�: .\LgSeqExec3.ini---ͨ��������[RunAsAdmin]/[Id]&[Password]���û���̽��Ͱ�װ.net Framework 4,Ȼ������һ������.netFx4���ⲿִ���ļ�����Ϊ������[IsQuietMode]=false, [IsDefaultMode]=false�����Իᵯ���в����б�Ͳ����������ڡ�

4. ����4: .exe �ļ�: LgSeqExec4.exe; .ini �ļ�: .\LgSeqExec4.ini---ͨ��������[RunAsAdmin]/[Id]&[Password]���û�������һ���ⲿ��LgXcopyA.exe(native����)���ļ�������c:\��c:\temp������UAC���ڵ����������������롣


5. ����5: .exe �ļ�: LgSeqExec5.exe; .ini �ļ�: .\LgSeqExec5.ini---�����ı���������������[RunAsAdmin]/[Id]&[Password]��[StartPolicy]��

## ʾ����ͼ
- ��¼
![��¼](https://liggin2019.gitee.io/static/images/Ligg.Vue-login.png)
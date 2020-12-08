# 关于Ligg.SeqExec
简体中文 | [English](./README.md)
- SeqExec是'Sequential Executor'的缩写。
- 当前版本: 1.2.1

## 介绍
本项目是一个顺序执行工具，能被用作.net程序的启动器。任务包括：探测.net framework版本、安装.net framework、校验启动密码、运行.exe文件(或以管理员帐号)、执行Windows脚本(或以管理员帐号)以及以不同选项安装MSI文件(或以管理员帐号)。


## 使用指南

1. 每个.exe文件关联一个同名的配置文件。
2. 在.ini文件里, [RunAsAdmin]/[Id]="Administrator"  [RunAsAdmin]/[Password]="P@s$w0rd". 为确保在有[RunByAdmin]=true的设置步骤里，以管理员身份运行成功，请设置'Administrator'本地帐号的密码为"P@s$w0rd"。当然你可以用用例5的方式修改你的[RunAsAdmin]/[Id][Password]与实际匹配。

3. 设置[IsDefaultMode]=false会弹出一个所有步骤的列表供选择是否执行。

4. 设置[IsQuietMode]=false,会在当步骤运行时在屏幕左上角弹出一个步骤描述窗口。

5. 设置[IsCompulsory]=true,表示该步骤是强制执行的。

6. 对于[RunType]=InstallNetFx, [Args]=v4\Full^Install^1，假如.net Framework的版本大于4小于5，探测程序默认正确，不会安装.net Framework；同理对于[Args]=v4.6\Full^Install^1，假如.net Framework的版本大于4.6小于4.7，探测程序默认正确。

7. 配置项[StartPolicy]决定了是否弹出密码验证窗口,规则是'运行文件名前缀+prefix' + '^0' or '运行文件名前缀' + '^n'(n>0)。也就是说，假如你的执行文件是LgSeqExec1.exe，'LgSeqExec1^0' or 'LgSeqExec1^1' or 'LgSeqExec1^2'...的加密文本就是你的[StartPolicy]的值。'LgSeqExec1^0'无启动密码，'LgSeqExec1^1' to 'LgSeqExec1^n'有启动密码。

## 用例
### 请下载‘demo.rar’对照
0. 用例0: .exe 文件: LgSeqExec.exe; .ini 文件: .\LgSeqExec.ini---直接运行一个基于.netFx4的外部执行文件，假如机器的.net Framework版本低于4.0，该执行文件会弹出错误提示。

1. 用例1: .exe 文件: LgSeqExec1.exe; .ini 文件: .\LgSeqExec1.ini---通过配置项[RunAsAdmin]/[Id]&[Password]的用户来探测和安装.net Framework 4,然后运行一个基于.netFx4的外部执行文件。

2. 用例2: .exe 文件: LgSeqExec2.exe; .ini 文件: .\LgSeqExec2.ini---通过配置项[RunAsAdmin]/[Id]&[Password]的用户来探测和安装.net Framework 4,然后校验密码(密码:123)，再运行一个基于.netFx4的外部执行文件。

3. 用例3: .exe 文件: LgSeqExec3.exe; .ini 文件: .\LgSeqExec3.ini---通过配置项[RunAsAdmin]/[Id]&[Password]的用户来探测和安装.net Framework 4,然后运行一个基于.netFx4的外部执行文件。因为设置了[IsQuietMode]=false, [IsDefaultMode]=false，所以会弹出有步骤列表和步骤描述窗口。

4. 用例4: .exe 文件: LgSeqExec4.exe; .ini 文件: .\LgSeqExec4.ini---通过配置项[RunAsAdmin]/[Id]&[Password]的用户来运行一个外部的LgXcopyA.exe(native程序)将文件拷贝到c:\和c:\temp。会有UAC窗口弹出，但不需输密码。


5. 用例5: .exe 文件: LgSeqExec5.exe; .ini 文件: .\LgSeqExec5.ini---加密文本。供加密配置项[RunAsAdmin]/[Id]&[Password]和[StartPolicy]。

## 示例截图
- 登录
![登录](https://liggin2019.gitee.io/static/images/Ligg.Vue-login.png)
本目录内容用于创建安装包。


目录：
Debug和Release目录中分别是两个版本的程序文件。
Common目录中是Debug和Release版的公用文件。
FileList中定义了各部分要打包的文件列表，用于文件自动更新。


脚本：
UpdateFiles.bat脚本用于将项目文件夹中的最新文件复制到本目录相应位置。执行UpdateFiles.bat脚本时会读取FileList中的文件。但注意以下几点：
1、RTPWare中的NetworkMeasurer不在任何一个列表中，而是在UpdateFiles.bat直接复制到CoolView目录中的，具体原因是因为这么做较方便。
2、QT和VC运行时文件不会由脚本自动更新，需要手动更新。如果误删除，需要手动恢复（可通过SVN的Revert操作完成）。
   VC运行时文件可通过
       C:\Program Files\Microsoft Visual Studio 11.0\VC\redist\x86  （Release） 或
       C:\Program Files\Microsoft Visual Studio 11.0\VC\redist\Debug_NonRedist\x86  （Debug）获得。
3、CvSipWrapper中的Boost文件不会自动更新，需要手动更新。如果误删除，需要手动恢复（可通过SVN的Revert操作完成）。
4、如果文件不存在（如没有Checkout相应项目或没有编译Debug版本），则忽略更新相应文件，不会影响脚本执行。
5、必要时，记得检查是否更新成功。


打包方法：
为了保证程序文件是最新的，建议遵循以下步骤：
1、对CvSetup执行Update。
2、执行UpdateFiles.bat脚本，将本机编译的最新文件复制到此处。遇到新旧文件替换提示，此时很可能CvSetup中的文件比自己的文件要新，判断是否要替换。
3、解决冲突，并Commit。
4、使用Inno Setup Compiler打开CoolViewSetup.iss脚本打包Release版本的安装包，或CoolViewSetupDebug.iss打包Debug安装包（包含pdb文件，可用于远程调试）。
5、使用BuildCoolViewDevDir.bat脚本，将最新文件同步到开发调试目录。

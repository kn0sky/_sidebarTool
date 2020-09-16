#include<Windows.h>
#include<stdio.h>
#include<string.h>


VOID EnumFile(DWORD type, LPCWSTR dir);
//��ʼ��_sidebar.md�ļ�
VOID Init_sidebar() {
	HANDLE hFile = CreateFile(L"_sidebar.md", GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);//������ļ�,���û�оʹ���
	char szHeader[MAX_PATH] = "<!-- docs/_sidebar.md created by koko-docsify_sidebarTool -->\r\n\r\n";
	WriteFile(hFile,szHeader,strlen(szHeader)+sizeof(char),NULL,NULL);
	CloseHandle(hFile);
}


//��_sidebar.md�ļ�д���������������Ϣ,ִ��һ��д��һ��
VOID Write_sidebar(LPCWSTR FileName,LPCWSTR FilePath) {
	//�����ļ�
	HANDLE hFile =	CreateFile(L"_sidebar.md",GENERIC_READ|GENERIC_WRITE,0,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);//������ļ�,���û�оʹ���
	
	//�����ļ�����
	WCHAR wcBuf[MAX_PATH] = { 0 };
	wcscat_s(wcBuf, L"- [");
	wcscat_s(wcBuf, FileName);
	wcscat_s(wcBuf, L"](");
	wcscat_s(wcBuf, FilePath);
	wcscat_s(wcBuf, L")\r\n");

	SetFilePointer(hFile,-1,NULL,FILE_END);

	char szTmpBuf[MAX_PATH] = { 0 };
	WideCharToMultiByte(CP_ACP,NULL,wcBuf,-1,szTmpBuf,MAX_PATH,NULL,NULL);
	WriteFile(hFile,szTmpBuf,strlen(szTmpBuf)+sizeof(char),NULL,NULL);

	CloseHandle(hFile);
}

//д��ո�
VOID Write_space() {
	HANDLE hFile = CreateFile(L"_sidebar.md", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);//������ļ�,���û�оʹ���
	char szTmpBuf[MAX_PATH] = "  ";
	SetFilePointer(hFile, 0, NULL, FILE_END);
	WriteFile(hFile, szTmpBuf, strlen(szTmpBuf) + sizeof(char), NULL, NULL);
	CloseHandle(hFile);
}


/*
ö�ٵ�ǰ�ļ�����ÿ��Ŀ¼
type��ֵΪ0,���ʾ��Ŀ¼����ģʽ,·��Ϊ�ļ�����/README.md
type��ֵΪ1,���ʾ��Ŀ¼����ģʽ,������Ŀ¼����md�ļ�
*/
VOID EnumDir(DWORD type=0) {
	WIN32_FIND_DATA FileData;
	HANDLE hSearch = FindFirstFile(L"*", &FileData);//��ȡ����Ŀ¼�������ļ�

	do{
		if (FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY && lstrcmp(FileData.cFileName, L".") != 0 && lstrcmp(FileData.cFileName, L"..") != 0) {
				WCHAR wcFilePath[MAX_PATH] = { 0 };
				lstrcat(wcFilePath, FileData.cFileName);
				lstrcat(wcFilePath, L"/README.md");
				Write_sidebar(FileData.cFileName, wcFilePath);
				if (type == 1) {
					WCHAR wcSubFilePath[MAX_PATH] = { 0 };
					lstrcat(wcSubFilePath, FileData.cFileName);
					EnumFile(1, wcSubFilePath);
				}


		}		
	} while (FindNextFile(hSearch, &FileData));

	//CloseHandle�����:http://www.heycode.com/a13141.html
	FindClose(hSearch);
}

/*
ö�ٵ�ǰ�ļ�����ÿ��md�ļ�
type��ֵΪ0,���ʾ��Ŀ¼����ģʽ,·��Ϊ�ļ���.md
type��ֵΪ1,���ʾ��Ŀ¼����ģʽ,·��ΪĿ¼/�ļ���.md,typeΪ1��Ҫָ��Ŀ¼��˭!
*/
VOID EnumFile(DWORD type=0,LPCWSTR dir=L"directory") {
	WIN32_FIND_DATA FileData;
	HANDLE hSearch = NULL;

	switch (type){
		case 0: {
			hSearch = FindFirstFile(L"*.md", &FileData);//��ȡ����Ŀ¼�������ļ�
			do {
				if (FileData.dwFileAttributes == FILE_ATTRIBUTE_ARCHIVE) {
					//ƴ���ļ���
					WCHAR wcFileName[MAX_PATH] = { 0 };
					wcsncpy_s(wcFileName, FileData.cFileName, wcslen(FileData.cFileName) - 3);
					Write_sidebar(wcFileName, FileData.cFileName);
				}
			} while (FindNextFile(hSearch, &FileData));
			break;
		}
		case 1: {
			WCHAR DirPath[MAX_PATH] = { 0 };
			wcscat_s(DirPath, L"./");
			wcscat_s(DirPath, dir);
			wcscat_s(DirPath, L"/*.md");
			hSearch = FindFirstFile(DirPath, &FileData);//��ȡ����Ŀ¼�������ļ�
			do {
				if (FileData.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE) {
					//ƴ���ļ���
					WCHAR wcFileName[MAX_PATH] = { 0 };
					wcsncpy_s(wcFileName, FileData.cFileName, wcslen(FileData.cFileName) - 3);
					//ƴ��·����
					WCHAR wcFilePath[MAX_PATH] = { 0 };
					wcscat_s(wcFilePath, dir);
					wcscat_s(wcFilePath, L"/");
					wcscat_s(wcFilePath, FileData.cFileName);
					Write_space();
					Write_sidebar(wcFileName, wcFilePath);
				}
			} while (FindNextFile(hSearch, &FileData));
			break;
		}
	default:
		break;
	}
	

	//CloseHandle�����:http://www.heycode.com/a13141.html
	FindClose(hSearch);
}

int main() {
	Init_sidebar();
	EnumDir(1);
	EnumFile();
	return 0;
}